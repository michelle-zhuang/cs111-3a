# NAME: Miles Kang,Michelle Zhuang

default:
	gcc -Wall -Wextra -g -o lab3a lab3a.c
clean:
	rm -f lab3a lab3a-405106565.tar.gz
dist:
	tar -czvf lab3a-405106565.tar.gz lab3a.c Makefile README ext2_fs.h
