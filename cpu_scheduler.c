#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 100

typedef struct {
    int id, arrival, burst;
    int remaining;         
    int start, finish;      
    int waiting, turnaround, response;
    int started;            
} Process;

Process p[MAX_PROCESS];
int n;

int gantt_pid[1000], gantt_start[1000], gantt_end[1000], gantt_len;

void input_processes() {
    printf("\nEnter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Enter Process ID, Arrival Time, Burst Time for P%d: ", i + 1);
        scanf("%d %d %d", &p[i].id, &p[i].arrival, &p[i].burst);
        p[i].remaining = p[i].burst;
        p[i].started = 0;
        p[i].response = -1;
    }
}

void print_gantt_chart() {
    printf("\n==================== GANTT CHART ====================\n");
    
    printf("|");
    for (int i = 0; i < gantt_len; i++) {
        int width = (gantt_end[i] - gantt_start[i]) * 2 + 1;
        for (int j = 0; j < width; j++) printf("-");
        printf("|");
    }
    printf("\n|");
    
    for (int i = 0; i < gantt_len; i++) {
        int width = (gantt_end[i] - gantt_start[i]) * 2 + 1;
        int padding = (width - 2) / 2;
        for (int j = 0; j < padding; j++) printf(" ");
        printf("P%d", gantt_pid[i]);
        for (int j = 0; j < width - padding - 2; j++) printf(" ");
        printf("|");
    }
    printf("\n|");
    
    for (int i = 0; i < gantt_len; i++) {
        int width = (gantt_end[i] - gantt_start[i]) * 2 + 1;
        for (int j = 0; j < width; j++) printf("-");
        printf("|");
    }
    
    printf("\n%d", gantt_start[0]);
    for (int i = 0; i < gantt_len; i++) {
        int width = (gantt_end[i] - gantt_start[i]) * 2 + 2;
        for (int j = 0; j < width - 1; j++) printf(" ");
        printf("%d", gantt_end[i]);
    }
    printf("\n=====================================================\n");
}

void calculate_and_print_times() {
    float avg_waiting = 0, avg_turnaround = 0, avg_response = 0;
    
    printf("\n%-10s %-10s %-10s %-12s %-12s %-12s %-12s\n", 
           "Process", "Arrival", "Burst", "Completion", "Turnaround", "Waiting", "Response");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("P%-9d %-10d %-10d %-12d %-12d %-12d %-12d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].finish,
               p[i].turnaround, p[i].waiting, p[i].response);
        avg_waiting += p[i].waiting;
        avg_turnaround += p[i].turnaround;
        avg_response += p[i].response;
    }
    
    printf("--------------------------------------------------------------------------------\n");
    printf("\nAverage Waiting Time    : %.2f ms\n", avg_waiting / n);
    printf("Average Turnaround Time : %.2f ms\n", avg_turnaround / n);
    printf("Average Response Time   : %.2f ms\n", avg_response / n);
}

void fcfs() {
    gantt_len = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival)
            current_time = p[i].arrival;
        
        p[i].start = current_time;
        p[i].response = p[i].start - p[i].arrival;
        p[i].finish = current_time + p[i].burst;
        p[i].turnaround = p[i].finish - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        
        gantt_pid[gantt_len] = p[i].id;
        gantt_start[gantt_len] = current_time;
        gantt_end[gantt_len] = p[i].finish;
        gantt_len++;
        
        current_time = p[i].finish;
    }
    
    printf("\n============ FCFS SCHEDULING ============\n");
    print_gantt_chart();
    calculate_and_print_times();
}

void round_robin(int quantum) {
    gantt_len = 0;
    
    for (int i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].started = 0;
        p[i].response = -1;
    }
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp; 
            }
        }
    }
    
    int queue[1000], front = 0, rear = 0;
    int in_queue[MAX_PROCESS] = {0};
    int completed = 0, current_time = 0;
    
    queue[rear++] = 0;
    in_queue[0] = 1;
    
    while (completed < n) {
        if (front == rear) {
            int min_arrival = 99999, min_idx = -1;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining > 0 && p[i].arrival < min_arrival) {
                    min_arrival = p[i].arrival;
                    min_idx = i;
                }
            }
            if (min_idx != -1) {
                current_time = p[min_idx].arrival;
                queue[rear++] = min_idx;
                in_queue[min_idx] = 1;
            }
        }
        
        int idx = queue[front++];
        
        if (!p[idx].started) {
            p[idx].response = current_time - p[idx].arrival;
            p[idx].started = 1;
        }
        
        int exec_time = (p[idx].remaining < quantum) ? p[idx].remaining : quantum;
        
        gantt_pid[gantt_len] = p[idx].id;
        gantt_start[gantt_len] = current_time;
        gantt_end[gantt_len] = current_time + exec_time;
        gantt_len++;
        
        current_time += exec_time;
        p[idx].remaining -= exec_time;
        
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && p[i].remaining > 0 && p[i].arrival <= current_time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        if (p[idx].remaining > 0) {
            queue[rear++] = idx;
        } else {
            p[idx].finish = current_time;
            p[idx].turnaround = p[idx].finish - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }
    
    printf("\n============ ROUND ROBIN SCHEDULING (Quantum = %d) ============\n", quantum);
    print_gantt_chart();
    calculate_and_print_times();
}

int main() {
    int choice, quantum;
    
    printf("========================================\n");
    printf("       CPU SCHEDULER SIMULATOR\n");
    printf("========================================\n");
    
    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                input_processes();
                fcfs();
                break;
            case 2:
                input_processes();
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                round_robin(quantum);
                break;
            case 5:
                printf("\nExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}
