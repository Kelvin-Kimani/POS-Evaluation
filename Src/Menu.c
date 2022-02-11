/**
 * Menu.c
 *
 * This file manage the menu.
 *
 */

#include <sdk_tplus.h>

#include "DateScreen.h"
#include "Menu.h"

/*+************* #DEFINES **************************************************+*/

/*+************* CONSTANTS *************************************************+*/

/*+************* STRUCTURES ************************************************+*/


/*+************* VARIABLES *************************************************+*/
static Telium_File_t *sKeyboard = NULL;
static Telium_File_t *sDisplay = NULL;

// Disable header, footer, led in order to Maximize canvas size
int sSavedStatusHeader, sSavedStatusLeds, sSavedStatusFooter;

void menuGoalDisplay(void)
{
  char choice = 0;

  const char *MenuUser[] =
  { "SALE", "BILL PAYMENT", "SALE WITH CASHBACK", "EXIT", 0 };

  initUIContext(_OFF_);

  do
  {
    //Display the menu using GOAL
    choice = GL_Dialog_Menu(gGoalGraphicLibInstance, "SELECT OPTION", MenuUser, choice,
            GL_BUTTON_NONE , GL_KEY_0, GL_TIME_INFINITE);

    switch (choice)
    {
    case 0:
    	processSale();
      break;
    case 1:
    	processBillPayment();
      break;
    case 2:

    	processSaleWithCashback();
    	break;

    default:

    	break;
    }

  } while (choice != 3);

  releaseUIContext();

  return;
}


/**
 * Initialize the display context to maximize the canvas on user area
 */
void initUIContext(int displayHeaderFooterLeds)
{
	if(displayHeaderFooterLeds != _OFF_ && displayHeaderFooterLeds != _ON_) {
		displayHeaderFooterLeds = _OFF_;
	}
	sSavedStatusHeader=DisplayHeader(displayHeaderFooterLeds);
	sSavedStatusLeds=DisplayLeds(displayHeaderFooterLeds);
	sSavedStatusFooter=DisplayFooter(displayHeaderFooterLeds);

    // Open display
    sDisplay = Telium_Fopen("DISPLAY", "w*");
    // Open keyboard
    sKeyboard = Telium_Fopen("KEYBOARD", "r*");

    return;
}

/**
 * Restore display context
 */
void releaseUIContext(void)
{
    // close keyboard
    if (sKeyboard)
    {
        Telium_Fclose(sKeyboard);
        sKeyboard = NULL;
    }
    // close display
    if (sDisplay)
    {
        Telium_Fclose(sDisplay);
        sDisplay = NULL;
    }

	// Restore display context
	DisplayHeader(sSavedStatusHeader);
	DisplayLeds(sSavedStatusLeds);
	DisplayFooter(sSavedStatusFooter);

	return;
}


