#include "stack-log.h"

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
	int log = open("stack.log", O_RDONLY);
	if (log == -1) {
		fprintf(stderr, "cannot open log: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	stack_log_entry_t entry;
	while (read(log, &entry, sizeof(entry)))
		stack_log_print_entry(&entry);
	return EXIT_SUCCESS;
}

const char *stack_log_event_type_name(stack_log_event_t event_type) {
	switch (event_type) {
	case STACK_LOG_EVENT_ENTER:
		return "enter";
	case STACK_LOG_EVENT_EXIT:
		return "exit";
	}
}

void stack_log_print_entry(stack_log_entry_t *entry) {
	printf(
		"%"PRId64"\t%s\t%p\t%p",
		entry->timestamp,
		stack_log_event_type_name(entry->event_type),
		(void *)entry->from,
		(void *)entry->to);
}
