/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: exitPanel */
#define  PANEL_CANVAS                     2       /* control type: canvas, callback function: (none) */
#define  PANEL_SCORE                      3       /* control type: numeric, callback function: (none) */
#define  PANEL_INVISIBLE                  4       /* control type: numeric, callback function: (none) */
#define  PANEL_STRING                     5       /* control type: string, callback function: (none) */
#define  PANEL_NUMERICSLIDE_2             6       /* control type: scale, callback function: (none) */
#define  PANEL_NUMERICSLIDE               7       /* control type: scale, callback function: (none) */
#define  PANEL_STRING_3                   8       /* control type: string, callback function: (none) */
#define  PANEL_BONUS_TIMER                9       /* control type: timer, callback function: BonusTimerFunc */
#define  PANEL_STRING_2                   10      /* control type: string, callback function: (none) */
#define  PANEL_TIMER                      11      /* control type: timer, callback function: timerFunc */

#define  PANEL_2                          2       /* callback function: exitPanel2 */
#define  PANEL_2_START_GAME               2       /* control type: pictButton, callback function: StartgameFunc */
#define  PANEL_2_PLAYER_NAME              3       /* control type: string, callback function: (none) */
#define  PANEL_2_SCORE_LIST               4       /* control type: pictButton, callback function: ScoreListFunc */
#define  PANEL_2_PICTURE                  5       /* control type: picture, callback function: (none) */
#define  PANEL_2_TEXTMSG_2                6       /* control type: textMsg, callback function: (none) */
#define  PANEL_2_TEXTMSG                  7       /* control type: textMsg, callback function: (none) */

#define  PANEL_3                          3       /* callback function: exitPanel3 */
#define  PANEL_3_SCORE_LIST               2       /* control type: table, callback function: (none) */

#define  PANEL_4                          4       /* callback function: exitPanel4 */
#define  PANEL_4_PICTURE                  2       /* control type: picture, callback function: (none) */
#define  PANEL_4_TEXTMSG                  3       /* control type: textMsg, callback function: (none) */
#define  PANEL_4_TEXTMSG_2                4       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_ABOUT                    2       /* callback function: aboutFunc */
#define  MENUBAR_HELP                     3       /* callback function: helpFunc */
#define  MENUBAR_DEMO                     4       /* callback function: demoFunc */


     /* Callback Prototypes: */

void CVICALLBACK aboutFunc(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK BonusTimerFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK demoFunc(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK exitPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitPanel2(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitPanel3(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitPanel4(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK helpFunc(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ScoreListFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartgameFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timerFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
