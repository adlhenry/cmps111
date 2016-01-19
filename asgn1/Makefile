# Author: Adam Henry, adlhenry@ucsc.edu

CC        = cc -g -O0 -Wall -Wextra -std=c11
SOURCE    = myshell.c
LSOURCE   = lex.l
LGEN      = lex.yy.c
LGENO     = ${LGEN:.c=.o}
OBJECTS   = ${SOURCE:.c=.o} ${LGENO}
LREPORT   = lex.err
LCREPORT  = lexc.err
EXECBIN   = myshell

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CC} -o $@ ${OBJECTS} -lfl

%.o : %.c
	${CC} -c $<

${LGENO} : ${LGEN}
	${CC} -c $< 2>${LCREPORT}

${LGEN} : ${LSOURCE}
	flex --outfile=${LGEN} ${LSOURCE} 2>${LREPORT}

clean :
	- rm ${OBJECTS} ${LGEN} ${LREPORT} ${LCREPORT}
