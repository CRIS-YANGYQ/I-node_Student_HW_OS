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
struct passing_massage{
	int err_code;
	std::vector<Request> meassage_request_vector;
	std::vector<homework> meassage_homework_vector;
	std::string meassage_grade_stats;
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
