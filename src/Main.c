/****************************************************************
 NAME: u2440mon.c
 DESC: u2440mon entry point,menu,download
 ****************************************************************/
#define	GLOBAL_CLK		1

#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"  
#include "profile.h"
#include "memtest.h"
#include  "stdarg.h"
#include "config.h" 


extern unsigned char __VGA[];//为字库添加的声明，英文ASCII编码
extern unsigned char __CHS[];//为字库添加的声明，汉子字库数组

void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);//为字库添加的声明
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st);//为字库添加的声明
extern void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...);//为字库添加的声明
extern void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
extern U8 Key_Scan( void );
//-----add bgn------------
#include "lcd_tft.h"
#ifdef CSTM_LCD
#include "cstm_lcd.h"

U8 g_type;
#endif
//-----add end------------
/*
#ifdef CSTM_LCD
volatile unsigned short *LCD_BUFFER;
#else
extern volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT];
#endif
*/
/*
#ifdef CSTM_LCD
extern volatile unsigned short *LCD_BUFFER;
#else
 volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT];
#endif
*/
extern char Image$$RO$$Limit[];
extern char Image$$RO$$Base[];
extern char Image$$RW$$Limit[];
extern char Image$$RW$$Base[];
extern char Image$$ZI$$Limit[];
extern char Image$$ZI$$Base[];

	extern unsigned char flat[];
	extern unsigned char dao[];
	
	extern unsigned char brave[];
	extern unsigned char flat1[];
	extern unsigned char flat2[];
	extern unsigned char flat3[];
	extern unsigned char flat4[];
	extern unsigned char dao1[];
	extern unsigned char dao2[];
	extern unsigned char dao3[];
	extern unsigned char brave1[];
	extern unsigned char brave2[];
	extern unsigned char brave3[];
	extern unsigned char jenny[];
	
	extern unsigned char birth[];
	extern unsigned char stars[];
	extern unsigned char shelf[];
	extern unsigned char huayao[];
	
	extern unsigned char wall[];
	extern unsigned char musicwall[];
	extern unsigned char pic1[];
	extern unsigned char pic2[];
	extern unsigned char pic3[];
	extern unsigned char pic4[];
	extern unsigned char pic5[];
	extern unsigned char jennys[];
	extern unsigned char pic1s[];
	extern unsigned char pic2s[];
	extern unsigned char pic3s[];
	extern unsigned char pic4s[];
	extern unsigned char pic5s[];
	extern unsigned char Doraemon[];

extern void  Buzzer_Stop(void);
extern void RecordTest(void);
extern void PlayMusicTest(void);
void Isr_Init(void);
void HaltUndef(void);
void HaltSwi(void);
void HaltPabort(void);
void HaltDabort(void);
void ClearMemory(void);
void photo( void );
void read( void );
void music( void );
void record( void );
void snake( void );
void uimenu(void);
void displaytime(void);
void Clk0_Enable(int clock_sel);	
void Clk1_Enable(int clock_sel);
void Clk0_Disable(void);
void Clk1_Disable(void);
void playsnake(void);
void show(int book);
extern void Lcd_TFT_Init(void);
extern void Lcd_TFT_Test( void ) ;
extern void Buzzer_Freq_Set( U32 freq );
#define ADCPRS 9

void action(int);

int direction=4,flag_Out=1,x11,x22,y11,y22,count=0,Pause,Gover=1;


/*
#ifdef CSTM_LCD
extern volatile unsigned short *LCD_BUFFER;
#else
volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT];
#endif
*/
/*extern void KeyScan_Test(void) ;
extern void Test_Touchpanel(void) ;
extern void Test_Adc(void) ;
extern void RTC_Display(void) ;
extern void Test_IrDA_Tx(void) ;
extern void PlayMusicTest(void) ;
extern void RecordTest( void ) ;
extern void Test_Iic(void) ;
extern void Test_SDI(void) ;
extern void Camera_Test( void ) ;*/
#define REQCNT 30
#define ADCPRS 9
#define LOOP 1
void __irq AdcTsAuto(void);
//int count=0;
volatile int xdata, ydata;
extern void Lcd_ClearScr1(int x0,int y0,int h,int l,U32 c);
extern void Lcd_ClearScr( U32 c);
extern void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[]);
extern  RTC_Display(void);
volatile U32 downloadAddress;

extern void Glib_Line(int x1,int y1,int x2,int y2,int color);


void (*restart)(void)=(void (*)(void))0x0;

volatile unsigned char *downPt;
volatile U32 downloadFileSize;
volatile U16 checkSum;
volatile unsigned int err=0;
volatile U32 totalDmaCount;

volatile int isUsbdSetConfiguration;

int download_run=0;
U32 tempDownloadAddress;
int menuUsed=0;

extern char Image$$RW$$Limit[];
U32 *pMagicNum=(U32 *)Image$$RW$$Limit;
int consoleNum;


unsigned char SONGhappy[] ={      //?  ?  
0x40,0x20,0x40,0x20,0x39,0x40,0x40,0x40,0x30,0x40,0x33,0x40,0x33,0x40,
0x40,0x20,0x40,0x20,0x39,0x40,0x40,0x40,0x2b,0x40,0x30,0x40,0x30,0x40,
0x40,0x20,0x40,0x20,0x20,0x40,0x26,0x40,0x30,0x40,0x33,0x40,0x39,0x40,
0x48,0x20,0x48,0x20,0x26,0x40,0x30,0x40,0x2b,0x40,0x30,0x40,0x30,0x40,0x30,0x40
};
unsigned char SONGstar[] ={      //?  ?  
0x30,0x40,0x30,0x40,0x20,0x40,0x20,0x40,0x1c,0x40,0x1c,0x40,0x20,0x40,0x20,0x40,
0x24,0x40,0x24,0x40,0x26,0x40,0x26,0x40,0x2b,0x40,0x2b,0x40,0x30,0x40,0x30,0x40,
0x20,0x40,0x20,0x40,0x24,0x40,0x24,0x40,0x26,0x40,0x26,0x40,0x2b,0x40,0x2b,0x40,
0x20,0x40,0x20,0x40,0x24,0x40,0x24,0x40,0x26,0x40,0x26,0x40,0x2b,0x40,0x2b,0x40,
0x30,0x40,0x30,0x40,0x20,0x40,0x20,0x40,0x1c,0x40,0x1c,0x40,0x20,0x40,0x20,0x40,
0x24,0x40,0x24,0x40,0x26,0x40,0x26,0x40,0x2b,0x40,0x2b,0x40,0x30,0x40,0x30,0x40
};
unsigned char SONGun[] ={      //?  ?  
0x19,0x40,0x1c,0x40,0x2b,0x40,0x2b,0x40, 0x26,0x20,0x1c,0x20,0x20,0x10,0x1c,0x10,0x2b,0x20,
0x30,0x20,0x39,0x20,0x30,0x40,0x26,0x20,0x26,0x10,0x2b,0x10,0x30,0x20,0x39,0x20,0x30,0x40,
0x40,0x20,0x39,0x10,0x30,0x10,0x30,0x10,0x39,0x10,0x40,0x10,0x4c,0x10,0x40,0x10,0x39,0x10,0x30,0x40,
0x2b,0x20,0x2b,0x10,0x26,0x10,0x30,0x20,0x39,0x10,0x40,0x10,0x40,0x20,0x40,0x40,0x40,0x40
};


static U32 cpu_freq;
static U32 UPLL;
static void cal_cpu_bus_clk(void)
{
	U32 val;
	U8 m, p, s;
	
	val = rMPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;

	//(m+8)*FIN*2 不要超出32位数!
	FCLK = ((m+8)*(FIN/100)*2)/((p+2)*(1<<s))*100;
	
	val = rCLKDIVN;
	m = (val>>1)&3;
	p = val&1;	
	val = rCAMDIVN;
	s = val>>8;
	
	switch (m) {
	case 0:
		HCLK = FCLK;
		break;
	case 1:
		HCLK = FCLK>>1;
		break;
	case 2:
		if(s&2)
			HCLK = FCLK>>3;
		else
			HCLK = FCLK>>2;
		break;
	case 3:
		if(s&1)
			HCLK = FCLK/6;
		else
			HCLK = FCLK/3;
		break;
	}
	
	if(p)
		PCLK = HCLK>>1;
	else
		PCLK = HCLK;
	
	if(s&0x10)
		cpu_freq = HCLK;
	else
		cpu_freq = FCLK;
		
	val = rUPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;
	UPLL = ((m+8)*FIN)/((p+2)*(1<<s));
	UCLK = (rCLKDIVN&8)?(UPLL>>1):UPLL;
}

typedef struct 
	{
		int Current_x;
		int Current_y;
		int Direction;
		int Eaten;
	}Point;
Point Snake_Point[10],point[5];

void Temp_function() { Uart_Printf("\nPlease input 1-11 to select test!!!\n"); }

/*struct {
	void (*fun)(void);
	char *tip;
}CmdTip[] = {
				{ Temp_function, "Please input 1-11 to select test" } ,
				{ BUZZER_PWM_Test, "Test PWM" } ,
				{ RTC_Display, "RTC time display" } ,
				{ Test_Adc, "Test ADC" } ,
				{ KeyScan_Test, "Test interrupt and key scan" } ,
				{ Test_Touchpanel, "Test Touchpanel" } ,
				{ Lcd_TFT_Test, "Test TFT LCD" } ,
				{ Test_Iic, "Test IIC EEPROM" } ,
				{ PlayMusicTest, "UDA1341 play music" } ,
				{ RecordTest, "UDA1341 record voice" } ,
				{ Test_SDI, "Test SD Card" } ,
				{ Camera_Test, "Test CMOS Camera"},
				{ 0, 0}						
			};*/

//----add bgn------------
//----add end------------	


void Main(void)
{
	char *mode;
	int i;
	//int gPictureNum;
	U8 key;
	U32 mpll_val = 0 ;
	//U32 divn_upll = 0 ;
//----add bgn------------
	U32 pix_x,pix_y;
//----add end------------	
	    
	#if ADS10   
//	__rt_lib_init(); //for ADS 1.0
	#endif

	Port_Init();
	
	Isr_Init();
	
	i = 2 ;	//don't use 100M!
	switch ( i ) {
	/*case 0:	//200  
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:	//300
		key = 13;
		mpll_val = (67<<12)|(1<<4)|(1);
		break;*/
	case 2:	//400
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	/*case 3:	//440!!!
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;*/
	}
	
	//init FCLK=400M, so change MPLL first
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(key, 12);
	cal_cpu_bus_clk();
	
	consoleNum = 0;	// Uart 1 select for debug.
	Uart_Init( 0,115200 );
	Uart_Select( consoleNum );
	
	Beep(2000, 100);
	
	Uart_SendByte('\n');
	Uart_Printf("<***************************************>\n");
	Uart_Printf("               TQ2440 Test Program\n");
	Uart_Printf("                www.embedsky.net\n");
//	Uart_Printf("      Build time is: %s  %s\n", __DATE__ , __TIME__  );
	Uart_Printf("<***************************************>\n");

	rMISCCR=rMISCCR&~(1<<3); // USBD is selected instead of USBH1 
	rMISCCR=rMISCCR&~(1<<13); // USB port 1 is enabled.


	rDSC0 = 0x2aa;
	rDSC1 = 0x2aaaaaaa;
	//Enable NAND, USBD, PWM TImer, UART0,1 and GPIO clock,
	//the others must be enabled in OS!!!
	rCLKCON = 0xfffff0;

	MMU_Init();	//

	pISR_SWI=(_ISR_STARTADDRESS+0xf0);	//for pSOS

	Led_Display(0x66);

	mode="DMA";

	Clk0_Disable();
	Clk1_Disable();
	
	mpll_val = rMPLLCON;

#ifdef CSTM_LCD
	cstmLcd_init();
#else
	Lcd_TFT_Init() ;		// LCD initial
#endif
		
	download_run=1; //The default menu is the Download & Run mode.

//************************************************************************	
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

#ifdef CSTM_LCD

	g_type=get_pix(&pix_x,&pix_y);
	
	Lcd_ClearScr(0);
	
	Paint_Bmp((pix_x-480)/2, (pix_y-272)/2,480, 272, wall);	
   
while(1)
	{	
		uimenu();
     }


#endif
}

  

void photo( void ) 
{
   	Uart_Printf("\nTest TFT LCD!\n");
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

#ifdef CSTM_LCD
  
	//g_type=get_pix(&pix_x,&pix_y);
	
	Lcd_ClearScr(0);
	
	Paint_Bmp(0,0,160,136, jennys);	
    Paint_Bmp(0, 136, 160,136, pic1s);	
    Paint_Bmp(160, 0, 160,136,pic2s);	
    Paint_Bmp(160, 136,160,136, pic3s);	
    Paint_Bmp(320,0, 160,136, pic4s);	
    Paint_Bmp(320, 136, 160,136, pic5s);
    //Paint_Bmp(0,0,50,272, rim);	
while(1)
	{	
		
		if(xdata<480*1/6)
		{
			if(ydata<272/2)
			{
				Paint_Bmp(0,0,160,136, jennys);	
   				Paint_Bmp(0, 136, 160,136, pic1s);	
    			Paint_Bmp(160, 0, 160,136,pic2s);	
    			Paint_Bmp(160, 136,160,136, pic3s);	
    			Paint_Bmp(320,0, 160,136, pic4s);	
    			Paint_Bmp(320, 136, 160,136, pic5s);
    			//Paint_Bmp(0,0,50,272, rim);
    		}
    		else
    			{ 
    				uimenu();
    			}
		}
	
		
		else if((xdata>480/6)&&(xdata<480/3))
		{
			if(ydata<272/2)
				
				Paint_Bmp(0,0, 480, 272, jenny);	
				
			else
				
				Paint_Bmp(0,0, 480, 272, pic1);	
		}
		else if(xdata>480*2/3)
			{
				if(ydata<272/2)
					
					Paint_Bmp(0,0, 480, 272, pic2);	
				else
					
					Paint_Bmp(0,0, 480, 272, pic3);	
			}
		else if((xdata>480/3)&&(xdata<480*2/3))
			{
				if(ydata<272/2)
					
					Paint_Bmp(0,0, 480, 272, pic4);	
				else
				
					Paint_Bmp(0,0, 480, 272, pic5);	
			}

        }
			

#endif
        
}



void uimenu( void ) 
{
   	Uart_Printf("\nTest TFT LCD!\n");
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

//#ifdef CSTM_LCD
  
	//g_type=get_pix(&pix_x,&pix_y);
	
	Lcd_ClearScr(0);
	 
	Paint_Bmp(0,0, 480, 272, wall);
while(1)
	{	
		
		if(xdata<480*1/6)
		{
			Paint_Bmp(0,0, 480, 272, wall);
		}
	
		
		else if((xdata>480/6)&&(xdata<480/3))
		{
			if(ydata<272/2)
				
				record();	
				
			else
				
				snake();	
		}
		else if(xdata>480*2/3)
			{
				if(ydata<272/2)
					
					photo();
				else
					{
						displaytime();	  
					}
			}
		else if((xdata>480/3)&&(xdata<480*2/3))
			{
				if(ydata<272/2)
					
					read();
				else
				
					music();	
			}
		
		
        }


//#endif
        
}


void displaytime(void)
{
	/*
	Uart_Printf("\nTest TFT LCD!\n");
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

  */
	//g_type=get_pix(&pix_x,&pix_y);
	
	Lcd_ClearScr(0);
	
    //Paint_Bmp(0,0,50,272, rim);	
while(1)
{	
		
		if(xdata<480*1/6)
		{
			
    		uimenu();
    			
		}
		else 
			RTC_Display();
			
}
}


void read( void )
{
	
	int book;
	//int flag=0;
	
	
	Uart_Printf("\nTest TFT LCD!\n");
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);
/*
#ifdef CSTM_LCD
	cstmLcd_init();
#else
	Lcd_TFT_Init() ;		// LCD initial
#endif
	download_run=1; //The default menu is the Download & Run mode.
	//Lcd_ClearScr((0x1f<<11)|(0x3f<<5) | (0x1f)	);//刷屏刷为青色
//--------add bgn-------将画线的直接放到while中方便屏的测试---------

*/
//Lcd_printf(100,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"GOD BLESS US");
while(1)
{


if(xdata<480*1/6)
	{
		if(ydata<272/2)
			{
				Paint_Bmp(0,0,480,272, shelf);	
				//Lcd_printf(50,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"但行好事，莫问前程！ to vec");
    		}
    	else
    		{ 
    			uimenu();
    		}
		
	}

else if((xdata>480/6)&&(xdata<480/3))
		{	
			book = 1;
			show (book);
			//brave
		}
else if(xdata>480*2/3)
		{
			book = 2;//dao
			show (book);
			
		}
else if((xdata>480/3)&&(xdata<480*2/3))
		{
			book=3;//flat
			show (book);
			
		}

	
}
    
}   


void show(int book)
{

 int flag=-1;
	
 while(1)
 {
 // key=Key_Scan();
  Uart_Printf("flag=%03d",flag);

    rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

if((xdata>0)&&(xdata<480*1/6))
	{
		if(ydata<272/2)
			{
				read();				//Paint_Bmp(0,0,480,272, shelf);	
				//Lcd_printf(50,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"但行好事，莫问前程！ to vec");
    		}
    	else
    		{ 
    			uimenu();
    		}
		
	}

else if((xdata>480/6)&&(ydata<272/2))
		{
			xdata=0;
			ydata=0;
			flag++;
			if(flag>4)
			{
				flag=0;
			}
			
		}
		
else if((xdata>480/6)&&(ydata>272/2))
		{
			xdata=0;
			ydata=0;
			if (flag>0)
			{
				flag--;	
			}
		}
	

//Lcd_ClearScr(0);
switch ( flag ) 
	{
	case 0:
		
		if (book==1)
		{
		Paint_Bmp(0,0,480,272, brave);	
		}
		else if (book==2)
		{
			Paint_Bmp(0,0,480,272, dao);	
		}
		else if(book ==3)
		{
			Paint_Bmp(0,0,480,272, flat);	
		}
		break;
	
	
	case 1:	
	
		if (book==1)
		{
		Paint_Bmp(0,0,480,272, brave1);	
		}
		else if (book==2)
		{
			Paint_Bmp(0,0,480,272, dao1);	
		}
		else if(book ==3)
		{
			Paint_Bmp(0,0,480,272, flat1);	
		}
		
	
		break;
	case 2:
		if (book==1)
		{
		Paint_Bmp(0,0,480,272, brave2);	
		}
		else if (book==2)
		{
			Paint_Bmp(0,0,480,272, dao2);	
		}
		else if(book ==3)
		{
			Paint_Bmp(0,0,480,272, flat2);	
		}
		
	
		break;
	case 3:	
		if (book==1)
		{
		Paint_Bmp(0,0,480,272, brave3);	
		}
		else if (book==2)
		{
			Paint_Bmp(0,0,480,272, dao3);	
		}
		else if(book ==3)
		{
			Paint_Bmp(0,0,480,272, flat3);	
		}
		
	
		break;
		
	case 4:	
		if (book==1)
		{
		Paint_Bmp(0,0,480,272, brave1);	
		}
		else if (book==2)
		{
			Paint_Bmp(0,0,480,272, dao1);	
		}
		else if(book ==3)
		{
			Paint_Bmp(0,0,480,272, flat4);	
		}
		
	
		break;
		
		
	default:
	Paint_Bmp(0,0,480,272, shelf);	
	
		break;
	}        
			
}
}	
//前景色为黑色	0x0000；背景色为黄色
//rgb配色原则(0x1f<<11) | (0x3f<<5) | (0)
//Red： (0x1f<<11) | (0x00<<5) | (0x00)其实就是二进制数：11111 000000 00000
//Green：(0x00<<11) | (0x3f<<5) | (0x00)其实就是二进制数：00000 111111 00000
//Bule：(0x00<<11) | (0x00<<5) | (0x1f) 其实就是二进制数：00000 000000 11111
//White：(0x1f<<11) | (0x3f<<5) | (0x1f)其实就是二进制数：11111 111111 11111
//Black：(0x00<<11) | (0x00<<5) | (0x00) 其实就是二进制数：00000 000000 00000



void music( void )   
{
	unsigned char Temp1,Temp2;
	unsigned int Addr=0;
/*
	Uart_Printf("\nTest TFT LCD!\n");  
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");
*/
	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

#ifdef CSTM_LCD
  
	//g_type=get_pix(&pix_x,&pix_y);
	
	
	
	Paint_Bmp(0,0,480,272, musicwall);	
   		
     Lcd_ClearScr(0xffffff);
   
    Buzzer_Stop();
		
while(1)
	{	
	
		 //Addr=0;	
			
		if(xdata<480*1/6)
		{
			if(ydata<272/2)
			{	
				 Buzzer_Stop();
				
				Paint_Bmp(0,0,480,272, musicwall);		
    			
    			
    		}
    		else
    			{ 
    				 Buzzer_Stop();
    				uimenu();
    			}
		}
	
		
		else if((xdata>480/6)&&(xdata<480/3))
		{	
			 //Buzzer_Stop();
			// 
			
			U16 freq;
			Paint_Bmp(0,0,480,272,Doraemon);
			Paint_Bmp(100,50,135,135, birth);
				
			Lcd_printf(250,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"hapyy birthday" );
			Temp1=SONGhappy[Addr++];	    
			Temp2=SONGhappy[Addr++];    	    
			freq=25000/Temp1;	
			Buzzer_Freq_Set( freq );
			Delay(12*Temp2);		    
			Uart_Printf("freq=%d,",freq);
			if(Addr>215)
			 {
		 	  	Addr=0;  
			 }
		}
		else if(xdata>480*2/3)
			{
			//
			// Buzzer_Stop();
			/*unsigned char Temp1,Temp2;
			unsigned int Addr=0;*/
			U16 freq;
			Paint_Bmp(0,0,480,272,Doraemon);
			Paint_Bmp(100,50,135,135, stars);	
			
			Lcd_printf(250,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"stars" );
			Temp1=SONGstar[Addr++];	    
			Temp2=SONGstar[Addr++];    	    
			freq=25000/Temp1;	
			Buzzer_Freq_Set( freq );
			Delay(12*Temp2);		    
			Uart_Printf("freq=%d,",freq);  
			if(Addr>215)
			 {
		 	  	Addr=0;  
			 }
			}
		else if((xdata>480/3)&&(xdata<480*2/3))
			{
				// Buzzer_Stop();
				//
				/*unsigned char Temp1,Temp2;
			unsigned int Addr=0;*/
			U16 freq;
			Paint_Bmp(0,0,480,272,Doraemon);
			Paint_Bmp(100,50,135,135, huayao);	
			
			Lcd_printf(250,100, 0x0000,(0x1f<<11) | (0x3f<<5) | (0),1,"hua yao" );
			Temp1=SONGun[Addr++];	    
			Temp2=SONGun[Addr++];    	    
			freq=25000/Temp1;	
			Buzzer_Freq_Set( freq );
			Delay(12*Temp2);		    
			Uart_Printf("freq=%d,",freq);
			if(Addr>215)
			 {
		 	  	Addr=0;  
			 }
			}
	
        }
			


#endif
	
}

void record( void )
{
	if(xdata<480*1/6)
		{
			uimenu();	
		}
	else 
		{
	 		RecordTest();
	 		PlayMusicTest();
		}
}

void snake( void )
{

	Uart_Printf("\nTest TFT LCD!\n");
	rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);

  
	//g_type=get_pix(&pix_x,&pix_y);
	
	Lcd_ClearScr(0);
	
    //Paint_Bmp(0,0,50,272, rim);	
while(1)
{	
		
		if(xdata<480*1/6)
		{
			
    		uimenu();
    			
		}
		else 
			playsnake();
			
}
}


void playsnake(void)
{
	int ii,num_Test,num=3;
	int flagTc=0;
	//Board_Init();
	
	#ifdef CSTM_LCD
		cstmLcd_init();
	#else
		Lcd_TFT_Init() ;		// LCD initial
	#endif
		
	download_run=1; //The default menu is the Download & Run mode.
	//*********************
    rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	rADCCON=(1<<14)+(ADCPRS<<6);   //ADCPRS En, ADCPRS Value

	Uart_Printf("\nTouch Screen test\n");

	rADCTSC=0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

	pISR_ADC = (int)AdcTsAuto;
	rINTMSK=~BIT_ADC;       //ADC Touch Screen Mask bit clear
	rINTSUBMSK=~(BIT_SUB_TC);
    //*********************     
    Snake_Point[0].Current_x=2;
    Snake_Point[0].Current_y=0;
    Snake_Point[1].Current_x=1;
    Snake_Point[1].Current_y=0;
    Snake_Point[2].Current_x=0;
    Snake_Point[2].Current_y=0;
    Snake_Point[0].Direction=4; 
        
    point[0].Current_x=4;
    point[0].Current_y=3;
    point[1].Current_x=5;
    point[1].Current_y=8;
    point[2].Current_x=10;
    point[2].Current_y=2;
	point[3].Current_x=9;
    point[3].Current_y=4;
    point[4].Current_x=4;
    point[4].Current_y=10;
    for(ii=0;ii<5;ii++)
    {
    	point[ii].Eaten=1;
    }
    
    while(1)
    {   
    	Lcd_ClearScr(0xffffff);
    	Glib_FilledRectangle(280,220,300,240,0x00ff00);
    	Glib_FilledRectangle(260,200,280,220,0x00ff00);
    	Glib_FilledRectangle(280,180,300,200,0x00ff00);
    	Glib_FilledRectangle(300,200,320,220,0x00ff00);
    	
    	Glib_FilledRectangle(280,140,300,160,0x00ff00);
    	Glib_FilledRectangle(282,202,298,218,0x00ff00);
    	Glib_Line(240,0,240,240,0xfff000);
    	
    	//画新蛇,画新点，蛇点之间无联系    	
    	for(ii=0;ii<num;ii++)
    	{
    		int iix,iiy;
    		iix=20*Snake_Point[ii].Current_x;
    		iiy=20*Snake_Point[ii].Current_y;
        	Glib_FilledRectangle(iix,iiy,iix+18,iiy+18,0x00ff00);      	
        }
        
        ii=0;
        while(ii<5)
        {
        	int ix,iy;
        	ix=20*point[ii].Current_x;
    		iy=20*point[ii].Current_y;
        	if( point[ii].Eaten==1 )
        	{      					
        		Glib_FilledRectangle(ix,iy,ix+18,iy+18,0x00ff00);	
        	}
        	ii++;
        }
    	//除蛇头外的数据更新，将前一点坐标给后一点
    	for(ii=num;ii>0;ii--)
    	{
    		Snake_Point[ii].Current_x=Snake_Point[ii-1].Current_x;
    		Snake_Point[ii].Current_y=Snake_Point[ii-1].Current_y;
    	}
   		
		Delay(500);//读按键信息
    	if(flagTc==1)
    	{
    		//判断是否结束	
    		if(Gover==0) 
    		flag_Out=0;
    		//暂停
        	while(Pause==1)
    		{
    			Delay(500);
    			if(Gover==0)
    			{flag_Out=0;break;}    		
    		} 
    		//若是方向键被按则检查是否出现与运动方向相反的情况，有则校正
    		if(direction==2 && Snake_Point[0].Direction==4)
    		direction=4;
    		if(direction==4 && Snake_Point[0].Direction==2)
    		direction=2;
    		if(direction==1 && Snake_Point[0].Direction==3)
    		direction=3;
    		if(direction==3 && Snake_Point[0].Direction==1)
    		direction=1;	
    	}
    	//头方块数据更新
    	Snake_Point[0].Direction=direction;
    	switch(Snake_Point[0].Direction)
    	{
    		case 1:	Snake_Point[0].Current_x=Snake_Point[0].Current_x;
    				Snake_Point[0].Current_y=Snake_Point[0].Current_y+1;
    				break;
    		case 2:	Snake_Point[0].Current_x=Snake_Point[0].Current_x-1;
    				Snake_Point[0].Current_y=Snake_Point[0].Current_y;
    				break;
    		case 3:	Snake_Point[0].Current_x=Snake_Point[0].Current_x;
    				Snake_Point[0].Current_y=Snake_Point[0].Current_y-1;
    				break;
    		case 4:	Snake_Point[0].Current_x=Snake_Point[0].Current_x+1;
    				Snake_Point[0].Current_y=Snake_Point[0].Current_y;
    				break;
    	}
    	//是否撞墙
    	if( Snake_Point[0].Current_x<0 || Snake_Point[0].Current_x>11)
    	flag_Out=0;
    	if( Snake_Point[0].Current_y<0 || Snake_Point[0].Current_y>11)
    	flag_Out=0;
    	//是否撞上自己
    	num_Test=num;
    	while(num_Test)
    	{
    		if((Snake_Point[0].Current_x==Snake_Point[num_Test].Current_x) && (Snake_Point[0].Current_y==Snake_Point[num_Test].Current_y))
    		{flag_Out=0;break;}
    		num_Test--;
    	}
    	if(flag_Out==0)
    	{
    		flag_Out=1;
    		Lcd_ClearScr(0xfff000);
    		Uart_Printf("Snake Game is over!!!\n");break;
    	}
    	//是否吃豆子
    	ii=0;
        while(ii<5)
        {
        	if( (Snake_Point[0].Current_x==point[ii].Current_x) && (Snake_Point[0].Current_y==point[ii].Current_y) )
        	{
        		if(point[ii].Eaten==1)
        		{point[ii].Eaten=0;
        		num++;}	
        	}
        	ii++;
        }
        //豆子是否吃完
        if(num==8) 
    	{Lcd_ClearScr(0x0fff00);
    	Uart_Printf("Snake Game is passed!!!\n");break;}	
    } 
    
    rINTSUBMSK|=BIT_SUB_TC;
	rINTMSK|=BIT_ADC;	
}



void action(int  color)
{	
	//四个方位键响应
	if( (x11>280&&x11<300) && (y11>220&&y11<240) )
	{
		direction=1;
		if((rADCDAT0|0x7fff)==0x7fff)	Beep(2000, 50);
		Glib_FilledRectangle(280,220,300,240,color);
	}
	else if( (x11>260&&x11<280) && (y11>200&&y11<220) )
	{
		direction=2;
		if((rADCDAT0|0x7fff)==0x7fff)	Beep(2000, 50);
		Glib_FilledRectangle(260,200,280,220,color);
	}
	else if( (x11>280&&x11<300) && (y11>180&&y11<200) )
	{
		direction=3;
		if((rADCDAT0|0x7fff)==0x7fff)	Beep(2000, 50);
		Glib_FilledRectangle(280,180,300,200,color);
	}
	else if( (x11>300&&x11<320) && (y11>200&&y11<220) )
	{
		direction=4;
		if((rADCDAT0|0x7fff)==0x7fff)	Beep(2000, 50);
		Glib_FilledRectangle(300,200,320,220,color);
	}
	//结束键响应
	else if( (x11>280&&x11<300) && (y11>140&&y11<160) )
	{
		if((rADCDAT0|0x7fff)==0x7fff)	
		{
		Gover=0;
		Beep(2000, 50);
		}
		Glib_FilledRectangle(280,140,300,160,color);
	}
	//暂停键响应
	else if( (x11>282&&x11<298) && (y11>202&&y11<218) )
	{
		if((rADCDAT0|0x7fff)==0x7fff)	
		{
		Pause=!Pause;
		Beep(2000, 50);
		}
		Glib_FilledRectangle(282,202,298,218,color);
	}
}



void __irq AdcTsAuto(void)
{
	U32 saveAdcdly;
	U32 temp;

	xdata=0;
	ydata=0;
	if(rADCDAT0&0x8000)
	{
		//Uart_Printf("\nStylus Up!!\n");
		rADCTSC&=0xff;	// Set stylus down interrupt bit
	}
	//else 
		//Uart_Printf("\nStylus Down!!\n");

	rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
	saveAdcdly=rADCDLY;
	rADCDLY=40000;                 //Normal conversion mode delay about (1/50M)*40000=0.8ms

	rADCCON|=0x1;                   //start ADC

	while(rADCCON & 0x1);		//check if Enable_start is low
	while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!!
		
	while(!(rSRCPND & (BIT_ADC)));  //check if ADC is finished with interrupt bit

	xdata=(rADCDAT0&0x3ff);
 	ydata=(rADCDAT1&0x3ff);

	//check Stylus Up Interrupt.
	rSUBSRCPND|=BIT_SUB_TC;
	ClearPending(BIT_ADC);
	rINTSUBMSK=~(BIT_SUB_TC);
	rINTMSK=~(BIT_ADC);
			 
	rADCTSC =0xd3;    //Waiting for interrupt
	rADCTSC=rADCTSC|(1<<8); // Detect stylus up interrupt signal.

	while(1)		//to check Pen-up state
	{
		if(rSUBSRCPND & (BIT_SUB_TC))	//check if ADC is finished with interrupt bit
		{
			//Uart_Printf("Stylus Up Interrupt~!\n");
			break;	//if Stylus is up(1) state
		}
	}	

//-----add bgn---------------------------------------
#ifdef CSTM_LCD
    xdata *= 0.272;
    ydata *= 0.480;
    ydata = 480-ydata;
    temp = ydata;
    ydata = xdata;
    xdata = temp;

    xdata = (xdata - 20.0303) / 0.8972;
    ydata = (ydata - 37.86667) / 0.7486;
	
	if (g_type == LCD_35)
	{
		xdata=480-xdata;
		xdata=(double)((double)320/480)*xdata;
		ydata=272-ydata;
		ydata=(double)((double)240/271)*ydata;
	}
	else if(g_type == LCD_70)
	{
		xdata=480-xdata;
		xdata=(double)((double)800/480)*xdata;
	//	ydata=272-ydata;
		ydata=ydata<0?0:ydata;
		ydata=(double)((double)480/272)*ydata;
	}
	

#endif
	Delay(20);
	x11=xdata;
	y11=ydata;
	action(0xff00ff);
//-----add end---------------------------------------------
    Uart_Printf("count=%03d  XP=%04d, YP=%04d\n", count++, xdata, ydata);
    
    
 
	rADCDLY=saveAdcdly; 
	rADCTSC=rADCTSC&~(1<<8); // Detect stylus Down interrupt signal.
	rSUBSRCPND|=BIT_SUB_TC;
	rINTSUBMSK=~(BIT_SUB_TC);	// Unmask sub interrupt (TC)     
	ClearPending(BIT_ADC);
}

void Isr_Init(void)
{
	pISR_UNDEF=(unsigned)HaltUndef;
	pISR_SWI  =(unsigned)HaltSwi;
	pISR_PABORT=(unsigned)HaltPabort;
	pISR_DABORT=(unsigned)HaltDabort;
	rINTMOD=0x0;	  // All=IRQ mode
	rINTMSK=BIT_ALLMSK;	  // All interrupt is masked.
}


void HaltUndef(void)
{
	Uart_Printf("Undefined instruction exception!!!\n");
	while(1);
}

void HaltSwi(void)
{
	Uart_Printf("SWI exception!!!\n");
	while(1);
}

void HaltPabort(void)
{
	Uart_Printf("Pabort exception!!!\n");
	while(1);

}

void HaltDabort(void)
{
	Uart_Printf("Dabort exception!!!\n");
	while(1);
}


void ClearMemory(void)
{
	int memError=0;
	U32 *pt;
	
	Uart_Printf("Clear Memory (%xh-%xh):WR",_RAM_STARTADDRESS,HEAPEND);

	pt=(U32 *)_RAM_STARTADDRESS;
	while((U32)pt < HEAPEND)
	{
		*pt=(U32)0x0;
		pt++;
	}
	
	if(memError==0)Uart_Printf("\b\bO.K.\n");
}

void Clk0_Enable(int clock_sel)	
{	// 0:MPLLin, 1:UPLL, 2:FCLK, 3:HCLK, 4:PCLK, 5:DCLK0
	rMISCCR = rMISCCR&~(7<<4) | (clock_sel<<4);
	rGPHCON = rGPHCON&~(3<<18) | (2<<18);
}
void Clk1_Enable(int clock_sel)
{	// 0:MPLLout, 1:UPLL, 2:RTC, 3:HCLK, 4:PCLK, 5:DCLK1	
	rMISCCR = rMISCCR&~(7<<8) | (clock_sel<<8);
	rGPHCON = rGPHCON&~(3<<20) | (2<<20);
}
void Clk0_Disable(void)
{
	rGPHCON = rGPHCON&~(3<<18);	// GPH9 Input
}
void Clk1_Disable(void)
{
	rGPHCON = rGPHCON&~(3<<20);	// GPH10 Input
}

