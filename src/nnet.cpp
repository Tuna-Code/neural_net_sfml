#include <iostream>
#include <string>
#include <math.h>
#include "layer.h"
#include "nnet.h"


// Neural net constructor
NNet::NNet(){
    layer_count = 0;
    input_layer = NULL;
    last_layer = NULL;
    cur_layer = NULL;
    training_data = NULL;

}

// Process data forward through the network
void NNet::forward_prop(){

    // Grab out first layer
    cur_layer = input_layer;

    // Process each layer in our linked list
    while(cur_layer != NULL){
        // Check if our weights aren't null (we aren't on output layer)
        if(cur_layer->weights != NULL){
            // Process layer (proc inputs, comp outputs, comp products)
            cur_layer->proc_layer();
        }
        // we're on the final layer, don't comp products
        else{
            cur_layer->proc_inputs();
            cur_layer->comp_outputs();
        }
        // Increment pointer
        cur_layer = cur_layer->next_layer;

    }
}

// Create new layer and add it to our linked list
void NNet::add_layer(int num, int num_nodes, string actv_func, double** weights, int weight_rows, int weight_cols){


   // printf("\n%i %i %f %i %i\n", num, num_nodes,weights[0][0], weight_rows, weight_cols);
    // Create new layer pointer
    Layer* new_layer = NULL;

    // Allocate our input and output arrays with given sizes
    double* input = new double[num_nodes];
    double* output = new double[num_nodes];
    for(int i = 0; i < num_nodes; i++){
        input[i] = 0;
        output[i] = 0;
    }
    // Create new layer object and assign to pointer
    new_layer = new Layer(num, num_nodes, actv_func, input, output, weights, weight_rows, weight_cols);
    //new_layer->proc_inputs();

    // If input layer is Null then this is our first layer. Add it to the linked list
    if((input_layer == NULL)){
        input_layer = new_layer;
        last_layer = new_layer;
        layer_count++;
    }
    // Else, there is more than one node so find the end and add it
    else{
        last_layer->next_layer = new_layer;
        new_layer->prev_layer = last_layer;
        last_layer = new_layer;
        layer_count++;
    }
}

// Compute dot product of a vector and a matrix (our input * weight matrix)
// Returns double array of proper size for next layer (it uses previous layer).
double* NNet::dot(double* input, double** weights, int input_len, int weight_rows, int weight_cols){

    // Verify correct dimensions were passed
    if(input_len != weight_rows){
        cout << "\nERROR! INCORRECT WEIGHT MATRIX DIMENSIONS PROVIED! EXITING....\n",
        exit(EXIT_FAILURE);
    }

    // Initialize our return array
    double* c = NULL;
    // The number of nodes needing a result, computed based off previous layers node count(input len) and dimenstions of weight matrix of previous layer.
    int result_len = (weight_rows*weight_cols)/input_len;

   // Allocate and initialize results to 0
    c = new double[result_len];
    for(int i = 0; i < result_len; i++){
        c[i] = 0;
    }

    // Temp variables for computation
    double a = 0;
    double b = 0;

    // Loop through each entry in our previous layers output (our input)
    for(int i = 0; i < input_len; i++){
        a = input[i];
        // Multiply it by the weight and add product to the proper node space
        for(int j = 0; j < weight_cols; j++){
            b = weights[i][j];
            c[j] += (a*b);
        }
    }
    // Return our completed array (In the form of N = [1 2 3 4] where N[0] is the value going into node 0, value is 1)
    return c;
}
