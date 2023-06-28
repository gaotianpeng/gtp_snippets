#include <iostream>
#include <limits>
#include <string>
using namespace std;

/*
数值类型（numeric type）有着与平台相依的极值
	C借由<climits>,<limits.h>,<cfloat>,<float.h>
	C++ 标准库借由 template numeric_limits提供这些极值
*/

int main(int argc, char* argv[]) {
	cout << boolalpha << endl;

	// print maximum of integral types
	cout << "max(short):	" << numeric_limits<short>::max() << endl;
	cout << "max(int):		" << numeric_limits<int>::max() << endl;
	cout << "max(long):		" << numeric_limits<long>::max() << endl;
	cout << endl;

	// print maximum of floating-point types
	cout << "max(float):		" << numeric_limits<float>::max() << endl;
	cout << "max(double):		" << numeric_limits<double>::max() << endl;
	cout << "max(long double):	" << numeric_limits<long double>::max() << endl;
	cout << endl;

	// print weather char is signed
	cout << "is_signed(char):	" << numeric_limits<char>::is_signed << endl;
	cout << endl;

	// print weather numeric limits for type string exist
	cout << "is_specialized(string): " << numeric_limits<string>::is_specialized << endl;
	cout << endl;
 	return 0;
}