#include "Platform/Platform.hpp"
#include "nn_gui_setup.h"

#include "layer.h"
#include "net_helper.h"
#include "nn_display.h"
#include "nnet.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	// ---------------------------------- Initialize Window Objs --------------------------------------

	// ---------------------------------- Initialize Neural Network Objs --------------------------------------

	// ---------------------------------- Initialize Graphics Objs --------------------------------------
	// Create our net and give it to our nehlper
	NNet* net = new NNet();
	Net_Helper* net_helper = new Net_Helper(net);
	net_helper->setup_test_net(0);



	// Object for handling initial menu and network setup loops
	NN_gui_setup* setup = new NN_gui_setup(net_helper);
	//NN_Display* display = new NN_Display(net_helper);
	// Object for displaying loaded network

	//net_display->net->print_network();


	//helper->display_title();
	//setup->display_int_setup();
	setup->display_title();
	//display->display_net();
	cout << "DONE!";

	return 0;
}
