#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const int CHECKINGSACCOUNT = 1;
const int SAVINGSACCOUNT = 2;
const int GROWTHACCOUNT = 3;

typedef struct{

    int typeOfAccount;
    int accountPin;
    float accountBalance;
    float growthRate;
}bank_account;

typedef struct{

    char username[30];
    char password[15];
    int numberOfAccounts;
    bank_account accounts[3];
}bank_user;

void bank_user_creation();
void bank_user_login();
void bank_account_creation(bank_user*);
void bank_account_login();

int main(){

    char response;

    printf("\nWelcome to Zealth bank!\n");
    do{

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
                bank_user_login();
                break;
            case '3':
                break;
            default:
                printf("You have not chosen a valid response\n");
                break;
        };
    }while(response != '3');

    printf("\nThank you for using Zealth bank!\n");
    return 0;
}
void bank_user_creation(){

    bank_user user;
    bank_user *pUser = &user;
    char response;

    printf("\nWhat username would you like to use (max 30 characters): ");
    scanf(" %c");
    fgets(user.username, 30, stdin);

    printf("What would you like your password to be (max 15 characters): ");
    scanf(" %c");
    fgets(user.password, 15, stdin);

    printf("would you like to set up an account now? (Y/N)");
    scanf(" %c", &response);
    if(response == 'Y' || response == 'y'){
        bank_account_creation(pUser);
    }
}
void bank_user_login(){

}
void bank_account_creation(bank_user *pUser){

    bank_account account;
    bank_account *pAccount;
    int response;

    printf("\nYou currently have %i accounts\n", pUser->numberOfAccounts);
    if(pUser->numberOfAccounts <= 3){
        printf("You can have up to 3 accounts\n");

        printf("\nWhat type of account will this be?\n");
        printf("1. Checkings account (0%% yearly growth rate)\n");
        printf("2. Savings account (1%% yearly growth rate)\n");
        printf("3. Growth savings account (2%% yearly growth rate)\n");

        scanf(" %i", response);
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
        };

        account.accountBalance = 0.00;
    }else{
        printf("You can only have 3 accounts\n");
    }
}
void bank_account_login(){

}