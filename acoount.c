#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "acoount.h"
#include <time.h>
#include <ctype.h>
#include <dirent.h>

char admin_password[50] = "56@23_78";


void capitalizeString(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]); // Convert each character to uppercase
    }
}

//counting_function
int count_num(long int num) {
    if (num == 0) {
        return 1;
    }
    int count = 0;
    while (num != 0) {
        num = num /= 10;
        count++;
    }
    return count;
}

// Function_of_check_if_a_file_with_the_given_account_number_already_exists
int accountExists(int accountNumber) {
    char filename[30];
    sprintf(filename, "account_%d.dat", accountNumber); // File name based on account number

    FILE *file = fopen(filename, "rb"); // Try to open the file
    if (file != NULL) {
        fclose(file);
        return 1; // Account already exists
    }
    return 0; // Account does not exist
}

//function_to_save_account
void saveAccountToFile(Account *acc) {
    char filename[30];
    sprintf(filename, "account_%ld.dat", acc->accountNumber); // Unique file for each account

    FILE *file = fopen(filename, "wb"); // Open in binary write mode
    if (file == NULL) {
        printf("Error creating file for account %ld!\n", acc->accountNumber);
        return;
    }

    fwrite(acc, sizeof(Account), 1, file); // Save struct to file
    fclose(file);

    printf("Account successfully saved in file: %s\n", filename);
}



//function_for_creating_unique_number
int generateUniqueAccountNumber() {
    int accountNumber;
    srand(time(0)); // Seed the random number generator

    do {
        accountNumber = (rand() % (100000 - 20000 + 1)) + 20000; // Generate a random number
    } while (accountExists(accountNumber)); // Repeat if the number already exists

    return accountNumber;
}

//function_for_creating_account
int account_creation_information_collection() {
    //input variables
    char name[50];
    double balance;
    char address[50];
    long Y_O_B, age, iden_card_no, kra_pin;



    //input_statements_for_name_and_place-of_residence
    printf("Please enter your name: ");
    scanf(" %[^\n]", name);

    while (getchar() != '\n');

    capitalizeString(name);


    printf("Please enter your place of residence: ");
    scanf(" %[^\n]s", address);

    while (getchar() != '\n');

    capitalizeString(address);

    //input_statements_for_the_thers

    printf("Please enter your national identification number: ");
    scanf("%ld", &iden_card_no);

    while (getchar() != '\n');

    //error_handling_for_identification_no
    int error_checkk = count_num(iden_card_no);
    while (error_checkk != 9 && error_checkk != 8){
        printf("Error: Please enter a a valid identity number: ");
        scanf("%ld", &kra_pin);

        while (getchar() != '\n');

        error_checkk = count_num(kra_pin);

    }


    //input_statements_for_kra
    printf("Please enter your KRA pin: ");
    scanf("%ld", &kra_pin);

    while (getchar() != '\n');

    //error_handling_for_kra
    error_checkk = count_num(kra_pin);

    while (error_checkk != 9 && error_checkk != 8) {
        printf("Error: Please enter a valid KRA pin: ");
        scanf("%ld", &kra_pin);
        while (getchar() != '\n');

        error_checkk = count_num(kra_pin);
    }

    //input_for_first_time_deposit
    printf("Please enter the amount you wish to deposit: ");
    scanf("%lf", &balance);

    while (getchar() != '\n');

    //error_handling_for_deposit
    if (balance < 1000) {
        while (balance < 1000) {
            printf("Error: Cannot deposit below 1000. Please enter a higher amount: ");
            scanf("%lf", &balance);
        }
    }

    //input_for_Y.O.B
    printf("Please enter your year of birth");
    scanf("%ld", &Y_O_B);

    while (getchar() != '\n');


    //error_handling_for_Y.O.B
    if (Y_O_B > 2007) {
        while (Y_O_B > 2007 ) {
            printf("Please enter a year below 2007");
            scanf("%ld", &Y_O_B);
        }
    }else if (Y_O_B < 1920){
        while (Y_O_B < 1920) {
            printf("Error: Please enter a year below 1920: ");
            scanf("%ld", &Y_O_B);
        }
    }
    //input_for_age
    printf("please enter your age: ");
    scanf("%ld", &age);

    while (getchar() != '\n');

    //error_handling_for_age

    // Get current system time

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_year = tm.tm_year + 1900;



    if (age < 0) {
        while (age < 0) {
            printf("Error: Please enter a valid age: ");
            scanf("%ld", &age);
        }
    }else if (age < 18 ) {
        while (age < 18) {
            printf("Error: must be 18 or above to open an account.\n");
            printf("Try again: ");
            scanf("%ld", &age);
        }
    }else if (age != (current_year) - Y_O_B) {
        while (age != (current_year) - Y_O_B) {
            printf("Error: Year of birth does not match with age, please enter correct age: ");
            scanf("%ld", &age);
        }
    }

    //input_for_gender
    char gender;
    printf("Please enter your gender(m/f): ");
    scanf(" %c", &gender);
    gender = (char)toupper(gender);
    while (getchar() != '\n');

    //error_handling_for_gender

    while (gender != 'M' && gender != 'F') {
        printf("Error: Please enter a valid gender: ");
        scanf(" %c", &gender);
        gender  = (char)toupper(gender);

        while (getchar() != '\n');
    }

    //generating_account_number
    long int  accountNumber = generateUniqueAccountNumber();
    printf("Hello %s, your account number is: %ld\n", name, accountNumber);

    //password_creation
    char account_password[50];
    printf("Please enter a password for your account: \n");
    scanf("%49s", account_password);

    while (getchar() != '\n');

    int times = 1;

    while (times > 0) {
        printf("Please enter your password one more time for confirmation: \n");
        scanf("%49s", account_password);

        while (getchar() != '\n');
        times--;
    }

    //saving_info_to_account_file

    //dynamically_creating_a_struct_and_dynamically_allocating_memory_for_it
    Account *acc = (Account *)malloc(sizeof(Account)); // Dynamically allocate memory
    if (acc == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    //copying_collected_info_to_the_struct
    strcpy(acc->name, name);
    strcpy(acc->address, address);
    strcpy(acc->account_password, account_password);
    acc->gender = gender;
    acc->iden_card_no = iden_card_no;
    acc->kra_pin = kra_pin;
    acc->balance = balance;
    acc->Y_O_B = Y_O_B;
    acc->age = age;
    acc->accountNumber = accountNumber;


    saveAccountToFile(acc);

    free(acc);

    return 0;

}

//function_to_load_existing_accounts
void loadAccount(int accountNumber) {
    char filename[30];
    sprintf(filename, "account_%d.dat", accountNumber);

    FILE *file = fopen(filename, "rb"); // Open in binary read mode
    if (file == NULL) {
        printf("Account not found!\n");
        return;
    }

    Account acc;
    fread(&acc, sizeof(Account), 1, file);
    fclose(file);

    printf("\n--- Account Details ---\n");
    printf("Account Number: %ld\n", acc.accountNumber);
    printf("Name: %s\n", acc.name);
    printf("Address: %s\n", acc.address);
    printf("ID Number: %ld\n", acc.iden_card_no);
    printf("KRA Pin: %ld\n", acc.kra_pin);
    printf("Year of Birth: %ld\n", acc.Y_O_B);
    printf("Age: %ld\n", acc.age);
    printf("Balance: %.2f\n", acc.balance);
}

//function_to_delete_account
void delete_account(long int accountNumber) {
    char filename[30];
    sprintf(filename, "account_%ld.dat",  accountNumber);

    //check_whether_the_account_exists
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Error: Account not be found.");
    }
    fclose(file);

    //password_verification_for_verification
    char Admin_password_for_deletion_function[50];
    printf("Please enter administrator password: ");
    scanf("%49s", Admin_password_for_deletion_function);

    while (getchar() != '\n');

    int attempts = 2;

    while (strcmp(Admin_password_for_deletion_function, admin_password) != 0 && attempts > 0) {
        printf("Wrong password entered: You have %d attempt(s) left, Enter your password again: ", attempts);
        scanf("%49s", Admin_password_for_deletion_function);

        while (getchar() != '\n');
        attempts--;
    }

    if (strcmp(Admin_password_for_deletion_function, admin_password) == 0 ) {
        if (remove(filename) == 0) {
            printf("Account %ld successfully deleted.", accountNumber);
        }else {
            printf("Error: Account %ld could not be deleted.", accountNumber);
        }
    }else {
        printf("Error: Too many incorrect password attempts. Account deletion aborted.");

    }
}

//function_to_load_all_accounts
void load_all_accounts() {
    struct dirent *entry;
    DIR *dirp = opendir(".");

    if (dirp == NULL) {
        return;
    }

    printf("\n--- All Accounts ---\n");

    while ((entry = readdir(dirp)) != NULL) {
        if (strncmp(entry->d_name, "account_", 8) == 0 && strstr(entry->d_name, ".dat")) {
            FILE *file = fopen(entry->d_name, "rb");
            if (file != NULL) {
                Account acc;
                if (fread(&acc, sizeof(Account), 1, file) == 1) {
                    printf("\nAccount Number: %ld\n", acc.accountNumber);
                    printf("Name: %s\n", acc.name);
                    printf("Address: %s\n", acc.address);
                    printf("ID Number: %ld\n", acc.iden_card_no);
                    printf("KRA Pin: %ld\n", acc.kra_pin);
                    printf("Year of Birth: %ld\n", acc.Y_O_B);
                    printf("Age: %ld\n", acc.age);
                    printf("Balance: %.2f\n", acc.balance);
                }
                fclose(file);
            }else {
                printf("Error opening account: %s\n", entry->d_name);
            }
        }
    }

    closedir(dirp);
}

