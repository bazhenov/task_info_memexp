#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <xpc/xpc.h>
#include <mach/mach_time.h>

extern void *sysmon_request_create(int64_t type, void*);
extern void sysmon_request_add_attribute(void *request, int64_t attribute);
extern void sysmon_request_execute(void *request);
extern void sysmon_request_cancel(void *request);
extern void sysmon_request_set_interval(void *request, int64_t delay);

extern int64_t sysmon_table_get_count(void *table);
extern int64_t sysmon_table_get_timestamp(void *table);

int main(void) {
    void *request = sysmon_request_create(2, ^(void *a, void *b) {
        printf("a = %p\n", a);
        printf("b = %p\n", b);
        printf("table size = %lld\n", sysmon_table_get_count(a));
        printf("table timestamp = %lld\n", sysmon_table_get_timestamp(a) / 1000000);
        return;
    });
    sysmon_request_add_attribute(request, 1);
    sysmon_request_add_attribute(request, 19);
    sysmon_request_add_attribute(request, 22);
    // sysmon_request_add_attribute(request, 0);
    sysmon_request_set_interval(request, 1000);

    printf("New request\n");
    sysmon_request_execute(request);
    usleep(1000);


    sleep(10);
    // printf("New request\n");
    // sysmon_request_execute(request);
    usleep(1000);

    // usleep(1000);
}
