CC=gcc

lab1p1: 
	$(CC) -g lab1Struct.c -o lab1p1

lab1p2: labFileIO.c
	$(CC) -g -std=c99 labFileIO.c -o lab1p2 -lcrypt

lab1p3: 
	$(CC) -g lab1BitwiseOp.c -o lab1p3
