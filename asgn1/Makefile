# Author: Adam Henry, adlhenry@ucsc.edu

CC       = cc -g -O0 -Wall -Wextra -std=c11
SOURCE    = myshell.c
LSOURCE   = lex.l
LGEN      = lex.yy.c
ALLSRC    = ${SOURCE} ${LGEN}
OBJECTS   = ${ALLSRC:.c=.o}
LREPORT   = lex.err
EXECBIN   = myshell

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CC} -o $@ ${OBJECTS}

%.o : %.c
	${CC} -c $<
	
${LGEN} : ${LSOURCE}
	flex --outfile=${LGEN} ${LSOURCE} 2>${LREPORT}

clean :
	- rm ${OBJECTS} ${LGEN} ${LREPORT}
