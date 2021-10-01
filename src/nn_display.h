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
		void display_side_menu();
		void display_training_options();
		void display_layer_stats();
		void display_node_stats();

		std::thread* t1;
		std::thread* t2;
		std::thread* t0;
		std::thread* t3;

		int max_font_size;
		int disp_layer;
		Layer* layer_ptr;

		int spacer;
		int label_spacer;
		int button_spacer;

		int* node_disp_layer;
		int* node_disp_node;
		int cur_node_disp_layer;
		int cur_node_disp_node;

		bool output_layer;
		bool update_stats_window;
		bool update_node_window;
		bool** display_weight_text;

		double win_x;
		double win_y;
		bool stats_window_open;
		bool node_window_open;

		bool close_net_disp;
		bool close_layer_disp;
		bool close_node_disp;
		bool close_training_disp;

		util::Platform platform;
		sf::RenderWindow net_disp_window;
		sf::RenderWindow layer_data_window;
		sf::RenderWindow node_data_window;
		sf::RenderWindow side_menu_window;
		sf::RenderWindow net_data_window;
		sf::Event net_disp_event;
		sf::Event layer_data_event;
		sf::Event node_data_event;
		sf::Event side_menu_event;
		sf::Event net_data_event;
		float screenScalingFactor;
};