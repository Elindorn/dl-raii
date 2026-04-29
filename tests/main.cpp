#include <catch2/catch_test_macros.hpp>

#include <dl_raii/library.hpp>
#include <dl_raii/util.hpp>

std::filesystem::path validPath = TEST_LIBRARY_PATH;

TEST_CASE("Loading", "[library]")
{
	SECTION("Invalid path")
	{
		CHECK_THROWS(dl::Library(""));
		CHECK_THROWS(dl::Library("."));
		CHECK_THROWS(dl::Library("/"));
		CHECK_THROWS(dl::Library("///"));
	}

	SECTION("Valid path")
	{
		CHECK_NOTHROW(dl::Library(validPath));
	}
}

TEST_CASE("Functionality", "[library]")
{
	SECTION("Getting symbols")
	{
		dl::Library library(validPath);

		auto* fibonacci = reinterpret_cast<uint64_t(*)(uint64_t)>(library.getSymbol("fibonacci"));
		auto* factorial = reinterpret_cast<uint64_t(*)(uint64_t)>(library.getSymbol("factorial"));

		CHECK(fibonacci == library.getFunction<uint64_t(uint64_t)>("fibonacci"));
		CHECK(factorial == library.getFunction<uint64_t(uint64_t)>("factorial"));

		CHECK(factorial != fibonacci);

		CHECK(factorial(1) == 1);
		CHECK(factorial(2) == 2);
		CHECK(factorial(3) == 6);
		CHECK(factorial(10) == 3'628'800);

		CHECK(fibonacci(1) == 1);
		CHECK(fibonacci(2) == 1);
		CHECK(fibonacci(3) == 2);
		CHECK(fibonacci(10) == 55);
	}

	SECTION("Move semantics")
	{
		dl::Library library1(validPath);
		auto* factorial1 = library1.getFunction<uint64_t(uint64_t)>("factorial");

		dl::Library library2(std::move(library1));
		auto* factorial2 = library2.getFunction<uint64_t(uint64_t)>("factorial");

		CHECK(factorial1 == factorial2);
		CHECK_THROWS(library1.getSymbol("fibonacci"));

		dl::Library library3 = std::move(library2);
		auto* factorial3 = library3.getFunction<uint64_t(uint64_t)>("factorial");

		CHECK(factorial2 == factorial3);
		CHECK_THROWS(library2.getSymbol("fibonacci"));
	}
}

TEST_CASE("Demangling", "[library]")
{
	SECTION("Demangling")
#if defined(_MSC_VER)
	{
		CHECK(dl::util::demangle("?f@@YAXHDN@Z") == "void f(int,char,double)");
		CHECK(dl::util::demangle("?f@@YAXKE@Z") == "void f(unsigned long,unsigned char)");
		CHECK(dl::util::demangle("?f@@YAXPAM@Z") == "void f(float *)");

		CHECK(dl::util::demangle("?Func@Namespace@@YAXXZ") == "void Namespace::Func()");
		CHECK(dl::util::demangle("?Func@Inner@Outer@@YAXH@Z") == "void Outer::Inner::Func(int)");
		CHECK(dl::util::demangle("?Method@MyClass@@QBEXXZ") == "void MyClass::Method()const ");
		CHECK(dl::util::demangle("??$f@H@@YAXH@Z") == "void f<int>(int)");

		CHECK(dl::util::demangle("??$f@H@@YAXH@Z") == "void f<int>(int)");
		CHECK(dl::util::demangle("??H@YAXHH@Z") == "void operator+(int,int)");
		CHECK(dl::util::demangle("??2@YAPAXI@Z") == "void * operator new(unsigned int)");
		CHECK(dl::util::demangle("??1MyClass@@UAE@XZ") == "MyClass::~MyClass()");
	}
#elif defined(__GNUC__) || defined(__clang__)
	{
		CHECK(dl::util::demangle("_Z1ficd") == "f(int, char, double)");
		CHECK(dl::util::demangle("_Z1fm") == "f(unsigned long)");
		CHECK(dl::util::demangle("_Z1fPf") == "f(float*)");

		CHECK(dl::util::demangle("_ZN9Namespace4FuncEv") == "Namespace::Func()");
		CHECK(dl::util::demangle("_ZN5Outer5Inner4FuncEi") == "Outer::Inner::Func(int)");
		CHECK(dl::util::demangle("_ZNK7MyClass6MethodEv") == "MyClass::Method() const");
		CHECK(dl::util::demangle("_Z1fIiEvT_") == "void f<int>(int)");

		CHECK(dl::util::demangle("_Zpl") == "operator+");
		CHECK(dl::util::demangle("_Zix") == "operator[]");
		CHECK(dl::util::demangle("_Znw") == "operator new");
		CHECK(dl::util::demangle("_ZN7MyClassD1Ev") == "MyClass::~MyClass()");
	}
#endif

	SECTION("Demangling errors")
	{
		CHECK(dl::util::demangle("_X123") == "_X123");
		CHECK(dl::util::demangle("???") == "???");
		CHECK(dl::util::demangle("") == "");
		CHECK(dl::util::demangle("_Z") == "_Z");
	}
}
