#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "start scheduler_test\n");

    int pid = fork();
    if (pid == 0) {
        set_proc_info(0, 0, 0, 0, 500);
        while(1);
        exit();
    } else {
        wait();
    }

    printf(1, "end of scheduler_test\n");
    exit();
}