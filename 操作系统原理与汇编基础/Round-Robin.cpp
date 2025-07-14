#include <stdio.h>

#define N 3 // 进程数
#define TIMESLICE 3

int process_time[N] = {5, 8, 6}; // 每个进程的运行总时间

int main() {
    int finished = 0;
    while (finished < N) {
        for (int i = 0; i < N; ++i) {
            if (process_time[i] > 0) {
                int run_time = process_time[i] < TIMESLICE ? process_time[i] : TIMESLICE;
                printf("Process %d runs for %d units\n", i, run_time);
                process_time[i] -= run_time;
                if (process_time[i] == 0) {
                    printf("Process %d finishes.\n", i);
                    finished++;
                }
            }
        }
    }
    return 0;
}