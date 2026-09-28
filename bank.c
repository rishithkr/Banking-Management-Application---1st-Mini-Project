#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Account
{
    char name[50];
    long mob_num;
    long acc_no;
    char password[25];
    double funds;
    struct Transaction *t_head;
    struct Transaction *t_tail;
    struct Account *a_prev;
    struct Account *a_next;
} account;

typedef struct Transaction
{
    char type[8];
    double amount;
    char timestamp[80];
    struct Transaction *t_next;
} transaction;

void create_new(account **head, account **tail, int *acc_num);

account *login(account *head); // binary search can be used

void transfer(account *current, int op, double money, account *head); //op: 1 = deposit, 2 = withdraw, 3 = sent

void history(account *current);

void display(account *head);

void delete(account *current);



int main()
{
    int op;
    double money;
    account *head = NULL;
    account *tail = NULL;
    account *current = NULL;
    int *acc_num = (int*) malloc(sizeof(int));
    *acc_num = 0;
    while (1)
    {
        printf("Enter 1 to create a new account.\nEnter 2 to login to existing account.\nEnter 3 to display all data.\nEnter 0 to exit.\n");
        scanf("%d", &op);
        switch (op)
        {
            case 1:
                    create_new(&head, &tail,acc_num);
                    break;
            case 2:
                    current = login(head);
                    if (current != NULL)
                    {
                        while(1)
                        {
                            printf("Login successfull. Enter 1 to check your balance.\nEnter 2 to deposit funds.\nEnter 3 withdraw money.\nEnter 4 to transfer funds.\nEnter 5 see transaction history.\nEnter 6 to delete account.\nEnter 0 to return to home screen.\n");
                            scanf("%d", &op);
                            switch (op)
                            {
                                case 1:
                                        printf("Current balance: Rs. %lf", current->funds);
                                        break;
                                case 2:
                                        printf("Enter the funds to be depositted\n");
                                        scanf("%lf", &money);
                                        transfer(current, 1, money, NULL);
                                        break;
                                case 3:
                                        printf("Enter the funds to be withdrawn\n");
                                        scanf("%lf", &money);
                                        if(money > current->funds)
                                            printf("Error! Transaction failed due to insufficient funds.\n");
                                        else
                                            transfer(current, 2, money, NULL);
                                        break;
                                case 4:
                                        transfer(current,3, money, head);
                                        break;
                                case 5:
                                        history(current);
                                        break;
                                case 6:
                                        delete(current);
                                        break;
                                case 0:
                                        break;
                                        break;
                                default:
                                        printf("Wrong Input. Please try again.\n");
                            }
                        }
                    }
                    else
                    printf("Error! Account not found.\n");
                    break;
        case 3:
                display(head);
                break;
        case 0:
                exit(0);
                break;
        default:
                printf("Invalid Input! Try Again.\n");
        }
    }
    return 0;
}


void create_new(account **head, account **tail, int *acc_num)
{
    char name[50];
    long mob_num;
    char password[25];
    double funds;
    long acc_no = 18700224000 + *acc_num;
    (*acc_num)++;
    printf("Please enter your name.\n");
    scanf("%s", name);   
    printf("Please enter your mobile number.\n");
    scanf("%ld", mob_num);   
    printf("Please enter the funds you would deposit.\n");
    scanf("%lf", funds);   
    printf("Please enter a strong password.\n");
    scanf("%s", password);
    account* newnode = (account*) malloc(sizeof(account));
    strcpy(newnode->name, name);
    newnode->mob_num = mob_num;
    strcpy(newnode->password, password);
    newnode->funds = funds;
    newnode->a_next = NULL;
    newnode->a_prev = *tail;
    *tail = newnode;
    if (*head == NULL)
        *head = newnode;
    newnode->t_head = newnode->t_tail = NULL;
    transfer(newnode, 1, funds, NULL);
}


account *login(account *head)
{
    long mob_num;
    char password[25];
    short count = 0;
    printf("Please enter your mobile number.\n");
    scanf("%ld", mob_num);
    while(head->mob_num == mob_num || head == NULL)
        head = head->a_next;
    if(head->mob_num == mob_num )
    {
        while(head->password == password)
        {
            printf("Please enter your password.\n");
            scanf("%s", password);
            if(head->password == password)
                return head;
            else if(count < 3)
                {
                    printf("Wrong password! %hd retries left", count-1);
                    count++;
                }
            else
                {
                    char name[50];
                    long acc_no;
                    printf("secondary authentication procedure initiated. Please enter your name.\n");
                    scanf("%s", name);
                    printf("Please enter your account number.\n");
                    scanf("%ld", acc_no);
                    if (acc_no == head->acc_no && name == head->name)
                        return head;
                    else
                        printf("Authentication failed!\n");
                        return NULL;
                }
        }
    }
    else
        {
            printf("Error404! Mobile number not found.\n");
            return NULL;
        }
}


void history(account *current)
{
    while(current->t_head != NULL)
    {
        printf("Rs. %lf %s at %s\n", (current->t_head)->amount, (current->t_head)->type, (current->t_head)->timestamp);
        current->t_head = (current->t_head)->t_next;
    }
}


void display(account *head)
{
    char pass[10];
    printf("Enter admin password.\n");
    scanf("%s", pass);
    if (pass == "1234567890")
    {
        if(head == NULL)
        {
            printf("List is empty.\n");
            return;
        }
        while(head != NULL)
            printf("%s   %ld   %lf   %ld \n", head->name, head->mob_num, head->funds, head->acc_no);
            head = head->a_next;
    }
    else{
        printf("Wrong Password!\n");
        return;
    }
}


void transfer(account *current, int op, double money, account* head)
{
    double r_acc; 
    time_t c_time;
    struct tm *local_time;
    char time_s[80]; 
    c_time = time(NULL);
    local_time = localtime(&c_time);
    strftime(time_s, sizeof(time_s), "%Y-%m-%d %H:%M:%S", local_time);
    transaction *newnode = (transaction*) malloc(sizeof(transaction));
    newnode->t_next = NULL;
    strcpy(newnode->timestamp, time_s);
    newnode->t_next = current->t_head;
    newnode = current->t_head;
    if(current->t_tail == NULL)
        current->t_tail = newnode;

    switch (op)
    {
    case 1:
            current->funds += money;
            newnode->amount = money;
            strcpy(newnode->type, "DEPOSIT");
            break;
    
    case 2:
            current->funds -= money;
            newnode->amount = money;
            strcpy(newnode->type, "WITHDRAW");
            break;

    case 3:
            printf("Enter the receiver's account number.\n");
            scanf("%ld", &r_acc);
            while(head->acc_no != r_acc || head == NULL)
                head = head->a_next;
            if(head->acc_no == r_acc)
            {
                current->funds -= money;
                newnode->amount = money;
                strcpy(newnode->type, "SENT");  
                transaction *new = (transaction*) malloc(sizeof(transaction));
                new->t_next = NULL;
                strcpy(new->timestamp, time_s);
                new->t_next = head->t_head;
                new = head->t_head;
                head->funds += money;
                new->amount = money;
                strcpy(new->type, "RECEIVED"); 
                if(head->t_tail == NULL)
                head->t_tail = new;        
            }
            else{
                printf("Error404! Account Not Found");
                newnode->amount = money;
                strcpy(newnode->type, "CANCELED");
            }
    }
}

void delete(account *current)
{
    char op;
    char pass[25];
    while(1)
    {

        printf("Are you sure you want to delete your account.\nEnter Y to confirm, enter N to cancel deletion.\n");
        scanf("%c", &op);
        if(op == 'Y')
        {    
            printf("Please enter your password.\n");
            scanf("%s", pass);
            if(pass == current->password)
            {
                current->a_prev->a_next = current->a_next;
                current->a_next->a_prev = current->a_prev;
                free(current);
            }
            else
                printf("Wrong Password! Reverting back to main menu.\n");
        }
        else if(op == 'N')
            return;
        else
            printf("Wrong Input! Try Again\n");
    }
}