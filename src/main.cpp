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
	//net_helper->load_from_file("data/testnet.top");
	//net_helper->load_training_from_file("data/testnet.training");
	net_helper->load_from_file("data/testnet_backprop.top");
	net_helper->load_training_from_file("data/testnet_backprop.training");
	//net->input_layer->input[0] = 0.1;
	//net->input_layer->input[1] = 0.2;
	//net->input_layer->input[2] = 0.7;
	//net_helper->print_network();

	//for(int i = 0; i <100; i++){
	net_helper->net->comp_error_training();
	//net->forward_prop();
	//net->back_prop();

	//}
	net_helper->delete_network();
	//net_helper->print_network();
	//net->cur_training_set = 1;
	//net->load_inputs();
	//net->forward_prop();
	//net_helper->print_network();
	//net_helper->print_network();
	//net_helper->delete_network();
	//cout << net_helper->net->last_layer->prev_layer->gradient_weights[0][0] << endl;
	//net->cur_training_set = 0;
	//net_helper->net->load_inputs();
	//net->forward_prop();

	//net_helper->delete_network();
	//net_helper->load_training_from_file("data/testnet.training");

	//NN_gui_setup* setup = new NN_gui_setup(net_helper);
	//NN_Display* display = new NN_Display(net_helper);
	//net_helper->setup_test_net(0);
	//helper->display_title();
	//setup->display_int_setup();
	//setup->display_title();
	//display->display_net();
	//display->display_side_menu();
	cout << "DONE!";

	return 0;
}
