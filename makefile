prog :codage_decodage_char.o compress_decompress.o shannon_fano.o main.o 
	gcc  main.c  codage_decodage_char.o compress_decompress.o shannon_fano.o -lm -o mini_compresseur
	rm -f *.o
main.o : main.c compress_decompress.h shannon_fano.h
	gcc -c main.c
codage_decodage_char.o : codage_decodage_char.c codage_decodage_char.h
	gcc -c codage_decodage_char.c
compress_decompress.o : compress_decompress.c codage_decodage_char.h  compress_decompress.h shannon_fano.h 
	gcc -c compress_decompress.c
shannon_fano.o : shannon_fano.c  shannon_fano.h
	gcc -c shannon_fano.c
	