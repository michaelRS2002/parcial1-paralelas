all: compile exe clean

compile:
	gcc -o main main.c -fopenmp

exe:
	time ./all.sh ./fotos 1
	time ./all.sh ./fotos 12
	time ./all.sh ./fotos 24

clean:
	rm -f main *.bin* fotos/*.bin fotos/*.new
