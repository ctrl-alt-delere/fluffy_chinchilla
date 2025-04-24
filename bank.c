#include "bank.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "acoount.h"



char login[50];
long int  Account_number;

void admin_menu() {
    char admin_input1;
    printf("--------------Admin menu-------------");
    printf("1. Delete account\n");
    printf("2. View accounts\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%c", &admin_input1);
    while (getchar() != '\n');

    switch (admin_input1) {
        case '1':
            long int accountNumber;
            printf("Enter account number of the account you want to delete: ");
            scanf("%ld", &accountNumber);
            while (getchar() != '\n');

            delete_account(accountNumber);
            break;
        case '2':
            char admin_pass_input1[50];
            printf("Enter admin passwpord: ");
            scanf("%49s", admin_pass_input1);
            while (getchar() != '\n');

            int attempts = 2;

            while (strcmp(admin_password, admin_pass_input1) != 0 && attempts > 0) {
                printf("Incorrect password.You have %d attempts remaining: " ,attempts);
                scanf("%49s", admin_pass_input1);
                while (getchar() != '\n');

                attempts--;
            }

            if (strcmp(admin_password, admin_pass_input1) == 0) {
                load_all_accounts();
                char input;
                printf("Go back to bank menu? (Y/N): ");
                scanf("%c" ,&input);
                input = toupper(input);
                while (getchar() != '\n');

                if (input == 'Y') {

                    bank_menu();
                }else if (input != 'N' && input != 'Y') {
                    printf("Error: invalid input!");
                    break;
                }else {
                    break;
                }


            }
            break;

        case '3':
            bank_menu();
            break;
    }
}



int deposit_money(long int accountNumber) {
    //opening_file_contents
    char filename[50];
    sprintf(filename, "account_%ld.dat", accountNumber);
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Account not found\n");
        bank_menu();
        return 0;
    }
    Account acc;
    fread(&acc, sizeof(Account), 1, file);

    double deposit_amount;
    printf("Enter the amount you would like to deposit: ");
    scanf("%lf", &deposit_amount);
    while (getchar() != '\n');

    while (deposit_amount <= 0) {
        printf("Cannot deposit less than 0! Please try again: \n");
        scanf("%lf", &deposit_amount);

    }
    //deposit_verification
    char confirm_deposit;
    printf("Deposit %lf shillings to your account?(Y/N): ", deposit_amount);
    scanf(" %c", &confirm_deposit);
    confirm_deposit = toupper(confirm_deposit);

    if (confirm_deposit == 'N') {
        fclose(file);
        account_menu(accountNumber);
        return 0;
    }
    if (confirm_deposit == 'Y') {
        //updating_balance
        acc.balance += deposit_amount;

        //saving_changes
        fseek(file, 0, SEEK_SET);
        fwrite(&acc, sizeof(Account), 1, file);
        fclose(file);

        //deposit_confirmation
        printf("You have successfully deposited %lf shillings to  account no.%ld. New balance is %lf\n", deposit_amount,
               accountNumber, acc.balance);
        account_menu(accountNumber);
        return 0;
    }
    printf("Invalid input\n");
    bank_menu();

    return 0;
}

int transfer_money(long int accountNumber) {
    //opening_the_file_so_contents_of_account_can_be_accessed
    char filename[30];
    sprintf(filename, "account_%ld.dat", accountNumber);
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }
    Account acc_sender;
    fread(&acc_sender, sizeof(Account), 1, file);


    double transfer_amount;
    char transfer_password_input[50];
    //password_verification
    printf("Enter your account password: ");
    scanf("%49s", transfer_password_input);
    while (getchar() != '\n');

    if (!check_password(acc_sender.accountNumber, transfer_password_input)) {
        printf("Invalid account password\n");
        fclose(file);
        bank_menu();
        return 0;
    }

    //transfer_of_money

    long int transfer_account;

    printf("Enter the account number you wish to transfer money to: ");
    scanf("%ld", &transfer_account);
    while (getchar() != '\n');
//checking_if_account_given_exists
    if (!accountExists(transfer_account)) {
        printf("Account not found\n");
        fclose(file);
        bank_menu();
        return 0;
    }

    if (transfer_account == accountNumber) {
        printf("You cannot transfer money to your own account. \n");
        fclose(file);
        account_menu(accountNumber);
        return 0;
    }

//amount_to_be_transferred
    printf("Enter the amount you wish to transfer: ");
    scanf("%lf", &transfer_amount);
    while (getchar() != '\n');

    if (transfer_amount >= acc_sender.balance-1000) {
        printf("Insufficient funds to carry out this transaction.\n");
        fclose(file);
        account_menu(accountNumber);
        return 0;
    }
//final_verification_of_transfer_details
    char transfer_verification;
    printf("Transfer %.2f shillings to account %ld . Correct?(Y/N): ", transfer_amount, transfer_account);
    scanf(" %c", &transfer_verification);
    while (getchar() != '\n');
    transfer_verification = toupper(transfer_verification);

    if (transfer_verification == 'N') {
        char transfer_verification1;
        printf("Transfer cancelled. Would you like to make another transaction? ");
        scanf(" %c", &transfer_verification1);
        while (getchar() != '\n');
        transfer_verification1 = toupper(transfer_verification1);

        if (transfer_verification1 == 'N') {
            fclose(file);
            printf("exit");
            return 0;

        }
        account_menu(accountNumber);
    }
    if (transfer_verification == 'Y') {
        char file_name[30];
        sprintf(file_name, "account_%ld.dat", transfer_account);
        FILE *file1 = fopen(file_name, "rb+");
        if (file1 == NULL) {
            printf("Account not found\n");
            bank_menu();
            return 0;
        }
        Account acc_reciever;
        fread(&acc_reciever, sizeof(Account), 1, file1);
        fclose(file1);

        //update_balances
        acc_sender.balance -= transfer_amount;
        acc_reciever.balance += transfer_amount;

        //save_changes_for_sender

        file = fopen(filename, "rb+");
        if (file == NULL) {
            printf("Account not found.\n");
            bank_menu();
            return 0;
        }

        fseek(file, 0, SEEK_SET);
        fwrite(&acc_sender, sizeof(Account),1, file);
        fclose(file);

        //save_changes_for_reciever

        file1 = fopen(file_name, "rb+");
        if (file1 == NULL) {
            printf("Account not found.\n");
            bank_menu();
            return 0;
        }

        fseek(file1, 0, SEEK_SET);
        fwrite(&acc_reciever, sizeof(Account), 1, file1);
        fclose(file1);

        printf("You have successfully transferred %.2f shillings to account no. %ld\n"
               "New account balance is %.2f \n", transfer_amount, transfer_account, acc_sender.balance);
        account_menu(accountNumber);
        return 0;
    }
    printf("Invalid entry.");
    account_menu(accountNumber);

    return 0;
}


int check_balance(long int accountNumber) {
    char filename[50];
    sprintf(filename, "account_%ld.dat", accountNumber);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Account not found.\n");
        return 0;
    }
    Account acc;
    fread(&acc, sizeof(Account), 1, file);
    fclose(file);

    char checkbalance_input;
    printf("Your account balance is: %.2f\n", acc.balance);
    printf("Would you like to perform other transactions?(Y/N): ");
    scanf(" %c", &checkbalance_input);
    while (getchar() != '\n');

    checkbalance_input = toupper(checkbalance_input);

    if (checkbalance_input == 'N') {
        bank_menu();
    }else {
        account_menu(accountNumber);
    }
    return 0;

}


bool check_password( long int accountNumber, const char *inputPassword) {
    char filename[100];
    sprintf(filename, "account_%ld.dat", accountNumber); // Each file named by account number

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Account not found.\n");
        return false;
    }

    Account acc;
    fread(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    if (strcmp(acc.account_password, inputPassword) == 0) {
        return true;
    }
    return false;
}



int withdraw_money(long int accountNumber) {
    char input_password[50];
    int withdraw_amount;


    char filename[30];
    sprintf(filename, "account_%ld.dat", accountNumber);
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Account not found.\n");
        bank_menu();
        return 0;
    }
    Account acc;
    fread(&acc, sizeof(Account), 1, file);

    //account_verification
    printf("Enter your account password: ");
    scanf("%49s", input_password);
    while (getchar() != '\n');
    if (!check_password(accountNumber, input_password)) {
        printf("Incorrect password.\n");
        fclose(file);
        account_menu(accountNumber);
        return 0;

    }
    //ask_for_amount_to_be_withdrawn
    printf("Enter amount to withdraw: ");
    scanf("%d", &withdraw_amount);
    while (getchar() != '\n');

    if (withdraw_amount > acc.balance-1000) {
        printf("Insufficint funds.\n");
        fclose(file);
        account_menu(accountNumber);
        return 0;
    }

    //update_balance_in_files
    acc.balance -= withdraw_amount;

    fseek(file, 0, SEEK_SET);
    fwrite(&acc, sizeof(Account), 1, file);
    fclose(file);

    printf("Withdrawal of %d successful. New account balance is: %.2f\n", withdraw_amount, acc.balance);
    return 0;
}




void account_menu(long int accountNumber) {
    printf("______________/Account menu/______________\n");
    printf("1. Withdraw money\n");
    printf("2. Check balance\n");
    printf("3. Transfer money\n");
    printf("4. Deposit money\n");
    printf("5. Exit\n");

    char account_menu_input1;

    printf("Please choose your preferred action: ");
    scanf(" %c", &account_menu_input1);
    while (getchar() != '\n');

    switch (account_menu_input1) {
        case '1':
            withdraw_money(accountNumber);
            account_menu(accountNumber);
            break;
        case '2':
            check_balance(accountNumber);
            break;
        case '3':
            transfer_money(accountNumber);
            break;
        case '4':
            deposit_money(accountNumber);
            break;
        case '5':
            break;
    }


}





void account_login() {
    char inputpassword[50];

    printf("Enter your account number: \n");
    scanf("%ld", &Account_number);

    while (getchar() != '\n');

    if (!accountExists(Account_number) ) {
        printf("Account does not exist.\n");
        bank_menu();
    }else {
        printf("Enter your account password: \n");
        scanf("%s", inputpassword);
        while (getchar() != '\n');
        bool pass_check = check_password( Account_number, inputpassword);
        if (pass_check == true) {

            char filename[30];
            sprintf(filename, "account_%ld.dat", Account_number);

            FILE *file = fopen(filename, "rb");
            if (file == NULL) {
                printf("Account not found.\n");
                return;
            }
            Account acc;
            fread(&acc, sizeof(Account), 1, file);
            fclose(file);

            printf("Welcome %s\n", acc.name);
            account_menu(acc.accountNumber);

        }
        else {
            printf("Incorrect password.\n");
            account_login();
        }



    }

}




void bank_menu() {
    char bank_menu_input1;
    printf("________________/ Login/Sign up /_______________\n");
    printf("1. Create a new account\n");
    printf("2. Log in\n");
    printf("3. Exit\n");

    scanf("%c", &bank_menu_input1);
    while (getchar() != '\n');



    switch (bank_menu_input1) {
        case '1':
            account_creation_information_collection();
            bank_menu();
            break;
        case '2':
            account_login();
            break;
        case '3':
            break;
    }
}

