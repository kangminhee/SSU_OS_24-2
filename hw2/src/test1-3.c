#include "types.h"
#include "stat.h"
#include "user.h"

int a1[3] = {2, 2, 2};
int a2[3] = {0, 0, 0};
int a3[3] = {0, 0, 0};
int a4[3] = {0, 0, 0};
int a5[3] = {300, 300, 300};

int main(int argc, char *argv[]) {
    int pid[3];

    printf(1, "start scheduler_test\n");

    for (int i = 0; i < 3; i++) {
        pid[i] = fork();

        if (pid[i] == 0) {
            set_proc_info(a1[i], a2[i], a3[i], a4[i], a5[i]);

            while(1);
            
            exit();
        }
    }
    for (int i = 0; i < 3; i++) wait();
    
    printf(1, "end of scheduler_test\n");
    exit();
}