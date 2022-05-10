#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
// #include <stdio.h>
void FCFS(int number_of_jobs,
    const int job_submitted_time[],
    const int job_required_time[],
    int job_sched_start[]
) {
    /*if submitted time equals, let the index lower first*/
    int time = 0, i;
    for (i = 0; i < number_of_jobs; ++i) {
        time = MAX(time, job_submitted_time[i]);
        job_sched_start[i] = time;
        time += job_required_time[i];
    }
}

void SJF(
    int number_of_jobs,
    const int job_submitted_time[],
    const int job_required_time[],
    int job_sched_start[]
) {
    int time = 0, count = 0, nearest, jobTime, jobNum;
    int finished[2000] = { 0 };
    int i, jobTimeInit;
    while (count++ < number_of_jobs) {
        nearest = -1;
        for (i = 0; i < number_of_jobs; ++i) {
            if (!finished[i]) {
                if (nearest == -1) nearest = job_submitted_time[i];
                else nearest = MIN(nearest, job_submitted_time[i]);
                if (nearest <= time) break;
            }
        }
        nearest = MAX(time, nearest);
        jobTimeInit = 0;
        for (i = 0; i < number_of_jobs; ++i) {
            if (finished[i]) continue;
            if (job_submitted_time[i] <= nearest) {
                if (!jobTimeInit) {
                    jobNum = i;
                    jobTime = job_required_time[i];
                    jobTimeInit = 1;
                } else if (job_required_time[i] < jobTime) {
                    jobNum = i;
                    jobTime = job_required_time[i];
                }
            }
        }
        // printf("jobNum: %d\n", jobNum);
        finished[jobNum] = 1;
        job_sched_start[jobNum] = nearest;
        time = nearest + jobTime;
    }
}
