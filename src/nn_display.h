#include <iostream>
#include "Platform/Platform.hpp"
#include <string>
#include "layer.h"
#include "nnet.h"
#include "net_helper.h"

#pragma once

class NN_Display{

	public:
		Net_Helper* net;
		NN_Display(Net_Helper* net);
		void display_net();

		double win_x;
		double win_y;
		util::Platform platform;
		sf::RenderWindow window;
		sf::Event event;
		float screenScalingFactor;
};