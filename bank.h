//
// Created by murage on 4/17/25.
//

#ifndef BANK_H
#define BANK_H
#include <stdbool.h>

void bank_menu();
void account_login();
bool check_password(long int accountNumber, const char *inputPassword);
int withdraw_money(long int accountNumber);
void account_menu(long int accountNumber);
int withdraw_money(long int accountNumber);
int check_balance(long int accountNumber);
int transfer_money(long int accountNumber);
int deposit_money(long int accountNumber);


#endif //BANK_H
