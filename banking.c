#include <stdio.h>

struct bank_account{

    int accountPin;
    int accountNumber;
    int accountBalance;
};

struct bank_user{

    char username[30];
    char password[15];
    struct bank_account accounts[3];
};

void bank_user_creation();
void bank_user_login();
void bank_account_creation();
void bank_account_login();

int main(){

    char response;

    printf("\nWelcome to Zealth bank!\n\n");
    do{

        printf("What action would you like to take?\n");
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

}
void bank_user_login(){

}
void bank_account_creation(){

}
void bank_account_login(){

}