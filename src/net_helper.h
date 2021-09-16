#include <iostream>
#include <string>
#include <math.h>
#include "layer.h"
#include "nnet.h"

#pragma once

class Net_Helper{

    public:
        NNet* net;
        Net_Helper(NNet* net);
        //Net_Helper();

        void setup_test_net(int option);
        void print_network();
        void setup_network();
        void reset_network();
        void delete_network();


};