.PHONY : default
default : library executable

.PHONY : clean
clean :
	rm -f libstacklog.a
	rm -f stack-log
	rm -f stack-log.o

library : libstacklog.a

libstacklog.a : stack-log.c stack-log.h
	$(CC) -c stack-log.c -I. -o stack-log.o
	$(AR) -qc libstacklog.a stack-log.o

executable : stack-log

stack-log : main.c stack-log.h
	$(CC) main.c -I. -o stack-log
