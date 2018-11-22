#include "../include/encryptolib.h"
#include "../include/extralib.h"
#include "../include/cryptolib.h"
#include "../include/hashtab.h"
#include "../include/emoneylib.h"
#include "../include/colorlib.h"
#include "../include/md5.h"

void bank_startup()
{
  unsigned long int Phi, euclid_res[3];
  generate_prime_number(1, MAXINT, &BANK.P);
  generate_prime_number(1, MAXINT, &BANK.Q);
  BANK.N = BANK.P * BANK.Q;
  Phi = (BANK.P - 1) * (BANK.Q - 1);

  do {
    generate_prime_number(1, BANK.P - 1, &BANK.c);//BANK.c = generate_mutually_prime_number(Phi, 1, Phi - 1);
    Phi > BANK.c ? euclid(abs(Phi), abs(BANK.c), euclid_res) : euclid(abs(BANK.c), abs(Phi), euclid_res);
    BANK.d = euclid_res[0];
  } while (BANK.d > 0xFFFFFF);

  printf("%s[DEBUG]%s\tGenerated next values:\n", BLUE, RESET);
  printf("P = %lu\tQ = %lu\tN = %lu\nC = %lu\tD = %lu\n", BANK.P, BANK.Q, BANK.N, BANK.c, BANK.d);
}

int oneside_encryption()
{
  uint8_t res[16];
  uint8_t arr[1];
  unsigned long int hash = 0;
  md5(arr, 1, res);
  for (int i = 0; i < 16; ++i)
    hash += res[i] << 4;
  return hash;
}

int initialization_transaction(int *account_state, int payment_amount)
{
  unsigned long int pseudo_banknote, pseudo_sign, encrypted_banknote, r, r_rev, euclid_res[3];

  generate_prime_number(2, BANK.N - 1, &EMONEY.banknote);
  r = generate_mutually_prime_number(BANK.N, 1, BANK.N - 1);
  encrypted_banknote = oneside_encryption();
  expmod_func(r, BANK.d, BANK.N, &pseudo_banknote);
  pseudo_banknote = (encrypted_banknote * pseudo_banknote) % BANK.N;
  expmod_func(pseudo_banknote, BANK.c, BANK.N, &pseudo_sign);
  euclid(r, BANK.N, euclid_res);
  r_rev = euclid_res[1];
  if (r_rev < 0) r_rev = r_rev + BANK.N;
  EMONEY.banksign = (pseudo_sign * r_rev) % BANK.N;
  expmod_func(encrypted_banknote, BANK.d, BANK.N, &EMONEY.banksign);

  if (pseudo_sign == EMONEY.banksign) {
    *account_state -= payment_amount;
    if (*account_state >= 0) {
      return 0;
    } else return 1;
  } else {
    printf("%s[BANK]%s Wrong banknote sign!\n", RED, RESET);
    return 1;
  }
}
