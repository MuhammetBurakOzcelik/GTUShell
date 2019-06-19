start: du cat lsf main pwd wc

main: main.c
	gcc -c main.c -o gtushell.o
	gcc gtushell.o -o gtushell
	
du: du.c
	gcc -c du.c -o bunedu.o
	gcc bunedu.o -o bunedu

cat: cat.c
	gcc -c cat.c -o cat.o
	gcc cat.o -o cat

lsf: lsf.c
	gcc -c lsf.c -o lsf.o
	gcc lsf.o -o lsf

pwd: pwd.c
	gcc -c pwd.c -o pwd.o
	gcc pwd.o -o pwd

wc: wc.c
	gcc -c wc.c -o wc.o
	gcc wc.o -o wc

clean: 
	rm *.o gtushell
	clear
