#include "def.h"
#include "LCD_TFT.h"
#include "calibrate.h"

volatile int xdata1,xdata2,ydata1,ydata2;
int A, B, C, D, E, F, flagTc=0, K;


//触摸屏校正
void TSCal()
{
       int i=0;
       int xt[3],yt[3];
       K=1;
       Lcd_ClearScr( (0x00<<11) | (0x00<<5) | (0x00)  )  ;
       drawCross(XD0,YD0,(0xFF<<11) | (0x00<<5) | (0x00));        
       drawCross(XD1,YD1,(0xFF<<11) | (0x00<<5) | (0x00));
       drawCross(XD2,YD2,(0xFF<<11) | (0x00<<5) | (0x00));
 
       //依次读取三个采样点的坐标值    :不能取在一条直线上
       i=0;
       while(i<3)
       {    
          if(flagTc==1)
          {
          	Beep(2000, 100);
          	Delay(500);
         	xt[i]=xdata1;		//在触摸屏上采集的点
         	yt[i]=ydata1;
         	flagTc=0;
          	i++; 
          }            
       } 
//计算参数
/*
K＝(XT0－XT2)×(YT1－YT2)－(XT1－XT2)×(YT0－YT2)
A＝[(XD0－XD2)×(YT1－YT2)－(XD1－XD2)×(YT0－YT2)] 
B＝[(XT0－XT2)×(XD1－XD2)－(XD0－XD2)×(XT1－XT2)] 
C＝[YT0×(XT2×XD1－XT1×XD2)＋YT1×(XT0×XD2－XT2×XD0)＋YT2×(XT1×XD0－XT0×XD1)] 
D＝[(YD0－YD2)×(YT1－YT2)－(YD1－YD2)×(YT0－YT2)] 
E＝[(XT0－XT2)×(YD1－YD2)－(YD0－YD2)×(XT1－XT2)] 
F＝[YT0×(XT2×YD1－XT1×YD2)＋YT1×(XT0×YD2－XT2×YD0)＋YT2×(XT1×YD0－XT0×YD1)] 
*/
       K=(xt[0]-xt[2])*(yt[1]-yt[2])-(xt[1]-xt[2])*(yt[0]-yt[2]);
       A=(XD0-XD2)*(yt[1]-yt[2])-(XD1-XD2)*(yt[0]-yt[2]);
       B=(xt[0]-xt[2])*(XD1-XD2)-(XD0-XD2)*(xt[1]-xt[2]);
       C=yt[0]*(xt[2]*XD1-xt[1]*XD2)+yt[1]*(xt[0]*XD2-xt[2]*XD0)+yt[2]*(xt[1]*XD0-xt[0]*XD1);
       D=(YD0-YD2)*(yt[1]-yt[2])-(YD1-YD2)*(yt[0]-yt[2]);
       E=(xt[0]-xt[2])*(YD1-YD2)-(YD0-YD2)*(xt[1]-xt[2]);
       F=yt[0]*(xt[2]*YD1-xt[1]*YD2)+yt[1]*(xt[0]*YD2-xt[2]*YD0)+yt[2]*(xt[1]*YD0-xt[0]*YD1);
}

//绘制“十”字型
void drawCross(unsigned int x,unsigned int y,U32 color)
{
	U32 i=10;
    Glib_Line(x-i,y,x+i,y,color);
    Glib_Line(x,y-i,x,y+i,color);  
}



