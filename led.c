#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#define uchar unsigned char;
#define uint unsigned int;
uchar code table[]=
{0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
uchar code led[]={110,101,11,100,10,1,0};
sbit ledRed=P0^0;
sbit ledGreen=P0^1;
sbit ledBlue=P0^2;
sbit statusKey=P3^0;
sbit funKey=P3^1;
sbit plusKey=P3^2;
sbit minusKey=P3^3;
sbit lcdLow=P3^7;
sbit lcdHigh=P3^6;
void init();
void statusInit();
int keyscan();
void status0();
void status1();
void status2();
void status3();
void status4();
void delay(unsigned int time);
void delayLed(unsigned int t);
int statusNum;
int funNum;
int breakflag;
int ttime;
int time;
int count;
void main(){
	P0=0x00;delay(1000);
	init();
	while(1){
		switch(statusNum){
			case 0:statusInit();status0();break;
			case 1:statusInit();status1();break;
			case 2:statusInit();status2();break;
		//	case 3:statusInit();status3();break;
		//	case 4:statusInit();status4();break;
			default:statusInit();statusNum=1;break;
		}
	}
}
void init(){
 	count=0;
	statusNum=0;
	funNum=0;
	ttime=0;
	time=0;
	TMOD=0x11;
	TH0=(65536-50000)/256;	
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	breakflag=0;
	P0=0xff;
	lcdLow=0;
	lcdHigh=0;
}
void statusInit(){
	count=0;
	P0=0xff;
	lcdLow=0;
	lcdHigh=0;
	funNum=0;
}
void status0(){
	while(1){
		P0=0x00;
		lcdLow=1;
		lcdHigh=1;
		P1=table[8];
		breakflag=keyscan();
		if(breakflag==1){
		breakflag=0;
		break;
		}	
	}
}
void status1(){
	while(1){
		ledRed=led[funNum]/100;
		ledGreen=(led[funNum]/100)%10;
		ledBlue=led[funNum]%10;
		delay(500);
		breakflag=keyscan();
		if(breakflag==1){
			breakflag=0;
			break;
		}
	}
}
void status2(){
	int record;
	int ledNumTem=0;
	while(1){	
		breakflag=keyscan();
		if(breakflag==2){
			breakflag=0;
			break;
		}
	}
	record=count;
	TR0=1;
	while(1){
		count=record;
		ledRed=led[ledNumTem]/100;
		ledGreen=(led[ledNumTem]/100)%10;
		ledBlue=led[ledNumTem]%10;	
		if(ttime==20)
			{ 
			    ttime=0;
				count--;
				if(count==-1)
					{
						ledNumTem++;
						ledRed=led[ledNumTem]/100;
						ledGreen=(led[ledNumTem]/100)%10;
						ledBlue=led[ledNumTem]%10;	
					}
			}
		breakflag=keyscan();
		if(breakflag==1){
			breakflag=0;
			break;
		}
	}	
}
int keyscan(){
	if(statusKey==0)			 //调节状态
	{
		delay(10);
		if(statusKey==0)
		{
			statusNum++;	
		while(!statusKey);
		return 1;
		}
	}
  	if(funKey==0)			 //调节功能
	{
		delay(10);
		if(funKey==0)
		{
			funNum++;	
		while(!funKey);
		return 2;
		}
	}
	if(plusKey==0)			 //加
	{
		delay(10);
		if(plusKey==0)
		{
			count++;
			if(count==10)count=0;	
		while(!plusKey);
		return 0;
		}
	}
	if(minusKey==0)			 //减
	{
		delay(10);
		if(minusKey==0)
		{
			count--;
			if(count==-1)count=9;	
		while(!minusKey);
		return 0;
		}
	}
	return 0;	
}
void delay(unsigned int time)
{		
	unsigned int x,y;
	for(x=time;x>0;x--)
		for(y=110;y>0;y--);
}
void delayLed(unsigned int t)
{
 	while(--t);
}
void t0() interrupt 1
{
	TH0=(65536-50000)/256;	
	TL0=(65536-50000)%256;
	ttime++;					
}
