#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Command {
    char name[100];
    char function[100];
    int tag;
};
struct Command cmds[100];

int count = 0;

// 初始化结构体数组 cmds
void init(){
    FILE * fp;
    fp = fopen("../cmd.txt","r");
    if(fp == NULL){
        printf("not found the file\n");
        exit(0);
    }

    while (1) {
        if(feof(fp)){
            break;
        }
        fscanf(fp, "%s%s%d", cmds[count].name, cmds[count].function, &cmds[count].tag);
        count++;
    }

    if (fclose(fp)) {
        printf("file close failed\n");
        exit(0);
    }

}

// 展示cmd.txt文件中的命令 即help命令的运行结果
void display(){
    for (int i = 0; i< count; i++) {
        if (cmds[i].tag == 1){
            printf("%-10s %s    内部命令\n", cmds[i].name, cmds[i].function);
        } else{
            printf("%-10s %s    外部命令\n", cmds[i].name, cmds[i].function);
        }
    }
}

// 主要运行函数
void run(){
    char input[100];
    printf("Scoheart Windows [ version 1.0.0 ]\n");
    printf("(c) 2022 corporation. \n");

    while (strcmp(input, "exit") != 0) {
        int is_command_flag = 0;

        printf("-----------------------------------------------------------------\n");
        printf("scoheart@Scohearts-Air ~ : ");
        scanf("%s", &input);

        if(strcmp(input, "help") == 0){
            display();
            is_command_flag = 1;
        }else{
            for (int i = 0; i< count; i++) {

                if(strcmp(input, cmds[i].name) == 0){ // == 返回0 ｜ != 返回非0

                    if (cmds[i].tag == 1){
                        printf("内部命令输入成功，它的作用是%s\n", cmds[i].function);
                    } else{
                        printf("外部命令输入成功，它的作用是%s\n", cmds[i].function);
                    }

                    system(input);
                    is_command_flag = 1;
                    break;
                }
            }
        }

        if(is_command_flag == 0){
            if (strcmp(input, "exit") != 0){
                printf("既不是内部或外部命令，也不是可运行程序或批处理命令!\n");
            }
        }
    }
    printf("程序运行结束\n");
}


int main() {
    init();
    run();
    return 0;
}
