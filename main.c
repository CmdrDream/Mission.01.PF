#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAM "orders.csv" // กำหนดชื่อไฟล์ CSV ที่จะใช้เก็บข้อมูลคำสั่งซื้อ

// ฟังก์ชันแสดงรายการคำสั่งซื้อทั้งหมด
void display_orders(){
    FILE *file = fopen(FILENAM, "r"); // เปิดไฟล์โหมดอ่าน
    if (!file){
        printf("Error: Cannot open file %s\n", FILENAM);
        return; // ถ้าเปิดไฟล์ไม่ได้ ให้จบฟังก์ชัน
    }
    char line[256];
    while (fgets(line, sizeof(line), file)){ // อ่านไฟล์ทีละบรรทัด
        printf("%s", line); // แสดงบรรทัด
    }
    fclose(file); // ปิดไฟล์หลังใช้งาน
}

// ฟังก์ชันเพิ่มคำสั่งซื้อใหม่
void add_order(){
    FILE *file = fopen(FILENAM, "a"); // เปิดไฟล์โหมดต่อท้าย
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
    getchar(); // เคลียร์ newline จาก input ก่อนหน้า
    fgets(BookTitle, sizeof(BookTitle), stdin); // รับชื่อหนังสือ

    size_t len = strlen(BookTitle);
    if (len > 0 && BookTitle[len - 1] == '\n') {
        BookTitle[len - 1] = '\0'; // ลบ newline ออกจากชื่อหนังสือ
    }

    printf("Enter Quantity: ");
    scanf("%d", &Quantity);

    printf("Enter Price Per Unit: ");
    scanf("%d", &PricePerUnit);

    fprintf(file, "%s,%s,%d,%d\n", OrderID, BookTitle, Quantity, PricePerUnit); // เขียนลงไฟล์ CSV
    fclose(file);
    
    printf("✅ Order added successfully!\n"); // แจ้งผลสำเร็จ
}

// ฟังก์ชันค้นหาคำสั่งซื้อด้วย OrderID
void search_order(){
    FILE *file = fopen(FILENAM, "r"); // เปิดไฟล์โหมดอ่าน
    if (!file){
        printf("Error: Cannot open file %s\n", FILENAM);
        return;
    }

    char searchID[10];
    char line[256];
    int found = 0;

    printf("Enter Order ID to search: ");
    scanf("%s", searchID);

    while (fgets(line, sizeof(line), file)){ // อ่านไฟล์ทีละบรรทัด
        char OrderID[10], BookTitle[100];
        int Quantity, PricePerUnit;

        // แยกข้อมูลแต่ละฟิลด์
        sscanf(line, "%[^,],%[^,],%d,%d", OrderID, BookTitle, &Quantity, &PricePerUnit);

        if (strcmp(OrderID, searchID) == 0){ // ถ้าเจอ OrderID ตรงกับที่ค้นหา
            printf("\n=== Order Found ===\n");
            printf("Order ID: %s\n", OrderID);
            printf("Book Title: %s\n", BookTitle);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %d\n", PricePerUnit);
            found = 1;
            break; // เจอแล้วออกจากลูป
        }
    }

    if (!found){
        printf("❌ Order ID %s not found!\n", searchID); // แจ้งถ้าไม่พบ
    }

    fclose(file);
}

// ฟังก์ชันแก้ไขคำสั่งซื้อ ตาม OrderID
void update_order(){
    FILE *file = fopen(FILENAM, "r"); // เปิดไฟล์อ่าน
    FILE *temp = fopen("temp.csv", "w"); // สร้างไฟล์ชั่วคราวเขียนข้อมูลใหม่

    if (!file || !temp){
        printf("Error: Cannot open file.\n");
        return;
    }

    char searchID[10];
    char line[256];
    int found = 0;

    printf("Enter Order ID to update: ");
    scanf("%s", searchID);

    while (fgets(line, sizeof(line), file)){ // อ่านไฟล์ทีละบรรทัด
        char OrderID[10], BookTitle[100];
        int Quantity, PricePerUnit;

        sscanf(line, "%[^,],%[^,],%d,%d", OrderID, BookTitle, &Quantity, &PricePerUnit);

        if (strcmp(OrderID, searchID) == 0){ // ถ้าเจอ OrderID ที่ต้องการแก้ไข
            printf("\n=== Current Data ===\n");
            printf("Order ID: %s\n", OrderID);
            printf("Book Title: %s\n", BookTitle);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %d\n", PricePerUnit);

            // รับข้อมูลใหม่จากผู้ใช้
            printf("\nEnter New Book Title: ");
            getchar(); // เคลียร์ newline
            fgets(BookTitle, sizeof(BookTitle), stdin);
            size_t len = strlen(BookTitle);
            if (len > 0 && BookTitle[len - 1] == '\n') {
                BookTitle[len - 1] = '\0';
            }

            printf("Enter New Quantity: ");
            scanf("%d", &Quantity);

            printf("Enter New Price Per Unit: ");
            scanf("%d", &PricePerUnit);

            fprintf(temp, "%s,%s,%d,%d\n", OrderID, BookTitle, Quantity, PricePerUnit); // เขียนข้อมูลใหม่ลงไฟล์ temp
            found = 1;
        } else {
            fputs(line, temp); // ถ้าไม่ใช่ OrderID ที่แก้ไข ก็เขียนข้อมูลเดิมลงไฟล์ temp
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAM); // ลบไฟล์เก่า
    rename("temp.csv", FILENAM); // เปลี่ยนชื่อไฟล์ temp เป็นไฟล์หลัก

    if (found){
        printf("✅ Order ID %s updated successfully!\n", searchID);
    } else {
        printf("❌ Order ID %s not found!\n", searchID);
    }
}

// ฟังก์ชันลบคำสั่งซื้อ ตาม OrderID
void delete_order(){
    FILE *file = fopen(FILENAM, "r"); // เปิดไฟล์อ่าน
    FILE *temp = fopen("temp.csv", "w"); // ไฟล์ชั่วคราวเขียนข้อมูลที่ไม่ถูกลบ

    if (!file || !temp){
        printf("Error: Cannot open file.\n");
        return;
    }

    char searchID[10];
    char line[256];
    int found = 0;

    printf("Enter Order ID to delete: ");
    scanf("%s", searchID);

    while (fgets(line, sizeof(line), file)){ // อ่านไฟล์ทีละบรรทัด
        char OrderID[10], BookTitle[100];
        int Quantity, PricePerUnit;

        sscanf(line, "%[^,],%[^,],%d,%d", OrderID, BookTitle, &Quantity, &PricePerUnit);

        if (strcmp(OrderID, searchID) == 0){ // ถ้าเจอ OrderID ต้องลบ
            printf("\n=== Order Deleted ===\n");
            printf("Order ID: %s\n", OrderID);
            printf("Book Title: %s\n", BookTitle);
            printf("Quantity: %d\n", Quantity);
            printf("Price Per Unit: %d\n", PricePerUnit);
            found = 1; // ระบุว่าพบแล้ว
        } else {
            fputs(line, temp); // ถ้าไม่ใช่ OrderID ที่ลบ ก็เขียนลง temp
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAM); // ลบไฟล์เก่า
    rename("temp.csv", FILENAM); // เปลี่ยนชื่อไฟล์ temp เป็นไฟล์หลัก

    if (!found){
        printf("❌ Order ID %s not found!\n", searchID);
    } else {
        printf("✅ Delete successful!\n");
    }
}

// ฟังก์ชันหลัก โปรแกรมเริ่มทำงานที่นี่
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
                display_orders(); // แสดงรายการทั้งหมด
                break;
            case 2:
                add_order(); // เพิ่มคำสั่งซื้อใหม่
                break;
            case 3:
                search_order(); // ค้นหาคำสั่งซื้อ
                break;
            case 4:
                update_order(); // แก้ไขคำสั่งซื้อ
                break;
            case 5:
                delete_order(); // ลบคำสั่งซื้อ
                break;
            case 6:
                printf("Exiting Program...\n");
                return 0; // ออกจากโปรแกรม
            default:
                printf("Invalid Choice!\n"); // แจ้งถ้าผู้ใช้เลือกผิด
        }
    }
}
