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
        int cur_training_set;
        int num_training_sets;
        double** training_data;
        string error_func;

        double learning_rate;

        // total_gradient_weights[w][x][y][z]
        // w = training set counter
        // x = layer
        // y = row
        // z = col
        double**** total_gradient_weights;
        
        double** error_raw;
        double** output_all;
        double** error_comp;

        Layer* cur_layer; // Pointer to current layer (for ease of visualization when iterating)


        // Net functions
        NNet();
        void comp_error_training();
        void add_layer(int num, int num_nodes, string actv_func, double** weights, int weight_rows, int weight_cols);
        double* dot(double* input, double** weights, int input_len, int weight_x, int weight_y);
        double** add(double** x, double** y, int rows, int cols);
        double* vector_matrix_mult(double* x, double** y, int len, int rows, int cols);
        void forward_prop();
        void back_prop();
        double deriv_loss(double w, double x, double t);
        void load_inputs();
        double* vector_mult(double* x, double* y, int len);


};