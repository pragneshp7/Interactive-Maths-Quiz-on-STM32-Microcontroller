/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Host
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Keyboard.c
 * Purpose: USB Host - HID Keyboard example
 *----------------------------------------------------------------------------*/

#include <stdio.h>



#include "RTE_Components.h"
#include "main.h"
#include "math.h"
#include "cmsis_os2.h"
#include "tm_stm32f4_rng.h"
#include "rl_fs.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "rl_usb.h"        /* RL-USB function prototypes    */     
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif


extern GLCD_FONT GLCD_Font_16x24;
void Timer_Callback (void *arg);
static void cmd_format  (char *par);
static char *get_drive (char *src, char *dst, uint32_t dst_sz);
static void init_filesystem (void);
bool time_up;
osTimerId_t id;

void Timer_Callback (void *arg){
	time_up = true;
}



// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (2048U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

extern int stdout_init (void);
int getKey(void);

/*------------------------------------------------------------------------------
 *        Application
 *----------------------------------------------------------------------------*/

__NO_RETURN void app_main (void *arg) {
	FILE *f;
  usbStatus usb_status;                 // USB status
	int       		status;
	int				q_no = 1;
	char			pol_char = 0;
	int timer = 5000U;
	char name[16];
	int ch;
	int op;
	char	opc;
	int x=0,y=2;
	int temp;
	char mode = 't';
	int pos = 0;
	char ans[10];
	char buff[16];
	int quesn[3];
	int score;

	time_up = false;

  (void)arg;

#ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize(0, 1);
  EventRecorderEnable (EventRecordError, 0xB0U, 0xB5U);  /* USBH Error Events */
  EventRecorderEnable (EventRecordAll  , 0xB0U, 0xB0U);  /* USBH Core Events */
  EventRecorderEnable (EventRecordAll  , 0xB4U, 0xB4U);  /* USBH HID Events */
#endif

	TM_RNG_Init();
	
  status = stdout_init ();              // Initialize retargeted stdout
  if (status != 0) {
    for (;;) {}                         // Handle stdout init failure
  }

  usb_status = USBH_Initialize(0U);     // Initialize USB Host 0
  if (usb_status != usbOK) {
    for (;;) {}                         // Handle USB Host 0 init failure
  }
  usb_status = USBH_Initialize(1U);     // Initialize USB Host 1
  if (usb_status != usbOK) {
    for (;;) {}                         // Handle USB Host 1 init failure
  }
	
	init_filesystem();
	
	id = osTimerNew(Timer_Callback,osTimerOnce,(void *)0,NULL);
	
	osDelay(3000U);
	
	GLCD_ClearScreen();
	
	while(1){
	quesn[0]=TM_RNG_Get() % 100;
	quesn[1]=TM_RNG_Get() % 100;
	op = quesn[0] % 3;
	switch(op){
		case 0:
			quesn[2]=quesn[1]+quesn[0];
			opc = '+';
			break;
		case 1:
			if(quesn[0]<quesn[1]){
				temp = quesn[0];
				quesn[0] = quesn[1];
				quesn[1] = temp;
			}
			quesn[2]=quesn[0]-quesn[1];
			opc = '-';
			break;
		case 2:
			quesn[0] %= 10;
			quesn[1] %= 10;
			quesn[2]=quesn[1]*quesn[0];
			opc = '*';
			break;
	}


	
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 0 * GLCD_Font_16x24.height,"Welcome to the");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 1 * GLCD_Font_16x24.height,"   Math Test  ");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 2 * GLCD_Font_16x24.height,"       by     ");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 3 * GLCD_Font_16x24.height,"  Devam Moosa ");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 4 * GLCD_Font_16x24.height,"Pragnesh Priyal");
	
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"    t = Test   ");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 8 * GLCD_Font_16x24.height,"  p = Practice ");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 10 * GLCD_Font_16x24.height," h =   Score  ");
	GLCD_DrawString(0 * GLCD_Font_16x24.width, 12 * GLCD_Font_16x24.height,"f=Clear Scores");

	
	while(1){
		pol_char=getKey();
		if(pol_char == 't' || pol_char == 'p' || pol_char == 'h' || pol_char == 'f'){
			break;
		}
		osDelay(10U);
	}
	mode = pol_char;

	
	GLCD_ClearScreen();
	switch (mode){
		case 't':
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 0 * GLCD_Font_16x24.height,"   Test Mode");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 2 * GLCD_Font_16x24.height,"Enter Your Name");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 3 * GLCD_Font_16x24.height,"and press Enter");

			timer = 5000U;
			while(pol_char != 13){
				pol_char=getKey();
				if(pol_char > 96 && pol_char < 123)
				{
					GLCD_DrawChar(pos * GLCD_Font_16x24.width, 5 * GLCD_Font_16x24.height,pol_char);
					name[pos] = pol_char;
					pos++;
					if(pos == 16)
						break;
				}
			}
			pos = 0;
			GLCD_ClearScreen();
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 2 * GLCD_Font_16x24.height,"Hello");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 3 * GLCD_Font_16x24.height,name);
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 5 * GLCD_Font_16x24.height,"You have");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"5 seconds for");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 7 * GLCD_Font_16x24.height,"each question");

			osDelay(3000U);
			GLCD_ClearScreen();
			GLCD_DrawString(8 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"3");
			osDelay(500U);
			GLCD_ClearScreen();
			GLCD_DrawString(8 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"2");
			osDelay(500U);
			GLCD_ClearScreen();
			GLCD_DrawString(8 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"1");
			osDelay(500U);
			break;
		case 'p':
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 0 * GLCD_Font_16x24.height," Practice Mode");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 2 * GLCD_Font_16x24.height,"    Choose a");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 3 * GLCD_Font_16x24.height,"   Difficulty");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 5 * GLCD_Font_16x24.height,"  1 = God");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"  2 = Demi-God");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 7 * GLCD_Font_16x24.height,"  3 = Peasant");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 9 * GLCD_Font_16x24.height,"Press r anytime");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 10 * GLCD_Font_16x24.height,"    to exit");
			while(1){
				pol_char=getKey();
				if(pol_char == '1')
				{
					timer = 2000U;
					break;
				}
				else if(pol_char == '2'){
					timer = 4000U;
					break;
				}
				else if(pol_char == '3'){
					timer = 7000U;
					break;
				}
			}
			break;
		case 'h':
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 0 * GLCD_Font_16x24.height,"    Scores   ");
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 10 * GLCD_Font_16x24.height,"Press e to exit");
			f = fopen("score.txt","r");
			if(f != NULL){
				while((ch=fgetc(f))!=EOF){
					if(ch == '\n' || x == 16){
						y++;
						x=0;
					}else{
						GLCD_DrawChar(x * GLCD_Font_16x24.width, y * GLCD_Font_16x24.height,ch);
						x++;
					}
				}
				fclose(f);
			}else{
				GLCD_DrawString(0 * GLCD_Font_16x24.width, 1 * GLCD_Font_16x24.height,"File Open Error");
			}
			while(1){
				pol_char = getKey();
				if(pol_char == 'e'){
					break;
				}
			}
			x=0;y=2;
			break;
		case 'f':
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 10 * GLCD_Font_16x24.height,"Press e to exit");
			f = fopen("score.txt","w");
			if(f != NULL){
				GLCD_DrawString(0 * GLCD_Font_16x24.width, 4 * GLCD_Font_16x24.height,"Clearing Scores");
				fprintf(f,"");
				fclose(f);
			}else{
				GLCD_DrawString(0 * GLCD_Font_16x24.width, 4 * GLCD_Font_16x24.height,"File Open Error");
			}
			while(1){
				pol_char = getKey();
				if(pol_char == 'e'){
					break;
				}
			}
			break;
	}
	GLCD_ClearScreen();
	
	if(mode == 't')
	{
		sprintf(buff,"Score = %d",score);
		GLCD_DrawString(0 * GLCD_Font_16x24.width, 8 * GLCD_Font_16x24.height,buff);		
	}
	
	if(mode == 't'|| mode == 'p'){
	for(int n=0;n<8;n++){
		quesn[0]=TM_RNG_Get() % 100;
		quesn[1]=TM_RNG_Get() % 100;
		op = quesn[0] % 3;
		switch(op){
			case 0:
				quesn[2]=quesn[1]+quesn[0];
				opc = '+';
				break;
		case 1:
				if(quesn[0]<quesn[1]){
					temp = quesn[0];
					quesn[0] = quesn[1];
					quesn[1] = temp;
				}
				quesn[2]=quesn[0]-quesn[1];
				opc = '-';
				break;
		case 2:
				quesn[0] %= 10;
				quesn[1] %= 10;
				quesn[2]=quesn[1]*quesn[0];
				opc = '*';
				break;
	}
		
		if(mode == 't'){
			sprintf(buff,"Question No. %d",q_no);
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 0 * GLCD_Font_16x24.height,buff);		
		}
		sprintf(buff,"%d %c %d = ",quesn[0],opc,quesn[1]);
		GLCD_DrawString(0 * GLCD_Font_16x24.width, 2 * GLCD_Font_16x24.height,buff);
		pol_char=0;
		q_no++;
		osTimerStart(id,timer);
		while(pol_char != 13 && time_up == 0){
			pol_char = getKey();
			if(pol_char > 47 && pol_char < 58){
				GLCD_DrawChar(pos * GLCD_Font_16x24.width, 3 * GLCD_Font_16x24.height,pol_char);
				ans[pos]=pol_char;
				pos++;
			}
			if(mode == 'p' && pol_char =='r')
				break;
			osDelay(10U);
		}
		if(mode == 'p' && pol_char =='r'){
			osTimerStop(id);
			time_up = false;
			score = 0;
			q_no = 1;
			break;
		}
		if(time_up == 0){
		osTimerStop(id);
		int a = pos;
		int power= a - 1;
		int answer=0;
		for(int w=0;w<a;w++){
		answer=answer+((ans[w]-48)*(pow(10,power)));
			power--;
		}

		if(answer==quesn[2]){
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"     Correct      ");
			score=score+5;
		}else{
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"      Wrong       ");
		}
		}else{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET);
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"     Time Up      ");
		}
		
		if(mode == 'p'){
			n=0;
		}
		
		pos = 0;
		
		if(mode == 't')
		{
			sprintf(buff,"Score = %d",score);
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 8 * GLCD_Font_16x24.height,buff);	
		}
		osDelay(1000U);
		if (time_up == true)
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);
		GLCD_ClearScreen();
		time_up = false;
	}
}
	if(mode == 't')
	{
		f = fopen("score.txt","a");
		if(f != NULL){
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 4 * GLCD_Font_16x24.height,"Writing Scores");
			fprintf(f,"%s %d\n",name,score);
			fclose(f);
		}else{
			GLCD_DrawString(0 * GLCD_Font_16x24.width, 4 * GLCD_Font_16x24.height,"File Open Error");
		}
		GLCD_DrawString(0 * GLCD_Font_16x24.width, 6 * GLCD_Font_16x24.height,"Final Score = ");
		sprintf(buff,"%d",score);
		GLCD_DrawString(0 * GLCD_Font_16x24.width, 7 * GLCD_Font_16x24.height,buff);
		osDelay(5000U);
		GLCD_DrawString(0 * GLCD_Font_16x24.width, 10 * GLCD_Font_16x24.height,"  Press r to ");
		GLCD_DrawString(0 * GLCD_Font_16x24.width, 11 * GLCD_Font_16x24.height,"    restart");
	}
	score = 0;
	pol_char = 0;
	if(mode == 't'){
	while(pol_char != 'r')
		{
			pol_char = getKey();
		}
		q_no = 1;	
	}
	GLCD_ClearScreen();
}
}

/*-----------------------------------------------------------------------------
 *        Initialize a Flash Memory Card
 *----------------------------------------------------------------------------*/
static void init_filesystem (void) {
  fsStatus stat;

  printf ("Initializing and mounting enabled drives...\n\n");

  /* Initialize and mount drive "N0" */
  stat = finit ("N0:");
  if (stat == fsOK) {
    stat = fmount ("N0:");
    if (stat == fsOK) {
      printf ("Drive N0 ready!\n");
    }
    else if (stat == fsNoFileSystem) {
      /* Format the drive */
      printf ("Drive N0 not formatted!\n");
      cmd_format ("N0:");
    }
    else {
      printf ("Drive N0 mount failed with error code %d\n", stat);
    }
  }
  else {
    printf ("Drive N0 initialization failed!\n");
  }

  printf ("\nDone!\n");
}

/*-----------------------------------------------------------------------------
 *        Format Device
 *----------------------------------------------------------------------------*/
static void cmd_format (char *par) {
  char  label[12];
  char  drive[4];
  int   retv;

  par = get_drive (par, drive, 4);

  printf ("\nProceed with Format [Y/N]\n");
  retv = getchar();
  if (retv == 'y' || retv == 'Y') {
    /* Format the drive */
    if (fformat (drive, par) == fsOK) {
      printf ("Format completed.\n");
      if (fvol (drive, label, NULL) == 0) {
        if (label[0] != '\0') {
          printf ("Volume label is \"%s\"\n", label);
        }
      }
    }
    else {
      printf ("Formatting failed.\n");
    }
  }
  else {
    printf ("Formatting canceled.\n");
  }
}

/*-----------------------------------------------------------------------------
 *        Extract drive specification from the input string
 *----------------------------------------------------------------------------*/
static char *get_drive (char *src, char *dst, uint32_t dst_sz) {
  uint32_t i, n;

  i = 0;
  n = 0;
  while (!n && src && src[i] && (i < dst_sz)) {
    dst[i] = src[i];

    if (dst[i] == ':') {
      n = i + 1;
    }
    i++;
  }
  if (n == dst_sz) {
    n = 0;
  }
  dst[n] = '\0';

  return (src + n);
}



/*------------------------------------------------------------------------------
 *        Get Key Function
 *----------------------------------------------------------------------------*/
int getKey(void){
  usbStatus hid_status;                 // HID status
  int       ch;                         // Character
  uint8_t   con = 0U;                   // Connection status of keyboard

	hid_status = USBH_HID_GetDeviceStatus(0U);  // Get HID device status
	if (hid_status == usbOK) {
		if (con == 0U) {                  // If keyboard was not connected previously
			con = 1U;                       // Keyboard got connected
			//printf("Keyboard connected!\n");
		}
	} else {
		if (con == 1U) {                  // If keyboard was connected previously
			con = 0U;                       // Keyboard got disconnected
			//printf("\nKeyboard disconnected!\n");
		}
	}
	if (con != 0U) {                    // If keyboard is active
		ch = USBH_HID_GetKeyboardKey(0U); // Get pressed key
		if (ch != -1) {                   // If valid key value
			if ((ch & 0x10000) != 0) {      // Handle non-ASCII translated keys (Keypad 0 .. 9)
																			// Bit  16:    non-ASCII bit (0 = ASCII, 1 = not ASCII)
																			// Bits 15..8: modifiers (SHIFT, ALT, CTRL, GUI)
																			// Bits  7..0: ASCII or HID key Usage ID if not ASCII
																			// HID Usage ID values can be found in following link:
																			// http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
				ch &= 0xFF;                   // Remove non-ASCII bit and modifiers
				if ((ch>=0x59)&&(ch<=0x61)) { // Keypad 1 .. 9 key convert to
					ch = (ch - 0x59) + '1';     // ASCII  1 .. 9
				} else if (ch == 0x62) {      // Keypad 0 key convert to
					ch = '0';                   // ASCII  0
				} else {                      // If not Keypad 0 .. 9
					ch = -1;                    // invalidate the key
				}
			}
		}
	}
	return(ch);
}
