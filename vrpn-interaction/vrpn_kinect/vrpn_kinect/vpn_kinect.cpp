#include "vrpn_Tracker.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <cmath>
using namespace std;
// rh = right hand, lf = left hand, rs = right shoulder, ls = left shoulder
// defining global constants
const int RH_SENSOR = 11, RS_SENSOR = 8, LH_SENSOR = 7, S_SENSOR = 4;
const float MIN_DISTANCE_X = 0.01,MIN_DISTANCE_Y = 0.1, MIN_DISTANCE_NO_CLAP = 0.2;

//defining the variables to track the movement of the body
float rh_y,rs_y,lh_y,ls_y,rh_x,lh_x;
// flag to track the important positions we need
bool left_down = true,right_down = true, clap_flag = false, paintOpen = true;
// defining the variables to mouse the mouse in the screen and draw
double xpaint, ypaint;


void VRPN_CALLBACK handle_tracker( void* userData, const vrpn_TRACKERCB b )
{
	if(b.sensor == RH_SENSOR){ //Right hand sensor
		rh_y = b.pos[1];
		rh_x = b.pos[0];
		xpaint = rh_x * 1920 * 2;
		ypaint =((rh_y-0.5) *(-1)) * 1080 * 2;
	}else if(b.sensor == RS_SENSOR){ //Right shoulder sensor
		rs_y = b.pos[1];
	}
	if(b.sensor == RH_SENSOR){
		lh_y = b.pos[1];
		lh_x = b.pos[0];
	}else if(b.sensor == RS_SENSOR){
		ls_y = b.pos[1];
	}
	if(rh_y > rs_y){
		if(right_down){
			cout << "Right Arm Raised" << endl;
			right_down = false;
		}
	}else{
		if(!right_down){
			cout << "Right Arm Down" << endl;
			right_down = true;
		}
	}
	if(lh_y > ls_y){
		if(left_down){
			cout << "Left Arm Raised" << endl;
			left_down = false;
		}
	}else{
		if(!left_down){
			cout << "Left Arm Down" << endl;
			if(paintOpen == true){
				mouse_event(MOUSEEVENTF_LEFTUP, ypaint, xpaint, 0, 0);	
			}
			left_down = true;
		}
		
	}

	if(lh_y > ls_y && paintOpen == true){
		SetCursorPos(xpaint, ypaint);
		mouse_event(MOUSEEVENTF_LEFTDOWN, ypaint, xpaint, 0, 0);
	}

	float dist_x = abs(rh_x - lh_x);
	float dist_y = abs(rh_y - lh_y);

	if(dist_x < MIN_DISTANCE_X && dist_y < MIN_DISTANCE_Y && !clap_flag){
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

	if(dist_x > MIN_DISTANCE_NO_CLAP){
		clap_flag = false;
	}
}

int main(int argc, char* argv[])
{

	vrpn_Tracker_Remote * vrpnTracker = new vrpn_Tracker_Remote("Tracker0@10.4.129.56:3883");

	vrpnTracker->register_change_handler(0, handle_tracker);
	while(1)
	{
		vrpnTracker->mainloop();
		Sleep(100);
	}

	return 0;
}
