#include <iostream>
#include <string>
#include <math.h>
#include "nnet.h"
#include "layer.h"

using namespace std;


// Default constructor
Layer::Layer(int num, int num_nodes, string actv_func, double* input, double* output, double** weights, int weight_rows, int weight_cols){
    // Initialize our layer with its data
    this->num_nodes = num_nodes;
    this->actv_func = actv_func;
    this->input = input;
    this->output = output;
    this->weights = weights;
    this->weight_rows = weight_rows;
    this->weight_cols = weight_cols;
    this->num = num;
    this->next_layer = NULL;
    this->prev_layer = NULL;
    this->layer_product = NULL;
}

// Manually compute layers procuts (input for next layer)
void Layer::compute_product(){
    NNet* temp = new NNet();
    this->layer_product = temp->dot(output, weights, num_nodes, weight_rows, weight_cols);
}

// Manuall compute our outputs from inputs with provided activation function
void Layer::comp_outputs(){

    // If layer is using Sigmoid
    if(actv_func == "Sigmoid"){
        for(int i = 0; i < num_nodes; i++){
            output[i] = sigmoid(input[i]);
        }
    }

    // If layer is raw (such as input layer)
    if(actv_func == "Null"){
        for(int i = 0; i < num_nodes; i++){
            output[i] = input[i];
        }
    }
}

// Process the current layer for forward prop
void Layer::proc_layer(){
    proc_inputs();
    comp_outputs();
    compute_product();
}

// Manually process this layers inputs (take previous layers computed product and copy to our input)
void Layer::proc_inputs(){
    if(this->prev_layer != NULL){
        this->input = this->prev_layer->layer_product;
    }
}

// Print the information of the layer
void Layer::print_layer_info(){
    printf("\n**Layer %i Info...\n-----------------------", num);
    printf("\n- Node count: %i\n- Activation Function: %s\n-----\n", num_nodes, actv_func.c_str());

    // If not on input layer
    if(num != 0){
        cout << "\n**Layer Net Sum Products:\n----------\n";
        for(int i = 0; i < num_nodes; i++){
            printf("%f ", input[i]);
        }
        cout << endl;
    }

    // Print our computed outputs
    cout << "\nLayer Outputs with " << actv_func << " activation function:\n----------\n";
    for(int i = 0; i < num_nodes; i++){
        printf("%f ", output[i]);
    }
    cout << endl;

    // Print current layer weights
    printf("\nLayer weights:\n----------\n");
    if(next_layer != NULL){
        for(int i = 0; i < weight_rows; i++){
            for(int j = 0; j < weight_cols; j++){
                printf("%f ", weights[i][j]);
            }
            cout << endl;
        }
    }

    // Print the computed net sum (input for next layer)
    cout << "\nComputed Net Sum Products for next layer:\n----------\n";
    if(layer_product != NULL){
        for(int i = 0; i < weight_cols; i++){
            printf("%f\n", layer_product[i]);
        }
    }
    else{
        cout << "*****Net Sum Products not computed yet!*****";
    }
    cout << endl << endl;
}

// Sigmoid activation function
double Layer::sigmoid(double input){
    return (1 / (1 + exp((-1)*input)));
}

// Fast sigmoid activation function (unsure if valid)
double Layer::fast_sigmoid(double input){
    return (input / (1 + abs(input)));
}