#include <stdio.h> 
#include <math.h>
#include <stdlib.h> 
#include "Board_GLCD.h"
#include "GLCD_Config.h"   
extern GLCD_FONT GLCD_Font_16x24;
extern int stdout_init (void);
extern int pol(void);
int test(void)
{
		char pol_char;
		int j=30;
		int m=72;
	char ans[10];
	int quesn[15][3];
	int score;
	//char queso[4]={"+","-","*","/"};
	for(int i=0;i<15;i++){
		quesn[i][0]=j;
		quesn[i][1]=m;
		quesn[i][2]=quesn[i][1]+quesn[i][0];
		j=j+7;
		m=m-3;
	}
	
	stdout_init();
	printf("\n\n\nWelcome to \nTest Mode\n");
	printf("\nPress c to continue\n");
	
	pol_char=pol();
	
	if (pol_char=='c')
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