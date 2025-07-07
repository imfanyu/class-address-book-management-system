/**
* @file student.c
 * @brief 班级通讯录管理系统核心实现
 * @author 樊彧（电气241）、何嘉萌（电气241）、李一诺（电气241）
 * @date 2025-06-28
 *
 * @details 实现班级和学生信息的存储、排序、文件I/O等核心功能
 *
 * 北京建筑大学 智能科学与技术学院
 * C语言程序设计课程设计 - 班级通讯录管理系统
 * 仅供课程设计使用，未经许可不得外传
 */

#include "student.h"

Class current_class;    ///< 全局班级数据
int class_loaded = 0;   ///< 班级加载状态标志

/* ========== 系统初始化 ========== */

void init_system() {
#ifdef _WIN32
    _mkdir(DATA_DIR);
#else
    mkdir(DATA_DIR, 0755);
#endif
}

/* ========== 班级管理 ========== */

void create_new_class() {
    printf("\n=== 创建新班级 ===\n");
    memset(&current_class, 0, sizeof(Class));

    printf("请输入班级名称: ");
    scanf("%30[^\n]", current_class.class_name);
    while(getchar() != '\n'){}  // 清理缓冲区

    current_class.count = 0;
    class_loaded = 1;
    printf("\n班级创建成功!\n");
}

void load_existing_class() {
    DIR *dir;
    struct dirent *ent;
    char file_list[20][50];
    int file_count = 0;

    printf("\n=== 可用的班级文件 ===\n");
    if ((dir = opendir(DATA_DIR)) != NULL) {
        while ((ent = readdir(dir)) != NULL && file_count < 20) {
            if(strstr(ent->d_name, ".csv")) {
                strcpy(file_list[file_count], ent->d_name);
                printf("%d. %s\n", file_count + 1, file_list[file_count]);
                file_count++;
            }
        }
        closedir(dir);
    }

    if (file_count == 0) {
        printf("没有找到班级文件!\n");
        return;
    }

    printf("\n请输入要加载的文件编号: ");
    int choice = 0;
    while (1) {
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'){}
            printf("输入无效，请输入数字。\n");
            continue;
        }
        while(getchar() != '\n'){}

        if (choice > 0 && choice <= file_count) break;
        printf("输入超出范围，请重新输入(1-%d): ", file_count);
    }

    char path[100];
    snprintf(path, sizeof(path), "%s/%s", DATA_DIR, file_list[choice-1]);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("无法打开文件 %s\n", path);
        return;
    }

    memset(&current_class, 0, sizeof(Class));
    current_class.count = 0;

    char line[512];
    int line_num = 0;
    int student_data_started = 0;

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) continue;

        if (line_num == 1) {
            if (strcmp(line, "class_name") != 0) {
                printf("错误: 无效的文件格式\n");
                fclose(file);
                return;
            }
            continue;
        }

        if (line_num == 2) {
            strncpy(current_class.class_name, line, sizeof(current_class.class_name)-1);
            continue;
        }

        if (line_num == 3) continue;

        if (line_num == 4) {
            student_data_started = 1;
            continue;
        }

        if (student_data_started && current_class.count < MAX_STUDENTS) {
            Student *stu = &current_class.students[current_class.count];

            char *token = strtok(line, ",");
            if (token) strncpy(stu->id, token, sizeof(stu->id)-1);

            token = strtok(NULL, ",");
            if (token) strncpy(stu->name, token, sizeof(stu->name)-1);

            token = strtok(NULL, ",");
            if (token && token[0]) stu->gender = token[0];

            token = strtok(NULL, ",");
            if (token) strncpy(stu->phone, token, sizeof(stu->phone)-1);

            token = strtok(NULL, ",");
            if (token) strncpy(stu->address, token, sizeof(stu->address)-1);

            token = strtok(NULL, ",");
            if (token) strncpy(stu->email, token, sizeof(stu->email)-1);

            current_class.count++;
        }
    }

    fclose(file);
    class_loaded = 1;
    printf("\n班级 [%s] 加载成功! 共加载 %d 名学生\n",
           current_class.class_name, current_class.count);
}

void save_current_class() {
    if (!class_loaded) {
        printf("没有需要保存的班级数据!\n");
        return;
    }

    for (int i = 1; i < current_class.count; i++) {
        Student key = current_class.students[i];
        int j = i - 1;
        while (j >= 0 && strcmp(current_class.students[j].id, key.id) > 0) {
            current_class.students[j+1] = current_class.students[j];
            j--;
        }
        current_class.students[j+1] = key;
    }

    char filename[50];
    snprintf(filename, sizeof(filename), "class_%s.csv", current_class.class_name);

    char path[100];
    snprintf(path, sizeof(path), "%s/%s", DATA_DIR, filename);

    FILE *file = fopen(path, "w");
    if (!file) {
        printf("无法保存文件到 %s\n", path);
        return;
    }

    fprintf(file, "class_name\n");
    fprintf(file, "%s\n\n", current_class.class_name);

    fprintf(file, "id,name,gender,phone,address,email\n");
    for (int i = 0; i < current_class.count; i++) {
        Student *s = &current_class.students[i];
        fprintf(file, "%s,%s,%c,%s,%s,%s\n",
               s->id, s->name, s->gender,
               s->phone, s->address, s->email);
    }

    fclose(file);
    printf("\n班级数据已按学号排序保存到: %s\n", path);
}

int delete_class(const char *class_name) {
    if (!class_name || strlen(class_name) == 0) return 0;

    char filename[50];
    snprintf(filename, sizeof(filename), "class_%s.csv", class_name);

    char path[100];
    snprintf(path, sizeof(path), "%s/%s", DATA_DIR, filename);

    if (remove(path) != 0) {
        if (errno == ENOENT) {
            printf("班级文件不存在: %s\n", path);
        } else {
            perror("删除班级失败");
        }
        return 0;
    }

    if (class_loaded && strcmp(current_class.class_name, class_name) == 0) {
        memset(&current_class, 0, sizeof(Class));
        class_loaded = 0;
    }

    printf("成功删除班级: %s\n", class_name);
    return 1;
}

void show_delete_class_ui() {
    DIR *dir;
    struct dirent *ent;
    char file_list[20][50];
    int file_count = 0;

    printf("\n=== 可删除的班级文件 ===\n");
    if ((dir = opendir(DATA_DIR)) != NULL) {
        while ((ent = readdir(dir)) != NULL && file_count < 20) {
            if (strstr(ent->d_name, ".csv")) {
                strcpy(file_list[file_count], ent->d_name);
                printf("%d. %s\n", file_count + 1, file_list[file_count]);
                file_count++;
            }
        }
        closedir(dir);
    }

    if (file_count == 0) {
        printf("没有找到班级文件!\n");
        return;
    }

    printf("\n请输入要删除的文件编号(0取消): ");
    int choice = -1;
    while (1) {

        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'){}
            printf("输入无效，请输入数字。\n");
            continue;
        }
        while(getchar() != '\n'){}

        if (choice >= 0 && choice <= file_count) break;
        printf("无效输入，请重新输入(0-%d): ", file_count);
    }

    if (choice == 0) {
        printf("取消删除操作\n");
        return;
    }

    char *filename = file_list[choice - 1];
    char class_name[30] = {0};
    if (sscanf(filename, "class_%[^.]", class_name) != 1) {
        printf("无法解析班级名称\n");
        return;
    }

    printf("\n确认删除班级 %s 吗？(y/n): ", class_name);
    char confirm[2];
    scanf("%1[^\n]", confirm);
    while(getchar() != '\n'){}

    if (tolower(confirm[0]) == 'y') {
        if (delete_class(class_name)) {
            printf("班级删除成功\n");
        }
    } else {
        printf("取消删除\n");
    }
}

/* ========== 学生管理 ========== */

int insert_student_sorted(Class *cls, const Student *stu) {
    int i = cls->count;
    while (i > 0 && strcmp(cls->students[i-1].id, stu->id) > 0) i--;

    for (int j = cls->count; j > i; j--) {
        cls->students[j] = cls->students[j-1];
    }

    cls->students[i] = *stu;
    cls->count++;
    return i;
}

int add_student(Class *cls, const Student *stu) {
    if (cls->count >= MAX_STUDENTS) {
        printf("班级已满! 最大容量: %d\n", MAX_STUDENTS);
        return 0;
    }

    if (find_student(cls, stu->id) != NULL) {
        printf("学号 %s 已存在!\n", stu->id);
        return -1;
    }

    insert_student_sorted(cls, stu);
    return 1;
}

int delete_student(Class *cls, const char *id) {
    for (int i = 0; i < cls->count; i++) {
        if (strcmp(cls->students[i].id, id) == 0) {
            for (int j = i; j < cls->count - 1; j++) {
                cls->students[j] = cls->students[j+1];
            }
            memset(&cls->students[cls->count-1], 0, sizeof(Student));
            cls->count--;
            return 1;
        }
    }
    printf("未找到学号为 %s 的学生!\n", id);
    return 0;
}

int modify_student(Class *cls, const char *id, const Student *new_data) {
    Student *stu = find_student(cls, id);
    if (stu == NULL) {
        printf("未找到学号为 %s 的学生!\n", id);
        return 0;
    }

    if (strcmp(id, new_data->id) != 0 && find_student(cls, new_data->id) != NULL) {
        printf("学号 %s 已存在!\n", new_data->id);
        return -1;
    }

    *stu = *new_data;
    return 1;
}

Student* find_student(const Class *cls, const char *id) {
    for (int i = 0; i < cls->count; i++) {
        if (strcmp(cls->students[i].id, id) == 0) {
            return (Student*)&cls->students[i];
        }
    }
    return NULL;
}

