#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILENAM "orders.csv"

void display_orders(){
    FILE *file = fopen(FILENAM, "r");
    if (!file){
        printf("Error: Cannot open file %s\n", FILENAM);
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)){
        printf("%s", line);
    }
    fclose(file);
}

void add_order(){
    FILE *file = fopen(FILENAM, "a");
    if (!file){
        printf("Error: Cannot open file %s\n", FILENAM);
        return;
    }

char OrderID[10];
char BookTitle[100];
int Quantity;
int PricePerUnit;

    printf("Enter Order ID: ");
    scanf("%s", OrderID);

    printf("Enter Book Title: ");
    getchar(); // Clear newline character from buffer
    fgets(BookTitle, sizeof(BookTitle), stdin);

    size_t len = strlen(BookTitle);
    if (len > 0 && BookTitle[len - 1] == '\n') {
        BookTitle[len - 1] = '\0'; // Remove newline character
    }

    printf("Enter Quantity: ");
    scanf("%d", &Quantity);

    printf("Enter Price Per Unit: ");
    scanf("%d", &PricePerUnit);
    fprintf(file, "%s,%s,%d,%d\n", OrderID, BookTitle, Quantity, PricePerUnit);
    fclose(file);
    
    printf("Order added successfully!\n");
}

int main(){
    int choice;

    while(1){
        printf("\n==== Order Management System =====\n");
        printf("1. Display all orders\n");
        printf("2. Add New Order\n");
        printf("3. Exit\n");
        printf("Choose Option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            display_orders();
            break;
            case 2:
            add_order();
            break;
            case 3:
            printf("Exiting Program...\n");
            return 0;
            default:
            printf("Invalid Choice!\n");
    
        }
    }
}






