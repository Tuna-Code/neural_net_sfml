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
		void display_node_stats();
		std::thread* t1;
		std::thread* t2;

		int max_font_size;
		int disp_layer;
		Layer* layer_ptr;

		int* node_disp_layer;
		int* node_disp_node;
		int cur_node_disp_layer;
		int cur_node_disp_node;

		bool output_layer;
		bool update_stats_window;
		bool update_node_window;

		double win_x;
		double win_y;
		bool stats_window_open;
		bool node_window_open;


		util::Platform platform;
		sf::RenderWindow window;
		sf::RenderWindow layer_data_window;
		sf::RenderWindow node_data_window;
		sf::Event event;
		sf::Event layer_data_event;
		sf::Event node_data_event;
		float screenScalingFactor;
};