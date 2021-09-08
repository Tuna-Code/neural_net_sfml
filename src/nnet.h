#include <iostream>
#include <string>
#include "layer.h"

#pragma once

using namespace std;

class NNet{
    public:
        // Net variables
        int layer_count; // How many layers total are in the network
        Layer* input_layer; // Pointer to input layer (Layer 0)
        Layer* last_layer; // Pointer to most recently added layer
        Layer* output_layer; // Pointer to outout layer (Layer n)

        Layer* cur_layer; // Pointer to current layer (for ease of visualization when iterating)
        

        // Net functions
        NNet();
        void add_layer(int num, int num_nodes, string actv_func, double** weights, int weight_rows, int weight_cols);
        double* dot(double* input, double** weights, int input_len, int weight_x, int weight_y);
        void forward_prop();
        

};