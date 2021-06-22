CC:=gcc
SERVER:=server
CLIENT:=client
.PHONY: all clean
LIBRARY=sctp
OBJ:=obj/
BIN:=bin/

all: ${addprefix ${OBJ},${addsuffix .o,${SERVER}}} ${addprefix ${OBJ},${addsuffix .o,${CLIENT}}}
	${CC} ${addprefix ${OBJ},${addsuffix .o,${SERVER}}} -o ${BIN}${SERVER} -l${LIBRARY}
	${CC} ${addprefix ${OBJ},${addsuffix .o,${CLIENT}}} -o ${BIN}${CLIENT} -l${LIBRARY}


${OBJ}%.o: %.c
	${CC} -c $< -o $@

clean:
	rm -rf bin/* obj/*
