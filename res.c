#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[20];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

void generatedBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t     ADV.Restaurant");
    printf("\n\t ------------------------");
    printf("\nDate : %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n----------------------------------");
    printf("\n\n");
}

void generatedBillBody(char item[20], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generatedBillFooter(float total) {
    printf("\n");
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float cgst = 0.09 * netTotal;
    float sgst = 0.09 * netTotal;
    float grandTotal = netTotal + 2 * cgst;

    printf("------------------------------------------------\n");
    printf("Sub Total\t\t\t%.2f\n", total);
    printf("Discount @10%%\t\t\t%.2f\n", discount);
    printf("\t\t\t\t--------\n");
    printf("Net Total\t\t\t%.2f\n", netTotal);
    printf("CGST @9%%\t\t\t%.2f\n", cgst);
    printf("SGST @9%%\t\t\t%.2f\n", sgst);
    printf("-------------------------------------\n");
    printf(" Grand Total\t\t\t%.2f\n", grandTotal);
    printf("-------------------------------------\n");
}

int main() 
{
    int opt, n;
    float total = 0.0;
    struct orders ord, order;
    char saveBill = 'y',contFlag='y';
    char name[50];
    FILE *fp;
	while(contFlag == 'y'){
	float total = 0;
	int invoiceFound = 0;
    printf("\t=============ADV. RESTAURANT=============");
    printf("\n1.Generate Invoice");
    printf("\n2.Show all Invoice");
    printf("\n3.Search Invoice");
    printf("\n4.Exit\n");

    printf("\n\nPlease select your preferred operation:\t ");
    scanf("%d", &opt);
    fgetc(stdin); // Consume newline character left by scanf

    switch (opt) {
        case 1:
            system("clear");
            printf("\nPlease enter the name of the customer: \t");
            fgets(ord.customer, 50, stdin);
            ord.customer[strlen(ord.customer) - 1] = '\0'; // Remove newline character

            strcpy(ord.date, __DATE__); // Use current date

            printf("\nPlease enter the number of items: \t");
            scanf("%d", &n);
            ord.numOfItems = n;

            for (int i = 0; i < n; i++) {
                fgetc(stdin); // Consume newline character left by scanf

                printf("\n\n");
                printf("Please enter the item %d: ", i + 1);
                fgets(ord.itm[i].item, sizeof(ord.itm[i].item), stdin);
                ord.itm[i].item[strlen(ord.itm[i].item) - 1] = '\0'; // Remove newline character

                printf("\nPlease enter the quantity: \t");
                scanf("%d", &ord.itm[i].qty);

                printf("\nPlease enter the unit price: ");
                scanf("%f", &ord.itm[i].price);

                total += ord.itm[i].qty * ord.itm[i].price;
            }

            generatedBillHeader(ord.customer, ord.date);

            for (int i = 0; i < ord.numOfItems; i++) {
                generatedBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
            }

            generatedBillFooter(total);

            printf("\nDo you want to save the invoice (y/n): ");
            scanf(" %c", &saveBill);

            if (saveBill == 'y' || saveBill == 'Y') {
                fp = fopen("RestaurantBill.dat", "a+");
                if (fp == NULL) {
                    printf("Error opening file!\n");
                    return 1;
                }
                fwrite(&ord, sizeof(struct orders), 1, fp);
                printf("\nInvoice saved successfully.\n");
                fclose(fp);
            }
            break;

        case 2:
            system("clear");
            fp = fopen("RestaurantBill.dat", "r");
            if (fp == NULL) {
                printf("No invoices found.\n");
                break;
            }
            printf("\n ***** Your Previous Invoices *****\n");
            while (fread(&order, sizeof(struct orders), 1, fp)) {
                float tot = 0;
                generatedBillHeader(order.customer, order.date);
                for (int i = 0; i < order.numOfItems; i++) {
                    generatedBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                    tot += order.itm[i].qty * order.itm[i].price;
                }
                generatedBillFooter(tot);
            }
            fclose(fp);
            break;

        case 3:
            system("clear");
            printf("Enter the name of the customer: ");
            fgets(name, 50, stdin);
            name[strlen(name) - 1] = '\0';

            fp = fopen("RestaurantBill.dat", "r");
            if (fp == NULL) {
                printf("No invoices found.\n");
                break;
            }

            printf("\n ***** Invoices of %s *****\n", name);
            while (fread(&order, sizeof(struct orders), 1, fp)) {
                float tot = 0;
                if (strcmp(order.customer, name) == 0) {
                    generatedBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generatedBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generatedBillFooter(tot);
                    invoiceFound = 1;
                }
            }
            fclose(fp);

            if (!invoiceFound) {
                printf("No invoice found for customer: %s\n", name);
            }
            break;

        case 4:
            printf("Exiting...\n");
            exit(0);
            break;
            
            default:
            printf("Sorry invalid option");
            break;
    }
    printf("\nDo you want to perform another operation?[y/n]:\t");
    scanf("%s",&contFlag);
}
    printf("\n\n");
    return 0;
}

