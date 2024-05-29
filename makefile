index:
	gcc -o executavel Arquivos-c/*.c
	clear
	./executavel

testes:
	@if [ -f executavel ]; then \
		rm executavel; \
	fi
	clear

	gcc -o executavel Arquivos-c/*.c -g
	./executavel

clear:
	@if [ -f executavel ]; then \
		rm executavel; \
	fi
	clear
