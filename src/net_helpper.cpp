#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>


using namespace std;

// Net constructor
Net_Helper::Net_Helper(NNet* net)
{
	this->net = net;
}

// Load a network from external file
void Net_Helper::load_from_file(){

	// Vars for holding initial net info

	int layer_count;
	int* nodes_per_layer = NULL;
	string* layer_actv_func = NULL;




	int input_tracker = 0;
	int npl_tracker = 0;
	int string_start = 1;
	int string_end = 0;
	int func_tracker = 0;

	bool beginning_weights = true;
	string temp_weights = "";

	bool end_weights = false;

	int weight_rows = 0;
	int weight_cols = 0;
	double** weights = NULL;

	int layer_tracker = 0;

	bool end_line = false;

	int cur_row = 0;
	int cur_col = 0;

	// Vars for file path and current line
	string file_path = "data/net_topology.data";
	string cur_line = "";

	// Try to open our file path
	ifstream input_file(file_path);

	// While file is open
	if(input_file.is_open()){

		// Grab current line and assign to cur_line
		while(getline(input_file, cur_line)){
			// If line contains # its a label, ignore
			if(cur_line.find("#") != string::npos){
				continue;
			}

			else{
				// If it's our first input val
				if(input_tracker == 0){
					layer_count = stoi(cur_line);
					//printf("%i\n", layer_count);
					nodes_per_layer = new int[layer_count];
					layer_actv_func = new string[layer_count];
					input_tracker++;
				}

				else if(input_tracker == 1){
					//cout << cur_line << endl;
					for(string::size_type i = 0; i < cur_line.size(); i++){
						if(cur_line[i] != '[' &&  cur_line[i] != ']' && cur_line[i] != ' ' && cur_line[i] != ','){
							nodes_per_layer[npl_tracker] = (int) cur_line[i] - 48;
							npl_tracker++;
						}
					}
					input_tracker++;
				}

				else if(input_tracker == 2){
					for(string::size_type i = 1; i < cur_line.size(); i++){
						if(cur_line[i] == ',' || cur_line[i] == ']'){
							string_end = i;
							layer_actv_func[func_tracker] = cur_line.substr(string_start,string_end-string_start);
							func_tracker++;
							string_start = string_end + 1;
						}
					}
					input_tracker++;
				}

				// Grab weights

				else if (input_tracker == 3){

					// If we're starting our weight matrix, start pos is 1 to avoid bracket, else its start of line
					if(beginning_weights){
						string_start = 1;
						weight_rows = nodes_per_layer[layer_tracker];
						weight_cols = nodes_per_layer[layer_tracker + 1];
						weights = new double*[weight_rows];

						for(int i = 0; i < weight_rows; i++){
							weights[i] = new double[weight_cols];
						}


						//cout << cur_row << cur_col << endl;
					}
					else{
						string_start = 0;
					}



					//(int num, int num_nodes, string actv_func, double** weights, int weight_rows, int weight_cols)

					// Iterate through our current line by char
					for(string::size_type i = string_start; i < cur_line.size(); i++){

						 if(cur_line[i] == ' '){
							continue;
						 }

						 // If we're at a comma, grab all proceeding text
						 if(cur_line[i] == ','){
							string_end = i;
							temp_weights = temp_weights + cur_line.substr(string_start,string_end-string_start);
							//printf("Weight: %s in pos [%i][%i]\n", temp_weights.c_str(), cur_row, cur_col);
							weights[cur_row][cur_col] = stod(temp_weights);
							temp_weights = "";
							cur_col++;
							//cout << cur_line.substr(string_start,string_end-string_start) << " ";
							string_start = string_end + 1;
						}
						// If we're at a right bracket, we're at end of weight matrix. Set bool and continue
						else if(cur_line[i] == ']'){
							string_end = i;
							//cout << cur_line.substr(string_start,string_end-string_start) << " ";
							temp_weights = temp_weights + cur_line.substr(string_start,string_end-string_start);
							//printf("Weight: %s in pos [%i][%i]\n", temp_weights.c_str(), cur_row, cur_col);
							weights[cur_row][cur_col] = stod(temp_weights);
							temp_weights = "";
							string_start = string_end + 1;
							beginning_weights = true;
							end_line = true;
							end_weights = true;

						}
						// If we're at the last character in the line grab weight value at end
						else if(i == cur_line.size() - 1){
							string_end = i +1;
							temp_weights = temp_weights + cur_line.substr(string_start,string_end-string_start);
							//printf("Weight: %s in pos [%i][%i]\n", temp_weights.c_str(), cur_row, cur_col);
							weights[cur_row][cur_col] = stod(temp_weights);
							temp_weights = "";
							end_line = true;
							beginning_weights = false;
							cur_row++;
							cur_col = 0;
						}

						if(end_line){
							//cout << temp_weights;
							temp_weights = "";

							end_line = false;
						}
						// Finished grabbing our weight matrix
						if(end_weights){
							//for(string::size_type i = 0; i < temp_weights.size(); i++){
							//	cout << temp_weights;
							//}

							for(int i = 0; i < nodes_per_layer[layer_tracker]; i++){
								for(int j = 0; j<nodes_per_layer[layer_tracker+1]; j++){
									printf("%f ",weights[i][j]);
								}
								cout << endl;
							}
							temp_weights = "";
							cur_row = 0;
							cur_col = 0;
							end_weights = false;
							cout << "\n------\n";
							//input_tracker++;
							layer_tracker++;
						}






						/*if(cur_line[i] == ']'){
							input_tracker++;
						}
						else if(cur_line[i] == ','){
							string_end = i;
							cout << cur_line.substr(string_start,string_end-string_start) << " ";
							string_start = string_end + 1;
						}*/
					}


				}


			}






		}

		// Finished. Close file
		input_file.close();






	}
	// Report error if ubable to open file path
	else{
		cout << "Unable to open file!\n";
	}







}

// Print topology of current network
void Net_Helper::print_network()
{

	// Grab out first layer and total number of laywers
	Layer* cur = net->input_layer;
	int layer_count = net->layer_count;
	printf("\nPrinting Neural Network Topology....\n-------------------------------------\nLayer Count: %i\n\n", layer_count);

	// Display whole network info
	cout << "**Network Inputs:\n----------\n";
	for (int i = 0; i < cur->num_nodes; i++)
	{
		printf("%f ", cur->input[i]);
	}
	cout << endl << endl;

	// Print info for each layer (iterating through linked list)
	while (cur != NULL)
	{
		cur->print_layer_info();
		cur = cur->next_layer;
	}
}

// Reset network to original values before forward/backward prop
void Net_Helper::reset_network(){

	// Grab input layer
	Layer* temp = net->input_layer;

	// Loop through each layer
	for(int i = 0; i < net->layer_count; i++){

		// Copy original output to output vector
		memcpy(temp->output, temp->orig_output, sizeof(double)*temp->num_nodes);
		// If not input layer, copy original input to layer
		if(temp != net->input_layer){
			memcpy(temp->input, temp->orig_input, sizeof(double)*temp->num_nodes);
		}
		// Loop through our weight arrays and reset to original values
		for (int j = 0; j < temp->weight_rows; j++)
   		{
        	memcpy(temp->weights[j], temp->orig_weights[j], sizeof(double)*temp->weight_cols );
		}

		// Go to next layer
		temp = temp->next_layer;
	}
}

// Delete and clear memory of all network values (start fresh)
// Clear out NN layers from memory
void Net_Helper::delete_network(){
	Layer* cur = net->input_layer;
	Layer* next = cur->next_layer;
	int i = 0;
	while(cur != NULL){
		cout << i << endl;
		cur->next_layer = NULL;
		cur->prev_layer = NULL;


		delete cur->input;
		delete cur->output;
		delete cur->weights;

		delete cur;

		cur = next;
		if(cur != NULL){
			next = cur->next_layer;
		}
	}
	net->cur_layer = NULL;
	net->last_layer = NULL;
	net->input_layer = NULL;
	net->layer_count = 0;
}

// Interactive network setup
void Net_Helper::setup_network()
{

	// Setup initial variables for creating network
	int layer_count = 0;
	int weight_rows = 0;
	int weight_cols = 0;
	double** weights = NULL;
	double* input = NULL;
	string func = "Sigmoid";
	int* node_count = NULL;
	// Prompt for layer count and allocate array
	cout << "How many layers will be in the network?: ";
	cin >> layer_count;
	node_count = new int[layer_count];
	cout << endl;

	// Get the number of nodes per layer
	for (int i = 0; i < layer_count; i++)
	{
		printf("How many nodes are in Layer #%i?\n", i);
		cin >> node_count[i];
	}

	// Prompt for and create weight Matrix for each layer
	for (int i = 0; i < layer_count - 1; i++)
	{

		// Get weight matrix dimenstions
		func = "Sigmoid";
		printf("\nCreating Weights for Layer %i....\n", i);
		weight_rows = node_count[i];
		weight_cols = node_count[i + 1];

		// If this is our input layer, set actv function to null and get input values
		if (i == 0)
		{
			func = "Null";
			input = new double[node_count[0]];
			printf("\nPlease enter the initial input values for our first layer:\n");
			for (int x = 0; x < node_count[0]; x++)
			{
				printf("For initial node %i:\n", x);
				cin >> input[x];
			}
		}

		// Allocate weight arrays
		weights = new double*[weight_rows];
		for (int j = 0; j < weight_rows; j++)
		{
			weights[j] = new double[weight_cols];
		}

		// Get values for weights in layer
		printf("\nPlease enter weights for Layer %i", i);
		for (int j = 0; j < weight_rows; j++)
		{
			for (int k = 0; k < weight_cols; k++)
			{
				printf("\nConnecting Node %i to Node %i\n", j, k);
				cin >> weights[j][k];
			}
		}
		// Display given layer weight matrix
		/*printf("\n**Layer weights:\n----------\n");
        for(int j = 0; j < weight_rows; j++){
            for(int k = 0; k < weight_cols; k++){
                printf("%f ", weights[j][k]);
            }
        cout << endl;
        }*/
		// Add out current layer with given values
		net->add_layer(i, node_count[i], func, weights, weight_rows, weight_cols);
	}
	net->add_layer(layer_count - 1, node_count[layer_count - 1], func, NULL, 0, 0);
	// Assign our input layer its initial values
	net->input_layer->input = input;
}

// Manual network setup (for pre-computed networks in my notebook - for testing)
void Net_Helper::setup_test_net(int option)
{
	// If network 0 is chosen
	if (option == 0)
	{
		// Setup Layer 0

		// Initialize our layer info
		int node_count = 1;
		int weight_rows = 1;
		int weight_cols = 2;
		double** weights;

		// Allocate our weight matrix
		weights = new double*[weight_rows];
		for (int i = 0; i < weight_rows; i++)
		{
			weights[i] = new double[weight_cols];
		}

		// Assign weight values
		weights[0][0] = .911;
		weights[0][1] = .420;

		// Add layer and update pointer
		net->add_layer(0, node_count, "Null", weights, weight_rows, weight_cols);
		Layer* prev_layer = net->input_layer;

		// Manually set input value
		prev_layer->input[0] = 5.3;

		// Setup Layer 1

		node_count = 2;
		weight_rows = 2;
		weight_cols = 3;
		weights = new double*[weight_rows];
		for (int i = 0; i < weight_rows; i++)
		{
			weights[i] = new double[weight_cols];
		}
		weights[0][0] = 0.9;
		weights[0][1] = 0.65;
		weights[0][2] = 0.32;
		weights[1][0] = 0.345;
		weights[1][1] = 0.734;
		weights[1][2] = 0.2357;
		net->add_layer(1, node_count, "Sigmoid", weights, weight_rows, weight_cols);
		prev_layer = prev_layer->next_layer;

		// Setup Layer 2
		node_count = 3;
		weight_rows = 3;
		weight_cols = 1;
		weights = new double*[weight_rows];
		for (int i = 0; i < weight_rows; i++)
		{
			weights[i] = new double[weight_cols];
		}
		weights[0][0] = 0.358;
		weights[1][0] = 0.432;
		weights[2][0] = 0.659;
		net->add_layer(2, node_count, "Sigmoid", weights, weight_rows, weight_cols);

		// Setup Layer 3
		node_count = 1;
		weight_rows = 0;
		weight_cols = 0;
		weights = NULL;

		net->add_layer(3, node_count, "Sigmoid", weights, weight_rows, weight_cols);
	}


}