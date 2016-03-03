# Author: Adam Henry, adlhenry@ucsc.edu

CC        = cc -g -O0 -Wall -Wextra -std=c11
SOURCE    = encrypt.c rijndael.c
HEADER    = rijndael.h
TEST      = test
OBJECTS   = ${SOURCE:.c=.o}
LREPORT   = /dev/null
EXECBIN   = protectfile

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CC} -o $@ ${OBJECTS} -lfl

%.o : %.c
	${CC} -c $<

rijndael.o: rijndael.h

clean :
	- rm ${OBJECTS}

test: all
	cp ${TEST} ${TEST}.orig
	./${EXECBIN} -e 0x1234 0x5678 ${TEST}
	./${EXECBIN} -d 0x1234 0x5678 ${TEST}
	diff ${TEST} ${TEST}.orig
