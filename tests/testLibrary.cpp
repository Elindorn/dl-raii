#include <cstdint>

#ifdef _WIN32
	#define TEST_API extern "C" __declspec(dllexport)
#else
	#define TEST_API extern "C" __attribute__((visibility("default")))
#endif

TEST_API uint64_t factorial(const uint64_t number)
{
	return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_API uint64_t fibonacci(const uint64_t number)
{
	return number <= 1 ? number : fibonacci(number - 1) + fibonacci(number - 2);
}
