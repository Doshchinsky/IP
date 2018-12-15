#ifndef EMONEYLIB_H
#define EMONEYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

struct data_BANK {
    unsigned long int P;
    unsigned long int Q;
    unsigned long int N;
    unsigned long int c;
    unsigned long int d;
} BANK;

struct electronic_money {
  unsigned long int banknote;
  unsigned long int banksign;
} EMONEY;

void bank_startup();
int oneside_encryption();
int initialization_transaction(int *account_state, int payment_amount);

#endif
