# Author: Adam Henry, adlhenry@ucsc.edu

CC			= cc -g -O0 -Wall -Wextra -std=c11
PSOURCE		= encrypt.c rijndael.c
SSOURCE   	= setkey.c
TEST		= test
POBJECTS	= ${PSOURCE:.c=.o}
SOBJECTS	= ${SSOURCE:.c=.o}
PEXECBIN	= protectfile
SEXECBIN	= setkey
EXECBINS	= ${PEXECBIN} ${SEXECBIN}

all : ${EXECBINS}

${PEXECBIN} : ${POBJECTS}
	${CC} -o $@ ${POBJECTS}

${SEXECBIN} : ${SOBJECTS}
	${CC} -o $@ ${SOBJECTS}

%.o : %.c
	${CC} -c $<

rijndael.o: rijndael.h

clean :
	- rm ${POBJECTS} ${SOBJECTS}

test: all
	cp ${TEST} ${TEST}.orig
	./${EXECBIN} -e 0123456789abcdef ${TEST}
	./${EXECBIN} -d 0123456789abcdef ${TEST}
	diff ${TEST} ${TEST}.orig
