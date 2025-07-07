/**
* @file main.c
 * @brief 班级通讯录管理系统主程序
 * @author 樊彧（电气241）、何嘉萌（电气241）、李一诺（电气241）
 * @date 2025-06-28
 *
 * @details 实现系统主循环和核心管理功能
 *
 * 开发环境：
 *   - 操作系统：Windows 11
 *   - 编译器：GCC 8.1.0 (MinGW-W64)
 *   - 构建工具：CMake 3.31
 *   - IDE：CLion 2025.1.3
 *   - 标准：C99
 *
 * 北京建筑大学 智能科学与技术学院
 * C语言程序设计课程设计 - 班级通讯录管理系统
 * 仅供课程设计使用，未经许可不得外传
 */

#include "student.h"
#include "ui.h"

int main() {
    init_system();
    int choice;
    printf("======================================================\n");
    printf("C语言程序设计-课程设计\n");
    printf("班级通讯录管理系统\n");
    printf("作者：电气241樊彧、电气241何嘉萌、电气241李一诺\n");
    printf("======================================================\n");
    while(1) {
        printf("\n======================================================\n");
        printf("1. 创建新班级\n");
        printf("2. 加载已有班级\n");
        printf("3. 删除班级\n");
        printf("4. 管理当前班级\n");
        printf("5. 保存当前班级\n");
        printf("0. 退出\n");
        printf("请选择: ");

        while (1) {

            if (scanf("%d", &choice) != 1) {
                while(getchar() != '\n'){}
                printf("输入无效，请输入数字:");
                continue;
            }
            while(getchar() != '\n'){}

            if (choice >= 0 && choice <= 5) break;
            printf("无效输入，请重新输入(0-%d): ", 5);
        }

        switch(choice) {
        case 1:
            create_new_class();
            break;
        case 2:
            load_existing_class();
            break;
        case 3:
            show_delete_class_ui();
            break;
        case 4:
            if(class_loaded) {
                manage_class_menu();
            } else {
                printf("请先创建或加载班级!\n");
            }
            break;
        case 5:
            if(class_loaded) {
                save_current_class();
            } else {
                printf("请先创建或加载班级!\n");
            }
            break;
        case 0:
            printf("结束进程!\n");
            system("pause");
            exit(0);
        default:
            printf("无效选择!\n");
        }
    }
}