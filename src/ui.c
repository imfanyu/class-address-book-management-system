//
// Created by fanyuo on 2025/6/28.
//
/**
 * @file ui.c
 * @brief 班级通讯录管理系统核心实现
 * @author 樊彧（电气241）、何嘉萌（电气241）、李一诺（电气241）
 * @created 2025-06-28
 */

/*****************************************************************
 * 北京建筑大学 智能科学与技术学院
 * 课程名称：C语言程序设计课程设计
 * 设计题目：班级通讯录管理系统
 *
 * 文件功能：实现班级和学生信息的存储、排序、文件I/O等核心功能
 *****************************************************************/

#include "ui.h"

/* ========== 信息显示 ========== */

void print_class_info(const Class *cls) {
    printf("\n=== 班级信息 ===\n");
    printf("班级名称: %s\n", cls->class_name);
    printf("学生人数: %d/%d\n", cls->count, MAX_STUDENTS);
}

void print_all_students(const Class *cls) {
    printf("\n=== 学生列表 (共 %d 人) ===\n", cls->count);
    for (int i = 0; i < cls->count; i++) {
        const Student *s = &cls->students[i];
        printf("\n────── 学生 %d ──────\n", i+1);
        printf("学号: %s\n姓名: %s\n性别: %c\n电话: %s\n邮箱: %s\n住址: %s\n",
               s->id, s->name, s->gender, s->phone, s->email, s->address);
        printf("────────────────────");
    }
    printf("\n=== 列表结束 ===\n");
}

void print_student(const Student *stu) {
    printf("\n=== 学生详细信息 ===\n");
    printf("学号: %s\n", stu->id);
    printf("姓名: %s\n", stu->name);
    printf("性别: %c\n", stu->gender);
    printf("电话: %s\n", stu->phone);
    printf("邮箱: %s\n", stu->email);
    printf("住址: %s\n", stu->address);
}

/* ========== UI 函数 ========== */

void show_add_student_ui(Class *cls) {
    Student stu;
    memset(&stu, 0, sizeof(Student));

    printf("\n=== 添加学生 ===\n");
    printf("请输入学号: ");
    scanf("%15[^\n]", stu.id);
    while(getchar() != '\n') {}

    printf("请输入姓名: ");
    scanf("%20[^\n]", stu.name);
    while(getchar() != '\n') {}

    printf("请输入性别(M/F): ");
    scanf("%c", &stu.gender);
    while(getchar() != '\n') {}
    stu.gender = (char)toupper((unsigned char)stu.gender);

    printf("请输入电话: ");
    scanf("%15[^\n]", stu.phone);
    while(getchar() != '\n') {}

    printf("请输入住址: ");
    scanf("%50[^\n]", stu.address);
    while(getchar() != '\n') {}

    printf("请输入邮箱: ");
    scanf("%30[^\n]", stu.email);
    while(getchar() != '\n') {}

    int result = add_student(cls, &stu);
    if (result == 1) {
        printf("添加成功!\n");
    } else if (result == -1) {
        printf("添加失败: 学号已存在!\n");
    } else {
        printf("添加失败: 班级已满!\n");
    }
}

void show_delete_student_ui(Class *cls) {
    printf("\n=== 删除学生 ===\n");
    if (cls->count == 0) {
        printf("当前班级没有学生记录\n");
        return;
    }

    char id[15];
    printf("请输入要删除的学生学号: ");
    scanf("%15[^\n]", id);
    while(getchar() != '\n'){}

    int result = delete_student(cls, id);
    if (result) {
        printf("删除成功!\n");
    } else {
        printf("删除失败: 学号不存在!\n");
    }
}

void show_modify_student_ui(Class *cls) {
    printf("\n=== 修改学生信息 ===\n");
    if (cls->count == 0) {
        printf("当前班级没有学生记录\n");
        return;
    }

    char id[15];
    printf("请输入要修改的学生学号: ");
    scanf("%15[^\n]", id);
    while(getchar() != '\n'){}

    Student *stu = find_student(cls, id);
    if (stu == NULL) {
        printf("未找到该学号的学生\n");
        return;
    }

    printf("\n当前学生信息:\n");
    print_student(stu);

    Student new_stu = *stu;
    printf("\n请输入新信息(留空保持不变):\n");

    printf("新学号(当前:%s): ", stu->id);
    char input[20];
    scanf("%19[^\n]", input);
    while(getchar() != '\n'){}
    if (input[0] != '\0') strncpy(new_stu.id, input, sizeof(new_stu.id)-1);

    printf("新姓名(当前:%s): ", stu->name);
    scanf("%19[^\n]", input);
    while(getchar() != '\n'){}
    if (input[0] != '\0') strncpy(new_stu.name, input, sizeof(new_stu.name)-1);

    printf("新性别(当前:%c): ", stu->gender);
    char gender_input[2];
    scanf("%1[^\n]", gender_input);
    while(getchar() != '\n'){}
    if (gender_input[0] != '\0') new_stu.gender = (char)toupper((unsigned char)gender_input[0]);

    printf("新电话(当前:%s): ", stu->phone);
    scanf("%19[^\n]", input);
    while(getchar() != '\n'){}
    if (input[0] != '\0') strncpy(new_stu.phone, input, sizeof(new_stu.phone)-1);

    printf("新住址(当前:%s): ", stu->address);
    scanf("%49[^\n]", input);
    while(getchar() != '\n'){}
    if (input[0] != '\0') strncpy(new_stu.address, input, sizeof(new_stu.address)-1);

    printf("新邮箱(当前:%s): ", stu->email);
    scanf("%29[^\n]", input);
    while(getchar() != '\n'){}
    if (input[0] != '\0') strncpy(new_stu.email, input, sizeof(new_stu.email)-1);

    int result = modify_student(cls, id, &new_stu);
    printf(result ? "修改成功!\n" : "修改失败!\n");
}

void show_search_student_ui(const Class *cls) {
    printf("\n=== 查找学生 ===\n");
    if (cls->count == 0) {
        printf("当前班级没有学生记录\n");
        return;
    }

    int choice;
    printf("\n1. 按学号查找\n");
    printf("2. 按姓名查找\n");
    printf("请选择查找方式: ");
    scanf("%d", &choice);
    while(getchar() != '\n'){}

    if (choice == 1) {
        char id[15];
        printf("请输入学号: ");
        scanf("%15[^\n]", id);
        while(getchar() != '\n'){}

        Student *stu = find_student(cls, id);
        if (stu) {
            print_student(stu);
        } else {
            printf("未找到该学号的学生\n");
        }
    }
    else if (choice == 2) {
        char name[20];
        printf("请输入姓名: ");
        scanf("%20[^\n]", name);
        while(getchar() != '\n'){}

        int found = 0;
        for (int i = 0; i < cls->count; i++) {
            if (strcmp(cls->students[i].name, name) == 0) {
                print_student(&cls->students[i]);
                found = 1;
            }
        }
        if (!found) {
            printf("未找到该姓名的学生\n");
        }
    }
    else {
        printf("无效的选择\n");
    }
}

void show_main_menu() {
    printf("\n======================================================\n");
    printf("当前正在管理班级：%s\n",current_class.class_name);
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 修改学生信息\n");
    printf("4. 查找学生\n");
    printf("5. 显示所有学生\n");
    printf("6. 显示班级信息\n");
    printf("0. 退出\n");
    printf("请选择: ");
}

void manage_class_menu() {
    int choice;
    do {
        show_main_menu();
        scanf("%d", &choice);
        while(getchar() != '\n'){}

        switch(choice) {
        case 1: show_add_student_ui(&current_class); break;
        case 2: show_delete_student_ui(&current_class); break;
        case 3: show_modify_student_ui(&current_class); break;
        case 4: show_search_student_ui(&current_class); break;
        case 5: print_all_students(&current_class); break;
        case 6: print_class_info(&current_class); break;
        case 0: return;
        default: printf("无效选择!\n");
        }
    } while(1);
}