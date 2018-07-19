#include "vrpn_Tracker.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <cmath>
using namespace std;
float rh,rs,lh,ls,rh_x,lh_x;
bool ld = true,rd = true, clap_flag = false, paintOpen = true;
int claps;
double xpaint;
double ypaint;


void VRPN_CALLBACK handle_tracker( void* userData, const vrpn_TRACKERCB b )
{
	if(b.sensor == 11){
		rh = b.pos[1];
		rh_x = b.pos[0];
	xpaint = rh_x * 1920 * 2;
	ypaint =((rh-0.5) *(-1)) * 1080 * 2;
	}else if(b.sensor == 8){
		rs = b.pos[1];
	}
	if(rh > rs){
		if(rd){
			cout << "Right Arm Raised" << endl;
			rd = false;
		}
	}else{
		if(!rd){
			cout << "Right Arm Down" << endl;
			rd = true;
		}
	}
	if(b.sensor == 7){
		lh = b.pos[1];
		lh_x = b.pos[0];
	}else if(b.sensor == 4){
		ls = b.pos[1];
	}
	if(lh > ls){
		if(ld){
			cout << "Left Arm Raised" << endl;
			ld = false;
		}
	}else{
		if(!ld){
			cout << "Left Arm Down" << endl;
			ld = true;
		}
	}

	if(lh > ls && paintOpen == true){
		SetCursorPos(xpaint, ypaint);
		mouse_event(MOUSEEVENTF_LEFTDOWN, ypaint, xpaint, 0, 0);
	}

	float dist_x = abs(rh_x-lh_x);
	float dist_y = abs(rh-lh);

	if(dist_x < 0.01 && dist_y < 0.1 && !clap_flag){
		cout << "Clap " << endl;
		if(paintOpen == false){
			WinExec("mspaint.exe", SW_SHOWNORMAL);
			paintOpen = true;
			SetCursorPos(xpaint, ypaint);
		} else {
			WinExec("TASKKILL /F /T /IM mspaint.exe", SW_HIDE);
			paintOpen = false;
		}
		clap_flag = true;
	} 

	if(dist_x > 0.2){
		clap_flag = false;
	}
}

int main(int argc, char* argv[])
{

	vrpn_Tracker_Remote* vrpnTracker = new vrpn_Tracker_Remote("Tracker0@10.4.129.56:3883");//Changer en local

	vrpnTracker->register_change_handler(0, handle_tracker);
	while(1)
	{
		vrpnTracker->mainloop();
		Sleep(100);
	}

	return 0;
}
