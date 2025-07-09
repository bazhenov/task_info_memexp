#include <stdio.h>
#include <stdlib.h>
#include <mach/task_info.h>
#include <sys/proc_info.h>
#include <libproc.h>

const char* MEMORY_UNITS[] = { "B", "KB", "MB", "GB", "TB" };

typedef struct {
    float memory_usage;
    const char* units;
} formatted_memory;

/// Formats a memory size into a human-readable string.
void format_memory(uint64_t bytes, formatted_memory *result) {
    float mem_size = (float)bytes;
    size_t order = 0;
    size_t max_order = sizeof(MEMORY_UNITS) / sizeof(MEMORY_UNITS[0]);
    while (mem_size >= 1024.0 && order < max_order - 1) {
        mem_size /= 1024.0;
        order++;
    }
    result->memory_usage = mem_size;
    result->units = MEMORY_UNITS[order];
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    int pid = atoi(argv[1]);
    if (pid <= 0) {
        printf("Invalid PID: %s\n", argv[1]);
        return 1;
    }

    struct proc_taskinfo tinfo;
    int ret = proc_pidinfo(pid, PROC_PIDTASKINFO, 0, (task_info_t)&tinfo, PROC_PIDTASKINFO_SIZE);
    if (ret != PROC_PIDTASKINFO_SIZE) {
        perror("Failed to get task info");
        return 1;
    }

    struct rusage_info_v6 rusage;
    if (proc_pid_rusage(pid, RUSAGE_INFO_V6, (rusage_info_t)&rusage) != KERN_SUCCESS) {
        perror("Failed to get rusage info");
        return 1;
    }

    formatted_memory mem;

    format_memory(tinfo.pti_virtual_size, &mem);
    printf("Virtual size: %.2f %s\n", mem.memory_usage, mem.units);

    format_memory(rusage.ri_resident_size, &mem);
    printf("Real memory: %.2f %s\n", mem.memory_usage, mem.units);

    format_memory(rusage.ri_phys_footprint, &mem);
    printf("Memory: %.2f %s\n", mem.memory_usage, mem.units);
    return 0;
}
