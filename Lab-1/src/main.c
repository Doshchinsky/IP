#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "../include/cryptolib.h"
#include "../include/extralib.h"
#include "../include/hashtab.h"

int main(int argc, char const *argv[])
{
	unsigned long long int fem_base = 2;
	unsigned long long int fem_exponent = 5;
	unsigned long long int fem_module = 23;

	unsigned long long int euclid_a = 150;
	unsigned long long int euclid_b = 180;

	unsigned long long int bgs_base = 2;
	unsigned long long int bgs_module = 23;
	unsigned long long int bgs_answer = 9;

	unsigned long long int KeyA = 5;
	unsigned long long int KeyB = 5;

	unsigned long long int fem_res = 1;
	unsigned long long int euclid_res[3];
	unsigned long long int bgs_res = 0;

	if (argc < 4)
		fprintf(stderr, "Not enough arguments for FEM func!\n");
	else {
		fem_base = atoi(argv[1]);
		fem_exponent = atoi(argv[2]);
		fem_module = atoi(argv[3]);
	}

	if (argc < 6)
		fprintf(stderr, "Not enough arguments for GEA func!\n");
	else {
		euclid_a = atoi(argv[4]);
		euclid_b = atoi(argv[5]);
	}

	if (argc < 9)
		fprintf(stderr, "Not enough arguments for BGS func!\nDefault values are set!\n\n");
	else {
		bgs_base = atoi(argv[6]);
		bgs_module = atoi(argv[7]);
		bgs_answer = atoi(argv[8]);
	}

	expmod_func(fem_base, fem_exponent, fem_module, &fem_res);
	euclid_a > euclid_b ? euclid(abs(euclid_a), abs(euclid_b), euclid_res) : euclid(abs(euclid_b), abs(euclid_a), euclid_res);
	diffyhellman(&KeyA, &KeyB);
	babygiant_steps(bgs_base, bgs_module, bgs_answer, &bgs_res);

	printf("FEM's result:\t%lld\n", fem_res);
	printf("GEA's result:\t%lld, %lld and their GCD: %lld\n", euclid_res[0], euclid_res[1], euclid_res[2]);
	printf("DH's result:\t%lld = %lld\n", KeyA, KeyB);
	printf("BGS's result:\t%lld\n", bgs_res);

	return EXIT_SUCCESS;
}