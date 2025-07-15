#include <mach/mach_time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <xpc/xpc.h>
#include <pthread.h>

extern void *sysmon_request_create(int64_t type, void *);
extern void sysmon_request_add_attribute(void *request, int64_t attribute);
extern void sysmon_request_execute(void *request);
extern void sysmon_request_cancel(void *request);
extern void sysmon_request_set_interval(void *request, int64_t delay);

extern int64_t sysmon_table_get_count(void *table);
extern int64_t sysmon_table_get_timestamp(void *table);
extern void *sysmon_table_get_row(void *table, int64_t row);

extern void *sysmon_row_get_value(void *row, int64_t value);

#define SYSMON_MSG_TYPE_PROCESS_REQUEST 1
#define SYSMON_MSG_TYPE_SYSTEM_REQUEST 2
#define SYSMON_MSG_TYPE_COALITION_REQUEST 3

#define SYSMON_ATTR_PROCESS_PID 0x4
#define SYSMON_ATTR_PROCESS_COMM 0xC
#define SYSMON_ATTR_PROCESS_REAL_MEMORY 0x16
#define SYSMON_ATTR_PROCESS_MEMORY 0x42

#define MEGABYTE_FLOAT (1024.0 * 1024.0)

int64_t attributes[] = {
    SYSMON_ATTR_PROCESS_PID,
    SYSMON_ATTR_PROCESS_COMM,
    SYSMON_ATTR_PROCESS_REAL_MEMORY,
    SYSMON_ATTR_PROCESS_MEMORY,
};

int main(void) {
    // Mutex and conditional variable required to synchronize main thread until callback is executed
    __block pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    __block pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    __block bool callback_finished = false;

    void *request = sysmon_request_create(SYSMON_MSG_TYPE_PROCESS_REQUEST, ^(void *table, void *b) {
        int64_t rows = sysmon_table_get_count(table);
        printf("%5s %-20s %13s %13s\n", "PID", "comm", "Memory", "Real Memory");
        for (int64_t i = 0; i < rows; i++) {
            void *row = sysmon_table_get_row(table, i);
            uint64_t pid = xpc_uint64_get_value(sysmon_row_get_value(row, SYSMON_ATTR_PROCESS_PID));
            uint64_t memory = xpc_uint64_get_value(sysmon_row_get_value(row, SYSMON_ATTR_PROCESS_MEMORY));
            uint64_t real_memory = xpc_uint64_get_value(sysmon_row_get_value(row, SYSMON_ATTR_PROCESS_REAL_MEMORY));
            const char *comm = xpc_string_get_string_ptr(sysmon_row_get_value(row, SYSMON_ATTR_PROCESS_COMM));
            printf("%5lld %-20s %12.1fM %12.1fM\n", pid, comm, memory / MEGABYTE_FLOAT, real_memory / MEGABYTE_FLOAT);
        }

        // Notifying main thread that callback was successfully finished
    	pthread_mutex_lock(&mutex);
    	callback_finished = true;
    	pthread_cond_signal(&cond);
    	pthread_mutex_unlock(&mutex);
    });

    for (int i = 0; i < sizeof(attributes) / sizeof(attributes[0]); i++) {
        sysmon_request_add_attribute(request, attributes[i]);
    }
    sysmon_request_execute(request);

    // Waiting until callback is executed
    pthread_mutex_lock(&mutex);
    while (!callback_finished) {
       	pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    return 0;
}
