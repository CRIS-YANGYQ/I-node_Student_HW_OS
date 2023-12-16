#ifndef BASIC_H
#define BASIC_H
#include <iostream>
#include "definedTime.h"
/*
* 结构体 homework 包括以下成员：
*   - __time submitTime：作业提交时间
*   - std::string student_name：学生姓名
*   - std::string student_id：学生ID
*   - std::string teacher_name：教师姓名
*   - std::string teacher_id：教师ID
*   - std::string course_name：课程名称
*   - std::string course_id：课程ID
*   - std::string title：作业标题
*   - std::string request_id：请求ID
*   - std::string hw_id：作业ID
*   - std::string content：作业内容
*   - bool isMarked：批改状态（true表示已批改，false表示未批改）
*   - int grade：成绩
*
* 结构体 Request 包括以下成员：
*   - __time submitTime：请求提交时间
*   - std::string teacher_name：教师姓名
*   - std::string teacher_id：教师ID
*   - std::string course_name：课程名称
*   - std::string course_id：课程ID
*   - std::string title：请求标题
*   - std::string id：请求ID
*   - std::string content：请求内容
*/
struct homework {
    __time submitTime;
	std::string student_name;
	std::string student_id;
	std::string teacher_name;
	std::string teacher_id;
	std::string course_name;
	std::string course_id;
	std::string title;
	std::string request_id;
	std::string hw_id;
	std::string content;
	int isMarked = 0;
	int grade = 0;
    friend std::istream& operator >> (std::istream& in, homework& hw) {
        in >> hw.submitTime >> hw.student_name >> hw.student_id >> hw.teacher_name >> hw.teacher_id
            >> hw.course_name >> hw.course_id >> hw.title >> hw.request_id >> hw.hw_id >> hw.content
            >> hw.isMarked >> hw.grade;
        return in;
    }


    friend std::ostream& operator << (std::ostream& out, const homework& hw) {
        out << "submit time: " << hw.submitTime<<std::endl
            << "student_name: " << hw.student_name << "\tstudent_id: " << hw.student_id
            << "\tteacher_name: " << hw.teacher_name << "\tteacher_id: " << hw.teacher_id
            << "\tcourse_name: " << hw.course_name << "\tcourse_id: " << hw.course_id
            << "\ttitle: " << hw.title << "\trequest_id: " << hw.request_id
            << "\thw_id: " << hw.hw_id << "\tcontent: " << hw.content
            << "\tisMarked: " << hw.isMarked << "\tgrade: " << hw.grade << std::endl;
        return out;
    }


};

struct Request {
	__time submitTime;
	std::string teacher_name;
	std::string teacher_id;
	std::string course_name;
	std::string course_id;
	std::string title;
	std::string id;
	std::string content;
    friend std::istream& operator >> (std::istream& in, Request& req)  {
		in >> req.submitTime >> req.teacher_name >> req.teacher_id
			>> req.course_name >> req.course_id >> req.title >> req.id >> req.content;
		return in;
	}


    friend std::ostream& operator << (std::ostream& out, const Request& req) {
		out << "submitTime: " << req.submitTime << "\nteacher_name: " << req.teacher_name
			<< "\tteacher_id: " << req.teacher_id << "\tcourse_name: " << req.course_name
			<< "\tcourse_id: " << req.course_id << "\ttitle: " << req.title
			<< "\tid: " << req.id << "\tcontent: " << req.content;
		return out;
	}


};
// 通用的拼接查询字符串的函数模板 Homework and Request
template<typename T>
std::vector<std::string> concatenateStrings2Init(const T& info) {
	static_assert(
        (std::is_same<T, homework>::value) ||
        (std::is_same<T, Request>::value),
        "concatenateStrings can only be used with homework or Request"
    );


    std::vector<std::string> result;

    std::string firstString;
    if constexpr (std::is_same<T, homework>::value) {
        firstString = "/user/" + info.student_id;
    } else if constexpr (std::is_same<T, Request>::value) {
        firstString = "/user/" + info.teacher_id;
    }

    result.push_back(firstString);

    std::string secondString = firstString + "/" + info.course_id;
    result.push_back(secondString);

	// std::string thirdString;
    // if constexpr (std::is_same<T, homework>::value) {
    //     thirdString = secondString + "/" + info.hw_id ;
    // } else if constexpr (std::is_same<T, Request>::value) {
    //     thirdString = secondString + "/" + info.id;
    // }
    // //thirdString = secondString + "/" + (std::is_same<T, homework>::value ? info.hw_id : info.id);
    // result.push_back(thirdString);

    return result;
}
template<typename T>
std::string pathStrings2Submit(const T& info) {
	static_assert(
        (std::is_same<T, homework>::value) ||
        (std::is_same<T, Request>::value),
        "concatenateStrings can only be used with homework or Request"
    );
    std::string firstString;
    if constexpr (std::is_same<T, homework>::value) {
        firstString = "/user/" + info.student_id;
    } else if constexpr (std::is_same<T, Request>::value) {
        firstString = "/user/" + info.teacher_id;
    }
    std::string secondString = firstString + "/" + info.course_id;
	std::string thirdString;
    if constexpr (std::is_same<T, homework>::value) {
        thirdString = secondString + "/" + info.hw_id ;
    } else if constexpr (std::is_same<T, Request>::value) {
        thirdString = secondString + "/" + info.id;
    }

    return thirdString;
}
#endif