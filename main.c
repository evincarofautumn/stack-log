#include "stack-log.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *stack_log_event_type_name(stack_log_event_t);
void stack_log_print_entry(stack_log_entry_t *);

int main(int argc, char **argv) {
	if (argc > 2) {
		fprintf(stderr, "usage: %s [stack.log]\n", argv[0]);
		return EXIT_FAILURE;
	}
	const char *filename = argc == 2 ? argv[1] : "stack.log";
	int log = open(filename, O_RDONLY);
	if (log == -1) {
		fprintf(stderr, "cannot open log: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	stack_log_entry_t entry;
	while (1) {
		int status = read(log, (void *)&entry, sizeof(entry));
		if (status != sizeof(entry))
			break;
		stack_log_print_entry(&entry);
	}
	return EXIT_SUCCESS;
}

const char *stack_log_event_type_name(stack_log_event_t type) {
	switch (type) {
	case STACK_LOG_EVENT_ENTER:
		return "enter";
	case STACK_LOG_EVENT_EXIT:
		return "exit";
	default:
		assert(0);
	}
}

void stack_log_print_entry(stack_log_entry_t *entry) {
	printf(
		"%.6f\t%s\t%p\t%p\n",
		(double)entry->seconds + (double)entry->microseconds / 10e6,
		stack_log_event_type_name(entry->type),
		(void *)entry->from,
		(void *)entry->to);
}
