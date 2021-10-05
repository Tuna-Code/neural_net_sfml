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
    num_training_sets = 0;
    cur_training_set = 0;
    error_func = "";

    total_gradient_weights = NULL;
    error_raw = NULL;
    error_comp = NULL;
    output_all = NULL;
    learning_rate = 0;


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
// Compute raw error + error w/current function and assign to object for all training sets
void NNet::comp_error_training(){
    double sum_sq_error = 0;

    // Loop through all our training sets
    for(int i = 0; i < num_training_sets; i++){

        // Assign trainign data to input nodes
        /*for(int j = 0; j < input_layer->num_nodes; j++){
            input_layer->input[j] = training_data[i][j];
        }*/
        // Prop inputs through network
        cur_training_set = i;
        load_inputs();
        forward_prop();
        // Copy outputs to our 2d array of all outputs
        memcpy(output_all[i], last_layer->output, sizeof(double)*last_layer->num_nodes);

        for(int j = 0; j < last_layer->num_nodes; j++){
            error_raw[i][j] = training_data[i][input_layer->num_nodes + j] - output_all[i][j];
           // error_raw[i][j] = output_all[i][j] - training_data[i][input_layer->num_nodes + j];
        }

    }


    // If loss function is Mean Squared Error
    if(error_func == "MSE"){
        for(int i = 0; i < num_training_sets; i++){
            for(int j = 0; j < last_layer->num_nodes; j++){
                sum_sq_error += pow((error_raw[i][j]) , 2.0);
            }
        }
        sum_sq_error = sum_sq_error / num_training_sets;
        error_comp[0][0] = sum_sq_error;
    }

    else if(error_func == "SOS"){
        for(int i = 0; i < num_training_sets; i++){
            sum_sq_error = 0;
            for(int j = 0; j < last_layer->num_nodes; j++){
                sum_sq_error += pow((error_raw[i][j]) , 2.0);
            }
            error_comp[i][0] = sum_sq_error;
        }
    }

}


// Backprop function
// Currently only have SOS loss function w/Sigmoid
void NNet::back_prop(){

    // Layer pointer for iterating throug network
    Layer* layer = last_layer;


    // Simple backprop if using SOS as loss function and Sigmoid activation on output
    if(error_func == "SOS" && layer->actv_func == "Sigmoid"){

        // Loop through each training set
        for(int i = 0; i < num_training_sets; i++){

            // For each training set, start at the back of the network (backrpop)
            layer = last_layer;
            cur_training_set = i;

            // Verify our current trainign set it loaded and propegated
            load_inputs();
            forward_prop();
            // Loop until we hit beginning of our network
            while(layer != input_layer){
               // cout << "\nON LAYER: " << layer->num << endl;

                // If we aren't on last layer, create gradient of outputs with calc
                if(layer != last_layer){

                   // cout << "\nNew Layer\n";
                    for(int j = 0; j < layer->num_nodes; j++){
                         for(int k = 0; k < layer->next_layer->num_nodes; k++){
                            layer->gradient_out[j] += layer->next_layer->gradient_in[k]*layer->weights[j][k];
                         }
                    }

                   // if(layer->num != 1){
                     //   layer->gradient_out = vector_matrix_mult(layer->next_layer->gradient_in, layer->weights, layer->next_layer->num_nodes, layer->num_nodes, layer->next_layer->num_nodes);
                    //}


                    //cout << "Gradient out\n";
                     //cout << layer->gradient_out[0] << " " << layer->gradient_out[1];
                }
                // Loop through nodes in current layor

                for(int j = 0; j < layer->num_nodes; j++){
                    //cout << "\nON NODE: " << j << endl;

                    // If on output layer, use SOS derivative
                    if(layer == last_layer){
                        layer->gradient_out[j] = 2*error_raw[i][j];
                    }

                    // Compute gradient of inputs (invese of Sigmoid)
                    layer->gradient_in[j] = (layer->output[j] * (1 - layer->output[j])) * layer->gradient_out[j];

                    // Going backwards, loop through next layer nodes
                    for(int k = 0; k < layer->prev_layer->num_nodes; k++){

                        // Compute weight gradient of current layer
                        layer->prev_layer->gradient_weights[k][j] = layer->prev_layer->output[k]*layer->gradient_in[j];

                        // Add to our master array of all gradients for all training sets
                        total_gradient_weights[i][layer->prev_layer->num][k][j] =  layer->prev_layer->gradient_weights[k][j];
                    }
               // cout << "\nFINISHED NODE: " << j << endl;

                }
                // Decrement our layer pointer
                layer = layer->prev_layer;
            }
        }

        // Starting on first layer with weights (second from last), loop through training sets
        for(int i = 0; i < num_training_sets; i++){
            layer = last_layer->prev_layer;

            // Whil our linked list isn't at the end
            while(layer != NULL){
                //Loop through current and next nodes
                for(int j = 0; j < layer->num_nodes; j++){
                    for(int k = 0; k < layer->next_layer->num_nodes; k++){
                        // Sum all of our layer gradients together
                        layer->summed_grad_weights[j][k] -= total_gradient_weights[i][layer->num][j][k];
                    }
                }
                // Decrement our pointer and continue
                layer = layer->prev_layer;
            }
        }

        // Starting on first layer with weights (second from last), loop through network once
        layer = last_layer->prev_layer;
        while(layer != NULL){
            for(int j = 0; j < layer->num_nodes; j++){
                for(int k = 0; k < layer->next_layer->num_nodes; k++){
                    // Go through our summed gradients and mult by learning rate
                    layer->summed_grad_weights[j][k] = layer->summed_grad_weights[j][k] * learning_rate;
                    // Modify our weights with computed inprovements
                    layer->weights[j][k] = layer->weights[j][k] - layer->summed_grad_weights[j][k];
                   // cout << layer->weights[j][k] << " ";
                }
                //cout << endl;
            }
            // Decrement and continue
            layer = layer->prev_layer;
        }
    }
}

// Load our input layer w values from set num
void NNet::load_inputs(){

    for(int i = 0; i < input_layer->num_nodes; i++){
        input_layer->input[i] = training_data[cur_training_set][i];
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
       // grad_in[i] = 0;
        //grad_out[i] = 0;
    }
    // Create new layer object and assign to pointer
    new_layer = new Layer(num, num_nodes, actv_func, input, output, weights, weight_rows, weight_cols);

    new_layer->gradient_in = new double[num_nodes];
    new_layer->gradient_out = new double[num_nodes];

     for(int i = 0; i < num_nodes; i++){
         new_layer->gradient_in[i] = 0;
         new_layer->gradient_out[i] = 0;
     }


    // If cols are 0 (output layer) don't allocate
    if(weight_cols != 0 && weight_cols != 0){
      //  grad_weights = new double*[weight_rows];
        new_layer->gradient_weights = new double*[weight_rows];
        for(int i = 0; i < weight_rows; i++){
            new_layer->gradient_weights[i] = new double[weight_cols];
            for(int j = 0; j < weight_cols; j++){
                new_layer->gradient_weights[i][j] = 0;
                //cout << new_layer->gradient_weights[i][j] << " ";
            }
           // cout << endl;
            //memcpy(new_layer->gradient_weights[i], grad_weights[i], sizeof(double)*weight_cols);

           // memcpy(temp->weights[j], temp->orig_weights[j], sizeof(double)*temp->weight_cols );
        }

        //cout << "\n-----\n" << endl;


    }
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

double** NNet::add(double** x, double** y, int rows, int cols){

    double** temp = NULL;
    temp = new double*[rows];

    for(int i = 0; i < rows; i++){
        temp[i] = new double[cols];
        for(int j = 0; j < cols; j++){
            temp[i][j] = x[i][j] + y[i][j];
            printf("a+b=c: %f %f %f\n",x[i][j], y[i][j],temp[i][j]);
        }
    }

    return temp;

}
double* NNet::vector_matrix_mult(double* x, double** y, int len, int rows, int cols){
     double* temp = NULL;

    // Verify correct dimensions were passed
    if(len != cols){
        cout << "\nERROR! INCORRECT WEIGHT MATRIX DIMENSIONS PROVIED! EXITING....\n",
        exit(EXIT_FAILURE);
    }

    // Initialize our return array

    // The number of nodes needing a result, computed based off previous layers node count(input len) and dimenstions of weight matrix of previous layer.


   // Allocate and initialize results to 0
    temp = new double[len];



    // Temp variables for computation


    // Loop through each entry in our previous layers output (our input)
    for(int i = 0; i < cols; i++){
        temp[i] = 0;
        // Multiply it by the weight and add product to the proper node space
        for(int j = 0; j < rows; j++){

            temp[i] += (y[i][j] * x[j]);
            //printf("a*b=c: %f %f %f\n",y[i][j], x[j],temp[i]);
        }
    }
    // Return our completed array (In the form of N = [1 2 3 4] where N[0] is the value going into node 0, value is 1)
    return temp;
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
           // printf("a*b=c: %f %f %f\n",a, b,c[j]);
        }
    }
    // Return our completed array (In the form of N = [1 2 3 4] where N[0] is the value going into node 0, value is 1)
    return c;
}
