#include <catch.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

namespace report {

void printHeading(const char* str)
{
	std::cout << '\n' << str << '\n';
	std::cout << "**************************************************************************\n";
}

void printPredefinedMacros()
{
	static constexpr int macroWidth = 20;
	static constexpr int valueWidth = 15;

#define PRINT_MACRO(MACRO) std::cout << std::setw(macroWidth) << #MACRO << std::setw(valueWidth) << (MACRO) << '\n'
	PRINT_MACRO(__cplusplus);

#if defined(_MSC_VER)
	PRINT_MACRO(_MSC_VER);
	PRINT_MACRO(_MSC_FULL_VER);
	PRINT_MACRO(_MSVC_LANG);
	PRINT_MACRO(_MSC_BUILD);
	PRINT_MACRO(_INTEGRAL_MAX_BITS);
#endif

#undef PRINT_MACRO
}

/**
 * Call functions inside of functions to determine how addresses change with
 * respect to function calls.
 */
class StackGrowth {
public:
	// Records the address of an identifying variable within the function.
	static std::uintptr_t topLevelAddress, midLevelAddress, lowLevelAddress;

private:
	static void lowLevel()
	{
		int32_t emptyVar = 0;
		lowLevelAddress = reinterpret_cast<uintptr_t>(&emptyVar);
	}

	static void midLevel()
	{
		int32_t emptyVar = 0;
		midLevelAddress = reinterpret_cast<uintptr_t>(&emptyVar);
		lowLevel();
	}

	static void topLevel()
	{
		int32_t emptyVar = 0;
		topLevelAddress = reinterpret_cast<uintptr_t>(&emptyVar);
		midLevel();
	}

public:
	static void print()
	{
		topLevel();

		report::printHeading("Address of first variable in function calls");
		if (topLevelAddress > midLevelAddress) {
			std::cout << "Stack grows down\n";
		}
		else {
			std::cout << "Stack grows up\n";
		}
		std::cout << std::hex;
		std::cout << topLevelAddress << '\n';
		std::cout << midLevelAddress << '\n';
		std::cout << lowLevelAddress << '\n';
	}
};

std::uintptr_t StackGrowth::topLevelAddress;
std::uintptr_t StackGrowth::midLevelAddress;
std::uintptr_t StackGrowth::lowLevelAddress;

/**
 * Print type names, sizes, alignment, and ranges.
 */
void printTypeRepresentations()
{
	static constexpr int WidthName = 20;
	static constexpr int WidthBytes = 6;
	static constexpr int WidthBits = 6;
	static constexpr int WidthAlign = 6;
	static constexpr int WidthMinMax = 25;
	static constexpr int WidthHexMinMax = 25;

	report::printHeading("Variable size checks");

	std::cout <<
		std::dec <<
		std::setw(WidthName) << std::left << "Name" <<
		std::setw(WidthBytes) << std::right << "Bytes" <<
		std::setw(WidthBytes) << "Bits" <<
		std::setw(WidthAlign) << "Align" <<
		std::setw(WidthMinMax) << "Min" <<
		std::setw(WidthMinMax) << "Max" <<
		std::setw(WidthHexMinMax) << "Min (hex)" <<
		std::setw(WidthHexMinMax) << "Max (hex)" <<
		std::endl;

	std::cout << std::setw(WidthName) << std::left << std::dec << "char"
		<< std::setw(WidthBytes) << std::right << sizeof(char)
		<< std::setw(WidthBits) << std::right << (sizeof(char) * 8)
		<< std::setw(WidthAlign) << alignof(char)
		<< std::setw(WidthMinMax) << std::dec << static_cast<int>(std::numeric_limits<char>::min())
		<< std::setw(WidthMinMax) << std::dec << static_cast<int>(std::numeric_limits<char>::max())
		<< std::setw(WidthHexMinMax) << std::hex << static_cast<int>(std::numeric_limits<char>::min())
		<< std::setw(WidthHexMinMax) << std::hex << static_cast<int>(std::numeric_limits<char>::max())
		<< '\n';

#define ADD_TYPE(TYPE) std::cout << std::setw(WidthName) << std::left << std::dec << #TYPE \
	<< std::setw(WidthBytes) << std::right << sizeof(TYPE) \
	<< std::setw(WidthBits) << std::right << (sizeof(TYPE) * 8) \
	<< std::setw(WidthAlign) << alignof(TYPE) \
	<< std::setw(WidthMinMax) << std::dec << std::numeric_limits<TYPE>::min() \
	<< std::setw(WidthMinMax) << std::dec << std::numeric_limits<TYPE>::max() \
	<< std::setw(WidthHexMinMax) << std::hex << std::numeric_limits<TYPE>::min() \
	<< std::setw(WidthHexMinMax) << std::hex << std::numeric_limits<TYPE>::max() \
	<< '\n'

	ADD_TYPE(short);
	ADD_TYPE(int);
	ADD_TYPE(long);
	ADD_TYPE(long long);

	std::cout << '\n';

	ADD_TYPE(unsigned short);
	ADD_TYPE(unsigned long);
	ADD_TYPE(unsigned long);
	ADD_TYPE(unsigned long long);

	std::cout << '\n';

	ADD_TYPE(float);
	ADD_TYPE(double);
	ADD_TYPE(long double);
#undef ADD_TYPE
}

} // namespace report

int main()
{
	report::printPredefinedMacros();
	report::StackGrowth::print();
	report::printTypeRepresentations();
	return 0;
}
