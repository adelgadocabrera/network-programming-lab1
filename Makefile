CC=gcc

part1: 
	$(CC) -g part1.c -o part1

part2: 
	$(CC) -g -std=c99 part2.c -o part2 -lcrypt

part3: 
	$(CC) -g part3.c -o part3

clean:
	rm part1 part2 part3 
