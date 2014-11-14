#ifndef STACK_LOG_H_FF02829C_100E_4C47_B119_1986906B9954
#define STACK_LOG_H_FF02829C_100E_4C47_B119_1986906B9954
#include <stdint.h>

typedef uint64_t stack_log_cell_t;

typedef struct {
	stack_log_cell_t timestamp, event_type, from, to;
} stack_log_entry_t;

typedef enum {
	STACK_LOG_EVENT_ENTER = 0,
	STACK_LOG_EVENT_EXIT = 1,
} stack_log_event_t;

#endif
