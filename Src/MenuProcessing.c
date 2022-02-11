/*********** Does all menus processing ***********/

#include <sdk_tplus.h>
#include "menu.h"

/* Global Variables */
const char amountMask[] = "/d/d/d,/d/d/d,/d/d/D";
const char currencyLabel[3 + 1] = "Ksh";
const char pinMask[] = "/d/d/d/d";
char successMessage[100];
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
		memset(saleAmount, 0, sizeof saleAmount);
		selectedMenu = GL_Dialog_Amount(gGoalGraphicLibInstance, "Sale", "Enter Sale Amount", amountMask, saleAmount, sizeof(saleAmount), currencyLabel, GL_ALIGN_BOTTOM_LEFT, GL_TIME_INFINITE);

	/* Handle Cancel */
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}



	/* Validate amount entered */
	int convertedAmount = atoi(saleAmount);

	if (convertedAmount >= 5000 || convertedAmount < 1) {
		GL_Dialog_Message(gGoalGraphicLibInstance, "Error", "Amount should be between 1 and 5000 Ksh", GL_ICON_ERROR, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		goto lblEnterAmount;
	}



	/* Prompt for card */
	/* Pass storage for card number */
	waitCard(cardDetails);

	/* Prompt for Pin */
	lblPinEntry:
		memset(salePin, 0, sizeof salePin);
		selectedMenu = GL_Dialog_Password(gGoalGraphicLibInstance, "Sale", "Enter Pin#: ", pinMask, salePin, sizeof(salePin), GL_TIME_INFINITE);


	/* Handle Cancel */
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}

	/* Validate pin length */
	if (strlen(salePin) < 3) {
		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Pin is short.", GL_ICON_ERROR, GL_BUTTON_NONE,3 * GL_TIME_SECOND);
		goto lblPinEntry;
	}


	memset(successMessage, 0, sizeof successMessage);

	strcat(successMessage, "Sale Amount\t");
	strcat(successMessage, saleAmount);
	strcat(successMessage, "\nCard Number\t");
	strcat(successMessage, cardDetails);
	strcat(successMessage, "\nDate");
	strcat(successMessage, tcDate);
	strcat(successMessage, tcTime);

	/*Transaction Successful*/
	GL_Dialog_Message(gGoalGraphicLibInstance,	"Transaction successful!", successMessage,	GL_ICON_INFORMATION, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);


	/* Print */
	//Create document to print
	xDocument = createDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);

	//Prepare Document
	prepareSaleDocument(xDocument, xLayout, myLine, cardDetails, saleAmount, tcDate, tcTime);

	//Print Document
	printDocument(xDocument);

	goto lblEnd;

	lblCancelled:
		GL_Dialog_Message(gGoalGraphicLibInstance,	NULL, "Transaction cancelled!",	GL_ICON_ERROR, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);
	lblEnd:
		return 1;

}



/* Bill Payment */
void processBillPayment(void){

	ulong selectedMenu = 0;
	const char mask[] = "/d/d/d/d/d/d/d/d";
	T_GL_HWIDGET xLayout = NULL;



	/* Get Start Time of Transaction */
	getDateAndTime();

	lblBillNumberEntry:
		memset(billNumber, 0, sizeof billNumber);
		selectedMenu = GL_Dialog_Text(gGoalGraphicLibInstance, "Bill Payment", "Enter Bill Number", mask, billNumber, sizeof(billNumber), GL_TIME_INFINITE);

	//Handle cancel
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}
	//validate length
	if(strlen(billNumber) < 5){
		GL_Dialog_Message(gGoalGraphicLibInstance,	"Error", "Bill Number should be more than 4 digits", GL_ICON_ERROR, GL_BUTTON_NONE,2 * GL_TIME_SECOND);
		goto lblBillNumberEntry;

	}

	/* Prompt for bill payment */
	lblEnterBillAmount:
		memset(billAmount, 0, sizeof billAmount);
		selectedMenu = GL_Dialog_Amount(gGoalGraphicLibInstance, "Bill Payment", "Enter Bill Amount", amountMask, billAmount, sizeof(billAmount), currencyLabel, GL_ALIGN_BOTTOM_LEFT, GL_TIME_INFINITE);

	/* Handle Cancel */
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}



	/* Validate amount entered */
	int convertedAmount = atoi(billAmount);

	if (convertedAmount < 1) {
		GL_Dialog_Message(gGoalGraphicLibInstance, "Error", "Please enter amount", GL_ICON_ERROR, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		goto lblEnterBillAmount;
	}



	//Let's display what happened
	memset(successMessage, 0, sizeof successMessage);

	strcat(successMessage, "Bill No\t");
	strcat(successMessage, billNumber);
	strcat(successMessage, "\nAmount\t");
	strcat(successMessage, billAmount);
	strcat(successMessage, "\nDate");
	strcat(successMessage, tcDate);
	strcat(successMessage, tcTime);

	/*Transaction Successful*/
	GL_Dialog_Message(gGoalGraphicLibInstance,	"Transaction successful!", successMessage,	GL_ICON_INFORMATION, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);




	/* Print */
	//Create document to print
	xDocument = createDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);

	//Prepare Document
	prepareBillPaymentDocument(xDocument, xLayout, myLine, billNumber, billAmount, tcDate, tcTime);

	//Print Document
	printDocument(xDocument);

	goto lblEnd;

	lblCancelled:
		GL_Dialog_Message(gGoalGraphicLibInstance,	NULL, "Transaction cancelled!",	GL_ICON_ERROR, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);

	lblEnd:
		return 1;

}


/* Sale with cashback */
void processSaleWithCashback(void){

	ulong selectedMenu = 0;
	T_GL_HWIDGET xLayout = NULL;

	memset(cardDetails, 0, sizeof(cardDetails));

	/* Get Start Time of Transaction */
	getDateAndTime();


	/* Prompt for Sale Amount */
	lblEnterSWCBAmount:
		memset(saleWithCashBackAmount, 0, sizeof saleWithCashBackAmount);
		selectedMenu = GL_Dialog_Amount(gGoalGraphicLibInstance, "Sale With Cashback", "Enter Sale Amount", amountMask, saleWithCashBackAmount, sizeof(saleWithCashBackAmount), currencyLabel, GL_ALIGN_BOTTOM_LEFT, GL_TIME_INFINITE);

	/* Handle Cancel */
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}



	/* Validate amount entered */
	int convertedSaleAmount = atoi(saleWithCashBackAmount);

	if (convertedSaleAmount < 1) {
		GL_Dialog_Message(gGoalGraphicLibInstance, "Error", "Please enter amount", GL_ICON_ERROR, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		goto lblEnterSWCBAmount;
	}




	/* Prompt for Cashback Amount */
	lblEnterCBAmount:
		memset(cashBackAmount, 0, sizeof cashBackAmount);
		selectedMenu = GL_Dialog_Amount(gGoalGraphicLibInstance, "Sale With Cashback", "Enter Cash Back Amount", amountMask, cashBackAmount, sizeof(cashBackAmount), currencyLabel, GL_ALIGN_BOTTOM_LEFT, GL_TIME_INFINITE);

	/* Handle Cancel */
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}


	/* Validate amount entered */
	int convertedCashBackAmount = atoi(cashBackAmount);

	if (convertedCashBackAmount < 1) {
		GL_Dialog_Message(gGoalGraphicLibInstance, "Error", "Please enter amount", GL_ICON_ERROR, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		goto lblEnterCBAmount;
	}



	/* Prompt for card */
	/* Pass storage for card number */
	waitCard(cardDetails);


	/* Prompt for Pin */
	lblPinEntry:
		memset(saleWithCashBackPin, 0, sizeof saleWithCashBackPin);
		selectedMenu = GL_Dialog_Password(gGoalGraphicLibInstance, "Sale with Cashback", "Enter Pin#: ", pinMask, saleWithCashBackPin, sizeof(saleWithCashBackPin), GL_TIME_INFINITE);


	/* Handle Cancel */
	if (selectedMenu == GL_KEY_CANCEL) {
		goto lblCancelled;
	}

	/* Validate pin length */
	if (strlen(saleWithCashBackPin) < 3) {
		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Pin is short.", GL_ICON_ERROR, GL_BUTTON_NONE,3 * GL_TIME_SECOND);
		goto lblPinEntry;
	}


	memset(successMessage, 0, sizeof successMessage);

	strcat(successMessage, "Sale Amount\t");
	strcat(successMessage, saleAmount);
	strcat(successMessage, "\nCash back amount\t");
	strcat(successMessage, cashBackAmount);
	strcat(successMessage, "\nCard Number\t");
	strcat(successMessage, cardDetails);
	strcat(successMessage, "\nDate");
	strcat(successMessage, tcDate);
	strcat(successMessage, tcTime);


	/*Transaction Successful*/
	GL_Dialog_Message(gGoalGraphicLibInstance,	"Transaction successful!", successMessage,	GL_ICON_INFORMATION, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);



	/* Print */
	//Create document to print
	xDocument = createDocument(gGoalGraphicLibInstance, GL_ENCODING_UTF8);

	//Prepare Document
	prepareSaleWithCashBackDocument(xDocument, xLayout, myLine, cardDetails, saleWithCashBackAmount, cashBackAmount, tcDate, tcTime);

	//Print Document
	printDocument(xDocument);

	goto lblEnd;

	lblCancelled:
		GL_Dialog_Message(gGoalGraphicLibInstance,	NULL, "Transaction cancelled!",	GL_ICON_ERROR, GL_BUTTON_NONE,	2 * GL_TIME_SECOND);

	lblEnd:
		return 1;

}
