#ifndef USER_FUNCTION_H
#define USER_FUNCTION_H

#include <iostream>
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include "basic.h"
#include "definedTime.h"



//学生提交作业的函数
bool submit_homework(homework,fileSystem &fs);
/*
传入一个homework对象，除了isMarked和grade，其他属性都要有
*/

//老师获取学生作业的函数
homework get_stu_homework(homework,fileSystem &fs);
/*
传入一个homework对象，除了isMarked和grade,要有学生id，课程id和作业id,获取这个struct
*/

//模糊查询学生作业
std::vector<homework> get_stu_homework_list(std::string,std::string,fileSystem &fs);
/*
只用传入学生id和课程id，就可以得到该学生下所有作业的vector
*/

//老师上传作业要求
bool submit_request(Request,fileSystem &fs);
/*
传入作业要求，struct要有所有内容
*/

//查看作业要求
Request get_request(Request,fileSystem &fs);
/*
传入一个homework对象,有三个id，获取老师id，课程id和作业id,获取这个struct
*/

//模糊查询教师课程全部作业要求
std::vector<Request> get_stu_request_list(std::string teacher_id,std::string course_id,fileSystem &fs);
/*
只用传入教师id和课程id，就可以得到该教师下所有要求的vector
*/





#endif