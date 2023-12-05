#include "basic.h"
std::istream& operator >> (std::istream& in, homework& hw) {
    in >> hw.submitTime >> hw.student_name >> hw.student_id >> hw.teacher_name >> hw.teacher_id
        >> hw.course_name >> hw.course_id >> hw.title >> hw.request_id >> hw.hw_id >> hw.content
        >> hw.isMarked >> hw.grade;
    return in;
}


std::ostream& operator << (std::ostream& out, const homework& hw) {
    out << "submit time: " << hw.submitTime
        << "student_name: " << hw.student_name << "\tstudent_id: " << hw.student_id
        << "\tteacher_name: " << hw.teacher_name << "\tteacher_id: " << hw.teacher_id
        << "\tcourse_name: " << hw.course_name << "\tcourse_id: " << hw.course_id
        << "\ttitle: " << hw.title << "\trequest_id: " << hw.request_id
        << "\thw_id" << hw.hw_id << "\tcontent: " << hw.content
        << "\tisMarked: " << hw.isMarked << "\tgrade: " << hw.grade << std::endl;
    return out;
}
std::istream& operator >> (std::istream& in, Request& req) {
    in >> req.submitTime >> req.teacher_name >> req.teacher_id
        >> req.course_name >> req.course_id >> req.title >> req.id >> req.content;
    return in;
}


std::ostream& operator << (std::ostream& out, const Request& req) {
    out << "submitTime: " << req.submitTime << "\nteacher_name: " << req.teacher_name
        << "\tteacher_id: " << req.teacher_id << "\tcourse_name: " << req.course_name
        << "\tcourse_id: " << req.course_id << "\ttitle: " << req.title
        << "\tid: " << req.id << "\tcontent: " << req.content;
    return out;
}