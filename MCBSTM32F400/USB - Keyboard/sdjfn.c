#include <stdio.h>
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "cmsis_os.h"
#include "rl_usb.h"
#include <math.h>
#include <stdlib.h> 

#include "stm32f4xx_hal.h"
#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 

extern int pol(void);
int test(void);
extern int stdout_init (void);
extern GLCD_FONT GLCD_Font_16x24;
#ifdef RTE_CMSIS_RTOS_RTX
extern   uint32_t               os_time;
uint32_t HAL_GetTick (void) {
  return os_time;
}
#endif


 static void SystemClock_Config (void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
}


 int main(void){
	 char pol_char;
	 
	usbStatus usb_status;                 // USB status
  int       status;                     // Generic status

  HAL_Init();                           // Initialize the HAL Library
  SystemClock_Config();                 // Configure the System Clock
  SystemCoreClockUpdate();              // Update system clock

  status = stdout_init ();              // Initialize retargeted stdout
  if (status != 0) {
    for (;;) {}                         // Handle stdout init failure
  }

  usb_status = USBH_Initialize (0U);    // Initialize USB Host 0
  if (usb_status != usbOK) {
    for (;;) {}                         // Handle USB Host 0 init failure
  }
  usb_status = USBH_Initialize (1U);    // Initialize USB Host 1
  if (usb_status != usbOK) {
    for (;;) {}                         // Handle USB Host 1 init failure
  }

		stdout_init();
		printf("Welcome to the MATH test\n");
	  printf("\n");
	  printf("\n");
	  printf(" Press 'p' for  practice mode\n\n");
		printf(" Press 't' for  test mode\n");
		printf("\n");
		pol_char=pol();
		if (pol_char=='t')
		{
			test();
		}
		
}
 
int test(void)
{
		char pol_char;
		int j=30;
		int m=72;
	char ans[10];
	int quesn[15][3];
	int score;
	//char queso[4]={"+","-","*","/"};
	for(int i=0;i<5;i++){
		quesn[i][0]=j;
		quesn[i][1]=m;
		quesn[i][2]=quesn[i][1]+quesn[i][0];
		j=j+7;
		m=m-3;
	}
	
	GLCD_ClearScreen();
	printf("\n\n\nWelcome to \nTest Mode\n");
	printf("\nPress c to continue\n");
	
	//pol_char=pol();
	
	if (1)
		{
			for(int n=3;n<8;n++){
				char buff[50];
			GLCD_ClearScreen();
			printf("\n\n\n\n\n\n\n\n\n\n\n");
			sprintf(buff,"%d + %d = ",quesn[n][0],quesn[n][1]);
			printf("%s",buff);
			printf("\n");
			pol_char='f';
				int m=0;
			while(pol_char!=13){
				pol_char=pol();
				ans[m]=pol_char;
				m++;
			}
			int a =m-1;
			int power=a-1;
			int answer=0;
			for(int w=0;w<a;w++){
			answer=answer+((ans[w]-48)*(pow(10,power)));
				power--;
			}
			sprintf(buff,"%d",answer);
			printf("%s",buff);
			/*if(answer==quesn[n][2]){
			printf("\n\n Correct!");
				score=score+5;
			}
			else{
			printf("\n\n Wrong!");
			}*/
			int im=100000000;
			while(im>0){
			im--;
			}
			}
		}
return(0);

}