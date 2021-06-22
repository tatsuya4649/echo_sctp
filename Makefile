CC:=gcc
SERVER:=server
.PHONY: all clean
LIBRARY=sctp
OBJ:=obj/
BIN:=bin/

all: ${addprefix ${OBJ},${addsuffix .o,${SERVER}}}
	${CC} ${addprefix ${OBJ},${addsuffix .o,${SERVER}}} -o ${BIN}${SERVER} -l${LIBRARY}

${OBJ}%.o: %.c
	${CC} -c $< -o $@

clean:
	rm -rf bin/* obj/*
