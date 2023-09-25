#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

const int CHECKINGSACCOUNT = 1;
const int SAVINGSACCOUNT = 2;
const int GROWTHACCOUNT = 3;

typedef struct bank_account{

    int typeOfAccount;
    int accountPin;
    double accountBalance;
    double growthRate;
}bank_account;

typedef struct bank_user{

    char username[15];
    char password[15];
    int numberOfAccounts;
    bank_account account;
}bank_user;

void bank_user_creation();
bank_user bank_user_login(bool*);
void bank_user_add_account(bank_user*, bank_account*);
bool bank_user_exists(bank_user*);
void bank_account_creation(bank_user*);
double bank_account_get_balance(bank_user*);
double bank_account_deposit(bank_user*);
double bank_account_withdrawl(bank_user*);
void str_remove_spaces(char*);
int file_find_line_number(bank_user*);
void file_change_bank_balance(bank_user*);

int main(){

    FILE *fptr;
    fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        fptr = fopen("banking.txt", "w");
    }

    bool loggedIn = 0;
    bank_user currentUser;

    bool quitApplication = 0;
    char response;
    printf("\nWelcome to Zealth bank!\n");
    do{

        if(!loggedIn){
            printf("\nWhat action would you like to take?\n");
            printf("1. new bank user\n");
            printf("2. user log in\n");
            printf("3. close application\n");
            scanf(" %c", &response);

            switch(response){
                case '1':
                    bank_user_creation();
                    break;
                case '2':
                    currentUser = bank_user_login(&loggedIn);
                    break;
                case '3':
                    quitApplication = 1;
                    break;
                default:
                    printf("You have not chosen a valid response\n");
                    break;
            };
        }else{
            printf("\nWhat action would you like to take %s?\n", currentUser.username);
            printf("1. retrieve bank account balance\n");
            printf("2. deposit into account\n");
            printf("3. withdrawl from account\n");
            printf("4. logout\n");
            printf("5. close application\n");
            scanf(" %c", &response);

            switch(response){
                case '1':
                        printf("\nAccount Balance: %.2lf\n", bank_account_get_balance(&currentUser));
                    break;
                case '2':
                        bank_account_deposit(&currentUser);
                    break;
                case '3':
                        bank_account_withdrawl(&currentUser);
                    break;
                case '4':
                    loggedIn = 0;
                    break;
                case '5':
                    quitApplication = 1;
                    break;
                default:
                    printf("You have not chosen a valid response\n");
                    break;
            }
        }
    }while(!quitApplication);

    printf("\nThank you for using Zealth bank!\n");
    fclose(fptr);
    return 0;
}
void bank_user_creation(){

    FILE* fptr = fopen("banking.txt", "a");
    if(fptr == NULL){
        printf("File can't be opened\n");
    }

    bank_user user;
    user.numberOfAccounts = 0;

    do{
        printf("\nWhat username would you like to use (max 15 characters): ");
        scanf(" ");
        fgets(user.username, 15, stdin);
        user.username[strlen(user.username)-1] = '\0';
        str_remove_spaces(&user.username[0]);

        printf("What would you like your password to be (max 15 characters): ");
        scanf(" ");
        fgets(user.password, 15, stdin);
        user.password[strlen(user.password)-1] = '\0';
        str_remove_spaces(&user.password[0]);

        if(bank_user_exists(&user)){
            printf("User already exists\n");
        }
    }while(bank_user_exists(&user));

    fprintf(fptr, "{\n");
    fprintf(fptr, "username:%s\n", user.username);
    fprintf(fptr, "password:%s\n", user.password);
    fprintf(fptr, "}\n");

    fclose(fptr);
    bank_account_creation(&user);
}
bank_user bank_user_login(bool* loggedIn){
    
    FILE* fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        printf("Unable to open file\n");
    }

    bank_user user;

    while(1){
        printf("What is your username: ");
        scanf(" ");
        fgets(user.username, 15, stdin);
        user.username[strlen(user.username)-1] = '\0';
        str_remove_spaces(&user.username[0]);

        printf("What is your password: ");
        scanf(" ");
        fgets(user.password, 15, stdin);
        user.password[strlen(user.password)-1] = '\0';
        str_remove_spaces(&user.password[0]);

        bool usernameFound = 0;
        char str[15];
        char line[256];
        while(fgets(line, sizeof(line), fptr) != NULL){
            if(line[0] == 'u') {
                sscanf(line, "username:%s", &str);
                if(strlen(user.username) != strlen(str)) continue;

                for(int i = 0; i <= strlen(user.username); i++){
                    if(user.username[i] == '\0' && str[i] == '\0') usernameFound = 1;
                    if(user.username[i] != str[i]) break;
                }
            } else if(line[0] == 'p' && usernameFound){

                sscanf(line, "password:%s", &str);
                if(strlen(user.password) != strlen(str)) {
                    usernameFound = 0;
                    break;
                }

                for(int i = 0; i<= strlen(user.password); i++){
                    if(user.password[i] != str[i]){
                        usernameFound = 0;
                        break;
                    }
                    if(user.password[i] == '\0' && str[i] == '\0'){
                        while(line[0] != '}'){
                            fgets(line, sizeof(line), fptr);
                            if(line[0] == '}') break;
                            if(line[0] == 't'){
                                bank_account account;

                                sscanf(line, "type:%i pin:%i rate:%lf balance:%lf", &account.typeOfAccount, 
                                &account.accountPin, &account.growthRate, &account.accountBalance);
                                
                                user.account = account;
                            }
                        }
                        *loggedIn = 1;
                        printf("You have logged in!\n");
                        fclose(fptr);
                        return user;
                    }
                }
            }
        }
        char response;
        printf("\nUsername or Password incorrect\n");
        printf("Would you like to go back to the main menu? (Y/N): ");
        scanf(" ", &response);
        if(response == 'Y' || response == 'y') {
            fclose(fptr);
            return user;
        }
    }
}
void bank_user_add_account(bank_user* pUser, bank_account* pAccount){

    FILE* fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        printf("File can't be opened\n");
    }

    FILE* temp = fopen("temp.txt", "w");
    if(temp == NULL){
        printf("temp file can't be created");
    }
    fclose(temp);
    temp = fopen("temp.txt", "a");

    char c;
    int lineNumber = file_find_line_number(pUser);
    int count = 1;
    while((c = fgetc(fptr)) != EOF){
        if(c == '\n') count++;
        fprintf(temp, "%c", c);

        if(count == lineNumber){
            fprintf(temp, "type:%i pin:%i rate:%.2lf balance:%.2lf\n", pAccount->typeOfAccount, pAccount->accountPin, 
            pAccount->growthRate, pAccount->accountBalance);
            count++;
        }
    }

    fclose(fptr);
    fptr = fopen("banking.txt", "w");
    fclose(fptr);
    fptr = fopen("banking.txt", "a");

    fclose(temp);
    temp = fopen("temp.txt", "r");

    while((c = fgetc(temp)) != EOF){
        fprintf(fptr, "%c", c);
    }

    fclose(fptr);
    fclose(temp);
    remove("temp.txt");
}
bool bank_user_exists(bank_user *pUser){

    FILE* fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        printf("Can not open file\n");
    }

    char username[15];

    char line[256];
    while(fgets(line, sizeof(line), fptr) != NULL){
        if(line[0] != 'u') continue;

        sscanf(line, "username:%s", &username);
        if(strlen(pUser->username) != strlen(username)) continue;

        for(int i = 0; i <= strlen(username); i++){
            if(pUser->username[i] == '\0' && username[i] == '\0'){
                fclose(fptr);
                return 1;
            }
            if(pUser->username[i] != username[i]) break;
        }
    }

    fclose(fptr);
    return 0;
}
void bank_account_creation(bank_user *pUser){

    bank_account account;
    int response = -1;

    printf("\nWhat type of account will this be?\n");
    printf("1. Checkings account (0%% yearly growth rate)\n");
    printf("2. Savings account (1%% yearly growth rate)\n");
    printf("3. Growth savings account (2%% yearly growth rate)\n");

    scanf(" %i", &response);
    switch(response){
        case 1:
            account.typeOfAccount = CHECKINGSACCOUNT;
            account.growthRate = 0.00;
            break;
        case 2:
            account.typeOfAccount = SAVINGSACCOUNT;
            account.growthRate = 0.01;
            break;
        case 3:
            account.typeOfAccount = GROWTHACCOUNT;
            account.growthRate = 0.02;
            break;
        default:
            printf("not a valid option\n");
            bank_account_creation(pUser);
            break;
    };

    account.accountBalance = 0.00;

    printf("create a 4 digit account pin: ");
    scanf(" %i", &account.accountPin);

    printf("typeOfAccount: %i, growthRate: %lf, accountPin: %i\n", account.typeOfAccount, account.growthRate, account.accountPin);

    bank_user_add_account(pUser, &account);
}
double bank_account_get_balance(bank_user* pUser){
    return pUser->account.accountBalance;
}
double bank_account_deposit(bank_user* pUser){

    double amount;
    char str[10];
    printf("How much would you like to deposit into your account: $");
    scanf(" ");
    fgets(str, 10, stdin);
    sscanf(str, "%lf", &amount);

    pUser->account.accountBalance += amount;
    file_change_bank_balance(pUser);

    return 0.00;
}
double bank_account_withdrawl(bank_user* pUser){

    double amount;
    char str[10];
    do{
        printf("How much do you want to withdrawl: $");
        scanf(" ");
        fgets(str, 10, stdin);
        sscanf(str, "%lf", &amount);

        if(pUser->account.accountBalance < amount){
            printf("\nInsufficient funds\n");
        } else{
            break;
        }
    }while(1);

    pUser->account.accountBalance -= amount;
    file_change_bank_balance(pUser);

    return 0.00;
}
void str_remove_spaces(char* str){
    
    int i = 0;
    int spaces = 0;
    while(str[i] != '\0'){
        if(str[i] == ' ') spaces++;
        str[i] = str[i+spaces];
        i++;
    }
}
int file_find_line_number(bank_user* pUser){

    FILE* fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        printf("Can't open file\n");
    }

    char username[15];

    int count = 0;
    char line[256];
    while(fgets(line, sizeof(line), fptr) != NULL){
        count++;
        if(line[0] == 'u'){
            sscanf(line, "username:%s", &username);
        }

        for(int i = 0; i <= strlen(pUser->username); i++){
            if(pUser->username[i] == '\0' && username[i] == '\0') return count + 2;
            if(pUser->username[i] != username[i]) break;
        }
    }

    return -1;
    fclose(fptr);
}
void file_change_bank_balance(bank_user* pUser){

    FILE* fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        printf("can not open file\n");
    }

    FILE* temp = fopen("temp.txt", "w");
    fclose(temp);
    temp = fopen("temp.txt", "a");
    if(temp == NULL){
        printf("Could not open temp file\n");
    }

    bank_account account;

    bool writeLine = 1;
    bool foundUser = 0;
    char username[15];
    char line[256];
    while(fgets(line, sizeof(line), fptr) != NULL){
        if(line[0] == 'u' && foundUser == 0){
            sscanf(line, "username:%s", &username);

            for(int i = 0; i <= strlen(username); i++){
                if(username[i] == '\0' && pUser->username[i] == '\0'){
                    foundUser = 1;
                } else if(username[i] != pUser->username[i]){
                    break;
                }
            }
        } else if(line[0] == 't' && foundUser == 1){
            sscanf(line, "type:%i pin:%i rate:%lf balance:%lf", &account.typeOfAccount,
            &account.accountPin, &account.growthRate, &account.accountBalance);

            account.accountBalance = pUser->account.accountBalance;

            fprintf(temp, "type:%i pin:%i rate:%.2lf balance:%.2lf\n", account.typeOfAccount,
            account.accountPin, account.growthRate, account.accountBalance);
            writeLine = 0;
            foundUser = 0;
        }
        if(writeLine == 1){
            fprintf(temp, "%s", line);
        }
        writeLine = 1;
    }

    fclose(temp);
    temp = fopen("temp.txt", "r");

    fclose(fptr);
    fptr = fopen("banking.txt", "w");
    fclose(fptr);
    fptr = fopen("banking.txt", "a");

    char c;
    while((c = fgetc(temp)) != EOF){
        fprintf(fptr, "%c", c);
    }

    fclose(fptr);
    fclose(temp);
    remove("temp.txt");
}