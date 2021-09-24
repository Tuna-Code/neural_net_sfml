#include <iostream>
#include <string>

#pragma once

using namespace std;

class Layer{

    public:
        // Layer variables
        int num; // Layer number
        int num_nodes; // Number of nodes in Layer
        int weight_rows; // How many columns in this layers weight matrix
        int weight_cols; // How many rows in this layers weight matrix
        double* input; // Layer input array
        double* output; // Layer output after computation
        double** weights; // Weights of connections leaving layer
        double* layer_product;

        double* orig_input; // Layer input array
        double* orig_output; // Layer output after computation
        double** orig_weights; // Weights of connections leaving layer

        string actv_func; // Which activation function we want for this layer
        Layer* next_layer = NULL; // Pointer to next layer in linked list
        Layer* prev_layer = NULL; // Points to previous layer in linked list

        // Layer functions
        Layer(int num, int num_nodes, string actv_func, double* input, double* output, double** weights, int weight_rows, int weight_cols);
        void print_layer_info();
        double sigmoid(double input);
        double fast_sigmoid(double input);
        void comp_outputs();
        void compute_product();
        void proc_layer();
        void proc_inputs();

};