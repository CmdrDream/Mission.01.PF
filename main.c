#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAM "orders.csv"

// แสดงรายการทั้งหมด
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

// เพิ่มข้อมูลใหม่
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
    getchar(); // Clear newline
    fgets(BookTitle, sizeof(BookTitle), stdin);

    size_t len = strlen(BookTitle);
    if (len > 0 && BookTitle[len - 1] == '\n') {
        BookTitle[len - 1] = '\0'; // Remove newline
    }

    printf("Enter Quantity: ");
    scanf("%d", &Quantity);

    printf("Enter Price Per Unit: ");
    scanf("%d", &PricePerUnit);

    fprintf(file, "%s,%s,%d,%d\n", OrderID, BookTitle, Quantity, PricePerUnit);
    fclose(file);
    
    printf("✅ Order added successfully!\n");
}

// ค้นหาข้อมูลตาม OrderID
void search_order(){
    FILE *file = fopen(FILENAM, "r");
    if (!file){
        printf("Error: Cannot open file %s\n", FILENAM);
        return;
    }

    char searchID[10];
    char line[256];
    int found = 0;

    printf("Enter Order ID to search: ");
    scanf("%s", searchID);

    while (fgets(line, sizeof(line), file)){
        char OrderID[10], BookTitle[100];
        int Quantity, PricePerUnit;

        sscanf(line, "%[^,],%[^,],%d,%d", OrderID, BookTitle, &Quantity, &PricePerUnit);

        if (strcmp(OrderID, searchID) == 0){
            printf("\n=== Order Found ===\n");
            printf("Order ID: %s\n", OrderID);
            printf("Book Title: %s\n", BookTitle);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %d\n", PricePerUnit);
            found = 1;
            break;
        }
    }

    if (!found){
        printf("❌ Order ID %s not found!\n", searchID);
    }

    fclose(file);
}

// แก้ไขข้อมูลตาม OrderID
void update_order(){
    FILE *file = fopen(FILENAM, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!file || !temp){
        printf("Error: Cannot open file.\n");
        return;
    }

    char searchID[10];
    char line[256];
    int found = 0;

    printf("Enter Order ID to update: ");
    scanf("%s", searchID);

    while (fgets(line, sizeof(line), file)){
        char OrderID[10], BookTitle[100];
        int Quantity, PricePerUnit;

        sscanf(line, "%[^,],%[^,],%d,%d", OrderID, BookTitle, &Quantity, &PricePerUnit);

        if (strcmp(OrderID, searchID) == 0){
            printf("\n=== Current Data ===\n");
            printf("Order ID: %s\n", OrderID);
            printf("Book Title: %s\n", BookTitle);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %d\n", PricePerUnit);

            printf("\nEnter New Book Title: ");
            getchar();
            fgets(BookTitle, sizeof(BookTitle), stdin);
            size_t len = strlen(BookTitle);
            if (len > 0 && BookTitle[len - 1] == '\n') {
                BookTitle[len - 1] = '\0';
            }

            printf("Enter New Quantity: ");
            scanf("%d", &Quantity);

            printf("Enter New Price Per Unit: ");
            scanf("%d", &PricePerUnit);

            fprintf(temp, "%s,%s,%d,%d\n", OrderID, BookTitle, Quantity, PricePerUnit);
            found = 1;
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAM);
    rename("temp.csv", FILENAM);

    if (found){
        printf("✅ Order ID %s updated successfully!\n", searchID);
    } else {
        printf("❌ Order ID %s not found!\n", searchID);
    }
}

// ลบข้อมูลตาม OrderID
void delete_order(){
    FILE *file = fopen(FILENAM, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!file || !temp){
        printf("Error: Cannot open file.\n");
        return;
    }

    char searchID[10];
    char line[256];
    int found = 0;

    printf("Enter Order ID to delete: ");
    scanf("%s", searchID);

    while (fgets(line, sizeof(line), file)){
        char OrderID[10], BookTitle[100];
        int Quantity, PricePerUnit;

        sscanf(line, "%[^,],%[^,],%d,%d", OrderID, BookTitle, &Quantity, &PricePerUnit);

        if (strcmp(OrderID, searchID) == 0){
            printf("\n=== Order Deleted ===\n");
            printf("Order ID: %s\n", OrderID);
            printf("Book Title: %s\n", BookTitle);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %d\n", PricePerUnit);
            found = 1;
            // ❌ ไม่เขียนทับลง temp.csv
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAM);
    rename("temp.csv", FILENAM);

    if (!found){
        printf("❌ Order ID %s not found!\n", searchID);
    } else {
        printf("✅ Delete successful!\n");
    }
}

// เมนูหลัก
int main(){
    int choice;

   while(1){
    printf("\n==== Order Management System =====\n");
    printf("1. Display all orders\n");
    printf("2. Add New Order\n");
    printf("3. Search Order by ID\n");
    printf("4. Update Order by ID\n");   
    printf("5. Delete Order by ID\n");   
    printf("6. Exit\n");                 
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
        search_order();
        break;
    case 4:
        update_order();
        break;
    case 5:
        delete_order();
        break;
    case 6:
        printf("Exiting Program...\n");
        return 0;
    default:
        printf("Invalid Choice!\n");
        }
    }
}
