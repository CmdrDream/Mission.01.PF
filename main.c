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

int main(){
    printf("=== Order List ===\n");
    display_orders();
    return 0;
}
