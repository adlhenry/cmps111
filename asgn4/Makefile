# Author: Adam Henry, adlhenry@ucsc.edu

CC        = cc -g -O0 -Wall -Wextra -std=c11
SOURCE    = encrypt.c rijndael.c
HEADER    = rijndael.h
TEST      = test
OBJECTS   = ${SOURCE:.c=.o}
EXECBIN   = protectfile

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CC} -o $@ ${OBJECTS}

%.o : %.c
	${CC} -c $<

rijndael.o: rijndael.h

clean :
	- rm ${OBJECTS}

test: all
	cp ${TEST} ${TEST}.orig
	./${EXECBIN} 0123456789abcdef ${TEST}
	./${EXECBIN} 0123456789abcdef ${TEST}
	diff ${TEST} ${TEST}.orig
