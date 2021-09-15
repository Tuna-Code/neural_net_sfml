#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <string>
#include "nn_display.h"
#include <thread>


using namespace std;

// NN_Display constructor (takes Net_Helper previously loaded w/network)
NN_Display::NN_Display(Net_Helper* net){

	// Update our objects network pointer to the one provided
	this->net = net;

	// Set initial resolution
	win_x = 1440;
	win_y = 900;
	// Set max font size (TODO: Change this based on resolution)
	max_font_size = 28;

	// Grab scaling factors for display (required here on Winows)
	screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());

	// Set initial value of open stats window to false, so we know it needs to be created
	stats_window_open = false;
	node_window_open = false;

	// Pointer to current layer and node position we wish to display
	layer_ptr = NULL;
	node_disp_layer = new int(0);
	node_disp_node = new int(0);

	// Variable to check if displayed layer is output layer
	output_layer = false;

	// Bool to track if our data needs updating + our current displayed layer
	update_stats_window = false;
	update_node_window = false;
	disp_layer = -1;
	cur_node_disp_layer = -1;
	cur_node_disp_node = -1;
}

// Main function to display loaded network visually and dynamically
void NN_Display::display_net(){

// ---------------------------------------- Initial Display Setup ------------------------------------------
	// Temp layer pointer used throughout app
	Layer* l = NULL;
	// How many pixels for spacers
	int spacer = 10;
	// Radius and thickness for each node displayed
	int node_rad = 40;
	int node_thickness = 5;

	// Grab total layers in our network
	int layer_count = net->net->layer_count;

	// Set font size for layer titles;
	int layer_font_size = max_font_size - 5;

	// Text objects for app (titles, menus, buttons, etc...)
	sf::Text title;
	sf::Text options;
	sf::Text forward_text;
	sf::Text reset_text;

	// 2d Text array for titles of our layer serctions
	sf::Text* layer_title = NULL;
	layer_title = new sf::Text[layer_count];

	// Rectangle shapes for menu buttons
	sf::RectangleShape forward_rect;
	sf::RectangleShape reset_rect;

	// Object for our text font and then load. Exit if not found
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	// Background color for Buttons (gray-ish);
	sf::Color bg_button(128, 128, 128);

	// Setup 2d node circle array and allocate
	sf::CircleShape** nodes = NULL;
	// Grab input layer
	l = net->net->input_layer;
	// Allocate a row for each layer
	nodes = new sf::CircleShape*[layer_count];
	for(int i=0; i<layer_count; i++){
		// For each layer, allocate num_nodes shapes for node display
		nodes[i] = new sf::CircleShape[l->num_nodes];
		// Go to next layer
		l = l->next_layer;
	}

	// Set font attributes for main heading
	title.setFont(title_font);
	title.setString("Neural Network Visualizer");
	title.setCharacterSize(max_font_size);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width) / 2, win_y / 50);

	// Set attributes for menu section heading
	options.setFont(title_font);
	options.setString("Options");
	options.setCharacterSize(max_font_size * .66667);
	options.setFillColor(sf::Color::White);
	options.setStyle(sf::Text::Bold | sf::Text::Underlined);
	options.setPosition(20,title.getPosition().y + options.getGlobalBounds().height/2);

	// Verticies for line dividing menu from net display
	sf::Vertex options_line[] =
	{
    sf::Vertex(sf::Vector2f(options.getPosition().x + options.getGlobalBounds().width + spacer, 0)),
    sf::Vertex(sf::Vector2f(options.getPosition().x + options.getGlobalBounds().width + spacer, win_y))
	};

	// Set attributes for forward prop button
	forward_text.setFont(title_font);
	forward_text.setString("-> Prop.");
	forward_text.setCharacterSize(max_font_size * .5);
	forward_text.setFillColor(sf::Color::Black);
	forward_text.setPosition(options.getPosition().x,options.getPosition().y +spacer*2.5 + options.getGlobalBounds().height/2);

	forward_rect.setPosition(forward_text.getPosition().x, forward_text.getPosition().y);
	forward_rect.setSize(sf::Vector2f(forward_text.getGlobalBounds().width + spacer, forward_text.getGlobalBounds().height*2));
	forward_rect.setFillColor(bg_button);
	forward_rect.setOutlineColor(sf::Color::White);
	forward_rect.setOutlineThickness(1);

	// Set attributes for reset button
	reset_text.setFont(title_font);
	reset_text.setString("Reset Net");
	reset_text.setCharacterSize(max_font_size * .5);
	reset_text.setFillColor(sf::Color::Black);
	reset_text.setPosition(options.getPosition().x,forward_rect.getPosition().y +spacer*2.5 + forward_rect.getGlobalBounds().height/2);

	reset_rect.setPosition(reset_text.getPosition().x, reset_text.getPosition().y);
	reset_rect.setSize(sf::Vector2f(reset_text.getGlobalBounds().width + spacer, reset_text.getGlobalBounds().height*2));
	reset_rect.setFillColor(bg_button);
	reset_rect.setOutlineColor(sf::Color::White);
	reset_rect.setOutlineThickness(1);

	// Create window and set position to far left and set icon
	window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");
	window.setPosition(sf::Vector2(20, 20));
	platform.setIcon(window.getSystemHandle());

// ---------------------------------------- Generate Layer Titles and Positions ----------------------------

	// Generate initial title and position for input layer heading
	layer_title[0].setString("Layer: " + to_string(0));
	layer_title[0].setFont(title_font);
	layer_title[0].setCharacterSize(layer_font_size);
	layer_title[0].setFillColor(sf::Color::White);
	layer_title[0].setStyle(sf::Text::Bold | sf::Text::Underlined);
	layer_title[0].setPosition((options.getPosition().x + options.getGlobalBounds().width + spacer*2) + layer_title[0].getGlobalBounds().width/2, (title.getPosition().y + title.getGlobalBounds().height  + spacer*2));

	// For each remaining layer (1 to end) generate it's title and set position based on previous layer title
	for(int i = 1; i < layer_count; i++){
		layer_title[i].setString("Layer: " + to_string(i));
		layer_title[i].setFont(title_font);
		layer_title[i].setCharacterSize(layer_font_size);
		layer_title[i].setFillColor(sf::Color::White);
		layer_title[i].setStyle(sf::Text::Bold | sf::Text::Underlined);
		layer_title[i].setPosition(layer_title[i-1].getPosition().x + ((win_x - (options.getPosition().x + options.getGlobalBounds().width + spacer*2)) / layer_count) + spacer, (title.getPosition().y + title.getGlobalBounds().height  + spacer*2));
	}

// ---------------------------------------- Generate and assign node positions -----------------------------

	// Grab out input layer and set our new 'centers' for node alignment
	l = net->net->input_layer;
	int center_x = 0;
	int start_y = (win_y - (title.getPosition().y + title.getGlobalBounds().height  + spacer*2))/2;

	// Loop through each layer
	for(int i = 0; i < layer_count; i++){

		// Grab the center of our layer headings (node center alignment)
		center_x = layer_title[i].getPosition().x + layer_title[i].getGlobalBounds().width/2 - node_rad;
		// Decrement the starting y position of the node based on the node count and radius
		start_y -= (l->num_nodes-1)*2*node_rad;
		// Loop through each node in our layer and set its attributes
		for(int j=0; j < l->num_nodes; j++){
			nodes[i][j].setRadius(node_rad);
			nodes[i][j].setFillColor(sf::Color::Transparent);
			nodes[i][j].setOutlineColor(sf::Color::White);
			nodes[i][j].setOutlineThickness(node_thickness);
			nodes[i][j].setPosition(center_x, start_y + j*4*node_rad);
		}
		// Reset our starting y position and move to next layer
		start_y = (win_y - (title.getPosition().y + title.getGlobalBounds().height  + spacer*2))/2;
		l = l->next_layer;
	}

//---------------------- Generate our line positions for weight representation -----------------------------

	// Initialize variables for line generation
	int cur_layer = 0;
	int cur_line = 0;
	int num_lines = 0;

	// Allocate our 3d array to store our lines
	sf::Vertex*** test_lines = NULL;
	test_lines = new sf::Vertex**[layer_count-1];

	// Shape vars to make math easier in line generation
	sf::CircleShape cur_node;
	sf::CircleShape dest_node;

	// Start on the input layer
	l = net->net->input_layer;
	// Setup our 3d array for lines (2 x 2 matrix but each entry is a vertex array of size 2 (start and end verticies))
	// x = layer number, i = line for that layer connection descending
	for(int x = 0; x < layer_count-1; x++){

		// Reset our line per layer counter to 0
		cur_line = 0;
		// Grab num nodes in current and next layer
		cur_layer = l->num;
		num_lines = l->num_nodes*l->next_layer->num_nodes;
		// Allocate position x to hold num_lines number of vertex pairs
		test_lines[x] = new sf::Vertex*[num_lines];

		// Allocate space for two verticies for each line in each layer
		for(int i=0; i < num_lines; i++){
			test_lines[x][i] = new sf::Vertex[2];
		}

		// Generate vertex positions for all lines
		// Loop through each node in our current layer and assgn it
		for(int i = 0; i < l->num_nodes; i++){
			cur_node = nodes[cur_layer][i];
			// For each node in our current layer, create a line connecting it to every node in our next layer (fully connected)
			for(int j = 0; j < l->next_layer->num_nodes; j++){
				// Grab destination node from next layer
				dest_node = nodes[cur_layer+1][j];
				// For layer x, current line cur_line, set start and end points (connecting cur_node to dest_node)
				test_lines[x][cur_line][0].position = sf::Vector2f(cur_node.getPosition().x + 2*node_rad + node_thickness , cur_node.getPosition().y + node_rad);
				test_lines[x][cur_line][0].color = sf::Color::Green;
				test_lines[x][cur_line][1].position = sf::Vector2f(dest_node.getPosition().x - node_thickness ,dest_node.getPosition().y + node_rad);
				test_lines[x][cur_line][1].color = sf::Color::Red;
				// Increment our line counter
				cur_line++;
			}
		}
		// Go to next layer in linked list
		l = l->next_layer;
	}

// ---------------------------------------- Open Main Loop --------------------------------------------------
	// Window open loop
	while (window.isOpen()){

		// Grab event
		while (window.pollEvent(event)){

			// If closing
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			// If clicked, check if clicked on objects
			if(event.type == sf::Event::MouseButtonPressed){

				// Iterate through to see if layer had a clicked object
				l = net->net->input_layer;
				for(int i = 0; i < layer_count; i++){
					// Check if clicked on Layer title
					if(layer_title[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){

							if(i != disp_layer){
								// Set our display pointer to input layer and increment linked list until arrived on chosen layer
								layer_ptr = net->net->input_layer;
								if(i != 0){
									for(int j = 0; j < i; j++){
										if(layer_ptr->next_layer != NULL){
											layer_ptr = layer_ptr->next_layer;
										}
									}
								}
								// If layer is output layer, mark flag for special rendering (no weights)
								if(layer_ptr == net->net->last_layer){
									output_layer = true;
								}
								else{
									output_layer = false;
								}
								update_stats_window = true;
								// If our window isn't open, open it. Otherwise updating the pointer above will change stats
								// Opens in new thread
								if(!stats_window_open){
									t1 = new std::thread(&NN_Display::display_layer_stats, this);
									stats_window_open = true;
								}
								disp_layer = i;
							}
					}
					// Iterate through layer node shapes and check if clicked on them.
					for(int j=0; j < l->num_nodes; j++){
						if(nodes[i][j].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){

							if((cur_node_disp_layer != i) || (cur_node_disp_node != j)){
								// Update our node layer/position pointers:D
								memcpy(node_disp_layer,&i,sizeof(int));
								memcpy(node_disp_node,&j,sizeof(int));

								update_node_window = true;

								// Check if we already have a node window open
								if(!node_window_open){
									t2 = new std::thread(&NN_Display::display_node_stats, this);
									node_window_open = true;
								}
								cur_node_disp_layer = *node_disp_layer;
								cur_node_disp_node = *node_disp_node;
							}

						}
					}
					l = l->next_layer;
				}

				// Check if clicked on control buttons
				// If forward prop is clicked
				if(forward_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
					net->net->forward_prop();
					update_stats_window = true;
					update_node_window = true;

				}

				// If reset is clicked
				if(reset_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
					net->reset_network();
					update_stats_window = true;
					update_node_window =true;
				}

			}

		}

// ------- Display all of our generated items ---------

	// Clear current frame
	window.clear();

	// Loop through our layers and draw their headings
	for(int i = 0; i < layer_count; i++){
		window.draw(layer_title[i]);
	}

	// Loop through each layer and draw nodes in layer
	l = net->net->input_layer;
	for(int i = 0; i < layer_count; i++){
		// Draw node j from layer i
		for(int j=0; j < l->num_nodes; j++){
			window.draw(nodes[i][j]);
		}
		l = l->next_layer;
	}

	// Draw our title text(s)
	window.draw(title);
	window.draw(options);

	// Draw our button rectangles (done before text)
	window.draw(forward_rect);
	window.draw(reset_rect);

	// Draw our button text
	window.draw(forward_text);
	window.draw(reset_text);

	// Draw line dividing options and display sections
	window.draw(options_line, 2, sf::Lines);

	// Loop through each layer (except output) and draw our connecting lines from 3d array
	l = net->net->input_layer;
	for(int i = 0; i < layer_count-1; i++){
		// Each layer contains nodes*next_nodes lines to connect them
		for(int j = 0; j < l->num_nodes*l->next_layer->num_nodes; j++){
			// Draw line j from layer i
			window.draw(test_lines[i][j], 2, sf::Lines);
		}
		l = l->next_layer;
	}

	// Display our window
	window.display();
	}
}



// Function to open window displaying stats of selected node. Called in new thread
void NN_Display::display_node_stats(){


	double x = 500;
	double y = 400;

	int spacer = 10;

	sf::Text title;
	sf::Text node_data;

	Layer* layer = net->net->input_layer;
	// Object for our text font and then load. Exit if not found
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf")){
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	title.setFont(title_font);
	title.setCharacterSize(max_font_size * 0.83);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);

	node_data.setFont(title_font);
	node_data.setCharacterSize(max_font_size * 0.6667);
	node_data.setFillColor(sf::Color::White);
	node_data.setStyle(sf::Text::Bold );



	// Create new window and set to right of main window
	node_data_window.create(sf::VideoMode(x * screenScalingFactor, y * screenScalingFactor), "");
	node_data_window.setPosition(sf::Vector2((int)window.getPosition().x + (int)window.getSize().x - (int)x, (int)window.getPosition().y + (int)window.getSize().y + 25));

	// ---------------------------------------- Main Open Loop --------------------------------------------------
	int layer_num = 0;
	int node_num = 0;
	while (node_data_window.isOpen()){

		// If a data update occurred, recompute our displayed info
		if(update_node_window){

			// Copy our pointers value to local variable to prevent race condition
			memcpy(&layer_num, node_disp_layer, sizeof(int));
			memcpy(&node_num, node_disp_node, sizeof(int));

			// If our layer pointer isn't pointing to chosen node, reset and increment until match
			if((layer->num) != layer_num){
				layer = net->net->input_layer;
				while((layer->num) != layer_num){
					layer = layer->next_layer;
				}
			}

			//printf("Display Data for Layer %i Node %i Current Layer: %i\n", layer_num, node_num, layer->num);
			title.setString("Node: " + to_string(node_num) + " Layer: " + to_string(layer_num));
			title.setPosition(x/2 - title.getGlobalBounds().width/2, y / 50);

			node_data.setString("Node Actv. Func.:\n" + layer->actv_func + "\n---------\n\n");
			node_data.setString(node_data.getString() + "Node Input:\n" + to_string(layer->input[node_num]) + "\n---------\n\n");
			node_data.setString(node_data.getString() + "Node Output:\n" + to_string(layer->output[node_num]) + "\n---------\n\n");



			node_data.setPosition(spacer, title.getPosition().y + title.getGlobalBounds().height + spacer*2);

			update_node_window = false;
		}

		// Get current events in window
		while (node_data_window.pollEvent(node_data_event)){

			// If closing, close and update object that window is closed
			if (node_data_event.type == sf::Event::Closed || node_data_event.key.code == sf::Keyboard::Escape)
			{
				node_data_window.close();
				node_window_open = false;

			}
		}


		node_data_window.clear();

		node_data_window.draw(title);
		node_data_window.draw(node_data);
		node_data_window.display();
	}


}
// Function to open window displaying stats of selected layer (from layer_ptr). Called in new thread
void NN_Display::display_layer_stats(){

// ---------------------------------------- Initial Setuo ---------------------------------------------------

	// Set initial resolution
	double x = 800;
	double y = 700;

	// Set pixel spacer
	int spacer = 20;

	// Text objects for displaying layer info
	sf::Text title;
	sf::Text layer_stats;
	sf::Text weight_title;
	sf::Text weight_sub;
	sf::Text weight_stats;

	// Font object used in window. Fail if unable to load
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	// Create new window and set to right of main window
	layer_data_window.create(sf::VideoMode(x * screenScalingFactor, y * screenScalingFactor), "");
	layer_data_window.setPosition(sf::Vector2((int)window.getPosition().x + (int)window.getSize().x + 10, (int)window.getPosition().y));


	// Set text attrubutes for items that don't need dynamically updated
	title.setFont(title_font);
	title.setCharacterSize(max_font_size * 0.83);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(20, y / 50);

	layer_stats.setFont(title_font);
	layer_stats.setCharacterSize(max_font_size * 0.66667);
	layer_stats.setFillColor(sf::Color::White);
	layer_stats.setStyle(sf::Text::Bold );
	layer_stats.setPosition(title.getPosition().x, title.getPosition().y + title.getGlobalBounds().height + spacer);

	weight_title.setFont(title_font);
	weight_title.setCharacterSize(max_font_size * 0.66667);
	weight_title.setFillColor(sf::Color::White);
	weight_title.setStyle(sf::Text::Bold );

	weight_sub.setFont(title_font);
	weight_sub.setString("Pos.(i,j) is weight of connection from\nCurrent layer Node i -> next layer Node j\n\n");
	weight_sub.setCharacterSize(max_font_size * 0.5);
	weight_sub.setFillColor(sf::Color::White);
	weight_sub.setStyle(sf::Text::Bold );

	weight_stats.setFont(title_font);
	weight_stats.setCharacterSize(max_font_size * 0.83);
	weight_stats.setFillColor(sf::Color::White);
	weight_stats.setStyle(sf::Text::Bold );

// ---------------------------------------- Main Open Loop --------------------------------------------------
	while (layer_data_window.isOpen()){
		if(update_stats_window){
			//cout << "UPDATE DETECTED!\n";
			// Generate our titles and stats from updated pointer
			title.setString("Layer " + to_string(layer_ptr->num) + " Stats");
			layer_data_window.setTitle(title.getString());
			layer_stats.setString("\n* Activation Function: " + layer_ptr->actv_func + "\n----------\n\n");
			layer_stats.setString(layer_stats.getString() + "* Node Count: " + to_string(layer_ptr->num_nodes) + "\n----------\n\n");

			// Generate layer input vector into string
			string temp = "[";
			for(int i=0; i < layer_ptr->num_nodes-1; i++){
				temp = temp + to_string(layer_ptr->input[i]) + ", ";
			}
			temp = temp + to_string(layer_ptr->input[layer_ptr->num_nodes-1]) + "]";
			// Add string form to stats
			layer_stats.setString(layer_stats.getString() + "* Layer Input Vector (raw): \n----------\n" + temp + "\n\n");

			// Generate layer output vector into string
			temp = "[";
			for(int i=0; i < layer_ptr->num_nodes-1; i++){
				temp = temp + to_string(layer_ptr->output[i]) + ", ";
			}
			temp = temp + to_string(layer_ptr->output[layer_ptr->num_nodes-1]) + "]";
			// Add string form to stats
			layer_stats.setString(layer_stats.getString() + "* Layer Output Vector: \n----------\n" + temp + "\n\n----------------------------------------\n");

			// Generate weight matrix in string form if not on output layer (no weights)
			if(!output_layer){

				// Update title and sub-headings
				weight_title.setString("* Layer Weight Matrix:\n----------");
				weight_title.setPosition(title.getPosition().x, layer_stats.getPosition().y + layer_stats.getGlobalBounds().height + spacer);
				weight_sub.setPosition(title.getPosition().x + spacer, weight_title.getPosition().y + weight_title.getGlobalBounds().height + spacer);

				// Generate weight matrix into string
				temp = "[";
				for(int i=0; i < layer_ptr->weight_rows; i++){
					for(int j=0; j < layer_ptr->weight_cols; j++){
						temp = temp + to_string(layer_ptr->weights[i][j]) + ", ";
					}
					temp = temp + "\n";
				}

				// In case layer_ptr was changed mid render, preventing segfault
				if(temp.length() > 3){
					temp.pop_back();
					temp.pop_back();
					temp.pop_back();
					temp = temp +  "]";
				}

				// Append weight matrix to weight data and update position
				weight_stats.setString(temp);
				weight_stats.setPosition(x / 2 - weight_stats.getGlobalBounds().width/2, weight_sub.getPosition().y + weight_sub.getGlobalBounds().height + spacer);
			}
			// If on output layer, notify user
			else{
				weight_title.setString("* Output layer doesn't have weights!:\n----------");
				weight_title.setPosition(title.getPosition().x, layer_stats.getPosition().y + layer_stats.getGlobalBounds().height + spacer);
			}
			update_stats_window = false;
		}

		// Get current events in window
		while (layer_data_window.pollEvent(layer_data_event)){

			// If closing, close and update object that window is closed
			if (layer_data_event.type == sf::Event::Closed || layer_data_event.key.code == sf::Keyboard::Escape)
			{
				layer_data_window.close();
				stats_window_open = false;
			}
		}

// ------- Display all of our generated items ---------

		// Clear our current window
		layer_data_window.clear();

		// Draw our titles/stats
		layer_data_window.draw(title);
		layer_data_window.draw(layer_stats);
		layer_data_window.draw(weight_title);

		// If no output layer, display weight data
		if(!output_layer){
			layer_data_window.draw(weight_sub);
			layer_data_window.draw(weight_stats);
		}

		// Dislay window with drawn objects
		layer_data_window.display();
	}

}