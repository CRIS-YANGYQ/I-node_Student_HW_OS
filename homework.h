/*
* @file homework.h
* @brief 定义作业结构体 Homework、请求结构体 Request 以及作业集合类 homeworkSet 和请求集合类 RequestSet，提供对作业和请求的输入输出运算符重载、集合的初始化、数据库读写等功能。
* 基于 C++11 标准。
*
* 该头文件声明了以下结构体和类：
*   - 结构体 Homework，包括作业的提交时间、学生姓名、学生ID、教师姓名、教师ID、课程名称、课程ID、标题、请求ID、作业ID、内容、批改状态、成绩等信息
*   - 结构体 Request，包括请求的提交时间、教师姓名、教师ID、课程名称、课程ID、标题、ID、内容等信息
*   - 类 HomeworkSet，用于管理作业集合，包括作业集合的初始化、数据库读写、显示作业信息等功能
*   - 类 RequestSet，用于管理请求集合，包括请求集合的初始化、数据库读写、显示请求信息等功能
*
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
*
* 类 homeworkSet 包括以下成员：
*   - int capacity：作业集合容量
*   - int size：作业集合当前大小
*   - std::vector<homework> homeworkVector：存储作业信息的向量
*
* 类 RequestSet 包括以下成员：
*   - int capacity：请求集合容量
*   - int size：请求集合当前大小
*   - std::vector<Request> requestVector：存储请求信息的向量
*
* @杨裕钦 2023/11/13 19:12
*/
#ifndef _HOMEWORK_H
#define _HOMEWORK_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>     // 文件的输入输出流操作
#include <iomanip>     // 对齐输出
#include <vector>
// #include "definedTime.h"
// #include "file.h"
#include "basic.h"
#include "user_function.h"
#include "parameters.h"
// /**
//  * @struct homework
//  * @brief 作业信息结构体，包括作业的提交时间、学生姓名、学生ID、教师姓名、教师ID、课程名称、课程ID、标题、请求ID、作业ID、内容、批改状态、成绩等信息
//  */
// struct homework {
// 	__time submitTime;
// 	std::string student_name;
// 	std::string student_id;
// 	std::string teacher_name;
// 	std::string teacher_id;
// 	std::string course_name;
// 	std::string course_id;
// 	std::string title;
// 	std::string request_id;
// 	std::string hw_id;
// 	std::string content;
// 	bool isMarked = false;
// 	int grade = 0;

// 	/**
// 	 * @brief 重载输入运算符 >>，用于从流中读取作业信息到 homework 结构体
// 	 * @param in 输入流
// 	 * @param hw 要读取的作业信息结构体
// 	 * @return 输入流
// 	 */
// 	friend std::istream& operator >> (std::istream& in, homework& hw);

// 	/**
// 	 * @brief 重载输出运算符 <<，用于将 homework 结构体的作业信息输出到流
// 	 * @param out 输出流
// 	 * @param hw 要输出的作业信息结构体
// 	 * @return 输出流
// 	 */
// 	friend std::ostream& operator << (std::ostream& out, const homework& hw);
// };

// /**
//  * @struct Request
//  * @brief 请求信息结构体，包括请求的提交时间、教师姓名、教师ID、课程名称、课程ID、标题、ID、内容等信息
//  */
// struct Request {
// 	__time submitTime;
// 	std::string teacher_name;
// 	std::string teacher_id;
// 	std::string course_name;
// 	std::string course_id;
// 	std::string title;
// 	std::string id;
// 	std::string content;

// 	/**
// 	 * @brief 重载输入运算符 >>，用于从流中读取请求信息到 Request 结构体
// 	 * @param in 输入流
// 	 * @param req 要读取的请求信息结构体
// 	 * @return 输入流
// 	 */
// 	friend std::istream& operator >> (std::istream& in, Request& req);

// 	/**
// 	 * @brief 重载输出运算符 <<，用于将 Request 结构体的请求信息输出到流
// 	 * @param out 输出流
// 	 * @param req 要输出的请求信息结构体
// 	 * @return 输出流
// 	 */
// 	friend std::ostream& operator << (std::ostream& out, const Request& req);
// };


/**
 * @class homeworkSet
 * @brief 作业集合类，用于管理作业集合，包括作业集合的初始化、数据库读写、显示作业信息等功能
 */
class homeworkSet {
private:
	int capacity;                     // 作业集合容量
	int size = 0;                     // 作业集合当前大小
	std::vector<homework> homeworkVector;  // 存储作业信息的向量
public:
	std::vector<homework> infoVector; // 存储作业索引信息student_id teacher_id course_id hw_id 其余为空

	/**
	 * @brief 构造函数，初始化作业集合容量
	 * @param capacity 作业集合的容量
	 */
	homeworkSet(int capacity);
	/**
	 * @brief 构造函数，初始化作业集合容量和作业信息向量
	 * @param capacity 作业集合的容量
	 * @param homeworkVector 作业信息向量
	 */
	homeworkSet(int capacity, std::vector<homework> homeworkVector);

	/**
	 * @brief 析构函数，释放资源
	 */
	~homeworkSet();
	std::vector<homework> getHomeworkVector();
	// 利用hw初始化info
	homework initInfo(homework origin);//student_id teacher_id course_id hw_id

	// 利用hwvector初始化infovector
	void initInfoVector();

	// 存储必要info到csv
	void updateInfoDatabase(std::string destinationPath, std::string databaseName);

	// 读取必要info从csv
	void loadInfoDatabase(std::string destinationPath, std::string databaseName);
	/**
	 * @brief 初始化作业信息结构体
	 * @param elements 包含作业信息的字符串向量
	 * @return 初始化后的作业信息结构体
	 */
	homework initHomework(std::vector<std::string> elements);

	/**
	 * @brief 初始化数据库，将作业信息写入文件
	 * @param destinationPath 目标路径
	 * @param databaseName 数据库文件名
	 */
	void initDatabase(std::string destinationPath, std::string databaseName);

	/**
	 * @brief 从数据库文件中读取作业信息
	 * @param destinationPath 目标路径
	 * @param databaseName 数据库文件名
	 */
	void readDatabase(std::string destinationPath, std::string databaseName);
	/**
	 * @brief Updates the database with homework information.
	 *
	 * @param destinationPath The path where the database will be saved.
	 * @param databaseName The name of the database file.
	 * @return void
	 */
	void updateDatabase(std::string destinationPath, std::string databaseName);
	/**
	 * @brief Add the database with homework vector.
	 *
	 * @param homework_vector The vector to be added to the database.
	 * @param destinationPath The path where the database will be saved.
	 * @param databaseName The name of the database file.
	 * @return void
	 */
	void appendDatabase(std::vector<homework> homework_vector, std::string destinationPath, std::string databaseName);
	/**
	 * @brief 使用其他vector对已存在的内容进行更新
	 *
	 * @param targetVector 目标更改。
	 * @return void
	 */
	void writeVector(const std::vector<homework> &targetVector);
	/**
	 * @brief Displays the homework vector.
	 *
	 * @return void
	 */

	void displayHomeworkVector() ;

	/**
	 * @brief get grade statistics for a given student.
	 *
	 * @param student_id The ID of the student.
	 * @return std::string The grade statistics.
	 */
	std::string getGradeStats(std::string student_id);


	/**
	 * @brief get grade statistics for a given student.
	 *
	 * @param student_id The ID of the student.
	 * @param course_id The ID of the course.
	 * @return std::string The grade statistics.
	 */
	std::string getGradeStats(std::string student_id, std::string course_id);

	
	/**
	 * @brief Displays grade statistics for a given student.
	 *
	 * @param student_id The ID of the student.
	 * @return void
	 */
	void displayGradeStats(std::string student_id);

	/**
	 * @brief Displays grade statistics for a given student and a given course.
	 *
	 * @param student_id The ID of the student.
	 * @param course_id The ID of the course.
	 * @return void
	 */
	void displayGradeStats(std::string student_id, std::string course_id);
	/**
	 * @brief Displays grade statistics for a given vector.
	 *
	 * @param std::vector<homework> homework_course_vector
	 * @return void
	 */
	void displayGradeStats(std::vector<homework> homework_course_vector);

	/**
	 * @brief Searches for homework entries based on the course ID.
	 *
	 * @param course_id The ID of the course.
	 * @return vector<homework> A vector containing homework entries for the specified course.
	 */
	std::vector<homework> searchHomeworkByCourseID(std::string course_id);
	/**
	 * @brief Searches for homework entries based on the teacher ID.
	 *
	 * @param teacher_id The ID of the teacher.
	 * @return vector<homework> A vector containing homework entries for the specified teacher.
	 */
	std::vector<homework> searchHomeworkByTeacherID(std::string teacher_id);
	/**
	 * @brief Searches for homework entries based on the student ID.
	 *
	 * @param student_id The ID of the student.
	 * @return vector<homework> A vector containing homework entries for the specified student.
	 */
	std::vector<homework> searchHomeworkByStudentID(std::string student_id);
	/**
	 * @brief Searches for homework entries based on the hw ID.
	 *
	 * @param homework_id The ID of the hw.
	 * @return vector<homework> A vector containing homework entries for the specified student.
	 */
	std::vector<homework> searchHomeworkByHomeworkID(std::string homework_id);
	/**
	 * @brief 根据标题搜索请求条目suoyin。
	 *
	 * @param request_id request的ID。
	 * @return std::vector<Request> 包含指定标题的请求条目的向量。
	 */
	int searchRequestIndeByHomeworkID(std::string request_id);
	/**
	 * @brief Checks if a given homework ID exists in the database.
	 *
	 * @param homework_id The ID of the homework.
	 * @return bool Returns true if the homework ID exists, false otherwise.
	 */
	bool isHomeworkIDexists(std::string homework_id);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FILE SEARCH~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 
	/**
	 * @brief Searches for homework entries based on the query in i-node file.
	 *
	 * @param query string
	 * @return vector<homework> A vector containing homework entries satisfying the query.
	 */
	homework queryHomeworkInFile(homework queryHomework, fileSystem &fs);

	std::vector<homework> queryCourseHomeworkInFile(std::string student_id, std::string course_id, fileSystem &fs);
};

	/**
	 * @brief Manages a set of requests, including initializing, updating, and searching.
	 */
class requestSet {
private:
	int capacity; // 容量
	int size = 0; // 大小
	std::vector<Request> requestVector; // 请求向量

public:
	std::vector<Request>infoVector;
	// 利用hw初始化info
	Request initInfo(Request origin);//teacher_id course_id hw_id

	// 利用hwvector初始化infovector
	void initInfoVector();

	std::vector<Request> getRequestVector();

	/**
	 * @brief requestSet类的构造函数。
	 *
	 * @param capacity 请求集的初始容量。
	 */
	requestSet(int capacity = 0);

	/**
	 * @brief 带有指定容量和初始向量的requestSet类构造函数。
	 *
	 * @param capacity 请求集的初始容量。
	 * @param requestVector Request对象的初始向量。
	 */
	requestSet(int capacity, std::vector<Request> requestVector);

	/**
	 * @brief requestSet类的析构函数。
	 */
	~requestSet();

	/**
	 * @brief 基于元素向量初始化Request对象。
	 *
	 * @param elements 包含初始化Request对象所需元素的向量。
	 * @return Request 初始化后的Request对象。
	 */
	Request initRequest(std::vector<std::string> elements);

	/**
	 * @brief 使用预定义数据初始化数据库。
	 *
	 * @param destinationPath 数据库将保存的路径。
	 * @param databaseName 数据库文件的名称。
	 * @return void
	 */
	void initDatabase(std::string destinationPath, std::string databaseName);

	/**
	 * @brief 从数据库文件中读取数据并填充请求向量。
	 *
	 * @param destinationPath 数据库所在的路径。
	 * @param databaseName 数据库文件的名称。
	 * @return void
	 */
	void readDatabase(std::string destinationPath, std::string databaseName);

	/**
	 * @brief 使用请求信息更新数据库。
	 *
	 * @param destinationPath 数据库将保存的路径。
	 * @param databaseName 数据库文件的名称。
	 * @return void
	 */
	void updateDatabase(std::string destinationPath, std::string databaseName);

	/**
	 * @brief Add the database with request vector.
	 *
	 * @param request_vector The vector to be added to the database.
	 * @param destinationPath The path where the database will be saved.
	 * @param databaseName The name of the database file.
	 * @return void
	 */
	void appendDatabase(std::vector<Request> request_vector, std::string destinationPath, std::string databaseName);

	/**
	 * @brief 使用其他vector对已存在的内容进行更新
	 *
	 * @param targetVector 目标更改。
	 * @return void
	 */
	void writeVector(const std::vector<Request> &targetVector);
	/**
	 * @brief 显示请求向量。
	 *
	 * @return void
	 */
	void displayRequestsVector();

	/**
	 * @brief 根据请求ID搜索请求条目。
	 *
	 * @param id 请求的ID。
	 * @return std::vector<Request> 包含指定ID的请求条目的向量。
	 */
	std::vector<Request> searchRequestByID(std::string id);

	/**
	 * @brief 根据课程ID搜索请求条目。
	 *
	 * @param course_id 课程的ID。
	 * @return std::vector<Request> 包含指定课程的请求条目的向量。
	 */
	std::vector<Request> searchRequestByCourseID(std::string course_id);

	/**
	 * @brief 根据标题搜索请求条目。
	 *
	 * @param teacher_id 老师的ID。
	 * @return std::vector<Request> 包含指定标题的请求条目的向量。
	 */
	std::vector<Request> searchRequestBytitle(std::string title);

	/**
	 * @brief 根据教师ID请求条目。
	 *
	 * @param request_id request的ID。
	 * @return std::vector<Request> 包含指定标题的请求条目的向量。
	 */
	std::vector<Request> searchRequestByTeacherID(std::string teacher_id);
	/**
	 * @brief 根据作业要求ID请求条目。
	 *
	 * @param request_id request的ID。
	 * @return std::vector<Request> 包含指定标题的请求条目的向量。
	 */
	std::vector<Request> searchRequestByRequestID(std::string request_id);
	/**
	 * @brief 根据标题搜索请求条目索引。
	 *
	 * @param request_id request的ID。
	 * @return int 目标req在vector中的索引。
	 */
	int searchRequestIndeByRequestID(std::string request_id);



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FILE SEARCH~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 
	/**
	 * @brief Searches for Request entries based on the query in i-node file.
	 *
	 * @param query string
	 * @return vector<Request> A vector containing Request entries satisfying the query.
	 */
	Request queryRequestInFile(Request queryRequest, fileSystem &fs);

	std::vector<Request> queryCourseRequestInFile(std::string teacher_id, std::string course_id, fileSystem &fs);
};
#endif
