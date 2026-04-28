#include <catch2/catch_test_macros.hpp>

#include <dl_raii/library.hpp>

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

		auto* factorial = reinterpret_cast<uint64_t(*)(uint64_t)>(library1.getSymbol("factorial"));
		dl::Library library2(std::move(library1));

		CHECK(factorial == reinterpret_cast<uint64_t(*)(uint64_t)>(library2.getSymbol("factorial")));
		CHECK_THROWS(library1.getSymbol("fibonacci"));
	}
}
