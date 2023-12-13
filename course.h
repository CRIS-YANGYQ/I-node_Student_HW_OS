/*
* @file Course.h
* @brief 定义课程结构体 Course、选课关系结构体 Course2User 以及选课表类 Course2UserTable，
*  提供对课程和选课关系的输入输出运算符重载、选课表的初始化、数据库读写等功能。
*
* 该头文件声明了以下结构体和类：
*   - 结构体 Course，包括课程的名称、ID、教师姓名和教师ID等信息
*   - 结构体 Course2User，包括用户类型、课程名称、课程ID、用户姓名和用户ID等信息，以及重载输入输出运算符
*   - 类 Course2UserTable，用于管理选课关系表，包括选课关系表的初始化、数据库读写、显示选课关系信息等功能
*
* 结构体 Course 包括以下成员：
*   - std::string name：课程名称
*   - std::string id：课程ID
*   - std::string teacher_name：教师姓名
*   - std::string teacher_id：教师ID
*
* 结构体 Course2User 包括以下成员：
*   - int kind：用户类型（0 学生 1 教师）
*   - std::string course_name：课程名称
*   - std::string course_id：课程ID
*   - std::string user_name：用户姓名
*   - std::string user_id：用户ID
*
* 类 Course2UserTable 包括以下成员：
*   - int capacity：选课关系表容量
*   - int size：选课关系表当前大小
*   - std::vector<Course2User> selectionVector：存储选课关系信息的向量
*
* @杨裕钦 2023/11/13 17:51
*/
#ifndef _COURSE_H
#define _COURSE_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>     // 文件的输入输出流操作
#include <iomanip>     // 对齐输出
#include <vector>
#include "parameters.h"

/**
 * @struct Course
 * @brief 课程信息结构体，包括课程的名称、ID、教师姓名和教师ID等信息
 */
struct Course {
	std::string name;
	std::string id;
	std::string teacher_name;
	std::string teacher_id;
};

/**
 * @struct Course2User
 * @brief 选课关系结构体，包括用户类型、课程名称、课程ID、用户姓名和用户ID等信息
 */
struct Course2User {
	int kind; // 0 学生 1 教师
	std::string course_name;
	std::string course_id;
	std::string user_name;
	std::string user_id;

	/**
	 * @brief 重载输入运算符 >>，用于从流中读取选课关系信息到 Course2User 结构体
	 * @param in 输入流
	 * @param course2user 要读取的选课关系信息结构体
	 * @return 输入流
	 */
	friend std::istream& operator >> (std::istream& in, Course2User& course2user);

	/**
	 * @brief 重载输出运算符 <<，用于将 Course2User 结构体的选课关系信息输出到流
	 * @param out 输出流
	 * @param course2user 要输出的选课关系信息结构体
	 * @return 输出流
	 */
	friend std::ostream& operator << (std::ostream& out, const Course2User& course2user);
};

/**
 * @class Course2UserTable
 * @brief 选课关系表类，用于管理选课关系信息，包括选课关系表的初始化、数据库读写、显示选课关系信息等功能
 */
class Course2UserTable {
private:
	int capacity;
	int size = 0;
	std::vector<Course2User> selectionVector;

public:
	std::vector<Course2User> infoVector;

    /**
     * @brief 构造函数，用于初始化选课关系表
     * @param capacity 选课关系表容量
     */ 
	// 存储必要信息
	Course2User initInfo(Course2User origin);
	// 利用rsel_vector初始化infovector
	void initInfoVector();
	/**
	 * @brief 构造函数，初始化选课关系表的容量
	 * @param capacity 选课关系表的容量
	 */
	Course2UserTable(int capacity);

	/**
	 * @brief 带参数的构造函数，初始化选课关系表的容量和选课关系信息向量
	 * @param capacity 选课关系表的容量
	 * @param homeworkVector 选课关系信息向量
	 */
	Course2UserTable(int capacity, std::vector<Course2User> homeworkVector);

	/**
	 * @brief 析构函数，释放资源
	 */
	~Course2UserTable();

	/**
	 * @brief 初始化选课关系信息结构体
	 * @param elements 包含选课关系信息的字符串向量
	 * @return 初始化后的选课关系信息结构体
	 */
	Course2User initCourse2User(std::vector<std::string> elements);

	/**
	 * @brief 初始化数据库，将选课关系信息写入文件
	 * @param destinationPath 目标路径
	 * @param databaseName 数据库文件名
	 */
	void initDatabase(std::string destinationPath, std::string databaseName);

	/**
	 * @brief 从数据库文件中读取选课关系信息
	 * @param destinationPath 目标路径
	 * @param databaseName 数据库文件名
	 */
	void readDatabase(std::string destinationPath, std::string databaseName);
	void updateInfoDatabase(std::string destinationPath, std::string databaseName);
	void updateDatabase(std::string destinationPath, std::string databaseName);
	/**
	 * @brief 将 Course2User 结构体的数据追加到指定路径和数据库文件名的文件中
	 * @param new_selection 要添加的 Course2User 数据
	 * @param destinationPath 目标路径
	 * @param databaseName 数据库文件名
	 * @return 如果成功将数据追加到文件中，则返回 true；否则返回 false
	 */
	bool addDatabase(const Course2User& new_selection, std::string destinationPath, std::string databaseName);
	/**
	 * @brief Checks if a given user ID exists in the database.
	 *
	 * @param user_id The ID of the user.
	 * @return bool Returns true if the user ID exists, false otherwise.
	 */
	bool isUserIDexists(std::string user_id);
	/**
	 * @brief Checks if a given selection ID exists in the database.
	 *
	 * @param user_id The ID of the user.
	 * @param course_id The ID of the course
	 * @return bool Returns true if the selection ID exists, false otherwise.
	 */
	bool isSelectionIDexists(std::string user_id, std::string course_id);
	/**
	 * @brief 显示选课关系信息向量
	 */
	void displaySelectionVector();
	/**
	 * @brief 根据用户ID删除选课关系信息
	 * @param userID 用户ID
	 * @return 如果成功删除返回true，否则返回false
	 */
	bool deleteSelectionFromVector(std::string userID) ;
	/**
	 * @brief 根据用户ID和课程ID删除选课关系信息
	 * @param userID 用户ID
	 * @param courseID 课程ID
	 * @return 如果成功删除返回true，否则返回false
	 */
	bool deleteSelectionFromVector(std::string userID, std::string courseID);

	/**
	 * @brief 根据选课关系元素删除选课关系信息
	 * @param selection_element 选课关系元素
	 * @return 如果成功删除返回true，否则返回false
	 */
	bool deleteSelectionFromVector(Course2User selection_element);

	/**
	 * @brief 根据课程ID搜索选课关系信息
	 * @param user_ID 用户ID
	 * @param course_id 课程ID
	 * @return 包含搜索结果的选课关系信息向量
	 */
	std::vector<Course2User> searchSelectionByUserIDandCourseID(std::string user_ID, std::string course_id);
	/**
	 * @brief 根据课程ID搜索选课关系信息
	 * @param course_id 课程ID
	 * @return 包含搜索结果的选课关系信息向量
	 */
	std::vector<Course2User> searchSelectionByCourse(std::string course_id);

	/**
	 * @brief 根据用户ID搜索选课关系信息
	 * @param user_id 用户ID
	 * @return 包含搜索结果的选课关系信息向量
	 */
	std::vector<Course2User> searchSelectionByUserId(std::string user_id);

	std::string searchTeacherIDByCourseID(std::string course_id);

	std::vector<std::string> searchStudentIDByCourseID(std::string course_id);
};

#endif // _COURSE_H

