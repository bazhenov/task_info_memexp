#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xpc/xpc.h>

#define SYSMON_XPC_SERVICE_NAME "com.apple.sysmond"
#define SYSMON_XPC_KEY_ATTRIBUTES "Attributes"
#define SYSMON_XPC_KEY_FLAGS "Flags"
#define SYSMON_XPC_KEY_TYPE "Type"
#define SYSMON_XPC_KEY_HEADER "Header"
#define SYSMON_XPC_KEY_TABLE "Table"

#define SYSMON_MSG_TYPE_PROCESS_REQUEST 1
#define SYSMON_MSG_TYPE_SYSTEM_REQUEST 2
#define SYSMON_MSG_TYPE_COALITION_REQUEST 3

#define SYSMON_ATTR_PROCESS_PID 0x4
#define SYSMON_ATTR_PROCESS_COMM 0xC
#define SYSMON_ATTR_PROCESS_MEMORY 0x16

#define SYSMON_ATTR_PROCESS_REAL_MEMORY 0x42

#define MEGABYTE_FLOAT (1024.0 * 1024.0)

char *create_attributes(int *attrs, size_t attr_len, size_t *bytes_len) {
  int max_attribute = 0;
  for (size_t i = 0; i < attr_len; i++) {
    if (attrs[i] > max_attribute) {
      max_attribute = attrs[i];
    }
  }
  *bytes_len = (max_attribute + 8) / 8;
  char *bytes = malloc(*bytes_len);
  memset(bytes, 0, *bytes_len);
  for (size_t i = 0; i < attr_len; i++) {
    bytes[attrs[i] / 8] |= 1 << (attrs[i] % 8);
  }
  return bytes;
}

int main() {
  size_t attr_buffer_len = 0;
  int attrs[] = {SYSMON_ATTR_PROCESS_PID, SYSMON_ATTR_PROCESS_COMM,
                 SYSMON_ATTR_PROCESS_MEMORY, SYSMON_ATTR_PROCESS_REAL_MEMORY};
  char *attr_buffer = create_attributes(attrs, sizeof(attrs) / sizeof(attrs[0]),
                                        &attr_buffer_len);

  xpc_object_t msg = xpc_dictionary_create(NULL, NULL, 0);
  xpc_dictionary_set_uint64(msg, SYSMON_XPC_KEY_TYPE,
                            SYSMON_MSG_TYPE_PROCESS_REQUEST);
  xpc_dictionary_set_uint64(msg, SYSMON_XPC_KEY_FLAGS, 0);
  xpc_dictionary_set_data(msg, SYSMON_XPC_KEY_ATTRIBUTES, attr_buffer,
                          attr_buffer_len);

  xpc_connection_t conn =
      xpc_connection_create_mach_service(SYSMON_XPC_SERVICE_NAME, NULL, 0);
  if (conn == NULL) {
    perror("xpc_connection_create_mach_service");
    return (1);
  }

  __block pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  __block pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  __block bool callback_finished = false;

  xpc_connection_set_event_handler(conn, ^(xpc_object_t obj) {
    xpc_object_t table = xpc_dictionary_get_array(obj, SYSMON_XPC_KEY_TABLE);

    size_t rows = xpc_array_get_count(table);
    printf("%5s %-20s %13s %13s\n", "PID", "comm", "Memory", "Real Memory");
    for (size_t i = 0; i < rows; i++) {
      xpc_object_t row = xpc_array_get_value(table, i);

      uint64_t pid = xpc_array_get_uint64(row, 0);
      const char *comm = xpc_array_get_string(row, 1);
      uint64_t memory = xpc_array_get_uint64(row, 2);
      uint64_t real_memory = xpc_array_get_uint64(row, 3);

      printf("%5lld %-20s %12.1fM %12.1fM\n", pid, comm,
             memory / MEGABYTE_FLOAT, real_memory / MEGABYTE_FLOAT);
    }

    // Notifying main thread that callback was successfully finished
    pthread_mutex_lock(&mutex);
    callback_finished = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  });

  xpc_connection_resume(conn);
  xpc_connection_send_message(conn, msg);

  // Waiting until callback is executed
  pthread_mutex_lock(&mutex);
  while (!callback_finished) {
    pthread_cond_wait(&cond, &mutex);
  }
  pthread_mutex_unlock(&mutex);
}
