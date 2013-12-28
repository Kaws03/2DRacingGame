/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/


#include <windows.h>
#include <iostream>
#include <hge.h>
#include <hgeresource.h>
#include <hgesprite.h>
#include <hgeanim.h>
#include <hgefont.h>
#include <hgeparticle.h>
#include <hgegui.h>
#include <hgeVector.h>
#include <math.h>

HGE *hge = 0;

HTEXTURE tex;
hgeFont	*fnt;
float dT;
double SpeedCoof=300;
double StartAngle;
int Width=800, Hight=600;
bool windowed=false;
float StartTimer=3;
float FinishTimer=3;
bool Start=false, StartFlag=true;
bool Finish=false;

float Distance(double x1, double y1, double x2, double y2){
	float buf=pow((x2-x1), 2)+pow((y2-y1),2);
	return sqrt(buf);
}

class RoadScheme{
public:
	hgeVector Location;
	int Arr[68][286];
	HTEXTURE tex, BridgeTex;
	hgeSprite *spr, *BridgeSpr;
}Highway;

class Car{
public:
	void Accelerate(int ReverseFlag=0){
		if(Direction.Angle()!=StartAngle&&ReverseFlag==0){
			Direction.Rotate(-3.1416);
			Speed.Rotate(-3.1416);
		}
		if(abs(Speed.Length())==0){
			Speed.Clamp(0.1);
		}
		if(abs(Speed.Length())<0.2){
			Speed*=1.5;
		}
		if(abs(Speed.Length())<1&&abs(Speed.Length())>0.2){
			Speed*=1.01;
		}
		if(abs(Speed.Length())>1){
			Speed*=3/(Speed.Length()*Speed.Length());
		}
	}
	void Reverse(){
		if(Direction.Angle()==StartAngle){
			Direction.Rotate(3.1416);
			Speed.Rotate(3.1416);
		}
		Accelerate(1);
	}
	void Bridge(){
		if(Highway.Arr[(int)(RealLocation.y/32.5)][(int)(RealLocation.x/17.486)]==4)
			OnBridge=true;
		if(Highway.Arr[(int)(RealLocation.y/32.5)][(int)(RealLocation.x/17.486)]==5)
			OnBridge=false;
	}
	void Break(){
		Speed*=0.995;
	}
	void Slow(){
		Speed*=0.9995;
	}
	void TurnRight(int ReverseFlag=0){
		if(Direction.Angle()!=StartAngle&&ReverseFlag==0)
			TurnLeft(1);
		else{
			if(abs(Speed.Length())<=1.3)
				Speed.Rotate(3*Speed.Length()*dT);
			if(abs(Speed.Length())>1.3)
				Speed.Rotate(4*dT/Speed.Length());
			if(Speed.Length()>=1&&DriftAngle<0.8){
				DriftAngle+=1*dT;
			}
		}
	}
	void TurnLeft(int ReverseFlag=0){
		if(Direction.Angle()!=StartAngle&&ReverseFlag==0)
			TurnRight(1);
		else{
			if(abs(Speed.Length())<=1.3)
				Speed.Rotate(-3*Speed.Length()*dT);
			if(abs(Speed.Length())>1.3)
				Speed.Rotate(-4*dT/Speed.Length());
			if(Speed.Length()>=1&&DriftAngle>-0.8){
				DriftAngle-=1*dT;
			}
		}
	}
	void Bump(int BotFlag=0){
		Front.Rotate(-Front.Angle());
		Rear.Rotate(-Rear.Angle());
		FrontLeft.Rotate(-FrontLeft.Angle());
		FrontRight.Rotate(-FrontRight.Angle());
		RearLeft.Rotate(-RearLeft.Angle());
		RearRight.Rotate(-RearRight.Angle());

		Front.Rotate(Speed.Angle()+DriftAngle);
		Rear.Rotate(Speed.Angle()+DriftAngle);
		FrontLeft.Rotate(Speed.Angle()+DriftAngle);
		FrontRight.Rotate(Speed.Angle()+DriftAngle);
		RearLeft.Rotate(Speed.Angle()+DriftAngle);
		RearRight.Rotate(Speed.Angle()+DriftAngle);
		
		Player.Front.Rotate(3.1416);
		Player.FrontLeft.Rotate(2.6529);
		Player.FrontRight.Rotate(-2.6529);
		Player.RearLeft.Rotate(0.4887);
		Player.RearRight.Rotate(-0.4887);
		if(OnBridge==false){
			if(BotFlag==0){
				if((Highway.Arr[(int)((Front.y+RealLocation.y)/32.5)][(int)((Front.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((Front.y+RealLocation.y)/32.5)][(int)((Front.x+RealLocation.x)/17.486)]==3)){
					Reverse();
					Speed*=0.3;
				}
			}
			if((Highway.Arr[(int)((FrontLeft.y+RealLocation.y)/32.5)][(int)((FrontLeft.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((FrontLeft.y+RealLocation.y)/32.5)][(int)((FrontLeft.x+RealLocation.x)/17.486)]==3)){
				Speed.Rotate(0.05);
			}
			if((Highway.Arr[(int)((FrontRight.y+RealLocation.y)/32.5)][(int)((FrontRight.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((FrontRight.y+RealLocation.y)/32.5)][(int)((FrontRight.x+RealLocation.x)/17.486)]==3)){
				Speed.Rotate(-0.05);
			}
			if((Highway.Arr[(int)((RearLeft.y+RealLocation.y)/32.5)][(int)((RearLeft.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((RearLeft.y+RealLocation.y)/32.5)][(int)((RearLeft.x+RealLocation.x)/17.486)]==3)){
				Speed.Rotate(-0.001);
			}
			if((Highway.Arr[(int)((RearRight.y+RealLocation.y)/32.5)][(int)((RearRight.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((RearRight.y+RealLocation.y)/32.5)][(int)((RearRight.x+RealLocation.x)/17.486)]==3)){
				Speed.Rotate(0.001);
			}
		}
		if(OnBridge==true){
			if((Highway.Arr[(int)((FrontLeft.y+RealLocation.y)/32.5)][(int)((FrontLeft.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((FrontLeft.y+RealLocation.y)/32.5)][(int)((FrontLeft.x+RealLocation.x)/17.486)]==2)){
				Speed.Rotate(0.05);
			}
			if((Highway.Arr[(int)((FrontRight.y+RealLocation.y)/32.5)][(int)((FrontRight.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((FrontRight.y+RealLocation.y)/32.5)][(int)((FrontRight.x+RealLocation.x)/17.486)]==2)){
				Speed.Rotate(-0.05);
			}
			if((Highway.Arr[(int)((RearLeft.y+RealLocation.y)/32.5)][(int)((RearLeft.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((RearLeft.y+RealLocation.y)/32.5)][(int)((RearLeft.x+RealLocation.x)/17.486)]==2)){
				Speed.Rotate(-0.001);
			}
			if((Highway.Arr[(int)((RearRight.y+RealLocation.y)/32.5)][(int)((RearRight.x+RealLocation.x)/17.486)]==1)||(Highway.Arr[(int)((RearRight.y+RealLocation.y)/32.5)][(int)((RearRight.x+RealLocation.x)/17.486)]==2)){
				Speed.Rotate(0.001);
			}
		}
	}
		void LapCounter(){
			if(OnStart==false){
				if(Highway.Arr[(int)(RealLocation.y/32.5)][(int)(RealLocation.x/17.486)]==6)
					Laps++;
				OnStart=true;
			}
			if(OnStart==true){
				if(Highway.Arr[(int)(RealLocation.y/32.5)][(int)(RealLocation.x/17.486)]==0)
					OnStart=false;
			}
		}
	double DriftAngle;
	bool OnBridge;
	int Laps;
	bool OnStart;
	hgeVector Location, RealLocation, Direction, Speed;
	hgeVector Front, Rear, FrontRight, FrontLeft, RearRight, RearLeft;
	HTEXTURE tex;
	hgeSprite *spr;
	hgeSprite *vecs;
	
	}Player;

class Bot:public Car{
public:
	void Scan(){
		ScanLeft.x=1;
		ScanLeft.y=1;
		ScanRight.x=1;
		ScanRight.y=1;
		ScanLeft*=0.1;
		ScanRight*=0.1;
		ScanLeft.Rotate(-ScanLeft.Angle());
		ScanRight.Rotate(-ScanLeft.Angle());
		ScanLeft.Rotate((Speed.Angle()+2.3));
		ScanRight.Rotate((Speed.Angle()-2.5));
		if(OnBridge==true){
			for(int i=0;;i++){
				if((Highway.Arr[(int)ScanRight.y+(int)(RealLocation.y/32.5)][(int)ScanRight.x+(int)(RealLocation.x/17.486)]==1)||(Highway.Arr[(int)ScanRight.y+(int)(RealLocation.y/32.5)][(int)ScanRight.x+(int)(RealLocation.x/17.486)]==2))
					break;
				ScanRight*=1.001;
			}
			for(int i=0;;i++){
				if((Highway.Arr[(int)ScanLeft.y+(int)(RealLocation.y/32.5)][(int)ScanLeft.x+(int)(RealLocation.x/17.486)]==1)||(Highway.Arr[(int)ScanLeft.y+(int)(RealLocation.y/32.5)][(int)ScanLeft.x+(int)(RealLocation.x/17.486)]==2))
					break;
				ScanLeft*=1.001;
			}
		}

		if(OnBridge==false){
			for(int i=0;;i++){
				if((Highway.Arr[(int)ScanRight.y+(int)(RealLocation.y/32.5)][(int)ScanRight.x+(int)(RealLocation.x/17.486)]==1)||(Highway.Arr[(int)ScanRight.y+(int)(RealLocation.y/32.5)][(int)ScanRight.x+(int)(RealLocation.x/17.486)]==3))
					break;
				ScanRight*=1.001;
			}
			for(int i=0;;i++){
				if((Highway.Arr[(int)ScanLeft.y+(int)(RealLocation.y/32.5)][(int)ScanLeft.x+(int)(RealLocation.x/17.486)]==1)||(Highway.Arr[(int)ScanLeft.y+(int)(RealLocation.y/32.5)][(int)ScanLeft.x+(int)(RealLocation.x/17.486)]==3))
					break;
				ScanLeft*=1.001;
			}
		}
	
		if(abs(ScanRight.Length()-ScanLeft.Length())>1){
			if(ScanRight.Length()>ScanLeft.Length())
				this->TurnRight();
			if(ScanRight.Length()<ScanLeft.Length())
				this->TurnLeft();
		}
		else{
			if(DriftAngle>0){
				DriftAngle-=1*dT;
					if(Player.DriftAngle<0.001)
						Player.DriftAngle=0;
			}
			if(DriftAngle<0){
				DriftAngle+=1*dT;
					if(Player.DriftAngle>0.001)
						Player.DriftAngle=0;
			}
		}
	}
	
	void CarBump(){
		float Dist=70;

		if(Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x, Player.RealLocation.y)<Dist){
			if(Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.RearLeft.x, Player.RealLocation.y+Player.RearLeft.x)>Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.RearRight.x, Player.RealLocation.y+Player.RearRight.x)){//Я слева
				Player.Speed.Rotate(-0.005);
				Speed.Rotate(0.005);
			}
			if(Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.RearLeft.x, Player.RealLocation.y+Player.RearLeft.x)<Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.RearRight.x, Player.RealLocation.y+Player.RearRight.x)){//Я справа
				Player.Speed.Rotate(0.005);
				Speed.Rotate(-0.005);
			}
			if(Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.RearLeft.x, Player.RealLocation.y+Player.RearLeft.x)==Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.RearRight.x, Player.RealLocation.y+Player.RearRight.x)){
				if(Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.Rear.x, Player.RealLocation.y+Player.Rear.x)>Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.Front.x, Player.RealLocation.y+Player.Front.x)){
					Player.Break();
				}
				if(Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.Rear.x, Player.RealLocation.y+Player.Rear.x)>Distance(RealLocation.x, RealLocation.y, Player.RealLocation.x+Player.Front.x, Player.RealLocation.y+Player.Front.x)){
					Break();
				}
			}
		}
	}


	hgeVector ScanLeft, ScanRight;
	hgeSprite *VecLeft;
	hgeSprite *VecRight;

}Opponent;


bool FrameFunc()
{	
	dT=hge->Timer_GetDelta();
	if(Start==true){
		StartTimer-=dT;
		if(StartTimer<0)
			Start=false;
	}
	if(Finish==true){
		FinishTimer-=dT;
		if(FinishTimer<0)
			return true;
	};
	if(Start==false){
	Player.Bump();
	Opponent.Bump(1);
	Player.Bridge();
	Opponent.Bridge();

	Player.LapCounter();
	Opponent.LapCounter();

	Opponent.CarBump();

	if((Player.Speed.Length()<1||(hge->Input_GetKeyState(HGEK_LEFT)==false&&hge->Input_GetKeyState(HGEK_RIGHT)==false)&&abs(Player.DriftAngle>0))){
		Player.DriftAngle-=1*dT;
		if(Player.DriftAngle<0.001)
			Player.DriftAngle=0;
	}
	if((Player.Speed.Length()<1||(hge->Input_GetKeyState(HGEK_LEFT)==false&&hge->Input_GetKeyState(HGEK_RIGHT)==false)&&abs(Player.DriftAngle<0))){
		Player.DriftAngle+=1*dT;
		if(Player.DriftAngle>0.001)
			Player.DriftAngle=0;
	}
	if (hge->Input_GetKeyState(HGEK_ESCAPE)){ 
		return true;
	}
	if (hge->Input_GetKeyState(HGEK_UP)){
		if(Player.Direction.Angle()!=StartAngle&&Player.Speed.Length()>0.1)
			Player.Break();
		else{
			if(Player.Speed.Length()<0)
				Player.Break();
			if(Player.Speed.Length()>=0)
				Player.Accelerate();
			if(Player.Direction.Angle()!=StartAngle)
				Player.Break();
		}
	}
	if (hge->Input_GetKeyState(HGEK_DOWN)){
		if(Player.Speed.Length()>0)
			Player.Break();
		if(Player.Speed.Length()<=0.1||Player.Direction.Angle()!=StartAngle)
			Player.Reverse();
	}
	if (hge->Input_GetKeyState(HGEK_RIGHT))
		Player.TurnRight();
	if (hge->Input_GetKeyState(HGEK_LEFT))
		Player.TurnLeft();

	///
	Opponent.Accelerate();
	Opponent.Scan();

	Player.Slow();
	Opponent.Slow();
	

	Highway.Location.x-=Player.Speed.x*dT*SpeedCoof;
	Highway.Location.y-=Player.Speed.y*dT*SpeedCoof;
	Player.RealLocation.x=Highway.Location.x+(Width/2);
	Player.RealLocation.y=Highway.Location.y+(Hight/2);
	Highway.spr->SetHotSpot(Highway.Location.x, Highway.Location.y);
	Highway.BridgeSpr->SetHotSpot(Highway.Location.x-1800, Highway.Location.y-1042);
	Opponent.Location.x-=(Opponent.Speed.x*dT*SpeedCoof)-(Player.Speed.x*dT*SpeedCoof);
	Opponent.Location.y-=(Opponent.Speed.y*dT*SpeedCoof)-(Player.Speed.y*dT*SpeedCoof);
	Opponent.RealLocation.x-=Opponent.Speed.x*dT*SpeedCoof;
	Opponent.RealLocation.y-=Opponent.Speed.y*dT*SpeedCoof;
	}
	if(StartFlag==true){
		StartFlag=false;
		Start=true;
	}
	// Continue execution
	return false;
}
bool RenderFunc()
{
	hge->Gfx_BeginScene();
	// Clear screen with black color
	hge->Gfx_Clear(0);

	Highway.spr->Render(0,0);
	
	if(Player.OnBridge==false)
		Player.spr->RenderEx(Player.Location.x, Player.Location.y, Player.Speed.Angle()+(3.9270+Player.Direction.Angle()+Player.DriftAngle), 0.15, 0.15);
	if(Opponent.OnBridge==false)
		Opponent.spr->RenderEx(Opponent.Location.x, Opponent.Location.y, Opponent.Speed.Angle()+(3.9270+Opponent.Direction.Angle()+Opponent.DriftAngle), 0.15, 0.15);

	Highway.BridgeSpr->Render(0,0);

	if(Player.OnBridge==true)
		Player.spr->RenderEx(Player.Location.x, Player.Location.y, Player.Speed.Angle()+(3.9270+Player.Direction.Angle()+Player.DriftAngle), 0.15, 0.15);
	if(Opponent.OnBridge==true)
		Opponent.spr->RenderEx(Opponent.Location.x, Opponent.Location.y, Opponent.Speed.Angle()+(3.9270+Opponent.Direction.Angle()+Opponent.DriftAngle), 0.15, 0.15);

	/*
	Player.vecs->Render(Player.Location.x, Player.Location.y);
	Player.vecs->Render(Opponent.Location.x, Opponent.Location.y);
	
	Player.vecs->Render(Player.Location.x+Player.RearRight.x, Player.Location.y+Player.RearRight.y);
	Player.vecs->Render(Player.Location.x+Player.RearLeft.x, Player.Location.y+Player.RearLeft.y);
	Player.vecs->Render(Player.Location.x+Player.FrontRight.x, Player.Location.y+Player.FrontRight.y);
	Player.vecs->Render(Player.Location.x+Player.FrontLeft.x, Player.Location.y+Player.FrontLeft.y);
	*/
	
	if(Start==true){
		fnt->printf(Width/2, Hight/2, HGETEXT_CENTER, "%d", (int)(StartTimer));
	}
	if(Player.Laps==4){
		fnt->printf(Width/2, Hight/2, HGETEXT_CENTER, "You won!");
		Finish=true;
	}

	if(Opponent.Laps==4){
		fnt->printf(Width/2, Hight/2, HGETEXT_CENTER, "You loose!");
		Finish=true;
	}
	
	// End rendering and update the screen
	hge->Gfx_EndScene();

	// RenderFunc should always return false
	
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Here we use global pointer to HGE interface.
	// Instead you may use hgeCreate() every
	// time you need access to HGE. Just be sure to
	// have a corresponding hge->Release()
	// for each call to hgeCreate()
	hge = hgeCreate(HGE_VERSION);

	// Set our frame function
	hge->System_SetState(HGE_LOGFILE, "Tetris Race.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Tetris Race");


	// Set the window title
	hge->System_SetState(HGE_TITLE, "Tetris Race");
	
	// Run in windowed mode
	// Default window size is 800x600
	hge->System_SetState(HGE_WINDOWED, windowed);
	hge->System_SetState(HGE_SCREENWIDTH, Width);
	hge->System_SetState(HGE_SCREENHEIGHT, Hight);
	hge->System_SetState(HGE_SCREENBPP, 32);

	// Tries to initiate HGE with the states set.
	// If something goes wrong, "false" is returned
	// and more specific description of what have
	// happened can be read with System_GetErrorMessage().
	if(hge->System_Initiate())
	{
		// Starts running FrameFunc().
		// Note that the execution "stops" here
		// until "true" is returned from FrameFunc()
		
		
		Player.tex=hge->Texture_Load("Car.png");
		Player.Location.x=Width/2;
		Player.Location.y=Hight/2;
		Player.Speed.x=-0.0001;
		Player.Speed.y=0;
		Player.Speed.Rotate(0.6);
		Player.Direction.x=1;
		Player.Direction.y=1;
		Player.Direction.Rotate(0);
		Player.DriftAngle=0;
		Player.OnBridge=false;
		Player.spr=new hgeSprite(Player.tex,0,0,385,720);
		Player.spr->SetHotSpot(162,360);
		Player.Rear.x=120;
		Player.Rear.y=120;
		Player.Rear.Clamp(54);
		Player.Front.x=120;
		Player.Front.y=120;
		Player.Front.Clamp(54);
		Player.FrontLeft.x=120;
		Player.FrontLeft.y=120;
		Player.FrontLeft.Clamp(61);
		Player.FrontRight.x=120;
		Player.FrontRight.y=120;
		Player.FrontRight.Clamp(61);
		Player.RearLeft.x=120;
		Player.RearLeft.y=120;
		Player.RearLeft.Clamp(61);
		Player.RearRight.x=120;
		Player.RearRight.y=120;
		Player.RearRight.Clamp(61);
		Player.vecs=new hgeSprite(hge->Texture_Load("Dot.png"), 0, 0, 20, 20);
		Player.vecs->SetHotSpot(10,10);



		Opponent.tex=hge->Texture_Load("Opponent.png");
		Opponent.Location.x=Width/2-110;
		Opponent.Location.y=Hight/2+130;
		Opponent.RealLocation.x=1050;
		Opponent.RealLocation.y=380;
		Opponent.Speed.x=-0.0001;
		Opponent.Speed.y=0;
		Opponent.Speed.Rotate(0.6);
		Opponent.Direction.x=1;
		Opponent.Direction.y=1;
		Opponent.Direction.Rotate(0);
		Opponent.DriftAngle=0;
		Opponent.OnBridge=false;
		Opponent.spr=new hgeSprite(Opponent.tex,0,0,385,720);
		Opponent.spr->SetHotSpot(162,360);
		Opponent.FrontLeft.x=120;
		Opponent.FrontLeft.y=120;
		Opponent.FrontLeft.Clamp(61);
		Opponent.FrontRight.x=120;
		Opponent.FrontRight.y=120;
		Opponent.FrontRight.Clamp(61);
		Opponent.RearLeft.x=120;
		Opponent.RearLeft.y=120;
		Opponent.RearLeft.Clamp(61);
		Opponent.RearRight.x=120;
		Opponent.RearRight.y=120;
		Opponent.RearRight.Clamp(61);

		StartAngle=Player.Direction.Angle();
		
		FILE* Bitmap;
		Bitmap=fopen("Bitmap", "r");
		fread (Highway.Arr, sizeof(Highway.Arr), 1, Bitmap);
		Highway.tex=hge->Texture_Load("Track.png");
		Highway.BridgeTex=hge->Texture_Load("Bridge.png");
		Highway.spr=new hgeSprite(Highway.tex, 0, 0, 5000, 2210);
		Highway.BridgeSpr=new hgeSprite(Highway.BridgeTex, 0, 0, 1488, 684);
		Highway.spr->SetHotSpot(0,0);
		Highway.Location.x=1150-(Width/2);
		Highway.Location.y=250-(Hight/2);

		
		Opponent.VecLeft=new hgeSprite(Highway.tex, 0, 0, 200, 5);
		Opponent.VecRight=new hgeSprite(Highway.tex, 0, 0, 200, 5);
		

		fnt=new hgeFont("font2.fnt");
		fnt->SetScale(20);
		hge->System_Start();
	}
	else
	{	
		// If HGE initialization failed show error message
		
	}

	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free
	// all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface.
	// If there are no more references,
	// the HGE object will be deleted.
	hge->Release();

	return 0;
}
