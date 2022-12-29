#include <stdio.h>
#include<stdlib.h>

int resourceNum; // 资源种类的数量
int progressNum; // 进程数
char resourceName[100]; // 各个资源的名称
int Max[100][100]={0};//最大需求矩阵
int Allocation[100][100]={0};// 系统已分配矩阵
int Need[100][100]={0};// 还需要资源矩阵
int Available[100]={0};// 可用资源矩阵
int Request[100]={0};//请求资源向量
int Work[100]={0};//存放系统可提供资源量
int Finish[100]={0}; //标记系统是否有足够的资源分配给各个进程
int Security[100]={0};//存放安全序列


void init(){
    printf("请输入资源种类数：");
    scanf("%d",&resourceNum);
    fflush(stdin);  //清空输入流缓冲区的字符，注意必须引入#include<stdlib.h>头文件

    for (int i = 0; i < resourceNum; i++) {
        printf("请输入资源名称：");
        scanf("%c",&resourceName[i]);

        printf("请输入资源 %c 的数量：", resourceName[i]);
        scanf("%d",&Available[i]);
        fflush(stdin);
    }

    printf("请输入进程数：");
    scanf("%d",&progressNum);

    printf("请输入各进程的最大需求矩阵的值[Max]:\n");
    int Max_flag;
    do {
        Max_flag = 0;
        for (int i = 0; i < progressNum; i++)          // 行
            for (int j = 0; j < resourceNum; j++) {        // 列
                scanf("%d",&Max[i][j]);
                if (Max[i][j] > Available[j]){
                    Max_flag = 1;
                }
            }
        if (Max_flag == 1){
            printf("资源最大需求量大于系统中资源最大量，请重新输入!\n");
        }
    } while (Max_flag == 1);

    printf("请输入各进程已经分配的资源量[Allocation]:\n");
    int Allocation_flag;
    do {
        Allocation_flag = 0;
        for (int i = 0; i < progressNum; i++)          // 行
            for (int j = 0; j < resourceNum; j++) {        // 列
                scanf("%d",&Allocation[i][j]);
                if (Allocation[i][j] > Max[i][j]){
                    Allocation_flag = 1;
                }
            }
        if (Allocation_flag == 1){
            printf("分配的资源大于最大量，请重新输入!\n");
        }
    } while (Allocation_flag == 1);

    // 计算Need
    for (int i = 0; i < progressNum; i++) {
        for (int j = 0; j < resourceNum; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
    // 计算Available
    for (int i = 0; i < progressNum; i++) {
        for (int j = 0; j < resourceNum; j++) {
            Available[j] = Available[j] - Allocation[i][j];
        }
    }
}

void showStatus(){
    printf("系统当前的资源分配情况如下：\n");
    printf("            Max   	   Allocation       Need\n");
    printf("进程名      ");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < resourceNum; j++) {
            printf("%c  ", resourceName[j]);
        }
        printf("     ");
    }
    printf("\n");

    for (int i = 0; i < progressNum; i++) {
        printf(" P%d        ",i);
        for (int j = 0; j < resourceNum; j++) {
            printf("%d  ", Max[i][j]);
        }
        printf("     ");

        for (int j = 0; j < resourceNum; j++) {
            printf("%d  ", Allocation[i][j]);
        }
        printf("     ");

        for (int j = 0; j < resourceNum; j++) {
            printf("%d  ", Need[i][j]);
        }
        printf("\n");
    }

    printf("\nAvailable\n");
    for (int j = 0; j < resourceNum; j++) {
        printf("%d ", Available[j]);
    }
    printf("\n\n");

}

int safe(){

    for (int i = 0; i < progressNum; i++) {
        Finish[i] = 0;
    }

    for (int i = 0; i < resourceNum; i++) {
        Work[i] = Available[i];
    }

    int safe_flag;
    int security_count = 0;
    for (int i = 0; i < progressNum; i++) {
        safe_flag = 0;
        for (int j = 0; j < resourceNum; j++) {
            if (Finish[i] == 0 && Need[i][j] <= Work[j]){
                safe_flag++;  // 每个Need都小于Work才可以
                if (safe_flag == resourceNum){
                    Finish[i] = 1;
                    for (int k = 0; k < resourceNum; k++) {
                        Work[k] = Work[k]+ Allocation[i][k];
                    }
                    Security[security_count++] = i;
                    i = -1;
                }
            }
        }
    }

    for (int i = 0; i < progressNum; i++) {
        if (Finish[i] == 0){
            printf("系统不安全\n");
            return 0;
        }
    }

    printf("系统安全\n");
    printf("存在一个安全序列是：");
    for (int i = 0; i < progressNum; i++) {
        printf("P%d", Security[i]);
        if (i < progressNum - 1){
            printf(" -> ");
        }
    }
    printf("\n");
    return 1;
}

int allocate(int progressNum){
    for (int i = 0; i < resourceNum; i++) {
        Available[i] = Available[i] - Request[i];
        Need[progressNum][i] = Need[progressNum][i] - Request[i];
        Allocation[progressNum][i]= Allocation[progressNum][i] + Request[i];
    }
    return 1;
}

int allocateReset(int progressNum){
    for (int i = 0; i < resourceNum; i++) {
        Available[i] = Available[i] + Request[i];
        Need[progressNum][i] = Need[progressNum][i] + Request[i];
        Allocation[progressNum][i]= Allocation[progressNum][i] - Request[i];
    }
    return 1;
}

void bank(){
    int requestProgressNum;
    int bank_flag = 1;

    printf("请输入请求分配资源的进程号(0-%d):",progressNum-1);
    scanf("%d", &requestProgressNum);

    printf("请输入进程P%d要申请的资源个数:\n",requestProgressNum);
    for (int i = 0; i < resourceNum; i++) {
        printf("%c: ", resourceName[i]);
        scanf("%d", &Request[i]);
    }

    for (int i = 0; i < resourceNum; i++) {
        if (Request[i] > Need[requestProgressNum][i]){
            bank_flag = 0;
            printf("进程P%d申请的资源大于Need\n", requestProgressNum);
            printf("分配不合理，不予分配\n");
            break;
        } else {
            if (Request[i] > Available[i]){
                bank_flag = 0;
                printf("进程P%d申请的资源大于Available\n", requestProgressNum);
                printf("尚无足够资源，不予分配\n");
                break;
            }
        }
    }

    if (bank_flag == 1){
        allocate(requestProgressNum);
        showStatus();
        if (safe() == 0){
            printf("请求失败请重试！\n\n");
            allocateReset(requestProgressNum);
            showStatus();
        }
    }

}

int main() {
    printf("\t---------------------------------------------------\n");
    printf("\t||                                               ||\n");
    printf("\t||                银行家算法的实现                  ||\n");
    printf("\t||                                               ||\n");
    printf("\t||                                               ||\n");
    printf("\t||                                               ||\n");
    printf("\t---------------------------------------------------\n");

    init();
    showStatus();
    if (safe() == 0){
        printf("系统不安全，无法进行后续操作！\n");
        exit(0);
    }

    char choice;
    do{
        printf("*************************************************************\n");
        printf("\n");
        printf("\n");
        printf("\t-------------------银行家算法演示------------------\n");
        printf("                     R(r):请求分配   \n");
        printf("                     E(e):退出       \n");
        printf("\t---------------------------------------------------\n");
        printf("请选择：");
        fflush(stdin);
        scanf("%c",&choice);
        switch(choice)
        {
            case 'r':
            case 'R':
                bank();break;
            case 'e':
            case 'E':
                exit(0);
            default: printf("请正确选择!\n");break;
        }
    } while(choice);

}
