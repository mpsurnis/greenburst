all: udp2fil.c
	gcc -pthread -I /usr/local/include/hiredis -g -Wall -o udp2fil udp2fil.c -lm -lhiredis

clean: 
	$(RM) udp2fil
