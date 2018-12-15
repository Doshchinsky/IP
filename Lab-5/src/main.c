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

#include "../include/colorlib.h"
#include "../include/extralib.h"
#include "../include/emoneylib.h"

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  if (argc < 2) {
    fprintf(stderr, "%s[ERROR]%s Not enough arguments!\n", RED, RESET);
    exit(EXIT_FAILURE);
  }
  int account_state = atoi(argv[1]);
  printf("%s[CUSTOMER]%s %sStarting balance:%s\t%s%d%s\n", YELLOW, RESET, WHITE, RESET, GREEN, account_state, RESET);
  int payment_amount;

  bank_startup();

  while (account_state > 0) {
    printf("%s[BANK]%s Enter the payment amount:\n", YELLOW, RESET);
    scanf("%d", &payment_amount);
    if (payment_amount == 0) {
      printf("%s[BANK]%s Recieved zero payment! Exiting now...\n", BLUE, RESET);
      break;
    }
    printf("%s[BANK]%s Starting transaction...\n", YELLOW, RESET);
    if(!initialization_transaction(&account_state, payment_amount)) {
      printf("%s[SHOP]%s Item was successfully purchased!\n", GREEN, RESET);
    } else {
      printf("%s[SHOP]%s Payment rejected!\n", RED, RESET);
      break;
    }
    account_state == 0 ? printf("%s[CUSTOMER]%s Out of money. Exiting now...\n", GREEN, RESET) : printf("%s[CUSTOMER]%s %sCurrent account state:%s\t%s%d%s\n\n", YELLOW, RESET, WHITE, RESET, GREEN, account_state, RESET);
  }

  return EXIT_SUCCESS;
}
