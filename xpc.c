#include <stdio.h>
#include <unistd.h>
#include <xpc/xpc.h>

#define SYSMON_XPC_SERVICE_NAME "com.apple.sysmond"
#define SYSMON_XPC_KEY_ATTRIBUTES "Attributes"
#define SYSMON_XPC_KEY_FLAGS "Flags"
#define SYSMON_XPC_KEY_TYPE "Type"
#define SYSMON_XPC_KEY_HEADER "Header"
#define SYSMON_XPC_KEY_TABLE "Table"

int main() {
    xpc_connection_t conn;
    xpc_object_t msg;

    msg = xpc_dictionary_create(NULL, NULL, 0);
	xpc_dictionary_set_uint64(msg, SYSMON_XPC_KEY_TYPE, 1);
	xpc_dictionary_set_uint64(msg, SYSMON_XPC_KEY_FLAGS, 0);
	char attributes[] = {};
	xpc_dictionary_set_data(msg, SYSMON_XPC_KEY_ATTRIBUTES, attributes, sizeof(attributes));

    conn = xpc_connection_create_mach_service(SYSMON_XPC_SERVICE_NAME, NULL, 0);
	if (conn == NULL) {
		perror("xpc_connection_create_mach_service");
		return (1);
	}

	printf("Connected\n");

	xpc_connection_set_event_handler(conn, ^(xpc_object_t obj) {
		printf("Received message event handler: %p\n", obj);
		printf("%s\n", xpc_copy_description(obj));
	});

	xpc_connection_resume(conn);
	xpc_connection_send_message(conn, msg);

	while (true) {
	   sleep(1000);
	}
}
