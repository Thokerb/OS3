CFLAGS = -std=gnu11 -Wall  -Wextra
LBLIBS = -lrt

.PHONY: all clean

all: task1 task2 task3

clean:
	$(RM) task1 task2 task3

task1: task1_client.c task1_server.c
task2: task2.c
task3: task3_client.c task3_server.c
