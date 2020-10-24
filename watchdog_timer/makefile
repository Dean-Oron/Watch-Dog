
FUNCDEF = watchdog.c
WATCHDOG = watchdog_process.c

DEPS = ./include/*.h
DSPATH = -I ./include

VALGRIND = valgrind --leak-check=yes --track-origins=yes

CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g			# For Debbuging
LDFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 # Not Debbug

watchdog_test: libwatchdog.so watchdog_process $(DEPS) watchdog_test.c
	@$(CC) -L./lib -Wl,-rpath=./lib $(CFLAGS) $(DSPATH) watchdog_test.c -o $@ -lwatchdog -lpthread -lds -lm

watchdog_process: $(WATCHDOG) $(DEPS) libwatchdog.so
	@$(CC) -L./lib -Wl,-rpath=../lib $(DSPATH) $(CFLAGS) $(WATCHDOG) -o $@ -lwatchdog -lpthread -lds -lm
	@mv $@ ./bin

libwatchdog.so: $(FUNCDEF) $(DEPS)
	@$(CC) $(CFLAGS) $(DSPATH) $(FUNCDEF) -fpic -shared -o $@
	@mv $@ ./lib

.PHONY: clean

