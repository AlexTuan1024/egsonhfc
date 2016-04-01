encrypt.o:encrypt.c encrypt.h
	gcc -c encrypt.c
checksum.o:checksum.c checksum.h
	gcc -c checksum.c
testrun.o:testrun.c
	gcc -c testrun.c
libhfsec.a:encrypt.o checksum.o
	ar -rcs stlib/libhfsec.a encrypt.o checksum.o
testrun:testrun.o libhfsec.a
	gcc testrun.o -L./stlib -lhfsec -o testrun
