#include "layer.h"
#include "nnet.h"
#include <iostream>
#include <math.h>
#include <string>

#pragma once

class Net_Helper
{

    public:
        NNet* net;
        Net_Helper(NNet* net);
        //Net_Helper();

        void setup_test_net(int option);
        void print_network();
        void setup_network();
        void reset_network();
        void delete_network();
        void load_from_file(string file_path);
};