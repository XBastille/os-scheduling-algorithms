#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROCESSES 5
#define NUM_RESOURCES 4

int allocation[NUM_PROCESSES][NUM_RESOURCES] = {
    {0, 0, 1, 2},   
    {2, 0, 0, 0},   
    {0, 0, 3, 4},  
    {2, 3, 5, 4},  
    {0, 3, 3, 2}    
};

int max_need[NUM_PROCESSES][NUM_RESOURCES] = {
    {0, 0, 1, 2}, 
    {2, 7, 5, 0},   
    {6, 6, 5, 6},   
    {4, 3, 5, 6},   
    {0, 6, 5, 2}    
};

int available[NUM_RESOURCES];
int need[NUM_PROCESSES][NUM_RESOURCES];
int total[NUM_RESOURCES] = {6, 7, 12, 12};

void calculate_need() {
    printf("\n==================== NEED MATRIX ====================\n");
    printf("Need[i][j] = Max[i][j] - Allocation[i][j]\n\n");
    printf("Process   R1   R2   R3   R4\n");
    printf("------------------------------\n");
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("P%d        ", i + 1);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = max_need[i][j] - allocation[i][j];
            printf("%-5d", need[i][j]);
        }
        printf("\n");
    }
    printf("======================================================\n");
}

void calculate_available() {
    for (int j = 0; j < NUM_RESOURCES; j++) {
        int sum = 0;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            sum += allocation[i][j];
        }
        available[j] = total[j] - sum;
    }
    
    printf("\nAvailable Resources: [R1=%d, R2=%d, R3=%d, R4=%d]\n",
           available[0], available[1], available[2], available[3]);
}

int is_safe(int safe_sequence[], int work[], int temp_allocation[NUM_PROCESSES][NUM_RESOURCES], 
            int temp_need[NUM_PROCESSES][NUM_RESOURCES]) {
    int finish[NUM_PROCESSES] = {0};
    int count = 0;
    
    while (count < NUM_PROCESSES) {
        int found = 0;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!finish[i]) {
                int can_run = 1;
                for (int j = 0; j < NUM_RESOURCES; j++) {
                    if (temp_need[i][j] > work[j]) {
                        can_run = 0;
                        break;
                    }
                }
                
                if (can_run) {
                    for (int j = 0; j < NUM_RESOURCES; j++) {
                        work[j] += temp_allocation[i][j];
                    }
                    safe_sequence[count++] = i + 1;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) return 0;
    }
    return 1; 
}

void check_safe_state() {
    int work[NUM_RESOURCES];
    int safe_sequence[NUM_PROCESSES];
    
    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i];
    }
    
    printf("\n=============== SAFETY ALGORITHM ===============\n");
    printf("Initial Work: [%d, %d, %d, %d]\n", work[0], work[1], work[2], work[3]);
    printf("\n");
    
    int temp_alloc[NUM_PROCESSES][NUM_RESOURCES];
    int temp_need[NUM_PROCESSES][NUM_RESOURCES];
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            temp_alloc[i][j] = allocation[i][j];
            temp_need[i][j] = need[i][j];
        }
    }
    
    if (is_safe(safe_sequence, work, temp_alloc, temp_need)) {
        printf("The system is in a SAFE STATE!\n\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < NUM_PROCESSES; i++) {
            printf("P%d", safe_sequence[i]);
            if (i < NUM_PROCESSES - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("The system is in an UNSAFE STATE! (Deadlock possible)\n");
    }
    printf("================================================\n");
}

void request_resources(int process, int request[]) {
    printf("\n============ RESOURCE REQUEST ==============\n");
    printf("P%d requests: [R1=%d, R2=%d, R3=%d, R4=%d]\n\n", 
           process + 1, request[0], request[1], request[2], request[3]);
    
    for (int j = 0; j < NUM_RESOURCES; j++) {
        if (request[j] > need[process][j]) {
            printf("ERROR: Request exceeds maximum claim!\n");
            printf("Request R%d=%d > Need R%d=%d\n", j+1, request[j], j+1, need[process][j]);
            printf("Request DENIED.\n");
            printf("============================================\n");
            return;
        }
    }
    printf("Step 1: Request <= Need ... PASSED\n");
    
    for (int j = 0; j < NUM_RESOURCES; j++) {
        if (request[j] > available[j]) {
            printf("Step 2: Request <= Available ... FAILED\n");
            printf("P%d must wait (R%d: request=%d, available=%d)\n", 
                   process + 1, j+1, request[j], available[j]);
            printf("Request CANNOT be granted immediately.\n");
            printf("============================================\n");
            return;
        }
    }
    printf("Step 2: Request <= Available ... PASSED\n");
    
    int temp_available[NUM_RESOURCES];
    int temp_allocation[NUM_PROCESSES][NUM_RESOURCES];
    int temp_need[NUM_PROCESSES][NUM_RESOURCES];
    
    for (int j = 0; j < NUM_RESOURCES; j++) {
        temp_available[j] = available[j] - request[j];
    }
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            temp_allocation[i][j] = allocation[i][j];
            temp_need[i][j] = need[i][j];
        }
    }
    
    for (int j = 0; j < NUM_RESOURCES; j++) {
        temp_allocation[process][j] += request[j];
        temp_need[process][j] -= request[j];
    }
    
    int work[NUM_RESOURCES];
    for (int j = 0; j < NUM_RESOURCES; j++) {
        work[j] = temp_available[j];
    }
    
    int safe_sequence[NUM_PROCESSES];
    
    printf("Step 3: Checking if new state is safe...\n\n");
    printf("New Available after allocation: [%d, %d, %d, %d]\n", 
           work[0], work[1], work[2], work[3]);
    
    if (is_safe(safe_sequence, work, temp_allocation, temp_need)) {
        printf("\nNew state is SAFE!\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < NUM_PROCESSES; i++) {
            printf("P%d", safe_sequence[i]);
            if (i < NUM_PROCESSES - 1) printf(" -> ");
        }
        printf("\n\nRequest can be GRANTED IMMEDIATELY!\n");
        
        for (int j = 0; j < NUM_RESOURCES; j++) {
            available[j] -= request[j];
            allocation[process][j] += request[j];
            need[process][j] -= request[j];
        }
    } else {
        printf("\nNew state would be UNSAFE!\n");
        printf("Request CANNOT be granted immediately. P%d must wait.\n", process + 1);
    }
    printf("============================================\n");
}

void print_current_state() {
    printf("\n=============== CURRENT STATE ===============\n");
    printf("\nALLOCATION MATRIX:\n");
    printf("Process   R1   R2   R3   R4\n");
    printf("------------------------------\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("P%d        ", i + 1);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%-5d", allocation[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMAX MATRIX:\n");
    printf("Process   R1   R2   R3   R4\n");
    printf("------------------------------\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("P%d        ", i + 1);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%-5d", max_need[i][j]);
        }
        printf("\n");
    }
    
    printf("\nTotal Resources: [R1=%d, R2=%d, R3=%d, R4=%d]\n",
           total[0], total[1], total[2], total[3]);
    printf("================================================\n");
}

int main() {
    printf("================================================\n");
    printf("       BANKER'S ALGORITHM - DEADLOCK AVOIDANCE\n");
    printf("================================================\n");
    print_current_state();
    calculate_need();
    calculate_available();
    check_safe_state();
    printf("\n\n========== TESTING P3's REQUEST ==========\n");
    printf("P3 requests 1 more instance each of R2 and R4\n");
    int request[NUM_RESOURCES] = {0, 1, 0, 1};  
    request_resources(2, request);  
    printf("\n\nPress Enter to exit...");
    getchar();
    getchar();
    
    return 0;
}
