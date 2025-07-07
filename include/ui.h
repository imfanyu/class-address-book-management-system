/**
* @file ui.h
 * @brief 班级通讯录管理系统用户界面声明
 * @author 樊彧（电气241）、何嘉萌（电气241）、李一诺（电气241）
 * @date 2025-06-28
 *
 * @details 定义用户界面相关函数声明，包括：
 *         - 信息显示函数
 *         - 用户交互界面函数
 *
 * 北京建筑大学 智能科学与技术学院
 * C语言程序设计课程设计 - 班级通讯录管理系统
 * 仅供课程设计使用，未经许可不得外传
 */

#ifndef UI_H
#define UI_H

#include "student.h"

void print_class_info(const Class *cls);
void print_all_students(const Class *cls);
void print_student(const Student *stu);

void show_add_student_ui(Class *cls);
void show_delete_student_ui(Class *cls);
void show_modify_student_ui(Class *cls);
void show_search_student_ui(const Class *cls);
void manage_class_menu();

#endif //UI_H