#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

// Net constructor
Net_Helper::Net_Helper(NNet* net)
{
	this->net = net;
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