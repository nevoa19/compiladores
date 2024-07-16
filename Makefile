all:
	flex valirian.l
	bison -d valirian.y -Wcounterexamples
	g++ -O0 -g *.c -o valirian