#define _CRT_SECURE_NO_WARNING
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>
using namespace std;
/*
希望能够处理 timer和clock在不同系统中可能不同的事实，同时也为了强化时间精准度
duration(时间段)：指某时间单位上的一个明确的tick
timepoint(时间点): 一个duration和一个epoch(起始点)的组合
	“2000 年新年午夜”时间点，它被描述为“自 1970 年 1 月 1 日开始的第1 262 300 400秒”

*/

template <typename C>
void PrintClockData() {
	using namespace std;
	cout << "- precision: ";
	typedef typename C::period P; // type of time unit
	// if time unit is less or equal one millisecond
	if (ratio_less_equal<P, milli>::value) {
		typedef typename ratio_multiply<P, kilo>::type TT;
		cout << fixed << double(TT::num) / TT::den << " milliseconds" << endl;
	}
	else {
		// print as seconds
		cout << fixed << double(P::num) / P::den << " seconds" << endl;
	}

	cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}

std::string AsString(const std::chrono::system_clock::time_point& tp) {
	// convert to system time
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::string ts = std::asctime(gmtime(&t));
	ts.resize(ts.size() - 1);
	return ts;
}

int main(int argc, char* argv[]) {
	/*
	Duration是一个数值(tick个数)和一个分数(时间单位)的组合
	*/
	std::chrono::duration<int> twenty_seconds(20);	// 以秒为单位				
	std::chrono::duration<double, std::ratio<60>> half_a_min(0.5);	// 以分钟为单位
	std::chrono::duration<long, std::ratio<1, 1000>> one_milliseconds(1);	// 以毫秒为单位
	// 标准库提供了各种类型定义
	std::chrono::seconds twenty_sec(20);
	std::chrono::hours  one_day(24);
	std::chrono::milliseconds one_millsecond(1);
	// Druation的算术运算
	chrono::seconds d1(42);
	chrono::milliseconds d2(10);
	cout << d1.count() << endl;
	cout << (d1 - d2).count() << endl;

	/*
	Clock(时钟)和Timepoint(时间点)
		Clock定义出一个epoch(超始点)和一个tick周期
			例如某个clock也许定义tick周期为毫秒，起始点是UNIX epoch（1970年1月1日）
		Timepoint表现出某个特定时间点，关联至某个clock的某个正值或负值duration
			如果duration是10天而其所关联的clock epoch是“1970年1月1日”，那么这个timepoint表现的就是1970年1月11日
	C++ std 提供了三个clock
		system_clock
		steady_clock
		high_resolution_clock
	*/
	std::cout << "system_clock: " << std::endl;
	PrintClockData<std::chrono::system_clock>();
	std::cout << "\nhigh_resolution_clock:" << std::endl;
	PrintClockData<std::chrono::high_resolution_clock>();
	std::cout << "\nsteady_clock: " << std::endl;
	PrintClockData<std::chrono::steady_clock>();

	// print the epoch of this system clock
	std::chrono::system_clock::time_point tp;
	std::cout << "epoch: " << AsString(tp) << std::endl;
	// print current time
	tp = std::chrono::system_clock::now();
	std::cout << "now: " << AsString(tp) << std::endl;

	return 0;
}