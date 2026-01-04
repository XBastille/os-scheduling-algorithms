# OS Scheduling Algorithms

C implementations of fundamental operating system scheduling and deadlock avoidance algorithms.

## Contents

- **CPU Scheduler** - Simulates process scheduling with FCFS and Round Robin algorithms
- **Banker's Algorithm** - Demonstrates deadlock avoidance using resource allocation safety checks

## Features

### CPU Scheduler (`cpu_scheduler.c`)
- First Come First Serve (FCFS) scheduling
- Round Robin (RR) with configurable time quantum
- Gantt chart visualization
- Calculates waiting time, turnaround time, and response time
- Displays average performance metrics

### Banker's Algorithm (`bankers_algorithm.c`)
- Deadlock detection and avoidance
- Safety algorithm implementation
- Resource request validation
- Safe sequence generation
- Need matrix calculation

## Compilation & Execution

```bash
# Compile CPU Scheduler
gcc cpu_scheduler.c -o cpu_scheduler
./cpu_scheduler

# Compile Banker's Algorithm
gcc bankers_algorithm.c -o bankers_algorithm
./bankers_algorithm
```

## Example Output

Both programs provide detailed output including:
- Process execution timelines
- Resource allocation states
- Performance metrics and analysis

## Tags

`operating-systems` `scheduling-algorithms` `fcfs` `round-robin` `bankers-algorithm` `deadlock-avoidance` `c-programming` `cpu-scheduling` `process-management` `resource-allocation`

## License

Open source - feel free to use for educational purposes.
