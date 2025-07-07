/**
* @file student.h
 * @brief 班级通讯录管理系统核心数据结构定义
 * @author 樊彧（电气241）、何嘉萌（电气241）、李一诺（电气241）
 * @date 2025-06-28
 *
 * @details 定义系统核心数据结构，包括：
 *         - 学生/班级数据结构
 *         - 文件操作接口声明
 *         - 全局变量声明
 *
 * 北京建筑大学 智能科学与技术学院
 * C语言程序设计课程设计 - 班级通讯录管理系统
 * 仅供课程设计使用，未经许可不得外传
 */

#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#define MAX_NAME_LEN 21
#define MAX_PHONE_LEN 16
#define MAX_ADDR_LEN 51
#define MAX_EMAIL_LEN 31
#define MAX_STUDENTS 100
#ifndef DATA_DIR
#define DATA_DIR "data"
#endif

typedef struct {
    char id[15];
    char name[MAX_NAME_LEN];
    char gender;
    char phone[MAX_PHONE_LEN];
    char address[MAX_ADDR_LEN];
    char email[MAX_EMAIL_LEN];
} Student;

typedef struct {
    char class_name[31];
    Student students[MAX_STUDENTS];
    int count;
} Class;

extern Class current_class;
extern int class_loaded;

void init_system();
void create_new_class();
void load_existing_class();
void save_current_class();
void show_delete_class_ui();
int delete_class(const char *class_name);

int add_student(Class *cls, const Student *stu);
int delete_student(Class *cls, const char *id);
int modify_student(Class *cls, const char *id, const Student *new_data);
Student* find_student(const Class *cls, const char *id);
int insert_student_sorted(Class *cls, const Student *stu);

#endif // STUDENT_H