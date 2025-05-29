#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_USERS 100
#define PIN_LENGTH 4
#define MAX_ATTEMPTS 3

// Structure to store user account information
struct Account {
    long long account_number;
    char pin[PIN_LENGTH + 1];
    double balance;
};

// Function declarations
void clearScreen();
void saveAccounts(struct Account *accounts, int count);
int loadAccounts(struct Account *accounts);
void createNewAccount(struct Account *accounts, int *count);
int login(struct Account *accounts, int count, int *currentUser);
void displayMenu();
void deposit(struct Account *accounts, int currentUser);
void withdraw(struct Account *accounts, int currentUser);
void checkBalance(struct Account *accounts, int currentUser);
void changePin(struct Account *accounts, int currentUser);

// Main function
int main() {
    struct Account accounts[MAX_USERS];
    int userCount = 0;
    int currentUser = -1;
    int choice;
    
    // Load existing accounts from file
    userCount = loadAccounts(accounts);
    
    while (1) {
        clearScreen();
        printf("\n=== Welcome to ATM Simulator ===\n\n");
        printf("1. Create New Account\n");
        printf("2. Login to Existing Account\n");
        printf("3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createNewAccount(accounts, &userCount);
                break;
            case 2:
                if (login(accounts, userCount, &currentUser)) {
                    while (1) {
                        displayMenu();
                        scanf("%d", &choice);
                        
                        switch (choice) {
                            case 1:
                                deposit(accounts, currentUser);
                                break;
                            case 2:
                                withdraw(accounts, currentUser);
                                break;
                            case 3:
                                checkBalance(accounts, currentUser);
                                break;
                            case 4:
                                changePin(accounts, currentUser);
                                break;
                            case 5:
                                currentUser = -1;
                                goto main_menu;
                            default:
                                printf("\nInvalid choice! Press any key to continue...");
                                getch();
                        }
                    }
                }
                main_menu:
                break;
            case 3:
                printf("\nThank you for using ATM Simulator!\n");
                return 0;
            default:
                printf("\nInvalid choice! Press any key to continue...");
                getch();
        }
    }
    return 0;
}

// Function to clear screen
void clearScreen() {
    system("cls");
}

// Function to save accounts to file
void saveAccounts(struct Account *accounts, int count) {
    FILE *file = fopen("accounts.dat", "wb");
    if (file == NULL) {
        printf("\nError saving accounts!");
        return;
    }
    fwrite(accounts, sizeof(struct Account), count, file);
    fclose(file);
}

// Function to load accounts from file
int loadAccounts(struct Account *accounts) {
    FILE *file = fopen("accounts.dat", "rb");
    if (file == NULL) {
        return 0;
    }
    
    int count = 0;
    while (fread(&accounts[count], sizeof(struct Account), 1, file) == 1) {
        count++;
    }
    fclose(file);
    return count;
}

// Function to create new account
void createNewAccount(struct Account *accounts, int *count) {
    clearScreen();
    printf("\n=== Create New Account ===\n");
    
    if (*count >= MAX_USERS) {
        printf("\nMaximum number of accounts reached!");
        printf("\nPress any key to continue...");
        getch();
        return;
    }
    
    srand(time(NULL));
    accounts[*count].account_number = rand() % 900000000000 + 100000000000;
    
    printf("\nYour Account Number: %lld", accounts[*count].account_number);
    
    while (1) {
        printf("\nEnter 4-digit PIN: ");
        scanf("%s", accounts[*count].pin);
        
        if (strlen(accounts[*count].pin) == PIN_LENGTH) {
            break;
        }
        printf("\nPIN must be 4 digits!");
    }
    
    accounts[*count].balance = 0.0;
    (*count)++;
    
    saveAccounts(accounts, *count);
    
    printf("\nAccount created successfully!");
    printf("\nPress any key to continue...");
    getch();
}

// Function to handle login
int login(struct Account *accounts, int count, int *currentUser) {
    clearScreen();
    printf("\n=== Login ===\n");
    
    long long acc_num;
    char pin[PIN_LENGTH + 1];
    int attempts = 0;
    
    printf("\nEnter Account Number: ");
    scanf("%lld", &acc_num);
    
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == acc_num) {
            while (attempts < MAX_ATTEMPTS) {
                printf("Enter PIN: ");
                scanf("%s", pin);
                
                if (strcmp(accounts[i].pin, pin) == 0) {
                    *currentUser = i;
                    return 1;
                }
                
                attempts++;
                printf("\nIncorrect PIN! %d attempts remaining.\n", MAX_ATTEMPTS - attempts);
            }
            printf("\nToo many incorrect attempts! Account locked.");
            printf("\nPress any key to continue...");
            getch();
            return 0;
        }
    }
    
    printf("\nAccount not found!");
    printf("\nPress any key to continue...");
    getch();
    return 0;
}

// Function to display menu
void displayMenu() {
    clearScreen();
    printf("\n=== ATM Menu ===\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Check Balance\n");
    printf("4. Change PIN\n");
    printf("5. Logout\n");
    printf("\nEnter your choice: ");
}

// Function to handle deposits
void deposit(struct Account *accounts, int currentUser) {
    clearScreen();
    printf("\n=== Deposit ===\n");
    
    double amount;
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("\nInvalid amount!");
    } else {
        accounts[currentUser].balance += amount;
        saveAccounts(accounts, MAX_USERS);
        printf("\nDeposit successful! New balance: $%.2f", accounts[currentUser].balance);
    }
    
    printf("\nPress any key to continue...");
    getch();
}

// Function to handle withdrawals
void withdraw(struct Account *accounts, int currentUser) {
    clearScreen();
    printf("\n=== Withdraw ===\n");
    
    double amount;
    printf("\nEnter amount to withdraw: $");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("\nInvalid amount!");
    } else if (amount > accounts[currentUser].balance) {
        printf("\nInsufficient funds!");
    } else {
        accounts[currentUser].balance -= amount;
        saveAccounts(accounts, MAX_USERS);
        printf("\nWithdrawal successful! New balance: $%.2f", accounts[currentUser].balance);
    }
    
    printf("\nPress any key to continue...");
    getch();
}

// Function to check balance
void checkBalance(struct Account *accounts, int currentUser) {
    clearScreen();
    printf("\n=== Balance Inquiry ===\n");
    printf("\nCurrent Balance: $%.2f", accounts[currentUser].balance);
    printf("\nPress any key to continue...");
    getch();
}

// Function to change PIN
void changePin(struct Account *accounts, int currentUser) {
    clearScreen();
    printf("\n=== Change PIN ===\n");
    
    char oldPin[PIN_LENGTH + 1];
    char newPin[PIN_LENGTH + 1];
    
    printf("\nEnter current PIN: ");
    scanf("%s", oldPin);
    
    if (strcmp(accounts[currentUser].pin, oldPin) == 0) {
        while (1) {
            printf("Enter new 4-digit PIN: ");
            scanf("%s", newPin);
            
            if (strlen(newPin) == PIN_LENGTH) {
                strcpy(accounts[currentUser].pin, newPin);
                saveAccounts(accounts, MAX_USERS);
                printf("\nPIN changed successfully!");
                break;
            }
            printf("\nPIN must be 4 digits!");
        }
    } else {
        printf("\nIncorrect current PIN!");
    }
    
    printf("\nPress any key to continue...");
    getch();
} 