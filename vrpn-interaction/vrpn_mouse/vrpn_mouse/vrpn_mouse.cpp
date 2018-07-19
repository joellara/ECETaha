#include "vrpn_Analog.h"
#include "vrpn_Button.h"
#include "vrpn_Tracker.h"

#include <iostream>
using namespace std;
bool active = true; //Flag to detect if the client should be active or not
int zone;

void VRPN_CALLBACK handle_analog( void* userData, const vrpn_ANALOGCB a )
{
	int nbChannels = a.num_channel;
	float x = a.channel[0],y=a.channel[1]; //First channel is 'x' and the second one is 'y'
	if(!active){
		return;
	}
	if(x <= 0.5 && y <= 0.5){ //Compare if it's in Zone A
		if(zone != 0){
			cout << '\a';
			cout << "Zone A" << endl;
			zone = 0;
		}
	}else if(x <= 0.5 && y > 0.5){ //Compare if it's in Zone C
		if(zone != 1){
			cout << '\a';
			zone = 1;
			cout << "Zone C" << endl;
		}
	}else if(x > 0.5 && y <= 0.5){ //Compare if it's in Zone B
		if(zone != 2){
			cout << '\a';
			cout << "Zone B" << endl;
			zone = 2;
		}
	}else{ //Otherwise it's in Zone D
		if(zone != 3){
			cout << '\a';
			zone = 3;
			cout << "Zone D" << endl;
		}
	}
}

void VRPN_CALLBACK handle_button( void* userData, const vrpn_BUTTONCB b )
{
	if(b.button == 0 && b.state == 1){ //Left button is button 0 in vrpn, if the state is 1 then it was clicked
		active = false;
	}
	if(b.button == 2 && b.state == 1){ //Right button is button 2 in vrp, if the state is 1 then it was clicked
		cout << '\a';
		active = true;
	}
}
int main(int argc, char* argv[])
{
	vrpn_Analog_Remote* vrpnAnalog = new vrpn_Analog_Remote("Mouse0@localhost:3883");
	vrpn_Button_Remote* vrpnButton = new vrpn_Button_Remote("Mouse0@localhost:3883");

	vrpnAnalog->register_change_handler( 0, handle_analog );
	vrpnButton->register_change_handler( 0, handle_button );
	while(1)
	{
		vrpnAnalog->mainloop();
		vrpnButton->mainloop();
		Sleep(100);
	}

	return 0;
}
