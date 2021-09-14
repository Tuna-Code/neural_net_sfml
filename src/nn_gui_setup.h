#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <string>

#pragma once

using namespace std;

class NN_gui_setup{

	public:
		Net_Helper* net;
		double win_x;
		double win_y;
		bool manual_net_loaded;
		bool file_net_loaded;
		string extra_text;
		util::Platform platform;
		sf::RenderWindow window;
		sf::Event event;
		float screenScalingFactor;



		NN_gui_setup(Net_Helper* net);
		void display_title();
		void display_int_setup();
		void resize_window(sf::Window* window, sf::Vector2u res);


};