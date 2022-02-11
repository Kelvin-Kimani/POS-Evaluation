/*********** Does all menus processing ***********/

#include <sdk_tplus.h>
#include "menu.h"

/* Global Variables */
const char amountMask[] = "/d/d/d,/d/d/d,/d/d/D";
const char currencyLabel[3 + 1] = "Ksh";
const char pinMask[] = "/d/d/d/d";
T_GL_HWIDGET xDocument;
T_GL_HWIDGET myLayout;
byte myLine;


/*Card Details */
char cardDetails[384];


/* Date and Time */
Telium_Date_t xDate;
char tcDate[32+1];
char tcTime[32+1];


//Sale
char saleAmount[10];
char salePin[4];

//Bill Payment
char billNumber[8 + 1];
char billAmount[12];

//Sale with cash
char saleWithCashBackAmount[10];
char cashBackAmount[10];
char saleWithCashBackPin[8];


/* Reusables */
void getDateAndTime(){

	//Read Date and Time from Terminal
	Telium_Read_date(&xDate);

	memset(tcDate, 0, sizeof tcDate);
	memset(tcTime, 0, sizeof tcTime);

	// Write to Variables
	Telium_Sprintf (tcDate, "%2.2s/%2.2s/20%2.2s  ",                              	// Retrieve date
				xDate.day, xDate.month, xDate.year);
	Telium_Sprintf (tcTime, "  %2.2s:%2.2s",                                        // Retrieve time
				xDate.hour, xDate.minute);

}

void transactionSuccessful(char * message){

	GL_Dialog_Message(gGoalGraphicLibInstance,	"Transaction Successful", message,	GL_ICON_INFORMATION, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);
	menuGoalDisplay();
}

void transactionCancelled(){

	GL_Dialog_Message(gGoalGraphicLibInstance,	NULL, "Transaction cancelled!",	GL_ICON_ERROR, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);
	menuGoalDisplay();
}

void enterAmount(char * title, char * text, char * mask, char * amount, ulong size){

	ulong selectedMenu = 0;

	lblAmountEntry:
		memset(amount, 0, sizeof amount);
		selectedMenu = GL_Dialog_Amount(gGoalGraphicLibInstance, title, text, mask, amount, size, currencyLabel, GL_ALIGN_BOTTOM_LEFT, 1 * GL_TIME_MINUTE);


	//Handle cancel
	if (selectedMenu == GL_KEY_CANCEL) {
		transactionCancelled();
	}

	//	Validation
	int convertedAmount = atoi(amount);

	if (convertedAmount < 1) {
		GL_Dialog_Message(gGoalGraphicLibInstance, "Error", "Minimum amount is Ksh 1", GL_ICON_ERROR, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		goto lblAmountEntry;
	}

}

void enterPin(char * title, char * pinMask, char * text, char * value){

	ulong selectedMenu = 0;

	lblPinEntry:
		memset(value, 0, sizeof value);
		selectedMenu = GL_Dialog_Password(gGoalGraphicLibInstance, title, text, pinMask, value, sizeof(value),30 * GL_TIME_SECOND);


	//Handle cancel
	if (selectedMenu == GL_KEY_CANCEL) {
		transactionCancelled();
	}

	//Validate
	if (strlen(value) < 3) {
		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Pin is short.", GL_ICON_ERROR, GL_BUTTON_NONE,3 * GL_TIME_SECOND);
		goto lblPinEntry;
	}

}



/* Sale */
void processSale(void){

	ulong selectedMenu = 0;
	const char amountMask[] = "/d,/d/d/D";
	T_GL_HWIDGET xLayout = NULL;

	memset(cardDetails, 0, sizeof(cardDetails));

	/* Get Start Time of Transaction */
	getDateAndTime();


	/*Enter Sale Amount */
	lblEnterAmount:
		enterAmount("Sale", "Enter Sale Amount", amountMask, saleAmount, sizeof(saleAmount));

	/* Cancel option */
	if(selectedMenu == GL_KEY_CANCEL){
		transactionCancelled();
	}

	/* Validate amount entered */
	int convertedAmount = atoi(saleAmount);

	if (convertedAmount >= 5000) {

		GL_Dialog_Message(gGoalGraphicLibInstance, "Error", "Amount should be between 1 and 5000 Ksh", GL_ICON_ERROR, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		goto lblEnterAmount;
	}


	/* Prompt for card */
	/* Pass storage for card number */
	waitCard(cardDetails);

	/* Prompt for Pin */
	enterPin("Sale", pinMask, "Enter Pin#: ", salePin);


	/* Print */
	//Create document to print
	xDocument = createDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);

	//Prepare Document
	prepareSaleDocument(xDocument, xLayout, myLine, cardDetails, saleAmount, tcDate, tcTime);

	//Print Document
	printDocument(xDocument);

}



/* Bill Payment */
void processBillPayment(void){

	const char mask[] = "/d/d/d/d/d/d/d/d";
	char successMessage[100];
	T_GL_HWIDGET xLayout = NULL;
	ulong selectedMenu = 0;


	/* Get Start Time of Transaction */
	getDateAndTime();

	lblBillNumberEntry:
		memset(billNumber, 0, sizeof billNumber);
		selectedMenu = GL_Dialog_Text(gGoalGraphicLibInstance, "Bill Payment", "Enter Bill Number", mask, billNumber, sizeof(billNumber), 1 * GL_TIME_MINUTE);

	//Handle cancel
	if (selectedMenu == GL_KEY_CANCEL) {
		transactionCancelled();
	}
	//validate length
	if(strlen(billNumber) < 5){
		GL_Dialog_Message(gGoalGraphicLibInstance,	"Error", "Bill Number should be more than 4 digits", GL_ICON_ERROR, GL_BUTTON_NONE,2 * GL_TIME_SECOND);
		goto lblBillNumberEntry;

	}

	/* Prompt for bill payment */
	enterAmount("Bill Payment", "Enter Bill Amount", amountMask, billAmount, sizeof(billAmount));

	//Let's display what happened
	memset(successMessage, 0, sizeof successMessage);

	strcat(successMessage, "Bill No\t");
	strcat(successMessage, billNumber);
	strcat(successMessage, "\nAmount\t");
	strcat(successMessage, billAmount);
	strcat(successMessage, "\nDate");
	strcat(successMessage, tcDate);
	strcat(successMessage, tcTime);


	/* Print */
	//Create document to print
	xDocument = createDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);

	//Prepare Document
	prepareBillPaymentDocument(xDocument, xLayout, myLine, billNumber, billAmount, tcDate, tcTime);

	//Print Document
	printDocument(xDocument);

	//Display success message
	transactionSuccessful(successMessage);

}


/* Sale with cashback */
void processSaleWithCashback(void){

	T_GL_HWIDGET xLayout = NULL;

	memset(cardDetails, 0, sizeof(cardDetails));

	/* Get Start Time of Transaction */
	getDateAndTime();


	/* Prompt for Sale Amount */
	enterAmount("Sale With Cashback", "Enter Sale Amount", amountMask, saleWithCashBackAmount, sizeof(saleWithCashBackAmount));

	/* Prompt for Cashback Amount */
	enterAmount("Sale With Cashback", "Enter Cashback Amount", amountMask, cashBackAmount, sizeof(cashBackAmount));

	/* Prompt for card */
	/* Pass storage for card number */
	waitCard(cardDetails);

	//Get users pin
	enterPin("Sale with Cashback",pinMask, "Enter Pin#:", saleWithCashBackPin);

	/* Print */
	//Create document to print
	xDocument = createDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);

	//Prepare Document
	prepareSaleWithCashBackDocument(xDocument, xLayout, myLine, cardDetails, saleWithCashBackAmount, cashBackAmount, tcDate, tcTime);

	//Print Document
	printDocument(xDocument);

}
