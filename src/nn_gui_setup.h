#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <string>
#include "nn_display.h"
#include <thread>

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
		sf::RenderWindow file_window;
		sf::Event event;
		sf::Event file_event;
		float screenScalingFactor;
		NN_Display* display;
		std::thread* t1;
		std::thread* t2;
		std::thread* t3;



		NN_gui_setup(Net_Helper* net);
		void display_title();
		void display_int_setup();
		void display_file_select();
		void resize_window(sf::Window* window, sf::Vector2u res);


};