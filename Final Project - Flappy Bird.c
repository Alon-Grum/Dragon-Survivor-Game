#include "bass.h"
#include "windows.h"  //include for keyboard monitoring 
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "Final Project - Flappy Bird.h"
#define G 1300
void update_bonus();
int Bonus_Check_Hits();
void draw_bonus();
void is_key_pressed_bonus();
void Finish_Bonus();
void Update_Score_List();


typedef struct
{
	int pic1;
	int trans_pic;
	int radius;
	int x_position;
	double y_position;
	double vy;
} flappy_bird;

typedef struct
{
	int pic;
	int x_position;
	int y_position;
	int length;
} tube;

typedef struct
{
	int x_position;
	int y_position;
	int pic;
} tip;

typedef struct
{
	int x_position;
	int y_position;
	int top;
	int left;
	int pic;
} coins;

typedef struct
{
	int x_position;
	int y_position;
	int radius;
	double vx;
	int pic;
	int width;
	int hit_pic;
	int hit_pic_counter;
	int hit_left;
	int hit_width;
	double vx_increase;
} enemies;

typedef struct
{
	int x_position;
	int y_position;
	int radius;
	int vx;
	int pic;
	int width;
	int pic_counter;
	int left;
	int calc_fire;
	int life;
} fire;


flappy_bird dragon;
tube upper_tube,down_tube,upper_tube2,down_tube2,upper_tube3,down_tube3;
tip upper_tip,down_tip,upper_tip2,down_tip2,upper_tip3,down_tip3;
coins coin;
enemies enemy;
fire fire_ball;

HSTREAM Wing,Hit,Points,Invisible,Bonus,Fire_Ball,Enemy_Hit;
FILE *fp;

static int panelHandle,panelHandle2,panelHandle3,panelHandle4;
int width,height,dragon_left,background,background_night,ground,ground_x_position=0,score=0,back_ground_counter=0,dragon_pic_counter=0,tubes_gap=97,square4_up=-10,square4_left=0,coin_img,coin_left=0,coin_pic_counter=0,coin_x_position,coin_y_position,coin_hit_counter=0,coin_flag=1,trans_pic_counter=0,trans_flag=0,trans_numeric=1000,coin4_flag=1,enemy_left,enemy_pic_counter=0,fire_key=0,bonus_numeric=2500;
double dt=0.01;
char player_name[20],csv_name[10][20],line_buffer[300],msg[50];
int csv_score[10];

//variables for keyboard monitoring
int Callback_Data;
int Posting_Handle;
int Keys_Down[256]= {0};

//Receives information from windows regarding key presses
int CVICALLBACK Key_DownFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key] = 1;
	return 0;
}

//Receives information from windows regarding key releases
int CVICALLBACK Key_UpFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key]= 0;
	return 0;
}

//asking windows to send keyboard press and release events to our software
//Specifing 'Key_DownFunc' and 'Key_UpFunc' as the functions to call
void Connect_To_Windows_Events(void)
{
	InstallWinMsgCallback(panelHandle,WM_KEYDOWN,Key_DownFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
	InstallWinMsgCallback(panelHandle,WM_KEYUP,Key_UpFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
}


void initialize()
{
	int i=0;

	GetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_WIDTH, &width);
	GetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_HEIGHT, &height);
	SetPanelSize (panelHandle, height, width);
	SetPanelAttribute (panelHandle, ATTR_SIZABLE,0 );
	dragon.x_position=width/5;
	dragon.y_position=height*3/5;
	dragon.vy=-G/3;
	dragon.radius=17;
	upper_tube.x_position=width*2-1;
	down_tube.x_position=upper_tube.x_position;
	upper_tube2.x_position=width*2+250;
	down_tube2.x_position=upper_tube2.x_position;
	upper_tube3.x_position=width*2+500;
	down_tube3.x_position=upper_tube3.x_position;
	upper_tip.x_position=width*2-((int)width/14+5)/5;
	down_tip.x_position=upper_tip.x_position;
	upper_tip2.x_position=width*2+250-((int)width/14+5)/5;
	down_tip2.x_position=upper_tip2.x_position;
	upper_tip3.x_position=width*2+500 -((int)width/14+5)/5;
	down_tip3.x_position=upper_tip3.x_position;
	srand(time(0));
	upper_tube.length=50+rand()%200;
	upper_tube2.length=50+rand()%200;
	upper_tube3.length=50+rand()%200;
	coin.x_position=width*2-((int)width/14+5)/5+150;
	coin.y_position=(upper_tube.length-(int)height/19.8)+rand()%(80+tubes_gap);
	enemy.radius=30;
	enemy.x_position=width+enemy.radius*2;
	enemy.y_position=enemy.radius+rand()%(height*4/5-2*enemy.radius);
	BASS_Init( -1,44100, 0,0,NULL);
	GetBitmapFromFile ("BackGround.png", &background);
	GetBitmapFromFile ("BackGround_Night.png", &background_night);
	GetBitmapFromFile ("dragon.png", &dragon.pic1);
	GetBitmapFromFile ("dragon_trans.png", &dragon.trans_pic);
	dragon_left=382;
	enemy_left=438;
	enemy.width=88;
	enemy.vx=200;
	enemy.vx_increase=1;
	fire_ball.pic_counter=0;
	fire_ball.radius=15;
	fire_ball.calc_fire=0;
	enemy.hit_pic_counter=0;
	fire_ball.life=0; // if fire_ball.life is zero it means it didnt hit the enemy yet.
	GetBitmapFromFile ("Groud.png", &ground);
	GetBitmapFromFile ("Tube.png", &upper_tube.pic);
	GetBitmapFromFile ("Tube.png", &down_tube.pic);
	GetBitmapFromFile ("Upper_Tube_Tip.png", &upper_tip.pic);
	GetBitmapFromFile ("Down_Tube_Tip.png", &down_tip.pic);
	GetBitmapFromFile ("coin.png", &coin.pic);
	GetBitmapFromFile ("Flying_Enemy.png", &enemy.pic);
	GetBitmapFromFile ("Fire_Ball.png", &fire_ball.pic);
	GetBitmapFromFile ("Enemy_Hit.png", &enemy.hit_pic);
	Wing=BASS_StreamCreateFile(FALSE,"d_wings_sound.mp3",0,0,0);
	Points=BASS_StreamCreateFile(FALSE,"point_sound.mp3",0,0,0);
	Invisible=BASS_StreamCreateFile(FALSE,"invisible_sound.mp3",0,0,0);
	Hit=BASS_StreamCreateFile(FALSE,"hit_sound.mp3",0,0,0);
	Bonus=BASS_StreamCreateFile(FALSE,"bonus_sound.mp3",0,0,0);
	Fire_Ball=BASS_StreamCreateFile(FALSE,"fire_ball_sound.mp3",0,0,0);
	Enemy_Hit=BASS_StreamCreateFile(FALSE,"enemy_hit_sound.mp3",0,0,0);
	//Get scores from excel into arrays.
	fp = fopen ("Score_List.CSV", "r");
	while (fgets(line_buffer, 300, fp)!=0)
	{
		sscanf(line_buffer,"%[^','],%d",csv_name[i],&csv_score[i]);
		i++;
		if(i==10)
			break;
	}
}

void Terminate()
{
	DiscardBitmap (background);
	DiscardBitmap (background_night);
	DiscardBitmap (dragon.pic1);
	DiscardBitmap (dragon.trans_pic);
	DiscardBitmap (ground);
	DiscardBitmap (upper_tube.pic);
	DiscardBitmap (down_tube.pic);
	DiscardBitmap (upper_tip.pic);
	DiscardBitmap (down_tip.pic);
	DiscardBitmap (coin.pic);
	DiscardBitmap (enemy.pic);
	DiscardBitmap (fire_ball.pic);
	DiscardBitmap (enemy.hit_pic);
	BASS_StreamFree(Wing);
	BASS_StreamFree(Hit);
	BASS_StreamFree(Points);
	BASS_StreamFree(Invisible);
	BASS_StreamFree(Bonus);
	BASS_StreamFree(Fire_Ball);
	BASS_StreamFree(Enemy_Hit);

}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Final Project - Flappy Bird.uir", PANEL)) < 0)
		return -1;
	if ((panelHandle2 = LoadPanel (0, "Final Project - Flappy Bird.uir", PANEL_2)) < 0)
		return -1;
	if ((panelHandle3 = LoadPanel (0, "Final Project - Flappy Bird.uir", PANEL_3)) < 0)
		return -1;
	if ((panelHandle4 = LoadPanel (0, "Final Project - Flappy Bird.uir", PANEL_4)) < 0)
		return -1;
	initialize();
	Connect_To_Windows_Events();  //asking windows to send keyboard press and release events to our software
	DisplayPanel (panelHandle2);
	RunUserInterface ();
	Terminate();
	DiscardPanel (panelHandle3);
	DiscardPanel (panelHandle2);
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK exitPanel (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (panelHandle);
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			SetCtrlAttribute (panelHandle, PANEL_BONUS_TIMER, ATTR_ENABLED, 0);
			initialize();
			DisplayPanel (panelHandle2);
			break;
	}
	return 0;
}

int CVICALLBACK exitPanel2 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK exitPanel3 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (panelHandle3);
			DisplayPanel (panelHandle2);
			break;
	}
	return 0;
}

int CVICALLBACK exitPanel4 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (panelHandle4);
			DisplayPanel (panelHandle2);
			break;
	}
	return 0;
}

void draw()
{
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
	CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, background, VAL_ENTIRE_OBJECT, MakeRect(0,0, height,width));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, ground, VAL_ENTIRE_OBJECT, MakeRect(height-height/5,ground_x_position, height/5,width*2));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, down_tip.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube.length+(int)height/19.8+tubes_gap,down_tip.x_position,(int)height/19.8 ,(int)width/14+20));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, down_tip.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube2.length+(int)height/19.8+tubes_gap,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, down_tip.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube3.length+(int)height/19.8+tubes_gap,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, down_tube.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube.length+2*(int)height/19.8+tubes_gap,down_tube.x_position,height*4/5-2*(int)height/19.8-upper_tube.length-tubes_gap ,(int)width/14));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, down_tube.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube2.length+2*(int)height/19.8+tubes_gap,down_tube2.x_position,height*4/5-2*(int)height/19.8-upper_tube2.length-tubes_gap ,(int)width/14));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, down_tube.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube3.length+2*(int)height/19.8+tubes_gap,down_tube3.x_position,height*4/5-2*(int)height/19.8-upper_tube3.length-tubes_gap ,(int)width/14));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, upper_tip.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube.length,down_tip.x_position,(int)height/19.8 ,(int)width/14+20));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, upper_tip.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube2.length,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, upper_tip.pic, VAL_ENTIRE_OBJECT, MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, upper_tube.pic, VAL_ENTIRE_OBJECT, MakeRect(0,upper_tube.x_position,upper_tube.length ,(int)width/14));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, upper_tube.pic, VAL_ENTIRE_OBJECT, MakeRect(0,upper_tube2.x_position,upper_tube2.length ,(int)width/14));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, upper_tube.pic, VAL_ENTIRE_OBJECT, MakeRect(0,upper_tube3.x_position,upper_tube3.length ,(int)width/14));
	if (trans_flag==1)
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, dragon.trans_pic,MakeRect(0,dragon_left,122,151), MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2));
	else
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, dragon.pic1, MakeRect(0,dragon_left,122,151), MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2));
	if(coin_flag==1 && trans_flag==0)
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, coin.pic, MakeRect(0,130+coin.left,127,130), MakeRect(coin.y_position,coin.x_position,25,25));

//check for accurate hit with small squares.
	/* CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9), VAL_DRAW_FRAME);
	CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14), VAL_DRAW_FRAME);
	CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12), VAL_DRAW_FRAME);
	CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24), VAL_DRAW_FRAME); */
	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
}

void update()
{

	dragon_pic_counter++;
	coin_pic_counter++;
	SetCtrlVal (panelHandle, PANEL_NUMERICSLIDE_2, 0);

	if (trans_flag==1)
	{
		trans_pic_counter++;
		trans_numeric--;
		SetCtrlVal (panelHandle, PANEL_NUMERICSLIDE, trans_numeric);
	}
	if (trans_pic_counter==1000)
	{
		trans_flag=0;
		trans_pic_counter=0;
		trans_numeric=1000;
	}

	ground_x_position=ground_x_position-2;
	upper_tube.x_position=upper_tube.x_position-2;
	upper_tube2.x_position=upper_tube2.x_position-2;
	upper_tube3.x_position=upper_tube3.x_position-2;
	down_tube.x_position=down_tube.x_position-2;
	down_tube2.x_position=down_tube2.x_position-2;
	down_tube3.x_position=down_tube3.x_position-2;
	upper_tip.x_position=upper_tip.x_position-2;
	upper_tip2.x_position=upper_tip2.x_position-2;
	upper_tip3.x_position=upper_tip3.x_position-2;
	down_tip.x_position=down_tip.x_position-2;
	down_tip2.x_position=down_tip2.x_position-2;
	down_tip3.x_position=down_tip3.x_position-2;
	coin.x_position=coin.x_position-2;

	if (ground_x_position<=-width)
		ground_x_position=0;
	if (upper_tube.x_position<=-width/14)
	{
		upper_tube.x_position=width;
		down_tube.x_position=width;
		upper_tip.x_position=width-((int)width/14+5)/5;
		down_tip.x_position=width-((int)width/14+5)/5;
		upper_tube.length=50+rand()%200;
	}

	if (upper_tube2.x_position<=-width/14)
	{
		upper_tube2.x_position=width;
		down_tube2.x_position=width;
		upper_tip2.x_position=width-((int)width/14+5)/5;
		down_tip2.x_position=width-((int)width/14+5)/5;
		upper_tube2.length=50+rand()%200;
	}

	if (upper_tube3.x_position<=-width/14)
	{
		upper_tube3.x_position=width;
		down_tube3.x_position=width;
		upper_tip3.x_position=width-((int)width/14+5)/5;
		down_tip3.x_position=width-((int)width/14+5)/5;
		upper_tube3.length=50+rand()%200;
	}

	if (coin.x_position<=-50)
	{
		coin.x_position=width;
		coin.y_position=(upper_tube2.length-(int)height/19.8)+rand()%(80+tubes_gap);
		coin_flag=1;
	}

	if (dragon.x_position==upper_tube.x_position+(int)width/14)
	{
		BASS_ChannelPlay(Points,TRUE);
		score++;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
	}
	if (dragon.x_position==upper_tube2.x_position+(int)width/14)
	{
		BASS_ChannelPlay(Points,TRUE);
		score++;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
	}
	if (dragon.x_position==upper_tube3.x_position+(int)width/14)
	{
		BASS_ChannelPlay(Points,TRUE);
		score++;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
	}

	if (dragon.x_position==upper_tube.x_position+(int)width/14-1)
	{
		BASS_ChannelPlay(Points,TRUE);
		score++;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
	}
	if (dragon.x_position==upper_tube2.x_position+(int)width/14-1)
	{
		BASS_ChannelPlay(Points,TRUE);
		score++;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
	}
	if (dragon.x_position==upper_tube3.x_position+(int)width/14-1)
	{
		BASS_ChannelPlay(Points,TRUE);
		score++;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
	}

	dragon.y_position=dragon.y_position+dragon.vy*dt+(G)*(pow(dt,2))/2;
	dragon.vy=dragon.vy+G*dt;

	if(dragon_pic_counter==15)
	{
		dragon_left=0;
		square4_up=0;
		square4_left=0;
	}
	if(dragon_pic_counter==30)
	{
		dragon_left=187;
		square4_up=9;
		square4_left=0;
	}
	if(dragon_pic_counter==45)
	{
		dragon_left=382;
		square4_up=11;
		square4_left=7;
		dragon_pic_counter=0;
	}

	if (coin_pic_counter==10)
		coin.left=0;
	if(coin_pic_counter==20)
		coin.left=165;
	if(coin_pic_counter==30)
		coin.left=165;
	if(coin_pic_counter==40)
		coin.left=350;
	if(coin_pic_counter==50)
	{
		coin.left=525;
		coin_pic_counter=0;
	}

	if(coin_hit_counter==3 && coin4_flag==1)
	{
		BASS_ChannelPlay(Invisible,TRUE);
		trans_pic_counter=0;
		trans_flag=1;
		coin4_flag=0;
		SetCtrlVal (panelHandle, PANEL_INVISIBLE, coin_hit_counter);
	}

	if (coin_hit_counter==6)
	{
		BASS_ChannelPlay(Bonus,TRUE);
		SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
		CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
		SetCtrlAttribute (panelHandle, PANEL_BONUS_TIMER, ATTR_ENABLED, 1);
//initialization all variables for bonus incase of a hit in the bonus round.
		ground_x_position=0;
		upper_tube.x_position=width*2;
		down_tube.x_position=upper_tube.x_position;
		upper_tube2.x_position=width*2+250;
		down_tube2.x_position=upper_tube2.x_position;
		upper_tube3.x_position=width*2+500;
		down_tube3.x_position=upper_tube3.x_position;
		upper_tip.x_position=width*2-((int)width/14+5)/5;
		down_tip.x_position=upper_tip.x_position;
		upper_tip2.x_position=width*2+250-((int)width/14+5)/5;
		down_tip2.x_position=upper_tip2.x_position;
		upper_tip3.x_position=width*2+500 -((int)width/14+5)/5;
		down_tip3.x_position=upper_tip3.x_position;
	}

}

int Check_Hits()
{
// 0. checking hits with the groud.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(height-height/5,ground_x_position, height/5,width*2), 0) )
	{
		dragon.y_position=height-height/5-dragon.radius;
		return 1;
	}

//1. checking hits with down_tip1.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube.length+(int)height/19.8+tubes_gap,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube.length+(int)height/19.8+tubes_gap,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube.length+(int)height/19.8+tubes_gap,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube.length+(int)height/19.8+tubes_gap,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube.length+(int)height/19.8+tubes_gap,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
		;
	}
//2. checking hits with down_tube1.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube.length+2*(int)height/19.8+tubes_gap,down_tube.x_position,height*4/5-2*(int)height/19.8-upper_tube.length-tubes_gap ,(int)width/14), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube.length+2*(int)height/19.8+tubes_gap,down_tube.x_position,height*4/5-2*(int)height/19.8-upper_tube.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube.length+2*(int)height/19.8+tubes_gap,down_tube.x_position,height*4/5-2*(int)height/19.8-upper_tube.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube.length+2*(int)height/19.8+tubes_gap,down_tube.x_position,height*4/5-2*(int)height/19.8-upper_tube.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube.length+2*(int)height/19.8+tubes_gap,down_tube.x_position,height*4/5-2*(int)height/19.8-upper_tube.length-tubes_gap ,(int)width/14), 0) )
			return 1;
	}
//3. checking hits with upper_tip1.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube.length,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube.length,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube.length,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube.length,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube.length,down_tip.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
	}
//4. checking hits with upper_tube1.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(0,upper_tube.x_position,upper_tube.length ,(int)width/14), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(0,upper_tube.x_position,upper_tube.length ,(int)width/14), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(0,upper_tube.x_position,upper_tube.length ,(int)width/14), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(0,upper_tube.x_position,upper_tube.length ,(int)width/14), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(0,upper_tube.x_position,upper_tube.length ,(int)width/14), 0) )
			return 1;
	}
//5. checking hits with down_tip2.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube2.length+(int)height/19.8+tubes_gap,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube2.length+(int)height/19.8+tubes_gap,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube2.length+(int)height/19.8+tubes_gap,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube2.length+(int)height/19.8+tubes_gap,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube2.length+(int)height/19.8+tubes_gap,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
	}
//6. checking hits with down_tube2.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube2.length+2*(int)height/19.8+tubes_gap,down_tube2.x_position,height*4/5-2*(int)height/19.8-upper_tube2.length-tubes_gap ,(int)width/14), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube2.length+2*(int)height/19.8+tubes_gap,down_tube2.x_position,height*4/5-2*(int)height/19.8-upper_tube2.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube2.length+2*(int)height/19.8+tubes_gap,down_tube2.x_position,height*4/5-2*(int)height/19.8-upper_tube2.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube2.length+2*(int)height/19.8+tubes_gap,down_tube2.x_position,height*4/5-2*(int)height/19.8-upper_tube2.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube2.length+2*(int)height/19.8+tubes_gap,down_tube2.x_position,height*4/5-2*(int)height/19.8-upper_tube2.length-tubes_gap ,(int)width/14), 0) )
			return 1;
	}
//7. checking hits with upper_tip2.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube2.length,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube2.length,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube2.length,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube2.length,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube2.length,down_tip2.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
	}
//8. checking hits with upper_tube2.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(0,upper_tube2.x_position,upper_tube2.length ,(int)width/14), 0) ) //general check.
	{
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(0,upper_tube2.x_position,upper_tube2.length ,(int)width/14), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(0,upper_tube2.x_position,upper_tube2.length ,(int)width/14), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(0,upper_tube2.x_position,upper_tube2.length ,(int)width/14), 0) )
			return 1;
	}
//9. checking hits with down_tip3.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube3.length+(int)height/19.8+tubes_gap,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube3.length+(int)height/19.8+tubes_gap,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube3.length+(int)height/19.8+tubes_gap,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube3.length+(int)height/19.8+tubes_gap,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube3.length+(int)height/19.8+tubes_gap,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
	}
//10. checking hits with down_tube3.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube3.length+2*(int)height/19.8+tubes_gap,down_tube3.x_position,height*4/5-2*(int)height/19.8-upper_tube3.length-tubes_gap ,(int)width/14), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube3.length+2*(int)height/19.8+tubes_gap,down_tube3.x_position,height*4/5-2*(int)height/19.8-upper_tube3.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube3.length+2*(int)height/19.8+tubes_gap,down_tube3.x_position,height*4/5-2*(int)height/19.8-upper_tube3.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube3.length+2*(int)height/19.8+tubes_gap,down_tube3.x_position,height*4/5-2*(int)height/19.8-upper_tube3.length-tubes_gap ,(int)width/14), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube3.length+2*(int)height/19.8+tubes_gap,down_tube3.x_position,height*4/5-2*(int)height/19.8-upper_tube3.length-tubes_gap ,(int)width/14), 0) )
			return 1;
	}
//11. checking hits with upper_tip3.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
	}
//11. checking hits with upper_tip3.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(upper_tube3.length,down_tip3.x_position,(int)height/19.8 ,(int)width/14+20), 0) )
			return 1;
	}
//12. checking hits with upper_tube3.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(0,upper_tube3.x_position,upper_tube3.length ,(int)width/14), 0) ) //general check.
	{
//square 1
		if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect(0,upper_tube3.x_position,upper_tube3.length ,(int)width/14), 0) )
			return 1;
//square 2
		if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect(0,upper_tube3.x_position,upper_tube3.length ,(int)width/14), 0) )
			return 1;
//square 3
		if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect(0,upper_tube3.x_position,upper_tube3.length ,(int)width/14), 0) )
			return 1;
//square 4
		if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect(0,upper_tube3.x_position,upper_tube3.length ,(int)width/14), 0) )
			return 1;
	}
// checking hits above upper_tube1
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(-4000,upper_tube.x_position,4000 ,1),0 ))
		return 1;
// checking hits above upper_tube2
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(-4000,upper_tube2.x_position,4000 ,1),0 ))
		return 1;
// checking hits above upper_tube2
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(-4000,upper_tube3.x_position,4000 ,1),0 ))
		return 1;

	if (coin_flag==1 && trans_flag==0)
	{
		if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(coin.y_position,coin.x_position,25,25), 0) ) //general check.
		{
			coin_hit_counter++;
			SetCtrlVal (panelHandle, PANEL_INVISIBLE, coin_hit_counter);
			coin_flag=0;
		}
	}
	return 0;
}



void is_key_pressed()
{
	static int jump=0,leave_game=0;
	if (Keys_Down[32]==1 && jump==0)
	{
		jump=1;
		dragon.vy=-(double)G/4;
		BASS_ChannelPlay(Wing,TRUE);
	}
	if (!Keys_Down[32]) jump=0;

	if (Keys_Down[27]==1 && leave_game==0)
	{
		leave_game=1;
		SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
		SetCtrlAttribute (panelHandle, PANEL_BONUS_TIMER, ATTR_ENABLED, 0);
		HidePanel (panelHandle);
		DisplayPanel (panelHandle2);
	}
	if (!Keys_Down[27]) leave_game=0;
}

void Dragon_Hit()
{
	BASS_ChannelPlay(Hit,TRUE);
	sprintf (msg, "Your Score is %d", score);
	MessagePopup ("Game Over", msg);
	Update_Score_List();
	SetCtrlAttribute (panelHandle, PANEL_BONUS_TIMER, ATTR_ENABLED, 0);
	SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
	dragon.vy=0;
	coin_hit_counter=0;
	dragon_pic_counter=0;
	score=0;
	trans_pic_counter=0;
	fire_ball.pic_counter=0;
	fire_ball.calc_fire=0;
	enemy.hit_pic_counter=0;
	trans_flag=0;
	coin4_flag=1;
	enemy.vx_increase=1;
	SetCtrlVal (panelHandle, PANEL_SCORE, score);
	SetCtrlVal (panelHandle, PANEL_INVISIBLE, coin_hit_counter);
	dragon.x_position=width/5;
	dragon.y_position=height*3/5;
	ground_x_position=0;
	upper_tube.x_position=width*2;
	down_tube.x_position=upper_tube.x_position;
	upper_tube2.x_position=width*2+250;
	down_tube2.x_position=upper_tube2.x_position;
	upper_tube3.x_position=width*2+500;
	down_tube3.x_position=upper_tube3.x_position;
	upper_tip.x_position=width*2-((int)width/14+5)/5;
	down_tip.x_position=upper_tip.x_position;
	upper_tip2.x_position=width*2+250-((int)width/14+5)/5;
	down_tip2.x_position=upper_tip2.x_position;
	upper_tip3.x_position=width*2+500 -((int)width/14+5)/5;
	down_tip3.x_position=upper_tip3.x_position;
	coin.x_position=width*2-((int)width/14+5)/5+150;
	coin.y_position=(upper_tube2.length-(int)height/19.8)+rand()%(80 +tubes_gap);
	enemy.x_position=width+enemy.radius*2;
	enemy.y_position=enemy.radius+rand()%(height*4/5-2*enemy.radius);
	coin_flag=1;
	bonus_numeric=2500;
	enemy.vx=200;
	dragon.vy=-G/4;
	BASS_ChannelPlay(Wing,TRUE);
	BASS_StreamFree(Wing);
	BASS_StreamFree(Hit);
	BASS_StreamFree(Points);
	BASS_StreamFree(Invisible);
	BASS_StreamFree(Bonus);
	BASS_StreamFree(Fire_Ball);
	BASS_StreamFree(Enemy_Hit);
	Hit=BASS_StreamCreateFile(FALSE,"hit_sound.mp3",0,0,0);
	Wing=BASS_StreamCreateFile(FALSE,"d_wings_sound.mp3",0,0,0);
	Points=BASS_StreamCreateFile(FALSE,"point_sound.mp3",0,0,0);
	Invisible=BASS_StreamCreateFile(FALSE,"invisible_sound.mp3",0,0,0);
	Bonus=BASS_StreamCreateFile(FALSE,"bonus_sound.mp3",0,0,0);
	Fire_Ball=BASS_StreamCreateFile(FALSE,"fire_ball_sound.mp3",0,0,0);
	Enemy_Hit=BASS_StreamCreateFile(FALSE,"enemy_hit_sound.mp3",0,0,0);
}

int CVICALLBACK timerFunc (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int status;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			update();
			status=Check_Hits();
			draw();
			if(status==1 && trans_flag==0)
				Dragon_Hit();
			is_key_pressed();
			break;
	}
	return 0;
}

int CVICALLBACK BonusTimerFunc (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int bonus_status;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			update_bonus();
			bonus_status=Bonus_Check_Hits();
			draw_bonus();
			if (bonus_status)
				Dragon_Hit();
			is_key_pressed_bonus();
			break;
	}
	return 0;
}

void update_bonus()
{
	dragon_pic_counter++;
	enemy_pic_counter++;
	bonus_numeric--;

	if(bonus_numeric==0)
		Finish_Bonus();

	SetCtrlVal (panelHandle, PANEL_NUMERICSLIDE_2,bonus_numeric );
	if (fire_ball.calc_fire==1)
		fire_ball.pic_counter++;

	if (fire_ball.life==1)
		enemy.hit_pic_counter++;

	ground_x_position=ground_x_position-2;
	if (ground_x_position<=-width)
		ground_x_position=0;

	if (enemy.x_position<=-enemy.radius*2)
	{
		enemy.vx_increase=enemy.vx_increase+0.1;
		enemy.vx=enemy.vx_increase*200;
		enemy.y_position=enemy.radius+rand()%(height*4/5-2*enemy.radius);
		enemy.x_position=width+enemy.radius*2;

	}

	dragon.y_position=dragon.y_position+dragon.vy*dt+(G)*(pow(dt,2))/2;
	dragon.vy=dragon.vy+G*dt;

	enemy.x_position=enemy.x_position-enemy.vx*dt;

	if (fire_ball.calc_fire==1)
		fire_ball.x_position=fire_ball.x_position+fire_ball.vx*dt;

//Changing dragon frames.
	if(dragon_pic_counter==15)
	{
		dragon_left=0;
		square4_up=0;
		square4_left=0;
	}
	if(dragon_pic_counter==30)
	{
		dragon_left=187;
		square4_up=9;
		square4_left=0;
	}
	if(dragon_pic_counter==45)
	{
		dragon_left=382;
		square4_up=11;
		square4_left=7;
		dragon_pic_counter=0;
	}

// Changing enemy frames.
	if(enemy_pic_counter==5)
	{
		enemy_left=0;
		enemy.width=82;
	}

	if(enemy_pic_counter==10)
	{
		enemy_left=102;
		enemy.width=110;
	}

	if(enemy_pic_counter==15)
	{
		enemy_left=232;
		enemy.width=75;
	}

	if(enemy_pic_counter==20)
	{
		enemy_left=315;
		enemy.width=115;
	}

	if(enemy_pic_counter==35)
	{
		enemy_left=438;
		enemy.width=88;
		enemy_pic_counter=0;
	}

//Changing fire ball frames.
	if(fire_ball.pic_counter==5)
	{
		fire_ball.left=0;
		fire_ball.width=100;
	}

	if(fire_ball.pic_counter==10)
	{
		fire_ball.left=124;
		fire_ball.width=116;
	}

	if(fire_ball.pic_counter==15)
	{
		fire_ball.left=259;
		fire_ball.width=105;
	}

	if(fire_ball.pic_counter==20)
	{
		fire_ball.left=390;
		fire_ball.width=110;
	}

	if(fire_ball.pic_counter==35)
	{
		fire_ball.left=520;
		fire_ball.width=105;
	}
	if(fire_ball.pic_counter==50)
	{
		fire_ball.left=659;
		fire_ball.width=100;
		fire_ball.pic_counter=0;
	}

// changing enemy hit frames.
	if(enemy.hit_pic_counter==5)
	{
		enemy.hit_left=0;
		enemy.hit_width=90;
	}

	if(enemy.hit_pic_counter==15)
	{
		enemy.hit_left=105;
		enemy.hit_width=95;
	}

	if(enemy.hit_pic_counter==25)
	{
		enemy.hit_left=211;
		enemy.width=90;
	}

	if(enemy.hit_pic_counter==35)
	{
		enemy.hit_left=305;
		enemy.hit_width=110;
	}

	if(enemy.hit_pic_counter==45)
	{
		enemy.hit_left=450;
		enemy.hit_width=26;
		enemy.hit_pic_counter=0;
		fire_ball.life=0;
		fire_ball.x_position=dragon.x_position;
		fire_ball.y_position=dragon.y_position;
		enemy.x_position=width+enemy.radius*2;
		enemy.y_position=enemy.radius+rand()%(height*4/5-2*enemy.radius);
		enemy.vx_increase=enemy.vx_increase+0.1;
		enemy.vx=enemy.vx_increase*200;
	}
}

void draw_bonus()
{
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
	CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, background_night, VAL_ENTIRE_OBJECT, MakeRect(0,0, height,width));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, ground, VAL_ENTIRE_OBJECT, MakeRect(height-height/5,ground_x_position, height/5,width*2));
	if (fire_ball.life==1)
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, enemy.hit_pic, MakeRect(0,enemy.hit_left,117,enemy.hit_width),MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2) );
	else
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, enemy.pic, MakeRect(0,enemy_left,148,enemy.width), MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2));
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, dragon.pic1, MakeRect(0,dragon_left,122,151), MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2));
	if (fire_ball.calc_fire==1&& fire_ball.life==0)
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, fire_ball.pic, MakeRect(0,fire_ball.left,61,fire_ball.width),MakeRect((int)fire_ball.y_position-fire_ball.radius,(int)fire_ball.x_position-fire_ball.radius,fire_ball.radius*2,fire_ball.radius*2) );
	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
}

int Bonus_Check_Hits()
{
// checking hits with the ground.
	if( RectIntersection (MakeRect((int)dragon.y_position-dragon.radius,(int)dragon.x_position-dragon.radius,dragon.radius*2,dragon.radius*2),MakeRect(height-height/5,ground_x_position, height/5,width*2), 0) )
	{
		dragon.y_position=height-height/5-dragon.radius;
		return 1;
	}

// checking hits between dragon and enemy.
	if( RectIntersection (MakeRect (dragon.y_position-1, dragon.x_position+8, 17, 9),MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2), 0) )
		return 1;
	if( RectIntersection (MakeRect (dragon.y_position+6, dragon.x_position-7, 8, 14),MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2), 0) )
		return 1;
	if( RectIntersection (MakeRect (dragon.y_position+8, dragon.x_position-17, 5, 12),MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2), 0) )
		return 1;
	if( RectIntersection (MakeRect (dragon.y_position-17+square4_up, dragon.x_position-11+square4_left, 5, 24),MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2), 0) )
		return 1;

// checking hits between fire ball and enemy.
	if( RectIntersection (MakeRect((int)fire_ball.y_position-fire_ball.radius,(int)fire_ball.x_position-fire_ball.radius,fire_ball.radius*2,fire_ball.radius*2), MakeRect((int)enemy.y_position-enemy.radius,(int)enemy.x_position-enemy.radius,enemy.radius*2,enemy.radius*2), 0) )
	{
		BASS_ChannelPlay(Enemy_Hit,TRUE);
		fire_ball.life=1;
		enemy.vx=0;
		fire_ball.calc_fire=0;
		fire_ball.x_position=dragon.x_position;
		fire_ball.y_position=dragon.y_position;
		score=score+2;
		SetCtrlVal (panelHandle, PANEL_SCORE, score);
		return 0;
	}

	else
		return 0;

}

void is_key_pressed_bonus()
{
	static int jump=0,leave_game=0;
	if (Keys_Down[32]==1 && jump==0)
	{
		jump=1;
		dragon.vy=-(double)G/4;
		BASS_ChannelPlay(Wing,TRUE);
	}
	if (!Keys_Down[32]) jump=0;

	if (Keys_Down[65]==1 && fire_key==0 && fire_ball.life==0)
	{
		BASS_ChannelPlay(Fire_Ball,TRUE);
		fire_key=1;
		fire_ball.calc_fire=1;
		fire_ball.vx=200;
		fire_ball.x_position=dragon.x_position;
		fire_ball.y_position=dragon.y_position;
	}
	if (!Keys_Down[65]) fire_key=0;

	if (Keys_Down[27]==1 && leave_game==0)
	{
		leave_game=1;
		SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
		SetCtrlAttribute (panelHandle, PANEL_BONUS_TIMER, ATTR_ENABLED, 0);
		HidePanel (panelHandle);
		DisplayPanel (panelHandle2);
	}
	if (!Keys_Down[27]) leave_game=0;
}

void Finish_Bonus()
{
	SetCtrlAttribute (panelHandle, PANEL_BONUS_TIMER, ATTR_ENABLED, 0);
	SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
	coin_hit_counter=0;
	dragon_pic_counter=0;
	trans_pic_counter=0;
	fire_ball.pic_counter=0;
	fire_ball.calc_fire=0;
	enemy.hit_pic_counter=0;
	bonus_numeric=2500;
	SetCtrlVal (panelHandle, PANEL_NUMERICSLIDE_2, 0);
	SetCtrlVal (panelHandle, PANEL_INVISIBLE, 0);
	trans_flag=0;
	coin4_flag=1;
	enemy.vx_increase=1;
	enemy.vx=200;
	ground_x_position=0;
	upper_tube.x_position=width*2;
	down_tube.x_position=upper_tube.x_position;
	upper_tube2.x_position=width*2+250;
	down_tube2.x_position=upper_tube2.x_position;
	upper_tube3.x_position=width*2+500;
	down_tube3.x_position=upper_tube3.x_position;
	upper_tip.x_position=width*2-((int)width/14+5)/5;
	down_tip.x_position=upper_tip.x_position;
	upper_tip2.x_position=width*2+250-((int)width/14+5)/5;
	down_tip2.x_position=upper_tip2.x_position;
	upper_tip3.x_position=width*2+500 -((int)width/14+5)/5;
	down_tip3.x_position=upper_tip3.x_position;
	coin.x_position=width*2-((int)width/14+5)/5+150;
	coin.y_position=(upper_tube2.length-(int)height/19.8)+rand()%(80 +tubes_gap);
	enemy.x_position=width+enemy.radius*2;
	enemy.y_position=enemy.radius+rand()%(height*4/5-2*enemy.radius);
	coin_flag=1;
}

void Update_Score_List()
{
	int temp_score=0;
	char temp_name[20];
	fp = fopen ("Score_List.CSV", "r+");
	for(int i=0; i<10; i++)
	{
		if(score>csv_score[i])
		{
			for(int j=i; j<10; j++)
			{
				temp_score=csv_score[j];
				strcpy(temp_name,csv_name[j]);
				csv_score[j]=score;
				strcpy(csv_name[j],player_name);
				score=temp_score;
				strcpy(player_name,temp_name);
			}
		}
	}
	for(int i = 0; i<10; i++)
		fprintf (fp, "%s,%d\n", csv_name[i], csv_score[i]);
	fclose(fp);
}

int CVICALLBACK StartgameFunc (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	player_name[0]=0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle2, PANEL_2_PLAYER_NAME,player_name);
			if(player_name[0]==0)
			{
				MessagePopup ("Alert", "Please enter Player's Name");
				break;
			}
			else
				DefaultCtrl (panelHandle2, PANEL_2_PLAYER_NAME);
			initialize();
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			HidePanel (panelHandle2);
			DisplayPanel (panelHandle);
			break;
	}
	return 0;
}

int CVICALLBACK ScoreListFunc (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayPanel (panelHandle3);
			for(int i=1; i<=10; i++)
			{
				SetTableCellVal (panelHandle3, PANEL_3_SCORE_LIST, MakePoint (1, i), csv_name[i-1]);
				SetTableCellVal (panelHandle3, PANEL_3_SCORE_LIST, MakePoint (2, i), csv_score[i-1]);
			}

			break;
	}
	return 0;
}

void CVICALLBACK aboutFunc (int menuBar, int menuItem, void *callbackData,
							int panel)
{
	DisplayPanel (panelHandle4);
}

void CVICALLBACK helpFunc (int menuBar, int menuItem, void *callbackData,
						   int panel)
{
	OpenDocumentInDefaultViewer ("Game Introduction.pdf",VAL_NO_ZOOM);
}

void CVICALLBACK demoFunc (int menuBar, int menuItem, void *callbackData,
						   int panel)
{
	OpenDocumentInDefaultViewer ("Demo.mp4", VAL_NO_ZOOM);
}
