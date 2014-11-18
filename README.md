Generates a log of the stack at all points in a program.

`make` builds a library (`libstacklog.a`) and an executable (`stack-log`).

Compile your program with `-finstrument-functions` and link with `-lstacklog`. When you run your program, a binary file named `stack.log` will be written. Run `stack-log` to render this file as text. Each line contains the timestamp (microseconds since the Unix epoch), whether the transfer was a function `entry` or `exit`, the address that the program just left, and the address at which the program just arrived. These addresses can be given to `addr2line` to extract function names.
