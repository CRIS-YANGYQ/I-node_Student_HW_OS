#ifndef _MODEAPPLICATION_H
#define _MODEAPPLICATION_H

#include <iostream>
#include <string>
#include <vector>
#include "authenticator.h"
#include "definedTime.h"
#include "homework.h"
#include "user.h"
#include "course.h"
#include "backup.h"
struct passing_massage{
	int err_code;
	std::vector<Request> meassage_request_vector;
	std::vector<homework> meassage_homework_vector;
	std::string meassage_grade_stats;
};
enum ErrorCodes {
    SUCCESS = 0,
    ERROR_CODE_DUPLICATE_USER = 1,
    ERROR_CODE_NO_USER = 2,
    ERROR_CODE_IDENTITY = 3,
    ERROR_CODE_NO_COURSE = 4,
    ERROR_CODE_NO_HOMEWORK = 5,
    ERROR_CODE_DUPLICATE_HOMEWORK = 6,
    ERROR_CODE_WRONG_HOMEWORK = 7,
    ERROR_CODE_DUPLICATE_REQUEST = 8,
	ERROR_CODE_WRONG_REQUEST = 9,
	ERROR_CODE_FAIL_DELETE_USER = 10,
	ERROR_CODE_FAIL_DELETE_USER_COURSE = 11,
	ERROR_CODE_FAIL_DELETE_HOMEWORK = 12,
	ERROR_CODE_FAIL_DELETE_REQUEST = 13
};
// #include "user_function.h"
// #include "basic.h"
/**
 * @function readHomeworkRequest
 * @brief 输入学生id 返回id权限的作业要求 
 *
 * @param student_id 学生id。
 */
void readHomeworkRequest(std::string student_id);
void readHomeworkRequestInFile(std::string student_id, fileSystem &fs);
/**
 * @function readHomeworkRequestInFileWithServer
 * @brief 从文件系统中读取学生提交的作业请求信息
 * 
 * @param student_id 学生id。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含作业请求信息的结构体。
 */
passing_massage readHomeworkRequestInFileWithServer(std::string student_id, fileSystem &myFS);

/**
 * @function submitHomework
 * @brief 输入学生id 提交id对应要求的作业
 *
 * @param student_id 学生id。
 */
void submitHomework(std::string student_id);
/**
 * @function submitHomeworkInFile
 * @brief 输入学生id 提交id对应要求的作业 
 * 
 * @param student_id 学生id。
*/
void submitHomeworkInFile(std::string student_id, fileSystem &fs);

/**
 * @function submitHomeworkInFileWithServer
 * @brief 提交学生作业到文件系统中
 * 
 * @param student_id 学生id。
 * @param submit_homework_vector 包含要提交的作业信息的向量。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含提交结果信息的结构体。
 */
passing_massage submitHomeworkInFileWithServer(std::string student_id, std::vector<homework> submit_homework_vector, fileSystem& myFS);

/**
 * @function assignHomeworkRequest
 * @brief 输入老师id 发布对应课程的作业要求
 *
 * @param teacher_id 老师id。
 */

void getGradeStats(std::string student_id);
void getGradeStatsInFile(std::string student_id, fileSystem &fs);
/**
 * @function getGradeStatsInFileWithServer
 * @brief 获取学生在文件系统中的成绩统计信息（所有作业）
 * 
 * @param student_id 学生id。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含成绩统计信息的结构体。
 */
passing_massage getGradeStatsInFileWithServer(std::string student_id, fileSystem& myFS);

/**
 * @function getGradeStatsInFileWithServer
 * @brief 获取学生在文件系统中的成绩统计信息（指定课程）
 * 
 * @param student_id 学生id。
 * @param course_id 课程id。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含成绩统计信息的结构体。
 */
passing_massage getGradeStatsInFileWithServer(std::string student_id, std::string course_id, fileSystem& myFS);


/**
 * @function getGradeStatsInFileWithServer
 * @brief 获取学生在文件系统中的成绩统计信息（指定课程和作业）
 * 
 * @param student_id 学生id。
 * @param course_id 课程id。
 * @param hw_id 作业id。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含成绩统计信息的结构体。
 */
passing_massage getGradeStatsInFileWithServer(std::string student_id, std::string course_id, std::string hw_id, fileSystem& myFS) ;
/**
 * @function assignHomeworkRequest
 * @brief 输入老师id 发布对应课程的作业要求
 *
 * @param teacher_id 老师id。
 */
void assignHomeworkRequest(std::string teacher_id);
void assignHomeworkRequestInFile(std::string teacher_id, fileSystem &fs);
/**
 * @function assignHomeworkRequestInFileWithServer
 * @brief 分配作业要求到文件系统中
 * 
 * @param teacher_id 教师id。
 * @param submit_request_vector 包含要分配的作业请求信息的向量。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含分配结果信息的结构体。
 */
passing_massage assignHomeworkRequestInFileWithServer(std::string teacher_id, std::vector<Request> submit_request_vector, fileSystem& myFS);
/**
 * @function acceptHomework
 * @brief 输入老师id 返回对应课程的已提交作业
 *
 * @param teacher_id 老师id。
 */
void acceptHomework(std::string teacher_id);
void acceptHomeworkInFile(std::string teacher_id, fileSystem &fs);
/**
 * @function acceptHomeworkInFileWithServer
 * @brief 教师在文件系统中接受学生提交的作业
 * 
 * @param teacher_id 教师id。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含接受作业结果信息的结构体。
 */
passing_massage acceptHomeworkInFileWithServer(std::string teacher_id, fileSystem &myFS) ;
/**
 * @function markHomework
 * @brief 输入老师id 提供更改作业分数的接口
 *
 * @param teacher_id 老师id。
 */
void markHomework(std::string teacher_id);
void markHomeworkInFile(std::string teacher_id, fileSystem &fs);
/**
 * @function markHomeworkInFileWithServer
 * @brief 教师在文件系统中批改学生提交的作业
 * 
 * @param teacher_id 教师id。
 * @param submit_homework_vector 包含要批改的作业信息的向量。
 * @param myFS 文件系统对象。
 * @return passing_massage_request 包含批改结果信息的结构体。
 */
passing_massage markHomeworkInFileWithServer(std::string teacher_id, std::vector<homework> submit_homework_vector, fileSystem& myFS);
/**
 * @brief 从文件系统和作业表中删除具有指定用户 ID 的学生
 * @param user_id 要删除的学生的用户 ID
 * @param myFS 文件系统对象
 * @return 包含操作结果的传递消息结构
 */
passing_massage deleteStudentInfileWithServer(std::string user_id, fileSystem& myFS);

/**
 * @brief 从文件系统和请求表中删除具有指定用户 ID 的教师
 * @param user_id 要删除的教师的用户 ID
 * @param myFS 文件系统对象
 * @return 包含操作结果的传递消息结构
 */
passing_massage deleteTeacherInfileWithServer(std::string user_id, fileSystem& myFS);
/**
 * @brief 从文件系统和作业表中删除具有指定用户 ID 和课程 ID 的学生的课程
 * @param user_id 要删除课程的学生的用户 ID
 * @param course_id 要删除的课程的课程 ID
 * @param myFS 文件系统对象
 * @return 包含操作结果的传递消息结构
 */
passing_massage deleteStudentCourseInfileWithServer(std::string user_id, std::string course_id, fileSystem& myFS);
/**
 * @brief 从文件系统和请求表中删除具有指定用户 ID 和课程 ID 的教师的课程
 * @param user_id 要删除课程的教师的用户 ID
 * @param course_id 要删除的课程的课程 ID
 * @param myFS 文件系统对象
 * @return 包含操作结果的传递消息结构
 */
passing_massage deleteTeacherCourseInfileWithServer(std::string user_id, std::string course_id, fileSystem& myFS);

/**
 * @brief 从文件系统和作业表中删除具有指定作业信息的作业
 * @param deleteHW 要删除的作业信息
 * @param myFS 文件系统对象
 * @return 包含操作结果的传递消息结构
 */
passing_massage deleteHomeworkInfileWithServer(homework deleteHW, fileSystem& myFS);

/**
 * @brief 从文件系统和请求表中删除具有指定请求信息的请求
 * @param deleteRequest 要删除的请求信息
 * @param myFS 文件系统对象
 * @return 包含操作结果的传递消息结构
 */
passing_massage deleteRequestInfileWithServer(Request deleteRequest, fileSystem& myFS);
/**
 * @function adminReadHomeworkReqest
 * @brief 管理员读取老师发布的作业要求
 *
 */
void adminReadHomeworkReqest();

/**
 * @function adminGetGradeStats
 * @brief 管理员读取学生的成绩统计量
 *
 */
void adminGetGradeStats();
/**
 * @function adminAcceptHomework
 * @brief 管理员接收目标学生作业
 *
 */
void adminAcceptHomework();
/**
 * @function adminMarkHomework
 * @brief 管理员批改目标学生作业
 *
 */
void adminMarkHomework();


#endif
