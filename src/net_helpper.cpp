#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <random>





using namespace std;
//using namespace boost::filesystem;

// Net constructor
Net_Helper::Net_Helper(NNet* net)
{
	this->net = net;
}

// Load a network from external file
void Net_Helper::load_from_file(string file_path){
	if(net->input_layer != NULL){
		delete_network();
	}
	// Randomizer engine
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);


	// Vars for holding initial net info

	int layer_count;
	int* nodes_per_layer = NULL;
	string* layer_actv_func = NULL;

	int input_tracker = 0;
	//int npl_tracker = 0;
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


	int cur_row = 0;
	int cur_col = 0;

	bool random_node_count = false;
	int layer_node_range_min = 0;
	int layer_node_range_max = 0;

	bool random_weights= false;
	double rand_weights_min = 0;
	double rand_weights_max = 0;



	// Vars for file path and current line
	//string file_path2 = "data/testnet_random.top";
	//string file_path = "data/net_topology.data";
	string cur_line = "";

	// Try to open our file path
	std::ifstream input_file(file_path);

	// While file is open
	if(input_file.is_open()){

		// Grab current line and assign to cur_line
		while(getline(input_file, cur_line)){
			// If line contains # its a label or is blank, ignore
			if(cur_line.find("#") != string::npos || cur_line.size() == 0){
				continue;
			}

			else{
				// If it's our 1st input val (layer count)
				if(input_tracker == 0){
					layer_count = stoi(cur_line);
					nodes_per_layer = new int[layer_count];
					layer_actv_func = new string[layer_count];
					input_tracker++;

				}

				// If it's our 2nd input val (random inner layer node count true/false)
				else if(input_tracker == 1){

					// Grab bool value if random node nums are desired
					if(cur_line == "0"){
						random_node_count = false;
					}
					else if(cur_line == "1"){
						random_node_count = true;
					}
					input_tracker++;
				}

				// If it's our 3nd input val (random layer node count min)
				else if(input_tracker == 2){
					layer_node_range_min = stoi(cur_line);
					input_tracker++;

				}

				// If it's our 3nd input val (random layer node count max)
				else if(input_tracker == 3){
					layer_node_range_max = stoi(cur_line);
					input_tracker++;
				}

				// If it's our 4th input val (nodes per layer array OR input/output layer node count w/random hidden layer nodes)
				else if(input_tracker == 4){
							// If we want random node counts on our hidden layers
							if(random_node_count){
								// Have we found first of 2 digits
								bool first_found = false;

								// Iterate through line and ignore all non ints
								for(string::size_type i = 0; i < cur_line.size(); i++){
									if(cur_line[i] != '[' && cur_line[i] != ']' && cur_line[i] != ' ' && cur_line[i] != ','){

										// If we havent found our first int yet, put it at start of our array
										if(!first_found){
											nodes_per_layer[0] =  (int) cur_line[i] - 48;
											first_found = true;
										}
										// If this is the output value, put it at end of array
										else if(first_found){
											nodes_per_layer[layer_count - 1] =  (int) cur_line[i] - 48;
										}
									}
								}
								// Loop through middle of array and assign random values from our range
								for(int i = 1; i < layer_count - 1; i++){
									std::uniform_int_distribution<int> range(layer_node_range_min,layer_node_range_max);
									nodes_per_layer[i] = range(generator);
								}
							}


							input_tracker++;
				}

				// If it's our 5th input val (nodes per layer if manually assigned)
				else if(input_tracker == 5){
					int cur_pos = 0;
					// If we want manually assigned layer node counts
					if(!random_node_count){

						// Iterate through line and ignore all non ints
						for(string::size_type i = 0; i < cur_line.size(); i++){
							if(cur_line[i] != '[' && cur_line[i] != ']' && cur_line[i] != ' ' && cur_line[i] != ','){
								nodes_per_layer[cur_pos] = (int) cur_line[i] - 48;
								cur_pos++;
							}

						}

					}
					input_tracker++;
					//for(int i=0; i < layer_count; i++){
					//	cout << nodes_per_layer[i];
					//}
				}

				// If it's our 6th input val (activation functions for each layer)
				else if(input_tracker == 6){
					for(string::size_type i = 1; i < cur_line.size(); i++){
						if(cur_line[i] == ',' || cur_line[i] == ']'){
							string_end = i;
							layer_actv_func[func_tracker] = cur_line.substr(string_start,string_end-string_start);
							func_tracker++;
							string_start = string_end + 1;
						}
					}
					input_tracker++;
					//for(int i=0; i < layer_count - 1; i++){
					//	cout << endl << layer_actv_func[i] << " ";
					//}
				}

				//If it's our 7th input val (bool value for assigned weights or random range)
\
				else if (input_tracker == 7){
					// Grab bool value if random node nums are desired
					if(cur_line == "0"){
						random_weights = false;
					}
					else if(cur_line == "1"){
						random_weights = true;
					}

					input_tracker++;
				}

				// If it's our 8th input val (random weight range min)
				else if (input_tracker == 8){
					rand_weights_min = stod(cur_line);
					input_tracker++;
				}

				// If it's our 9th input val (random weight range max)
				else if (input_tracker == 9){
					rand_weights_max = stod(cur_line);


					// If we chose random weights, generate and assign. Otherwise move onto next section
					if(random_weights){





						for(int l = 0; l < layer_count - 1; l++){

							// Allocate weight matrix based on layer nodes
							weight_rows = nodes_per_layer[l];
							weight_cols = nodes_per_layer[l + 1];
							weights = new double*[weight_rows];

							for(int i = 0; i < weight_rows; i++){
								weights[i] = new double[weight_cols];
							}

							for(int i = 0; i < weight_rows; i++){
								for(int j = 0; j < weight_cols; j++){
									std::uniform_real_distribution<double> range(rand_weights_min,rand_weights_max);
									weights[i][j] = range(generator);
								}
							}

							// Add all layers with weights
							//cout << l << endl;
							net->add_layer(l, nodes_per_layer[l], layer_actv_func[l], weights, weight_rows, weight_cols);
						}


						net->add_layer(layer_count - 1, nodes_per_layer[layer_count - 1], layer_actv_func[layer_count - 1], NULL, 0, 0);
						input_file.close();
						return;
					}
					//// Add output layer
					//add_layer(int num, int num_nodes, string actv_func, double** weights, int weight_rows, int weight_cols);

					input_tracker++;



				}


				// If it's our 10th input val (manual weight assignment)
				else if (input_tracker == 10){
					// If we're starting a new weight matrix, start pos is 1 to avoid bracket, else its start of line
					if(beginning_weights){
						string_start = 1;

						// Allocate weight matrix based on layer nodes
						weight_rows = nodes_per_layer[layer_tracker];
						weight_cols = nodes_per_layer[layer_tracker + 1];
						weights = new double*[weight_rows];

						for(int i = 0; i < weight_rows; i++){
							weights[i] = new double[weight_cols];
						}
					}
					else{
						string_start = 0;
					}

					// Iterate through our current line by char
					for(string::size_type i = string_start; i < cur_line.size(); i++){

						// Ignore spaces
						 if(cur_line[i] == ' '){
							continue;
						 }
						 // If we're at a comma, grab all preceeding text
						 else if(cur_line[i] == ','){
							// Grab current position in string and assign to substring
							string_end = i;
							temp_weights = cur_line.substr(string_start,string_end-string_start);
							// Convert current weight value and put in matrix
							weights[cur_row][cur_col] = stod(temp_weights);
							temp_weights = "";
							// Increment to next column
							cur_col++;
							string_start = string_end + 1;
						}
						// If we're at a right bracket, we're at end of weight matrix
						else if(cur_line[i] == ']'){
							// Grab string position
							string_end = i;
							// Grab current weight
							temp_weights = cur_line.substr(string_start,string_end-string_start);
							// Convert weight and store in matrix
							weights[cur_row][cur_col] = stod(temp_weights);
							temp_weights = "";
							string_start = string_end + 1;
							// Finished grabbing weight matrix, set bools to reflect a new matrix (to re-allocate weights) and end of matrix (to process and add layer)
							beginning_weights = true;
							end_weights = true;

						}
						// If we're at the last character in the line
						else if(i == cur_line.size() - 1){
							// Grab last string in line
							string_end = i + 1;
							temp_weights = cur_line.substr(string_start,string_end-string_start);
							// Convert and store
							weights[cur_row][cur_col] = stod(temp_weights);
							temp_weights = "";
							// At end of line but not matrix, change bool and increment row and reset column.
							beginning_weights = false;
							cur_row++;
							cur_col = 0;
						}

						// Finished grabbing our weight matrix
						if(end_weights){
							//(int num, int num_nodes, string actv_func, double** weights, int weight_rows, int weight_cols)

							// Grab our collected values and start creating layers
							net->add_layer(layer_tracker, nodes_per_layer[layer_tracker], layer_actv_func[layer_tracker], weights, nodes_per_layer[layer_tracker], nodes_per_layer[layer_tracker + 1]);


							// Reset our position vars for new matrix and reset bool
							cur_row = 0;
							cur_col = 0;
							end_weights = false;

							// Finished adding layer, incrememnt out counter for next layer weight processing
							layer_tracker++;

							/*for(int i = 0; i < nodes_per_layer[layer_tracker]; i++){
								for(int j = 0; j<nodes_per_layer[layer_tracker+1]; j++){
									printf("%f ",weights[i][j]);
								}
								cout << endl;
							}
							cout << "\n------\n";
							*/

						}
					}
				}
			}
		}
		// Add our final output layer
		net->add_layer(layer_tracker, nodes_per_layer[layer_tracker], layer_actv_func[layer_tracker], NULL, 0, 0);

		// Finished. Close file
		input_file.close();






	}
	// Report error if ubable to open file path
	else{
		cout << "Unable to open file!\n";
	}







}

// Load training data from external file
void Net_Helper::load_training_from_file(string file_path){

	if(net->training_data == NULL){
		cout << "No Data Loaded!\nStarting fresh...\n";
	}
	else{
		cout << "Training data already loaded... Starting again\n";
	}

	double temp = 0;
	string cur_line = "";
	int string_start = 0;
	int string_end = 0;
	int num_data_entries = net->input_layer->num_nodes + net->last_layer->num_nodes;
	string temp_d = "";
	int cur_row = 0;
	int cur_entry = 0;
	int num_data_inputs = 0;

	// Create out input file stream object
	std::ifstream input_file(file_path);

	// While file is open
	if(input_file.is_open()){
		// Grab current line and assign to cur_line
		while(getline(input_file, cur_line)){
			// If line contains # its a label or is blank, ignore
			if(cur_line.find("#") != string::npos || cur_line.size() == 0){
				continue;
			}
			// If line is data, increment our counter
			else{
				num_data_inputs++;
			}
		}
	}
	// Close our file when finished
	input_file.close();

	// Allocate our 2d array of training data and initialize values to 0
	net->training_data = new double*[num_data_inputs];
	for(int i = 0; i < num_data_inputs; i++){
		net->training_data[i] = new double[num_data_entries];
		for(int j = 0; j < num_data_entries; j++){
			net->training_data[i][j] = 0;
		}
	}

	// Try to open our file path (for parsing CSV and storing input values)
	input_file.open(file_path);
	// While file is open
	if(input_file.is_open()){
		// Grab current line and assign to cur_line
		while(getline(input_file, cur_line)){
			// If line contains # its a label or is blank, ignore
			cur_entry = 0;
			if(cur_line.find("#") != string::npos || cur_line.size() == 0){
				continue;
			}
			// Line is valid, iterate through and grab values
			else{
				// Starting position of text in line
				string_start = 0;
				// Loop from starting position until EOL
				for(string::size_type i = string_start; i < cur_line.size(); i++){
						// If current char is space, ignore
						if(cur_line[i] == ' '){
							continue;
						}
						// If on comma, grab preceeding text and convert to double and store
						else if (cur_line[i] == ','){
							string_end = i;
							temp_d = cur_line.substr(string_start,string_end-string_start);
							temp = stod(temp_d);
							// Store value in our 2d array and increment position
							net->training_data[cur_row][cur_entry] = temp;
							cur_entry++;
							string_start = string_end + 1;
						}
						// If at end of line, grab remaining text (last value) and store
						if(i == cur_line.size() -1){
							string_end = i;
							temp_d = cur_line.substr(string_start,string_end);
							temp = stod(temp_d);
							// Store value in our 2d array and increment position
							net->training_data[cur_row][cur_entry] = temp;
							cur_entry++;
						}
				}
			}
			// Finished parsing line, increment our row to next entry in our 2d array
			cur_row++;
		}
		// Finished reading data, close input file
		input_file.close();
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
		memcpy(temp->output, temp->orig_output, sizeof(double)*temp->num_nodes);
		// Copy original output to output vector
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
	//delete net->training_data;
	while(cur != NULL){

		//cur->next_layer = NULL;
		//cur->prev_layer = NULL;


		delete cur->input;
		delete cur->output;
		delete cur->weights;


		delete cur->orig_input;
		delete cur->orig_output;
		delete cur->orig_weights;


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