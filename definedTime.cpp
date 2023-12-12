// definedTime.cpp

#include"definedTime.h"
std::istream& operator >> (std::istream& in, __time& temp_time) {
	in >> temp_time.year >> temp_time.month >> temp_time.date >> temp_time.hour
		>> temp_time.minute >> temp_time.second;
	return in;
}

//������������
std::ostream& operator << (std::ostream& out, const __time& temp_time) {
	out << temp_time.year << " " << temp_time.month
		<< " " << temp_time.date << " " << temp_time.hour
		<< " " << temp_time.minute << " " << temp_time.second;
	return out;
}
bool operator<(const __time& lhs, const __time& rhs) {
	// 使用std::tie创建元组，确保比较时按照指定顺序比较各个字段
	return std::tie(lhs.year, lhs.month, lhs.date, lhs.hour, lhs.minute, lhs.second) <
		std::tie(rhs.year, rhs.month, rhs.date, rhs.hour, rhs.minute, rhs.second);
}
void __time::displayTimestamp() {
	std::cout << "\nyear: " << year << "\tmonth: " << month
		<< "\tdate: " << date << "\thour: " << hour
		<< "\tminute: " << minute << "\tsecond:" << second;
}
std::string __time::getTimeString(){
	std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << year << " "
        << std::setfill('0') << std::setw(2) << month << " "
        << std::setfill('0') << std::setw(2) << date << " "
        << std::setfill('0') << std::setw(2) << hour << " "
        << std::setfill('0') << std::setw(2) << minute << " "
        << std::setfill('0') << std::setw(2) << second;
    return ss.str();
}
// std::string getLocaltime() {
// 	// i.e. 2023:11:04 16:19:47
// 	time_t nowtime = time(0);
// 	struct tm p;
// 	localtime_s(&p, &nowtime);
// 	// Use a stringstream to format the date and time into a string
// 	std::stringstream ss;
// 	ss << std::setfill('0') << std::setw(4) << (p.tm_year + 1900) << " "
// 		<< std::setfill('0') << std::setw(2) << (p.tm_mon + 1) << " "
// 		<< std::setfill('0') << std::setw(2) << p.tm_mday << " "
// 		<< std::setfill('0') << std::setw(2) << p.tm_hour << " "
// 		<< std::setfill('0') << std::setw(2) << p.tm_min << " "
// 		<< std::setfill('0') << std::setw(2) << p.tm_sec << "\n";
// 	std::string res = ss.str();
// 	std::cout << res;
// 	return res;
// }
std::string getLocaltime() {
    // i.e. 2023:11:04 16:19:47
    time_t nowtime = time(0);
    struct tm p;

#ifdef _WIN32
    localtime_s(&p, &nowtime); // Windows
#else
    localtime_r(&nowtime, &p); // Linux and others
#endif

    // Use a stringstream to format the date and time into a string
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << (p.tm_year + 1900) << " "
       << std::setfill('0') << std::setw(2) << (p.tm_mon + 1) << " "
       << std::setfill('0') << std::setw(2) << p.tm_mday << " "
       << std::setfill('0') << std::setw(2) << p.tm_hour << " "
       << std::setfill('0') << std::setw(2) << p.tm_min << " "
       << std::setfill('0') << std::setw(2) << p.tm_sec << "\n";

    std::string res = ss.str();

    std::cout << res;
    return res;
}
__time localtime2__time(std::string dateTimeStr) {
	int year, month, day, hour, minute, second;
	__time nowtime;
	std::tm tm = {};
	std::istringstream iss(dateTimeStr);
	iss >> tm.tm_year >> tm.tm_mon >> tm.tm_mday >> tm.tm_hour >> tm.tm_min >> tm.tm_sec;

	if (!iss.fail()) {
		// Adjust the year and month since tm_year is years since 1900 and tm_mon is 0-based
		nowtime.year = tm.tm_year;
		nowtime.month = tm.tm_mon;
		nowtime.date = tm.tm_mday;
		nowtime.hour = tm.tm_hour;
		nowtime.minute = tm.tm_min;
		nowtime.second = tm.tm_sec;

		return nowtime;
	}
	else {
		std::cout << "Invalid date and time format" << std::endl;
		exit(0);
	}
}
