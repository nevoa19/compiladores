all:
	flex valirian.l
	bison -d valirian.y -Wcounterexamples
	g++ *.c -o valirian