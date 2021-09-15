#include <iostream>
#include "Platform/Platform.hpp"
#include <string>
#include "layer.h"
#include "nnet.h"
#include "net_helper.h"
#include <thread>

#pragma once

class NN_Display{

	public:
		Net_Helper* net;
		NN_Display(Net_Helper* net);
		void display_net();
		//void display_layer_stats(Net_Helper* net, int active_layer);
		void display_layer_stats();
		std::thread* t1;

		int max_font_size;
		Layer* layer_ptr;
		bool update_stats;
		bool output_layer;

		double win_x;
		double win_y;
		bool stats_window_open;
		bool kill_stats;

		util::Platform platform;
		sf::RenderWindow window;
		sf::RenderWindow layer_data_window;
		sf::Event event;
		sf::Event layer_data_event;
		float screenScalingFactor;
};