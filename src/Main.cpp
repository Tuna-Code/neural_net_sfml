#include "utils.h"



using namespace std;

int main()
{


	//cout << "TEST";

	// ---------------------------------- Initialize Window Objs --------------------------------------

	// ---------------------------------- Initialize Neural Network Objs --------------------------------------

	// ---------------------------------- Initialize Graphics Objs --------------------------------------
	// Create our net and give it to our nehlper

	NNet* net = new NNet();

	Net_Helper* net_helper = new Net_Helper(net);

	//boost::filesystem::path targetDir("C:\\CPP_SFML_TOOLS");
	//net_helper->load_from_file();
	//net_helper->net->input_layer->input[0] = -1;
	//net_helper->net->input_layer->input[1] = 4;
	// Object for handling initial menu and network setup loops
	NN_gui_setup* setup = new NN_gui_setup(net_helper);
	NN_Display* display = new NN_Display(net_helper);
	//net_helper->setup_test_net(0);
	//helper->display_title();
	//setup->display_int_setup();
	setup->display_title();
	//display->display_side_menu();


	cout << "\nDone!\n";
	return 0;
}
