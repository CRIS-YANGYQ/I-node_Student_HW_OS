#include<iostream>
#include"file.h"
#include <stdio.h>
#include <string.h>
#include"user_function.h"
#include<vector>

fileSystem globel_fs;
void test()
{
    //提交获取作业内容测试
    homework myHomework;
    myHomework.student_name = "John Doe";
    myHomework.student_id = "s123456";
    myHomework.teacher_name = "Dr. Smith";
    myHomework.teacher_id = "t789012";
    myHomework.course_name = "Computer Science";
    myHomework.course_id = "CS101";
    myHomework.title = "Homework Assignment 1";
    myHomework.request_id = "R123";
    myHomework.hw_id = "HW001";
    myHomework.content = "Write a program that...";
    globel_fs.createDirectory("/user/s123456","admin");
    globel_fs.createDirectory("/user/s123456/CS101","admin");
    
    submit_homework(myHomework,globel_fs);
    
    myHomework.hw_id = "HW002";
    submit_homework(myHomework,globel_fs);
    myHomework.hw_id = "HW003";
    submit_homework(myHomework,globel_fs);
    homework myHk;
    myHk.student_id = "s123456";
    myHk.teacher_id = "t789012";
    myHk.course_id = "CS101";
    myHk.hw_id = "HW001";
    myHk = get_stu_homework(myHomework,globel_fs);
    std::cout << myHk.content<<std::endl;
    std::vector<homework> stu_homeworks;
    stu_homeworks = get_stu_homework_list("s123456","CS101",globel_fs);
    for (int i =0;i<stu_homeworks.size();i++)
    {
        std::cout<<"学生id:"<<stu_homeworks[i].student_id<<"作业id:"<<stu_homeworks[i].hw_id<<std::endl;
    }
    //---------------------------------------------------------------------------------------------------
    //提交和获取作业要求测试
    globel_fs.createDirectory("/user/T123","admin");
    globel_fs.createDirectory("/user/T123/CS101","admin");


    Request myRequest;
    myRequest.teacher_name = "John Doe";
    myRequest.teacher_id = "T123";
    myRequest.course_name = "Computer Science";
    myRequest.course_id = "CS101";
    myRequest.title = "Request Title";
    myRequest.id = "R001";
    myRequest.content = "This is the content of the request.";
    submit_request(myRequest,globel_fs);
    myRequest.id = "R002";
    submit_request(myRequest,globel_fs);
    myRequest.id = "R003";
    submit_request(myRequest,globel_fs);

    Request hwRequest;
    hwRequest.teacher_id = "T123";
    hwRequest.course_id = "CS101";
    hwRequest.id = "R001";
    hwRequest = get_request(hwRequest,globel_fs);

    std::vector<Request> request_list;
    request_list = get_stu_request_list("T123","CS101",globel_fs);
    for (int i =0;i<request_list .size();i++)
    {
        std::cout<<"教师id:"<<request_list[i].teacher_id<<"需求id:"<<request_list[i].id<<std::endl;
    }
    
    globel_fs.displayDirectory("/");
    //globel_fs.deleteFile("/user/T123/CS101/R001/homework_request","admin");
    globel_fs.deleteDirectory("/user/T123","admin");
    globel_fs.displayDirectory("/");
	
     if(homework_id_exist("HW001",globel_fs))
     {
	     printf("作业存在\n");
     }
}



int main()
{
    test();
    return 0;
}