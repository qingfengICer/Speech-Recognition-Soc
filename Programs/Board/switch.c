#include"switch.h"
#include "lcd.h"
//#include "snake.h"

extern uint32_t LCD_ini_finish;

 uint16_t x, y;
	uint8_t dx, dy;
  uint8_t t;

void gameinit(void)
{
  LCD->LCD_MODE = 1;
	LCD_RST_CLR;
	LCD_RST_SET;
	LCD_BL_CTR_SET;
	x  = y  = 110;
	dx = dy = 20;
	LCD_Init();
	while(!LCD_ini_finish) ;
	LCD_ini_finish = 0;
	LCD -> LCD_MODE = 1;
	LCD_CS_SET;
	LCD_RS_SET;
	LCD_WR_SET;
	LCD_RD_SET;
  LCD_Display_Dir(0);
	
	LCD_Fill(0,0,8,320,YELLOW);//Rectangle(8,8-1,85,120,0xaa);//绘制界面
	LCD_Fill(232,0,240,320,YELLOW);//
	LCD_Fill(0,0,240,8,YELLOW);//
	LCD_Fill(0,312,240,320,YELLOW);//
	LCD_Fill(64,144,68,170,YELLOW);//贪吃蛇选择界面
	LCD_Fill(64,140,170,144,YELLOW);//
	LCD_Fill(64,170,170,174,YELLOW);//
	LCD_Fill(166,144,170,174,YELLOW);//
	
	//LCD_Fill(64,154,68,180,YELLOW);//俄罗斯方块选择界面
	//LCD_Fill(64,150,170,154,YELLOW);//
	//LCD_Fill(64,180,170,184,YELLOW);//
	//LCD_Fill(166,154,170,184,YELLOW);//
	//LCD_ShowPicture(120,120,140,140,(uint16_t *)gImage_snake);
	Chinese_Show_one(60,50,0,16,0);
	Chinese_Show_one(80,50,1,16,0);
	Chinese_Show_one(100,50,2,16,0);
	Chinese_Show_one(120,50,3,16,0);
	Chinese_Show_one(140,50,4,16,0);
	Chinese_Show_one(160,50,5,16,0);//队伍名称
	
	Chinese_Show_one(70,150,6,16,0);
	Chinese_Show_one(110,150,7,16,0);
	Chinese_Show_one(150,150,8,16,0);//贪吃蛇
	
	Chinese_Show_one(70,230,9,16,0);
	Chinese_Show_one(90,230,10,16,0);
	Chinese_Show_one(110,230,11,16,0);
	Chinese_Show_one(130,230,12,16,0);
	Chinese_Show_one(150,230,13,16,0);//俄罗斯方块
	//游戏三
	
	LCD_ShowPicture(70,90,1);
	LCD_ShowPicture(70,175,2);
}

void choose(uint8_t game)
{
	
	if (game==1)
		{
	LCD_Fill(64,144,68,170,WHITE);//贪吃蛇选择界面
	LCD_Fill(64,140,170,144,WHITE);//
	LCD_Fill(64,170,170,174,WHITE);//
	LCD_Fill(166,144,170,174,WHITE);//
	
	LCD_Fill(64,224,68,250,YELLOW);//选择界面
	LCD_Fill(64,220,170,224,YELLOW);//
	LCD_Fill(64,250,170,254,YELLOW);//
	LCD_Fill(166,224,170,254,YELLOW);//
	}	
	else if (game==0)
	{
	LCD_Fill(64,144,68,170,YELLOW);//贪吃蛇选择界面
	LCD_Fill(64,140,170,144,YELLOW);//
	LCD_Fill(64,170,170,174,YELLOW);//
	LCD_Fill(166,144,170,174,YELLOW);//
	
	LCD_Fill(64,224,68,250,WHITE);//选择界面
	LCD_Fill(64,220,170,224,WHITE);//
	LCD_Fill(64,250,170,254,WHITE);//
	LCD_Fill(166,224,170,254,WHITE);//

	}
}

void gamestart(void)
{
  LCD->LCD_MODE = 1;
	LCD_RST_CLR;
	LCD_RST_SET;
	LCD_BL_CTR_SET;
	x  = y  = 110;
	dx = dy = 20;
	LCD_Init();
	while(!LCD_ini_finish) ;
	LCD_ini_finish = 0;
	LCD -> LCD_MODE = 1;
	LCD_CS_SET;
	LCD_RS_SET;
	LCD_WR_SET;
	LCD_RD_SET;
  LCD_Display_Dir(0);
	
	LCD_Fill(0,0,8,320,YELLOW);//Rectangle(8,8-1,85,120,0xaa);//绘制界面
	LCD_Fill(232,0,240,320,YELLOW);//
	LCD_Fill(0,0,240,8,YELLOW);//
	LCD_Fill(0,312,240,320,YELLOW);//
	LCD_ShowString(55,160,240,24,24,"GAME START!"); 
	for(t=0;t<10;t++){
	Delay(2147483647);
	}
	LCD->LCD_MODE = 1;
	LCD_RST_CLR;
	LCD_RST_SET;
	LCD_BL_CTR_SET;
	x  = y  = 110;
	dx = dy = 20;
	LCD_Init();
	while(!LCD_ini_finish) ;
	LCD_ini_finish = 0;
	LCD -> LCD_MODE = 1;
	LCD_CS_SET;
	LCD_RS_SET;
	LCD_WR_SET;
	LCD_RD_SET;
  LCD_Display_Dir(0);
	
	LCD_Fill(0,0,8,320,YELLOW);//Rectangle(8,8-1,85,120,0xaa);//绘制界面
	LCD_Fill(232,0,240,320,YELLOW);//
	LCD_Fill(0,0,240,8,YELLOW);//
	LCD_Fill(0,312,240,320,YELLOW);
}

void play(uint8_t en)
{
if(en==1)
	{
	
}
	}

