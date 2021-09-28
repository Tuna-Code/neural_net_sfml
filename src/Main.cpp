#include "utils.h"

using namespace std;

int main()
{

	// ---------------------------------- Initialize Window Objs --------------------------------------

	// ---------------------------------- Initialize Neural Network Objs --------------------------------------

	// ---------------------------------- Initialize Graphics Objs --------------------------------------
	// Create our net and give it to our nehlper
	NNet* net = new NNet();
	Net_Helper* net_helper = new Net_Helper(net);


	// Object for handling initial menu and network setup loops
	net_helper->load_from_file("data/testnet.top");
	net_helper->load_training_from_file("data/testnet.training");
	//NN_gui_setup* setup = new NN_gui_setup(net_helper);
	NN_Display* display = new NN_Display(net_helper);
	//net_helper->setup_test_net(0);
	//helper->display_title();
	//setup->display_int_setup();
	//setup->display_title();
	//display->display_net();
	display->display_side_menu();
	cout << "DONE!";

	return 0;
}
