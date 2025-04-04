CC = gcc -D_GNU_SOURCE

CFLAGS = -Wall -Werror -std=c99

OBJ = main.o perProcess_table.o sysWide_table.o Vnode_table.o composite.o summary_table.o threshold_flag.o

.PHONY: all
all: showFDtables

showFDtables: ${OBJ}
	${CC} ${CFLAGS} -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY: clean
clean: 
	rm -f ${OBJ} showFDtables compositeTable.txt compositeTable.bin

	 