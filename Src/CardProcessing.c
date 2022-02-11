//Contains card reading and processing
#include "sdk_tplus.h"
#include "GL_GraphicLib.h"
#include "menu.h"

/* Global Variable */

//Tracks
char tcTrk1[128];
char tcTrk2[128];
char tcTrk3[128];

int checkISO(int res){

	char err[20];

	memset(err, 0, sizeof(err));

	if (res == ISO_OK) {

		return TRUE;

	} else {

		switch (res)
		{
			case DEF_SEP: strcpy(err, "Separator fault");  break;
			case DEF_PAR: strcpy(err, "Parity fault");     break;
			case DEF_LRC: strcpy(err, "LRC fault");        break;
			case DEF_LUH: strcpy(err, "Luhn fault");       break;
			case DEF_NUM: strcpy(err, "Numerical fault");  break;
			case NO_DATA: strcpy(err, "No readable data"); break;
			default:      strcpy(err, "Unknown error");    break;
		}

		GL_Dialog_Message(gGoalGraphicLibInstance, "Card Swipe Error", err , GL_ICON_ERROR, GL_BUTTON_VALID_CANCEL, 2*1000);
		return FALSE;
	}

}

void swipeOrCard(char * message){
	GL_Dialog_Message(gGoalGraphicLibInstance, "Card", message , GL_ICON_INFORMATION, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
}

void waitCard(char* cardDetails){

	Telium_File_t * hCam = NULL;
	Telium_File_t * hSwipe2 = NULL;
	Telium_File_t * hSwipe3 = NULL;
	Telium_File_t * hSwipe31 = NULL;
	byte ucLen = 0;
	int res, event, cardPresent, check;
	unsigned char ucStatus;

	char *pcSrc, *pcDst;

	char tcTmp[128];

	//Initialization
	cardPresent = FALSE;

	while(!cardPresent) {

		GL_Dialog_Message(gGoalGraphicLibInstance, NULL, "Please Insert or Swipe Card", GL_ICON_INFORMATION, GL_BUTTON_NONE, 0);


		//Card Swipe

		//Check MagStripe
		if (IsISO1() == 1) {
			//Open MagStripe
			hSwipe31 = Telium_Fopen("SWIPE31", "r*");
		}

		if (IsISO2() == 1) {
			//Open MagStripe
			hSwipe2 = Telium_Fopen("SWIPE2", "r*");
		}

		if (IsISO3() == 1) {
			//Open MagStripe
			hSwipe3 = Telium_Fopen("SWIPE3", "r*");
		}


		//Open CAM
		hCam = Telium_Fopen("CAM0", "rw");


		//Wait for swipe event/card insertion
		event = Telium_Ttestall(CAM0 | SWIPE31 | SWIPE2 | SWIPE3, 0);

		//Retrieve and Analyze ISO1
		if (event & SWIPE31) {

			//Show event
			swipeOrCard("Swipe 31");

			memset(tcTmp, 0, sizeof(tcTmp));
			memset(tcTrk1, 0, sizeof(tcTrk1));

			res = Telium_Is_iso1(hSwipe31, &ucLen, (byte*)tcTmp);
			check = checkISO(res);

			if (check == TRUE) {

				cardPresent = TRUE;

				pcSrc = tcTmp;
				pcDst = tcTrk1;

				while(*pcSrc) {
					if(*pcSrc++ == '%')
						break;
				}

				// Copy all cardDetails between start and end sentinels
				while(*pcSrc) {
					if(*pcSrc == '?')
						break;
					*pcDst++ = *pcSrc++;
				}

				GL_Dialog_Message(gGoalGraphicLibInstance, "Card Details", tcTrk1 , GL_ICON_INFORMATION, GL_BUTTON_NONE, 3*1000);

				strcpy(cardDetails, tcTrk1);

			}
		}

		if (event & SWIPE2) {

			//Show event
			swipeOrCard("Swipe 2");

			memset(tcTmp, 0, sizeof(tcTmp));
			memset(tcTrk2, 0, sizeof(tcTrk2));
			res = Telium_Is_iso2(hSwipe2, &ucLen, (byte*)tcTmp);
			check = checkISO(res);

			if (check == TRUE) {

				cardPresent = TRUE;

				pcSrc = tcTmp;
				pcDst = tcTrk2;

				while(*pcSrc) {                                  // Find start sentinel
					if(*pcSrc++ == 'B')
						break;
				}
				while(*pcSrc) {                                  // Copy all cardDetails between start and end sentinels
					if(*pcSrc == 'F')
						break;
					if(*pcSrc == 'D')

						//Truncate Data to return Card Number only
						//*pcSrc = '=';
						break;

						*pcDst++ = *pcSrc++;
				}

				GL_Dialog_Message(gGoalGraphicLibInstance, "Card Details", tcTrk2 , GL_ICON_INFORMATION, GL_BUTTON_NONE, 3*1000);

				strcpy(cardDetails, tcTrk2);

			}

		}

		if (event & SWIPE3) {


			//Show event
			swipeOrCard("Swipe 3");

			memset(tcTmp, 0, sizeof(tcTmp));
			memset(tcTrk3, 0, sizeof(tcTrk3));

			res = Telium_Is_iso3(hSwipe3, &ucLen, (byte*)tcTmp);
			check = checkISO(res);


			if (check == TRUE) {

				cardPresent = TRUE;

				pcSrc = tcTmp;
				pcDst = tcTrk3;

				while(*pcSrc) {
					if(*pcSrc++ == 'B')
						break;
				}

				while(*pcSrc) {
					if(*pcSrc == 'F')
						break;
					if(*pcSrc == 'D')
						*pcSrc = '=';
						*pcDst++ = *pcSrc++;
				}

				GL_Dialog_Message(gGoalGraphicLibInstance, "Card Details", tcTrk3 , GL_ICON_INFORMATION, GL_BUTTON_NONE, 3*1000);

				strcpy(cardDetails, tcTrk3);

			}
		}

		//Analyze and retrieve CAM
		if (event & CAM0) {

			//Show event
			swipeOrCard("Card Inserted");

			Telium_Status(hCam, &ucStatus);
			if ((ucStatus & CAM_PRESENT) != 0) {
					// A card is present
					cardPresent = TRUE;
					strcpy(cardDetails, "Read card");
			} else {

				cardPresent = FALSE;
			}

		}


		Telium_Fclose(hSwipe31);
		Telium_Fclose(hSwipe2);
		Telium_Fclose(hSwipe3);
		Telium_Fclose(hCam);
	}

}
