#include "stack-log.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

static int stack_log;

void __attribute__ ((constructor)) stack_log_begin(void) {
	stack_log = open("stack.log", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (stack_log == -1)
		stack_log = 0;
}
 
void __attribute__ ((destructor)) stack_log_end(void) {
	if (stack_log)
		close(stack_log);
}

void __cyg_profile_func_enter(void *callee, void *caller) {
	if (!stack_log)
		return;
	struct timeval time;
	gettimeofday(&time, NULL);
	const stack_log_entry_t entry = {
		(stack_log_cell_t)time.tv_sec,
		(stack_log_cell_t)time.tv_usec,
		(stack_log_cell_t)pthread_self(),
		STACK_LOG_EVENT_ENTER,
		(stack_log_cell_t)caller,
		(stack_log_cell_t)callee
	};
	ssize_t status = write(stack_log, (const void *)&entry, sizeof(entry));
	if (status == -1) {
		close(stack_log);
		stack_log = 0;
	}
}

void __cyg_profile_func_exit(void *callee, void *caller) {
	if (!stack_log)
		return;
	struct timeval time;
	gettimeofday(&time, NULL);
	const stack_log_entry_t entry = {
		(stack_log_cell_t)time.tv_sec,
		(stack_log_cell_t)time.tv_usec,
		(stack_log_cell_t)pthread_self(),
		STACK_LOG_EVENT_EXIT,
		(stack_log_cell_t)callee,
		(stack_log_cell_t)caller
	};
	ssize_t status = write(stack_log, (const void *)&entry, sizeof(entry));
	if (status == -1) {
		close(stack_log);
		stack_log = 0;
	}
}
