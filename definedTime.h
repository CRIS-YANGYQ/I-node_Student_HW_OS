/*
* @file definedTime.h
* @brief 定义时间结构体 __time，提供输入输出运算符重载和显示时间戳的功能，以及获取本地时间和将时间字符串转换为 __time 结构体的辅助函数。
*
* 该头文件声明了一个时间结构体 __time，包括以下功能：
*   - 重载输入运算符 >>，用于从流中读取时间数据到 __time 结构体
*   - 重载输出运算符 <<，用于将 __time 结构体的时间数据输出到流
*   - 显示时间戳的功能，通过 displayTimestamp() 函数实现
*   - 提供了一个获取本地时间的辅助函数 getLocaltime()
*   - 提供了一个将时间字符串转换为 __time 结构体的辅助函数 localtime2__time()
*
* 注意：为了避免头文件被多次包含，采用了 #ifndef、#define 和 #endif 的条件编译宏。
*
* @杨裕钦 2023/11/13 17:51
*/

#ifndef _DEFINEDTIME_H
#define _DEFINEDTIME_H

#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <string>
#include <tuple>
#include "parameters.h"

/**
 * @struct __time
 * @brief 表示年、月、日、时、分、秒的时间结构体
 */
struct __time {
    int year;
    int month;
    int date;
    int hour;
    int minute;
    int second;

    /**
     * @brief 重载输入运算符 >>，用于从流中读取时间数据到 __time 结构体
     * @param in 输入流
     * @param temp_time 要读取的时间结构体
     * @return 输入流
     */
    friend std::istream& operator >> (std::istream& in, __time& temp_time);

    /**
     * @brief 重载输出运算符 <<，用于将 __time 结构体的时间数据输出到流
     * @param out 输出流
     * @param temp_time 要输出的时间结构体
     * @return 输出流
     */
    friend std::ostream& operator << (std::ostream& out, const __time& temp_time);


    friend bool operator<(const __time& lhs, const __time& rhs);
    /**
     * @brief 显示时间戳的函数
     */
    void displayTimestamp();
    /**
     * @brief 输出时间字符串
     * @return std::string 字符串
     */
    std::string getTimeString();
};

/**
 * @brief 获取本地时间的辅助函数
 * @return 返回格式化的本地时间字符串
 */
std::string getLocaltime();

/**
 * @brief 将时间字符串转换为 __time 结构体的辅助函数
 * @param dateTimeStr 待转换的时间字符串，格式为"YYYY MM DD HH MM SS"
 * @return 转换后的 __time 结构体
 */
__time localtime2__time(std::string dateTimeStr);

#endif // !_DEFINEDTIME_H
