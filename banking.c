#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

const int CHECKINGSACCOUNT = 1;
const int SAVINGSACCOUNT = 2;
const int GROWTHACCOUNT = 3;

typedef struct{

    int typeOfAccount;
    int accountPin;
    double accountBalance;
    double growthRate;
}bank_account;

typedef struct{

    char username[15];
    char password[15];
    int numberOfAccounts;
    bank_account accounts[3];
}bank_user;

void bank_user_creation();
bank_user bank_user_login(bool*);
void bank_account_creation(bank_user*);
void bank_account_login();
void bank_user_add_account(bank_user*, bank_account*);
bool bank_user_exists(bank_user*);
void str_remove_spaces(char*);
int file_find_line_number(bank_user*);

int main(){

    FILE *fptr;
    fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        fptr = fopen("banking.txt", "w");
    }

    bool loggedIn = 0;
    bank_user currentUser;

    bool logout = 0;
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
                    bank_user_login(&loggedIn);
                    break;
                case '3':
                    logout = 1;
                    break;
                default:
                    printf("You have not chosen a valid response\n");
                    break;
            };
        }else{
            printf("Features not unlocked yet\n");
            logout = 1;
        }
    }while(!logout);

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
    char response;

    do{
        printf("\nWhat username would you like to use (max 15 characters): ");
        scanf(" %c");
        fgets(user.username, 15, stdin);
        user.username[strlen(user.username)-1] = '\0';
        str_remove_spaces(&user.username[0]);

        printf("What would you like your password to be (max 15 characters): ");
        scanf(" %c");
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

    printf("would you like to set up an account now? (Y/N)");
    scanf(" %c", &response);
    if(response == 'Y' || response == 'y'){
        fclose(fptr);
        bank_account_creation(&user);
    }
    fclose(fptr);
}
bank_user bank_user_login(bool* loggedIn){
    
    FILE* fptr = fopen("banking.txt", "r");
    if(fptr == NULL){
        printf("Unable to open file\n");
    }

    bank_user user;

    while(1){
        printf("What is your username: ");
        scanf(" %c");
        fgets(user.username, 15, stdin);
        user.username[strlen(user.username)-1] = '\0';
        str_remove_spaces(&user.username[0]);

        printf("What is your password: ");
        scanf(" %c");
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
                if(strlen(user.password) != strlen(str)) continue;

                for(int i = 0; i<= strlen(user.password); i++){
                    if(user.password[i] == '\0' && str[i] == '\0'){
                        // will create some sort of loop to retrieve bank data as well!
                        while(line[0] != '}'){
                            fgets(line, sizeof(line), fptr);
                            if(line[0] == '}') break;
                            printf("%s\n", line);
                        }
                        *loggedIn = 1;
                        printf("You have logged in!\n");
                        return user;
                    }
                    if(user.password[i] != str[i]) break;
                }
            }
        }
        char response;
        printf("\nUsername or Password incorrect\n");
        printf("Would you like to go back to the main menu? (Y/N): ");
        scanf(" %c", &response);
        if(response == 'Y' || response == 'y') {
            return user;
        }
    }
}
void bank_account_creation(bank_user *pUser){

    bank_account account;
    int response;

    printf("\nYou currently have %i accounts\n", pUser->numberOfAccounts);
    if(pUser->numberOfAccounts <= 3){
        printf("You can have up to 3 accounts\n");

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
    }else{
        printf("You can only have 3 accounts\n");
    }
}
void bank_account_login(FILE* fptr){

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

    fptr = fopen("banking.txt", "w");
    fptr = fopen("banking.txt", "a");

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
            if(pUser->username[i] == '\0' && username[i] == '\0') return 1;
            if(pUser->username[i] != username[i]) break;
        }
    }

    return 0;
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

    int count;
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