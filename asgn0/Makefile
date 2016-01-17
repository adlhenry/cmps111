# Author: Adam Henry, adlhenry@ucsc.edu

CC       = cc -g -O0 -Wall -Wextra -std=c11
SOURCE    = cat.c
OBJECTS   = ${SOURCE:.c=.o}
EXECBIN   = cat

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CC} -o $@ ${OBJECTS}

%.o : %.c
	${CC} -c $<

clean :
	- rm ${OBJECTS}
