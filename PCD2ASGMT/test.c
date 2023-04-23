#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

void displaySalesRecord();
void addSalesOrder();
void modifySales();
void deleteSales();

// Q1 - declaration of structure

typedef struct {
	char member_Id[5];
	char itemCode[6];
	int quantitySold;
    char salesOrderID;
}SalesOrder;

void displaySalesRecord() {
	SYSTEMTIME t;
	FILE* disPtr;
	int count = 0;
	SalesOrder s1;
	disPtr = fopen("Sales2.dat", "rb");
	if (disPtr == NULL) {
		printf("Error");
		exit(-1);
	}

	GetLocalTime(&t);
	printf("\n");
	printf("\tSales Record - as at %d-%d-%d\n", t.wDay, t.wMonth, t.wYear);
	printf("\n");
	printf("Order ID Item Code \t Member ID \t Quantity Sold\t \n");
	printf("======\t ============== \t ========= \t ============= \n");
	while (fread(&s1, sizeof(SalesOrder), 1, disPtr)) {
		printf("%s \t %s \t %s \t %d \n", s1.itemCode, s1.member_Id, s1.quantitySold);
		count++;
	}
	printf("<%d Sales record listed>\n", count);
	fclose(disPtr);
}

void addSalesOrder() {

	SalesOrder order;
	order.salesOrderID = 1;
	FILE* ptr;
	char response;
	ptr = fopen("Sales2.dat", "ab");
	if (ptr == NULL) {
		printf("Error Opening File ！");
		exit(-1);
	}

	do {
		printf("Sales Order ID : S%03d\n", order.salesOrderID++);
		printf("==================================================\n");
		//user input for product
		printf("Enter Member ID: ");
		rewind(stdin);
		scanf("%s", order.member_Id);//& - optional

		printf("Enter Item Code: ");
		rewind(stdin);
		scanf("%s", order.itemCode);//& - compulsory

		printf("Enter Quantity: ");
		rewind(stdin);
		scanf("%d", &order.quantitySold);//& - optional

		//write into sales.dat
		fwrite(&order, sizeof(SalesOrder), 1, ptr);

		printf("Any more records?");//get user to continue with next record
		rewind(stdin);
		scanf("%c", &response);

	} while (toupper(response) == 'Y');
	fclose(ptr);
}


void modifySales() {

	SalesOrder modifyS[20];
	int mCount = 0;
	FILE* mPtr;
	mPtr = fopen("Sales2.dat", "rb");

	if (mPtr == NULL) {
		printf("Error Opening File !");
		exit(-1);
	}
	int i = 0;
	while (fread(&modifyS[i], sizeof(SalesOrder), 1, mPtr)) {
		i++;
		//p count keep track how many records are store in Product.dat and array p[20]
		mCount++;
	}
	fclose(mPtr);

	//2nd point
	char tempCode[6], tempId[5];
	int tempQty = 0;
	int found = 0, tempI;
	char response;
	char con;
	int modifiedCount = 0;//4th point
	// found- if record exist, change file = 1
	// temp I- If record found, copy the index value of the particular code

	do { // 4th point - do - while(con == 'Y')
		printf("Enter the item code to modify: ");
		scanf("%s", tempCode);

		for (int i = 0; i < mCount; i++) {
			if (strcmp(tempCode, modifyS[i].itemCode) == 0) {
				found = 1;// found = 1, record found
				tempI = i;// modification done outside of the for loop
				printf("Enter updated Member ID: ");
				scanf("%s", &tempId);// & -compulsory
				printf("Enter updated Quantity: ");
				rewind(stdin);
				scanf("%d", &tempQty);
			}// end if
		}//end for

		if (found == 0)
			printf("Record not found!");
		else if (found == 1) {
			printf("Sure to modify ( Y= YES)? ");
			rewind(stdin);// charcater 
			scanf("%c", &response);// 3rd point

			if (toupper(response) == 'Y') {

				strcpy(modifyS[tempI].member_Id, tempId);
				modifyS[tempI].quantitySold = tempQty;
				printf("Updated record: ");
				printf("===================================\n\n");
				printf("Item Code: %s\n", modifyS[tempI].itemCode);
				printf("Member ID: %s\n", modifyS[tempI].member_Id);
				printf("Quantity: %d\n", modifyS[tempI].quantitySold);
				printf("===================================\n\n");
				modifiedCount++;//4th point
			}//end if
		}//end else if

		printf("Do you want to modify another record? ");
		rewind(stdin);//4th point
		scanf("%c", &con);//4th point

	} while (toupper(con) == 'Y');//4th point

	//5th point
	FILE* wPtr;
	wPtr = fopen("Sales2.dat", "wb");// you can use product.dat

	fwrite("&modifyS", sizeof(SalesOrder), mCount, wPtr);

	printf("%d records modified\n", modifiedCount);
	fclose(wPtr);
}

void deleteSales() {

	FILE* salesReadPtr, * salesWritePtr;
	char itemC[6], confirm;
	SalesOrder delSales;

	char nextItem;
	do {
		printf("Enter Item Code to delete: ");
		scanf("%s", &itemC);

		salesReadPtr = fopen("Sales.dat", "rb");
		if (salesReadPtr == NULL) {
			printf("Error");
			exit(-1);
		}

		salesWritePtr = fopen("temp.dat", "wb");
		if (salesWritePtr == NULL) {
			printf("Error Opening File !");
			exit(-1);
		}

		while (fread(&delSales, sizeof(SalesOrder), 1, salesReadPtr)) {
			if (strcmp(delSales.itemCode, itemC) != 0) {
				fwrite(&delSales, sizeof(SalesOrder), 1, salesWritePtr);
			}
		}

		fclose(salesReadPtr);
		fclose(salesWritePtr);

		// Prompt user for confirmation before deleting member
		do {
			printf("Are you sure you want to delete Item Code %s? (Y/N)", itemC);
			scanf(" %c", &confirm);
		} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');

		if (toupper(confirm) == 'Y') {
			remove("Sales.dat");
			rename("temp.dat", "Sales.dat");
			printf("Item with Code %s has been deleted.\n", itemC);
		}

		else {
			remove("temp.dat");
			printf("Item Code %s has NOT been deleted.\n", itemC);
		}

		printf("Do you want to delete another Item? Y/N :");
		rewind(stdin);
		scanf("%c", &nextItem);

	} while (toupper(nextItem) == 'Y');
}

int menu() {

	int choice;
	printf("Welcome to Pro Sdn.Bhd.\n");
	printf("1. Add Sales Order\n");
	printf("2. Display Sales Order\n");
	printf("3. Modify Sales Order\n");
	printf("4. Delete Sales Order\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);

	return choice;
}
void main() {
	int choice;
	do {
		choice = menu();//function call - menu()
		switch (choice) {
		case 1: addSalesOrder();
			system("pause");
			break;
		case 2: displaySalesRecord();
			system("pause");
			break;
		case 3: modifySales();
			system("pause");
			break;
		case 4: deleteSales();
			system("pause");
			break;
		case 5:
			return;
			break;
		default: printf("Invalid input! Re-Enter again!\n");

		}
	} while (choice != 5);
}//end main