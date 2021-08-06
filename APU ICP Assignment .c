//ICP ASSIGNMENT - INVENTORY MANAGEMENT SYSTEM.

//TABLE OF CONTENTS
//----------------
//MAIN() - 47
//MAIN MENU - 630
//VACCINE - 139
//INVENTORY - 428
//MISC/HELPER METHODS - 672
//BUBBLESORT - 762
//----------------

//LIBRARIES USED/IMPORTED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//import for allowing use of clrscr() or system("cls") 
#include<conio.h>

//ENUM for Distribution methods
//is a global variable.
enum type { addQuantity = 1, subQuantity = 2};


//FUNCTION DEFINITIONS
void displayMenu();
void displayVaccineMenu();
void displayInventoryMenu();

void displayVaccines();
void enterNewVaccine();
void searchVaccine();
void updateVaccine();

void distributeInventory();
void recieveInventory();
void displayInventory();
void updateVaccineInventory(char code[3], int sQuantity, int type);
int vaccineQuantity(char code[3]);

int validCountry(char countryName[20]);
void waitForUser();
FILE* createFile(char filename[10]);
int getStrLength(char str[]);
int fileSize(char filename[]);

void bubbleSort(int quantities[], char codes[]);
void swapArrElem(int* firstQuantity, int* secondQuantity);
void swapArrCode(char* firstCode, char* secondCode);

//list of approved manufacturers - for validation.
const char* vaccineManufacturers[21] = { "Australia","Belgium","Brasil","Bulgaria","Canada","China","Cuba","Denmark",
"France","Germany","India","Indonesia","Italy","Japan","Netherlands","RepublicofKorea","RussianFederation",
"Sweden","Switzerland","UK","USA" };

//NOTE: FOR TESTING ONLY ADD THESE CODES 
//VALID CODES IN ARRAY AND INDICES AS ENUM
enum vaccineCodes {PF = 0, SV, AZ, SP, CS};//enum representing index for vaccine array. - ignore.
char* vaccineArr[5] = {"PF", "SV", "AZ", "SP", "CS"};

//array of ints and char for code and quantity
int vaccineQuantities[5]; 

int main(void)
{
	//FILES - commented out 
	//File Pointer - for storing records.
	//FILE* vaccineFile;
	//FILE* distFile;

	//MAINMENU OPTION
	int choice = -1; //init to -1 - for while loop condition below.

	//while option is not "exit"
	while (choice != 6) {
		displayMenu(); //call display menu - for options.
	
		//get choice
		scanf("%d", &choice);

		//CHOICE VALIDATION CHECK
		while (choice == -1) { //while the choice is still invalid
			scanf("%d", &choice); //re obtain choice from user
		}


		//options seperated based on txt file being accessed
		//1 for manipulatingvaccine.txt 
		//2 for dist.txt

		//VAC
		if (choice == 1) {
			displayVaccineMenu();

			//get user's vaccine choice 
			printf("\nEnter choice: ");
			scanf("%d", &choice);

			//check choice n run appropriate function
			switch (choice) {
				case 1: //display
					displayVaccines();
					break;
				case 2: //enter new vaccine
					enterNewVaccine();
					break;
				case 3:
					searchVaccine();
					break;
				case 4:
					updateVaccine();
					break;
				case 5:
					continue;
					break;
				default:
					printf("\nINVALID CHOICE!!\n");
					break;
			}

		}//end of Vaccine

		//INV
		else if (choice == 2) {
			displayInventoryMenu();

			//get user's inventory choice 
			printf("\nEnter choice: ");
			scanf("%d", &choice);

			//check choice n run appropriate function
			switch (choice) {
				case 1:
					distributeInventory();
					break;
				case 2:
					recieveInventory();
					break;
				case 3:
					displayInventory();
					break;
				case 4:
					break;
				case 5:
					continue;
					break;
				default:
					printf("\nINVALID CHOICE!!\n");
					break;
			}
		}//End of Inventory

	}//!= 5
}
//###########################
//##        VACCINE        ##
//###########################

void displayVaccines() {
	char vaccineName[20];
	char vaccineCode[3];
	char vaccineProdCountry[20];
	int vaccineDosage;
	float vaccinePopCovered;
	int quantity;

	//table headings.
	printf("+---------------- - +-------------- + ------------------ - +---------------- - +------------------------ +----------+\n");
	printf("| Name of Vaccine   | Vaccine Code  |  Producing Country  |  Dosage Required  |  Population Covered(%)   | Quantity  |\n");
	printf("+---------------- - +-------------- + ------------------ - +---------------- - +------------------------ +----------+\n");

	FILE* vaccineFile = fopen("vaccine.txt", "r");

	//if file is null i.e. not found. - create and abort.
	if (vaccineFile == NULL) {
		printf("vaccine file not found.. No results to show...");
		vaccineFile = createFile("vaccine.txt");
		return;
	}

	//continuously read a line till no more lines
	//retrieve each word in read line
	//and store in appropriate variable.
	while (fscanf(vaccineFile, "%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, &vaccineDosage, &vaccinePopCovered, &quantity) != EOF)
	{
		//output variables.
		printf("%s ~  %s  ~  %s  ~  %d  ~  %f  ~  %d    \n", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);

		//check if end of file. 
		if (feof(vaccineFile)) //if at end of file - break the loop
			break;

	}

	while (fclose(vaccineFile) != 0) {//if close not successful i.e. 0
		fclose(vaccineFile);//close till closed.
	}

	waitForUser();
}

void enterNewVaccine() {
	//sample - Pfizer	PF	USA	2	50
	FILE* vaccineFile = fopen("vaccine.txt", "a"); //file opened for writing.

	//VARIABLES FOR INPUTTING 
	//section defining array of char (strings)
	//with the amount of characters they store.
	char vaccineName[20];
	char vaccineCode[3];
	char vaccineProdCountry[20];
	int vaccineDosage;
	float vaccinePopCovered;

	int initialQuantity = 1000000; //all vaccines init to 1M in quantity

	//if file is null - create
	if (vaccineFile == NULL)
		vaccineFile = createFile("vaccine.txt");

	if (vaccineFile != NULL) {
		//get new record
		printf("\nEnter vaccine name: ");
		scanf("%s", vaccineName);

		

		//validate code
		int validCode = -1;
		//is a valid vaccine in malaysia
		while (validCode == -1) { //infinite loop - while true
			printf("\nEnter vaccine code: ");
			scanf("%s", vaccineCode);

			for (int i = 0; i < 5; i++) {//compare each vaccine available with given vaccine
				printf("%s==%s\n", vaccineArr[i], vaccineCode);

				if (strcmp(vaccineCode, vaccineArr[i])) {//if same - break the loop - continue code.
					validCode = 1;
					break;
				}
			
				while (getStrLength(vaccineCode) != 2) {//validate length first.
					//re enter code.
					printf("\nInvalid Code - vaccines must be those planned for people in Malaysia ");

					printf("\nRe-Enter vaccine code: ");
					scanf("%s", vaccineCode);
				}
			}
		}
		

		printf("\nEnter vaccine origin - country produced: ");
		scanf("%s", vaccineProdCountry);

		//Validate that the country is a genuine manufacturer 
		//approved by World Health Organization (WHO).
		int isValid = validCountry(vaccineProdCountry);

		while (isValid == -1) {
			printf("\nRe-Enter vaccine origin - country produced: ");
			scanf("%s", vaccineProdCountry);
		}

		printf("\nEnter vaccine dosage: ");
		scanf("%d", &vaccineDosage);

		//validate doses - only 1 or 2
		while (vaccineDosage < 1 || vaccineDosage > 2) {
			printf("\nRe-Enter vaccine dosage: ");
			scanf("%d", &vaccineDosage);
		}

		printf("\nEnter vaccine population covered: ");
		scanf("%f", &vaccinePopCovered);

		//validate popCovered - 0 to 100
		//if not in range.
		while (!(vaccinePopCovered > 0 && vaccinePopCovered < 100)) {
			printf("\nRe-Enter vaccine population covered: ");
			scanf("%f", &vaccinePopCovered);
		}

		//get amount of lines in vaccine file.
		int lines = fileSize("vaccine.txt");
		
		//if only 0 - dnt add \n infront
		// if 1 or more - then add \b infront to add record to new line.
		if (lines == 0) {
			//1 record per line
			fprintf(vaccineFile, "%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, initialQuantity);
		}
		else
			fprintf(vaccineFile, "\n%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, initialQuantity);
		

		fclose(vaccineFile);
	}
}

void searchVaccine() {
	char searchCode[3];
	int found = -1;//intially -1 to indicate not found.

	char vaccineName[20]; //variables for storing read details.
	char vaccineCode[3];
	char vaccineProdCountry[20];
	int vaccineDosage;
	float vaccinePopCovered;
	int quantity;

	FILE* vaccineFile = fopen("vaccine.txt", "r"); //file opened for reading.

	//if file is null i.e. not found. - create and abort.
	if (vaccineFile == NULL) {
		printf("vaccine file not found.. aborting Search...");
		vaccineFile = createFile("vaccine.txt");
		return;
	}

	printf("\nEnter a vaccine code for searching: ");
	scanf("%s", searchCode);

	//validate code
	while (getStrLength(searchCode) != 2) {
		//re enter code.
		printf("\nInvalid Code");

		printf("\nRe-Enter vaccine code: ");
		scanf("%s", searchCode);
	}


	//continuously read a line till no more lines
	//retrieve each word in read line
	//and store in appropriate variable.
	while (fscanf(vaccineFile, "%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, &vaccineDosage, &vaccinePopCovered, &quantity) != EOF)
	{
		//compare search code with file code
		if (strcmp(vaccineCode, searchCode)) {
			found = 1;
		}
		
		//check if end of file.
		if (feof(vaccineFile)) //if at end of file - break the loop
			break;

	}

	
	if (found == 1) {
		printf("\nVaccine found: ");
		printf("%s ~ %s ~ %s ~ %d ~ %f ~ %d\n\n", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
	}
	else
		printf("\nVaccine not Found..");

	waitForUser();
}

//similar concept to search
//we move each record to a new file till we reach code searching
//update code
//move new code to new file
//delete old file and rename new file.
void updateVaccine() {
	char searchCode[3];
	int found = -1;//intially -1 to indicate not found.

	char vaccineName[20]; //variables for storing read details.
	char vaccineCode[3];
	char vaccineProdCountry[20];
	int vaccineDosage;
	float vaccinePopCovered;
	int quantity;

	int lastLine = fileSize("vaccine.txt"); //get file size = total lines
	
	//abort if empty file.
	if (lastLine == 0) {
		printf("File is empty...");
	}

	int currentLine = 0;//current line is line 0

	FILE* vaccineFile = fopen("vaccine.txt", "r"); //file opened for reading.
	FILE* newVaccineFile = fopen("newvaccine.txt", "w");//no need to validate - "w" creates file.. not NULL

	//if file is null i.e. not found. - create and abort.
	if (vaccineFile == NULL) {
		printf("vaccine file not found.. aborting Search...");
		vaccineFile = createFile("vaccine.txt");
		return;
	}

	printf("\nEnter a vaccine code for searching: ");
	scanf("%s", searchCode);

	//validate code
	while (getStrLength(searchCode) != 2) {
		//re enter code.
		printf("\nInvalid Code");

		printf("\nRe-Enter vaccine code: ");
		scanf("%s", searchCode);
	}


	//continuously read a line till no more lines
	//retrieve each word in read line
	//and store in appropriate variable.
	while (fscanf(vaccineFile, "%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, &vaccineDosage, &vaccinePopCovered, &quantity) != EOF)
	{
		currentLine++;

		if (strcmp(vaccineCode, searchCode) == 0) {
			found = 1;

			printf("\nEnter new vaccine code: ");
			scanf("%s", searchCode); //input new code into searchCode.

			//check if updating first or last line to determine where to put new line character.
			//if updating first line 
			if (currentLine == 0 || currentLine == 1) {
				fprintf(newVaccineFile, "%s %s %s %d %f %d\n", vaccineName, searchCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
				continue; //continue loop
			}

			//if currentline is before last line - dont add new line.
			if ((lastLine + 1) == currentLine) {
				fprintf(newVaccineFile, "\n%s %s %s %d %f", vaccineName, searchCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
			}
			else//print record with new code.
				fprintf(newVaccineFile, "%s %s %s %d %f", vaccineName, searchCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
		}
		else {
			if ((lastLine + 1) == currentLine) {//print unchanged records in new file.
				fprintf(newVaccineFile, "\n%s %s %s %d %f", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
			}
			else
				fprintf(newVaccineFile, "%s %s %s %d %f", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
		}
	}

	fclose(newVaccineFile);
	fclose(vaccineFile);

	//delete old file
	//rename old file to new
	remove("vaccine.txt");
	
	int success = rename("newvaccine.txt","vaccine.txt");

	//check if successful rename
	if (success) {
		printf("file updated successfully");
	}

	//check if found n ouput appropriately.
	if (found == 1) {
		printf("\nVaccine found and updated...");
	}
	else
		printf("\nVaccine not Found..");
}

//###########################


//###########################
//##      INVENTORY        ##
//###########################


void distributeInventory() {
	FILE* distFile = fopen("dist.txt", "a");
	
	char code[3];
	int quantity = 0;

	//if file is null - create
	if (distFile == NULL)
		distFile = createFile("dist.txt"); //creates file and returns file pointer.

	if (distFile != NULL) {
		printf("Enter vaccine to distribute: ");
		scanf("%s", code);

		//validate code
		while (getStrLength(code) != 2) {
			//re enter code.
			printf("\nInvalid Code");

			printf("\nRe-Enter vaccine code: ");
			scanf("%s", code);
		}

		printf("Enter quantity to distribute: ");
		scanf("%d", &quantity);

		//get amount of lines in vaccine file.
		int lines = fileSize("dist.txt");

		//if only 0 - dnt add \n infront
		// if 1 or more - then add \b infront to add record to new line.
		if (lines == 0) {
			//1 record per line
			fprintf(distFile, "%s %d", code, quantity); //store code and quantity
		}
		else
			fprintf(distFile, "\n%s %d", code, quantity);


		fclose(distFile);

		//update quantity in vaccine file // addQuantity for + subQuantity for -
		updateVaccineInventory(code, quantity, subQuantity);
	}
}

void recieveInventory() {
	char code[3];
	int quantity = 0;

	printf("Enter vaccine to recieve: ");
	scanf("%s", code);

	//validate code
	while (getStrLength(code) != 2) {
		//re enter code.
		printf("\nInvalid Code");

		printf("\nRe-Enter vaccine code: ");
		scanf("%s", code);
	}

	printf("Enter quantity to recieve: ");
	scanf("%d", &quantity);

	//dist file does not store recieved inv hence just update below.

	//update quantity in vaccine file // addQuantity for + subQuantity for -
	updateVaccineInventory(code, quantity, addQuantity);
	
}

//reads the file n retrieves quantity for each vaccine type and store in an array.
//then sorts the array of vaccine codes and quantities.
//then loops over array and displays.
void displayInventory() {
	char vaccineCode[3];
	int quantity;

	//table headings.
	printf("++-------------- + ----------+\n");
	printf("| Vaccine Code   |  Quantity  |\n");
	printf("+ +------------- + ----------+\n");

	//for each available vaccine - calculate the quantity in dist file and store in array for quantities.
	for (int i = 0; i < 5; i++) {
		vaccineQuantities[i] = vaccineQuantity(vaccineArr[i]);//pass in the code from array above as argument to method.
	}

	//sort vaccineArr and vaccineQuantities.
	bubbleSort(vaccineQuantities, vaccineArr);

	//for each vaccine in vaccineArr - output on screen.
	for (int i = 0; i < 5; i++) {
		printf("\n%s -> %d", vaccineArr[i], vaccineQuantities[i]);//pass in the code from array above as argument to method.
	}

	printf("\n");
	waitForUser();//simple outputs waiting msg and calls getchar to wait for enter key.
}

//same concept as updateVaccine however takes in values n performs code automatically
//with no user input.
//type determines if to add or substract quantity
//type of 1 = 
//type of 2 = sub
void updateVaccineInventory(char code[3], int sQuantity, int type) {
	int found = -1;//intially -1 to indicate not found.

	char vaccineName[20]; //variables for storing read details.
	char vaccineCode[3];
	char vaccineProdCountry[20];
	int vaccineDosage;
	float vaccinePopCovered;
	int quantity;

	int lastLine = fileSize("vaccine.txt"); //get file size = total lines

	//abort if empty file.
	if (lastLine == 0) {
		printf("File is empty...");
	}

	int currentLine = 0;//current line is line 0

	FILE* vaccineFile = fopen("vaccine.txt", "r"); //file opened for reading.
	FILE* newVaccineFile = fopen("newvaccine.txt", "w");//no need to validate - "w" creates file.. not NULL

	//if file is null i.e. not found. - create and abort.
	if (vaccineFile == NULL) {
		printf("vaccine file not found.. aborting Search...");
		vaccineFile = createFile("vaccine.txt");
		return;
	}


	//continuously read a line till no more lines
	//retrieve each word in read line
	//and store in appropriate variable.
	while (fscanf(vaccineFile, "%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, &vaccineDosage, &vaccinePopCovered, &quantity) != EOF)
	{
		currentLine++;

		if (strcmp(vaccineCode, code) == 0) {
			found = 1;

			//new quantity is calculated and stored. // 1 for + 2 for -
			if (type == addQuantity) {
				sQuantity = quantity + sQuantity;
			}
			else if (type == subQuantity) {
				sQuantity = quantity - sQuantity;
			}

			//if updating first line
			if (currentLine == 0 || currentLine == 1) {
				fprintf(newVaccineFile, "%s %s %s %d %f %d\n", vaccineName, code, vaccineProdCountry, vaccineDosage, vaccinePopCovered, sQuantity);
				continue; //continue loop
			}

			//if currentline is before last line - dont add new line.
			if ((lastLine + 1) == currentLine) { 
				fprintf(newVaccineFile, "\n%s %s %s %d %f %d", vaccineName, code, vaccineProdCountry, vaccineDosage, vaccinePopCovered, sQuantity);
			}
			else//print record with new code.
				fprintf(newVaccineFile, "%s %s %s %d %f %d", vaccineName, code, vaccineProdCountry, vaccineDosage, vaccinePopCovered, sQuantity);
		}
		else {
			if ((lastLine + 1) == currentLine) {//print unchanged records in new file.
				fprintf(newVaccineFile, "\n%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
			}
			else
				fprintf(newVaccineFile, "%s %s %s %d %f %d", vaccineName, vaccineCode, vaccineProdCountry, vaccineDosage, vaccinePopCovered, quantity);
		}
	}

	int succClose = fclose(newVaccineFile);

	//if file has not closed successfully - close again.
	while (succClose != 0) {
		fclose(newVaccineFile);
	}

	succClose = fclose(vaccineFile);

	while (succClose != 0) {
		fclose(vaccineFile);
	}

	//delete old file
	//rename old file to new
	remove("vaccine.txt");

	int success = rename("newvaccine.txt", "vaccine.txt");

	//check if successful rename
	if (success) {
		printf("vaccine file updated successfully");
	}
	else {
		//try again. if failed 
		//remove("vaccine.txt");

		int success = rename("newvaccine.txt", "vaccine.txt");
	}

}

//traverses through file
//searches for code
//where code found - quantity is added to total quantity
//quantity returned when done
int vaccineQuantity(char code[3]) {
	int found = -1;//intially -1 to indicate not found.

	char vaccineName[20]; //variables for storing read details.
	char vaccineCode[3];
	char vaccineProdCountry[20];
	int vaccineDosage;
	float vaccinePopCovered;
	int quantity;

	int totalQuantity = 0; //variable for storing total quantity.

	FILE* distFile = fopen("dist.txt", "r"); //file opened for reading.

	//if file is null i.e. not found. - create and abort.
	if (distFile == NULL) {
		printf("dist file not found.. aborting Search...");
		distFile = createFile("dist.txt");
		return -1;
	}

	//continuously read a line till no more lines
	//retrieve each word in read line
	//and store in appropriate variable.
	while (fscanf(distFile, "%s %d", vaccineCode, &quantity) != EOF)
	{
		//compare search code with file code
		if (strcmp(vaccineCode, code) == 0) {
			found = 1;
			totalQuantity += quantity; //add quantity of line to total
		}

		//check if end of file.
		if (feof(distFile)) //if at end of file - break the loop
			break;//exits loop

	}

	while (fclose(distFile) != 0) {//if close not successful i.e. 0
		fclose(distFile);//close till closed.
	}


	if (found != 1) {
		return 0;
	}

	return totalQuantity;//return total.
}

//###########################

//###########################
//##        MENU           ##
//###########################
void displayMenu() {
	system("cls");
	//section displaying menu options 1 and 2
	printf("	+ ---------- + ------------ +\n");
	printf("	| Option 1   |  Option 2    |\n");
	printf("	+----------- + ------------ +\n");
	printf("	| Vaccine    |  Inventory   |\n");
	printf("	|            |  System      |\n");
	printf("	+----------- + ------------ +\n");
	printf("	******Option 6 to Exit*******\n");
}

void displayVaccineMenu() {
	system("cls");
	printf("	+ ---------------------- +\n");
	printf("	|          VACCINE       |\n");
	printf("	+----------------------- +\n");
	printf("	| 1. Display Vaccines    |\n");
	printf("	| 2. Enter New Vaccine   |\n");
	//extra features
	printf("	| 3. Search Vaccine      |\n");
	printf("	| 4. Update Vaccine      |\n");
	printf("	| 5. Return to MainMenu |\n");
	printf("	+----------------------- +\n");
}

void displayInventoryMenu() {
	system("cls");
	printf("	+ ---------------------- +\n");
	printf("	|        INVENTORY       |\n");
	printf("	+----------------------- +\n");
	printf("	| 1. Distribute Vaccines |\n");
	printf("	| 2. Recieve Vaccines    |\n");
	printf("	| 3. List All Vaccines   |\n");
	printf("	| 4. Return to MainMenu  |\n");
	printf("	+----------------------- +\n");
}
//###########################

//###########################
//##         MISC		   ## this section is for helper functions.
//###########################


//SORTING METHOD - BUBBLESORT.
void bubbleSort(int quantities[], char* codes[])
{
	int i, j; //variable for indexing array in loop
	for (i = 0; i < 5 - 1; i++) // size -> 5 - 1

		// Last i elements are already in place  //also swap codes to match swapping in quantities.
		for (j = 0; j < 5 - i - 1; j++)
			if (quantities[j] > quantities[j + 1]) {
				swapArrCode(&codes[j], &codes[j + 1]);
				swapArrElem(&quantities[j], &quantities[j + 1]);
			}
}

void swapArrElem(int* firstQuantity, int* secondQuantity) {//pointers for poiting to elements for swapping them.
	int temp = *firstQuantity;
	*firstQuantity = *secondQuantity;
	*secondQuantity = temp;
}

void swapArrCode(char* firstCode, char* secondCode) {//pointers for poiting to elements for swapping them.
	int temp = *firstCode;
	*firstCode = *secondCode;
	*secondCode = temp;
}

//checks size of file to determine 
//if first record starts on new line.
int fileSize(char filename[]) {
	FILE* fp;
	fp = fopen(filename, "r");

	if (fp == NULL)
		return 0;

	char line[90];
	int maxChars = 85, lines = 0;

	while (fgets(line, maxChars, fp))
	{
		lines++;

		//precaution
		if (feof(fp))
			break;
	}

	
	fclose(fp);

	return lines;
}

int validCountry(char countryName[20]) {
	//iterate over vaccineManufacturers
	//and compare given country with each country in vaccineManufacturers.
	int found = -1;


	//EXTRA PRECAUTION:
	//if using other scanning methods
	//remove trailing newline character that makes comparison invalid.
	int len = strlen(countryName);
	if (countryName[len - 1] == '\n') {
		printf("ASD");
		countryName[len - 1] = 0;
	}


	for(int i = 0; i < 21; i++){ // 0 - first country 20 - last 
		if (strcmp(countryName,vaccineManufacturers[i])) {
			found = 1; //country is in list of valid countries.
			printf("Country entered is a genuine vaccine manufacturer!!\n");
			return found;
		}
	}

	printf("Country entered is NOT a genuine vaccine manufacturer!!\n");
	return found;
}

int getStrLength(char str[]) {
	int i;

	//iterate over string until end of line char found
	//inc after each iteration/each char 
	for (i = 0; str[i] != '\0'; i++);

	return i;
}

void waitForUser() {
	printf("Press any key to continue....");
	getchar();//to retrieve "enter" key sometimes left in stdin
	getchar();//to retreive next enter key of user.
}

//creates file with given file name.
FILE* createFile(char filename[10]) {
	FILE* fp;
	fp = fopen(filename, "w"); //creates file //file opened for writing.
	
	if (fp == NULL) {
		printf("File failed to be created\n");
		printf("TIP: Exit and Create Manually");
		return NULL;
	}
		
	//return file pointer for use in other functions 
	printf("File Created Successully!!\n");
	waitForUser();
	return fp;
}

//###########################

