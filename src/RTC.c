//====================================================================
// File Name : RTC.c
// Function  : TQ2440 RTC Test 
// Version   : 1.0
//====================================================================

#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 
extern void Glib_Line(int x1,int y1,int x2,int y2,int color);
extern void Lcd_ClearScr( U32 c);
extern void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...);
//==================================================================================		
void RTC_Time_Set( void )
{
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 0x23 ;		//��
	rBCDMON  = 0x11 ;		//��
	rBCDDATE = 0x24 ;		//��	
	rBCDDAY  = 0x00 ;		//����
	rBCDHOUR = 0x00 ;		//Сʱ
	rBCDMIN  = 0x00 ;			//��
	rBCDSEC  = 0x00 ;		//��
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}

//==================================================================================
void RTC_Display(void) 
{
	int i;
	U16 year ;
	U8 month, day ;		// week
	U8 hour, minute, second ;

	RTC_Time_Set() ;
	   
   	Uart_Printf( "\nRTC TIME Display, press ESC key to exit !\n" ) ;

	while( Uart_GetKey() != ESC_KEY )
	{
		rRTCCON = 1 ;		//RTC read and write enable

		year = 0x2000+rBCDYEAR  ;		//��
		month = rBCDMON  ;		//��
		day = rBCDDATE  ;		//��	
//		week = rBCDDAY  ;		//����
		hour = rBCDHOUR  ;		//Сʱ
		minute = rBCDMIN  ;		//��
		second = rBCDSEC  ;		//��
		
		rRTCCON &= ~1 ;		//RTC read and write disable

		Uart_Printf( "RTC time : %04x-%02x-%02x %02x:%02x:%02x\n", year, month, day, hour, minute, second );
				Lcd_ClearScr((0x1f<<11)|(0x3f<<5) | (0x1f)	);//ˢ��ˢΪ��ɫ
//--------add bgn-------�����ߵ�ֱ�ӷŵ�while�з������Ĳ���---------
		
	
	for(i=1;(i*50)<480||(i*50)<272;i++)
	{//������Ϊ50Ϊһ��ĸ��ӣ�����鿴�ֵ�λ��
		Glib_Line(0,50*i,480,50*i,0x0000);
		Glib_Line(50*i,0,50*i,272,0x0000);    
	}
		Lcd_printf(100,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"RTC time : %04x-%02x-%02x %02x:%02x:%02x\n", year, month, day, hour, minute, second );
		Delay( 900 ) ;
	}
}
