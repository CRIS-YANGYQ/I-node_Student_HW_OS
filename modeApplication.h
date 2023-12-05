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
 * @function submitHomework
 * @brief 输入学生id 提交id对应要求的作业
 *
 * @param student_id 学生id。
 */
void submitHomework(std::string student_id);
void submitHomeworkInFile(std::string student_id, fileSystem &fs);
/**
 * @function assignHomeworkRequest
 * @brief 输入老师id 发布对应课程的作业要求
 *
 * @param teacher_id 老师id。
 */

void getGradeStats(std::string student_id);
void getGradeStatsInFile(std::string student_id, fileSystem &fs);

/**
 * @function assignHomeworkRequest
 * @brief 输入老师id 发布对应课程的作业要求
 *
 * @param teacher_id 老师id。
 */
void assignHomeworkRequest(std::string teacher_id);
void assignHomeworkRequestInFile(std::string teacher_id, fileSystem &fs);

/**
 * @function acceptHomework
 * @brief 输入老师id 返回对应课程的已提交作业
 *
 * @param teacher_id 老师id。
 */
void acceptHomework(std::string teacher_id);
void acceptHomeworkInFile(std::string teacher_id, fileSystem &fs);

/**
 * @function markHomework
 * @brief 输入老师id 提供更改作业分数的接口
 *
 * @param teacher_id 老师id。
 */
void markHomework(std::string teacher_id);
void markHomeworkInFile(std::string teacher_id, fileSystem &fs);
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
