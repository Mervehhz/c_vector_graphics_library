target: a.out

a.out: test_prog.c eps.c epslib.h
		   gcc test_prog.c eps.c -lm
		   ./a.out