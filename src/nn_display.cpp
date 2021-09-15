#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <string>
#include "nn_display.h"
#include <thread>


using namespace std;

NN_Display::NN_Display(Net_Helper* net){
	this->net = net;

	win_x = 1440;
	win_y = 900;
	max_font_size = 28;
	// Window dimensionts

	// in Windows at least, this must be called before creating the window
	screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	stats_window_open = false;
	kill_stats = false;

	layer_ptr = NULL;
	update_stats = false;


}

void NN_Display::display_net(){
	// ---------------------------------------- SETUP ------------------------------

	int layer_count = net->net->layer_count;

	int layer_font_size = max_font_size - 5;
	sf::Text title;
	sf::Text menu;

	sf::RectangleShape forward_rect;
	sf::Text forward_text;

	sf::RectangleShape reset_rect;
	sf::Text reset_text;

	sf::Font title_font;
	sf::Text* layer_title = NULL;

	sf::Color bg_button(128, 128, 128);


	//bool display_layer = false;


	layer_title = new sf::Text[layer_count];

	int spacer = 10;

	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}
		// Set font attributes for heading
	title.setFont(title_font);
	title.setString("Neural Network Visualizer");
	title.setCharacterSize(max_font_size);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width) / 2, win_y / 50);

	menu.setFont(title_font);
	menu.setString("Options");
	menu.setCharacterSize(max_font_size * .66667);
	menu.setFillColor(sf::Color::White);
	menu.setStyle(sf::Text::Bold | sf::Text::Underlined);
	menu.setPosition(20,title.getPosition().y + menu.getGlobalBounds().height/2);



	forward_text.setFont(title_font);
	forward_text.setString("-> Prop.");
	forward_text.setCharacterSize(max_font_size * .5);
	forward_text.setFillColor(sf::Color::Black);
	//forward_text.setStyle(sf::Text::Bold);
	forward_text.setPosition(menu.getPosition().x,menu.getPosition().y +spacer*2.5 + menu.getGlobalBounds().height/2);

	forward_rect.setPosition(forward_text.getPosition().x, forward_text.getPosition().y);
	forward_rect.setSize(sf::Vector2f(forward_text.getGlobalBounds().width + spacer, forward_text.getGlobalBounds().height*2));
	forward_rect.setFillColor(bg_button);
	forward_rect.setOutlineColor(sf::Color::White);
	forward_rect.setOutlineThickness(1);


	reset_text.setFont(title_font);
	reset_text.setString("Reset Net");
	reset_text.setCharacterSize(max_font_size * .5);
	reset_text.setFillColor(sf::Color::Black);
	//reset_text.setStyle(sf::Text::Bold);
	reset_text.setPosition(menu.getPosition().x,forward_rect.getPosition().y +spacer*2.5 + forward_rect.getGlobalBounds().height/2);


	reset_rect.setPosition(reset_text.getPosition().x, reset_text.getPosition().y);
	reset_rect.setSize(sf::Vector2f(reset_text.getGlobalBounds().width + spacer, reset_text.getGlobalBounds().height*2));
	reset_rect.setFillColor(bg_button);
	reset_rect.setOutlineColor(sf::Color::White);
	reset_rect.setOutlineThickness(1);



	sf::Vertex menu_line[] =
	{
    sf::Vertex(sf::Vector2f(menu.getPosition().x + menu.getGlobalBounds().width + spacer, 0)),
    sf::Vertex(sf::Vector2f(menu.getPosition().x + menu.getGlobalBounds().width + spacer, win_y))
	};


	window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");
	window.setPosition(sf::Vector2(50, window.getPosition().y));

	platform.setIcon(window.getSystemHandle());


	//cout << l2_size;
	// Setup layer 2 circles
	double op_x = menu.getPosition().x + menu.getGlobalBounds().width + spacer*2 ;
	double op_y = title.getPosition().y + title.getGlobalBounds().height  + spacer*2;
	double layer_space_x = (win_x - op_x) / layer_count;


	//double chunk_start_x = workable_x / layer_count;
// ------------------------------------------ IMPLEMENTATION------------------------------

	string temp = "";

	temp = "Layer: " + to_string(0);
	layer_title[0].setString(temp);
	layer_title[0].setFont(title_font);
	layer_title[0].setCharacterSize(layer_font_size);
	layer_title[0].setFillColor(sf::Color::White);
	layer_title[0].setStyle(sf::Text::Bold | sf::Text::Underlined);
	layer_title[0].setPosition(op_x + layer_title[0].getGlobalBounds().width/2, op_y);


	for(int i = 1; i < layer_count; i++){
		temp = "Layer: " + to_string(i);
		layer_title[i].setString(temp);
		layer_title[i].setFont(title_font);
		layer_title[i].setCharacterSize(layer_font_size);
		layer_title[i].setFillColor(sf::Color::White);
		layer_title[i].setStyle(sf::Text::Bold | sf::Text::Underlined);
		layer_title[i].setPosition(layer_title[i-1].getPosition().x + layer_space_x + spacer, op_y);


	}


	/*cir1[0].setRadius(100);
	cir1[1].setRadius(75);
	cir1[0].setFillColor(sf::Color::Green);
	cir1[1].setOutlineThickness(5);
	cir1[1].setFillColor(sf::Color::Black);
	cir1[1].setOutlineColor(sf::Color::White);

	cir1[0].setPosition(sf::Vector2f(100,100));
	cir1[1].setPosition(sf::Vector2f(500,500));*/
	// Load font from file



	while (window.isOpen()){


		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)// || event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			// If clicked, check if clicked on objects
			if(event.type == sf::Event::MouseButtonPressed){

				// Check if clicked on layer labels
				for(int i = 0; i < layer_count; i++){
					if(layer_title[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
							layer_ptr = net->net->input_layer;
							if(i != 0){
								for(int j = 0; j < i; j++){
									layer_ptr = layer_ptr->next_layer;
								}
							}
							if(!stats_window_open){
								t1 = new std::thread(&NN_Display::display_layer_stats, this);
								stats_window_open = true;
							}
					}
				}

				// Check if clicked on control buttons
				if(forward_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
					net->net->forward_prop();
				}
				if(reset_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
					Layer* temp = net->net->input_layer;
					for(int i = 0; i < layer_count; i++){
						memcpy(temp->output, temp->orig_output, sizeof(double)*temp->num_nodes);

						temp = temp->next_layer;
					}

				}

			}

		}



	window.clear();


	for(int i = 0; i < layer_count; i++){
		window.draw(layer_title[i]);
	}

	window.draw(title);
	window.draw(menu);
	window.draw(forward_rect);
	window.draw(reset_rect);

	window.draw(forward_text);
	window.draw(reset_text);
	window.draw(menu_line, 2, sf::Lines);
	window.display();
	}

}

void NN_Display::display_layer_stats(){

	// Set initial resolution
	double x = 800;
	double y = 1000;

	// Pixel spacer and bool to determine if we're on the output layer
	int spacer = 20;
	bool output_layer = false;

	//Layer* layer = layer_ptr;


	if(layer_ptr == net->net->last_layer){
		output_layer = true;
	}
	else{
		output_layer = false;
	}
	//layer->print_layer_info();


	sf::Text title;
	sf::Text layer_stats;
	sf::Text weight_title;
	sf::Text weight_sub;
	sf::Text weight_stats;

	sf::Font title_font;

	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}


	layer_data_window.create(sf::VideoMode(x * screenScalingFactor, y * screenScalingFactor), "");
	layer_data_window.setPosition(sf::Vector2((int)window.getPosition().x + (int)window.getSize().x + 10, (int)window.getPosition().y));



// ---------------- Window Open -------------
	while (layer_data_window.isOpen()){

		if(layer_ptr == net->net->last_layer){
			output_layer = true;
		}
		else{
			output_layer = false;
		}
		title.setFont(title_font);
		title.setString("Layer " + to_string(layer_ptr->num) + " Stats");
		title.setCharacterSize(max_font_size * 0.83);
		title.setFillColor(sf::Color::White);
		title.setStyle(sf::Text::Bold | sf::Text::Underlined);
		title.setPosition(20, y / 50);
		layer_data_window.setTitle(title.getString());

		layer_stats.setFont(title_font);
		layer_stats.setString("\n* Activation Function: " + layer_ptr->actv_func + "\n----------\n\n");
		layer_stats.setCharacterSize(max_font_size * 0.66667);
		layer_stats.setFillColor(sf::Color::White);
		layer_stats.setStyle(sf::Text::Bold );
		layer_stats.setPosition(title.getPosition().x, title.getPosition().y + title.getGlobalBounds().height + spacer);

		layer_stats.setString(layer_stats.getString() + "* Node Count: " + to_string(layer_ptr->num_nodes) + "\n----------\n\n");



		// Generate layer inputs into matrix string
		string temp = "[";
		for(int i=0; i < layer_ptr->num_nodes-1; i++){
			temp = temp + to_string(layer_ptr->input[i]) + ", ";
		}
		temp = temp + to_string(layer_ptr->input[layer_ptr->num_nodes-1]) + "]";

		layer_stats.setString(layer_stats.getString() + "* Layer Input Vector (raw): \n----------\n" + temp + "\n\n");

		// Generate layer outputs into matrix string
		temp = "[";
		for(int i=0; i < layer_ptr->num_nodes-1; i++){
			temp = temp + to_string(layer_ptr->output[i]) + ", ";
		}
		temp = temp + to_string(layer_ptr->output[layer_ptr->num_nodes-1]) + "]";

		layer_stats.setString(layer_stats.getString() + "* Layer Output Vector: \n----------\n" + temp + "\n\n----------------------------------------\n");

		// ---- Setup weight data
		if(!output_layer){
			weight_title.setFont(title_font);
			weight_title.setString("* Layer Weight Matrix:\n----------");
			weight_title.setCharacterSize(max_font_size * 0.66667);
			weight_title.setFillColor(sf::Color::White);
			weight_title.setStyle(sf::Text::Bold );
			weight_title.setPosition(title.getPosition().x, layer_stats.getPosition().y + layer_stats.getGlobalBounds().height + spacer);

			weight_sub.setFont(title_font);
			weight_sub.setString("Pos.(i,j) is weight of connection from\nCurrent layer Node i -> next layer Node j\n\n");
			weight_sub.setCharacterSize(max_font_size * 0.5);
			weight_sub.setFillColor(sf::Color::White);
			weight_sub.setStyle(sf::Text::Bold );
			weight_sub.setPosition(title.getPosition().x + spacer, weight_title.getPosition().y + weight_title.getGlobalBounds().height + spacer);

			// Generate weight matrix string
			temp = "[";
			for(int i=0; i < layer_ptr->weight_rows; i++){
				for(int j=0; j < layer_ptr->weight_cols; j++){
					temp = temp + to_string(layer_ptr->weights[i][j]) + ", ";
				}
				temp = temp + "\n";
			}
			temp.pop_back();
			temp.pop_back();
			temp.pop_back();
			temp = temp +  "]";

			weight_stats.setFont(title_font);
			weight_stats.setString(temp);
			weight_stats.setCharacterSize(max_font_size * 0.83);
			weight_stats.setFillColor(sf::Color::White);
			weight_stats.setStyle(sf::Text::Bold );
			weight_stats.setPosition(x / 2 - weight_stats.getGlobalBounds().width/2, weight_sub.getPosition().y + weight_sub.getGlobalBounds().height + spacer);
		}else{
			weight_title.setFont(title_font);
			weight_title.setString("* Output layer doesn't have weights!:\n----------");
			weight_title.setCharacterSize(max_font_size * 0.66667);
			weight_title.setFillColor(sf::Color::White);
			weight_title.setStyle(sf::Text::Bold );
			weight_title.setPosition(title.getPosition().x, layer_stats.getPosition().y + layer_stats.getGlobalBounds().height + spacer);
		}




		while (layer_data_window.pollEvent(layer_data_event)){
			if (layer_data_event.type == sf::Event::Closed || layer_data_event.key.code == sf::Keyboard::Escape)
			{
				layer_data_window.close();
				stats_window_open = false;
			}
		}

	layer_data_window.clear();

	layer_data_window.draw(title);
	layer_data_window.draw(layer_stats);
	layer_data_window.draw(weight_title);

	if(!output_layer){
		layer_data_window.draw(weight_sub);
		layer_data_window.draw(weight_stats);
	}

	layer_data_window.display();


	}

}