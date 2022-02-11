//Contains all print functions

#include "sdk_tplus.h"
#include "GL_GraphicLib.h"

#define LAYOUT_ID     0x0900

T_GL_HWIDGET createDocument (T_GL_HGRAPHIC_LIB hGraphicLib, T_GL_ENCODING_CHARSET eCharSet)
{
	T_GL_HWIDGET hDocument, hLayout;

	// Build document context
	// Changes the encoding charset of the graphic library
	GL_GraphicLib_SetCharset(hGraphicLib, eCharSet);

	// Create a document to print
	hDocument = GL_Document_Create(hGraphicLib);
	if (hDocument!=NULL) {
			goto lblEnd;
		}

	// Create a layout on this document
	hLayout = GL_Layout_Create(hDocument);
	if (hLayout!=NULL) {
		goto lblError;
	}

	// Layout Identifier LAYOUT_ID
	GL_Widget_SetId(hLayout, LAYOUT_ID);

	goto lblEnd;

	// Errors
	// Document creation failure
lblError:
	if (hDocument)
	{
		GL_Widget_Destroy(hDocument);
		hDocument=NULL;
	}
	goto lblEnd;
lblEnd:
	return hDocument;
}

void prepareBillPaymentDocument(T_GL_HWIDGET xDocument, T_GL_HWIDGET xLayout, byte xline, char * billNumber, char * billAmount, char * date, char * time){

	T_GL_HWIDGET xPrint;
	char PrintData[100 + 1];

	//Create layout
	xLayout = GL_Layout_Create(xDocument);

	memset(PrintData, 0, sizeof PrintData);

	//Print Transaction
	strcat(PrintData, "BILL PAYMENT\n======================================");

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 0, 20, 0, 20, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_CENTER);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Bill Number: \t");
	strcat(PrintData, billNumber);


	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Bill Amount: \tKsh");
	strcat(PrintData, billAmount);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Date: \t");
	strcat(PrintData, date);

	strcat(PrintData, "Time: \t");
	strcat(PrintData, time);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Served By: \t");
	strcat(PrintData, "Kelvin Kimani");

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 100, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);

}

void prepareSaleDocument(T_GL_HWIDGET xDocument, T_GL_HWIDGET xLayout, byte xline, char * trackMessage , char * saleAmount, char * date, char * time){

	T_GL_HWIDGET xPrint;
	char PrintData[100 + 1];

	//Create layout
	xLayout = GL_Layout_Create(xDocument);

	memset(PrintData, 0, sizeof PrintData);

	//Print Transaction
	strcat(PrintData, "SALE\n======================================");

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 0, 20, 0, 20, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_CENTER);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Card Details:\tTrack Message");
	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);
	strcat(PrintData, trackMessage);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Amount: \tKsh");
	strcat(PrintData, saleAmount);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Date: \t");
	strcat(PrintData, date);

	strcat(PrintData, "Time: \t");
	strcat(PrintData, time);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Served By: \t");
	strcat(PrintData, "Kelvin Kimani");

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 100, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);

}

void prepareSaleWithCashBackDocument(T_GL_HWIDGET xDocument, T_GL_HWIDGET xLayout, byte xline, char * trackMessage, char * saleAmount,  char * cashBackAmount, char * date, char * time){

	T_GL_HWIDGET xPrint;
	char PrintData[100 + 1];
	char totalAmount[20];

	//Adding Sale amount and Cashback amount
	int convertedSaleAmount = atoi(saleAmount);
	int convertedCashBackAmount = atoi(cashBackAmount);

	int total = convertedSaleAmount + convertedCashBackAmount;

	sprintf(totalAmount, "%d", total);

	//Create layout
	xLayout = GL_Layout_Create(xDocument);

	memset(PrintData, 0, sizeof PrintData);

	//Print Transaction
	strcat(PrintData, "SALE WITH CASHBACK\n======================================");

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 0, 20, 0, 20, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_CENTER);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Card Number:");
	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);




	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, trackMessage);
	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Sale Amount: \tKsh");
	strcat(PrintData, saleAmount);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Cashback Amount: \tKsh");
	strcat(PrintData, cashBackAmount);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Total Amount: \tKsh");
	strcat(PrintData, totalAmount);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);



	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Date: \t");
	strcat(PrintData, date);

	strcat(PrintData, "Time: \t");
	strcat(PrintData, time);

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 15, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);


	memset(PrintData, 0, sizeof PrintData);

	strcat(PrintData, "Served By: \t");
	strcat(PrintData, "Kelvin Kimani");

	xPrint = GL_Print_Create    (xLayout);
	GL_Widget_SetText      (xPrint, PrintData);
	GL_Widget_SetItem      (xPrint, 0, xline++);
	GL_Widget_SetFontScale (xPrint, GL_SCALE_LARGE);
	GL_Widget_SetMargins   (xPrint, 60, 15 , 0 , 100, GL_UNIT_PIXEL);
	GL_Widget_SetBackAlign (xPrint, GL_ALIGN_LEFT);
	GL_Widget_SetFontStyle (xPrint, GL_FONT_STYLE_BOLD);

}

int printDocument(T_GL_HWIDGET hDocument)
{
	T_GL_ERROR_CODE eRet;
	int iRet=-1;

	// Printing in progress
	eRet = GL_Document_Print(hDocument, 0);
	if ((eRet == GL_SUCCESS) || !IsPrinter())
		iRet=1;

	return iRet;
}
