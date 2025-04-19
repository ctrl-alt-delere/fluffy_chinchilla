//
// Created by murage on 4/1/25.
//

#ifndef ACOOUNT_H
#define ACOOUNT_H

typedef struct {
    long int accountNumber;
    char name[50];
    double balance;
    char address[50];
    long Y_O_B, age, iden_card_no, kra_pin;
    char gender;
    char account_password[50];
} Account;


extern char admin_password[50];



int count_num(long int num);
int account_creation_information_collection();
void loadAccount(int accountNumber);
void capitalizeString(char *str);
int accountExists(int accountNumber);
void saveAccountToFile(Account *acc);
int generateUniqueAccountNumber();
void delete_account(long int accountNumber);
void load_all_accounts();


#endif //ACOOUNT_H
