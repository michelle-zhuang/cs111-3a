# NAME: Miles Kang
# EMAIL: milesjkang@gmail.com
# ID: 405106565

default:
	gcc -Wall -Wextra -g -o lab3a lab3a.c
clean:
	rm -f lab3a lab3a-405106565.tar.gz
dist:
	tar -czvf lab0-405106565.tar.gz lab3a.c Makefile README
