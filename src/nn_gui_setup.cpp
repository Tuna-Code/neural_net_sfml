#include "nn_gui_setup.h"
#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <string>

using namespace std;

NN_gui_setup::NN_gui_setup(Net_Helper* net)
{
	// Assign our NN helper obj
	this->net = net;
	win_x = 1920;
	win_y = 1080;
	// Window dimensionts

	// in Windows at least, this must be called before creating the window
	screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	extra_text = "";
	net_loaded = false;
}

// Open window and display interactive setup for node
void NN_gui_setup::display_int_setup()
{
	win_x = 1000;
	win_y = 500;


	// Create text and font objs
	sf::Text title;
	sf::Text cur_text;
	sf::String playerInput;
	sf::Text playerText;
	sf::Text layer_num_text;
	sf::Text layer_data;
	sf::Text input_data;
	sf::Text weight_data;
	sf::Font title_font;
	sf::RectangleShape rectangle;

	// Load font from file
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	// Set font attributes for heading
	title.setFont(title_font);
	title.setString("Neural Network Interactive Setup");
	title.setCharacterSize(25);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width / 2),  win_y / 50);

	// Set font attributes for question
	cur_text.setFont(title_font);
	cur_text.setCharacterSize(20);
	cur_text.setString("How many layers are in the network?: ");
	cur_text.setFillColor(sf::Color::White);
	//cur_text.setStyle(sf::Text::Bold);
	cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), title.getPosition().y + cur_text.getGlobalBounds().height + 35);

	// Set attributes for rectangle (text box)

	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(20, cur_text.getPosition().y + cur_text.getGlobalBounds().height * 2 + 20);
	rectangle.setSize(sf::Vector2f(win_x - 40, win_y - rectangle.getPosition().y - cur_text.getPosition().y + cur_text.getGlobalBounds().height * 2 + 80));

	// Set text attributes for input
	// middle pos playerText.setPosition((rectangle.getSize().x/2), rectangle.getPosition().y + (rectangle.getGlobalBounds().height/2));
	//playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + rectangle.getSize().y - 100);
	playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - playerText.getGlobalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y - 50);
	playerText.setFont(title_font);
	playerText.setCharacterSize(40);
	playerText.setFillColor(sf::Color::White);
	bool display_player_text = true;

	// Set text attributes displaying input data
	layer_num_text.setFont(title_font);
	layer_num_text.setCharacterSize(25);
	layer_num_text.setString("- Layers in Network: ");
	layer_num_text.setStyle(sf::Text::Bold);
	layer_num_text.setFillColor(sf::Color::White);
	layer_num_text.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - layer_num_text.getGlobalBounds().width / 2, rectangle.getPosition().y + 20);
	bool display_num_layers = true;

	// Set text attributes displaying layer data
	layer_data.setPosition((rectangle.getPosition().x + 20), layer_num_text.getPosition().y + layer_num_text.getGlobalBounds().height -20);
	layer_data.setFont(title_font);
	layer_data.setCharacterSize(25);
	layer_data.setString("- Layer 0: ");
	layer_data.setStyle(sf::Text::Bold);
	layer_data.setFillColor(sf::Color::White);
	bool display_layer_data = false;

	// Set text attributes displaying initial data
	input_data.setPosition((rectangle.getPosition().x + 20), layer_data.getPosition().y + layer_data.getGlobalBounds().height + 40);
	input_data.setFont(title_font);
	input_data.setCharacterSize(25);
	input_data.setString("- Initial Values:\n");
	input_data.setStyle(sf::Text::Bold);
	input_data.setFillColor(sf::Color::White);
	bool display_input_data = false;

	// Set text attributes displaying initial data
	//weight_data.setPosition((rectangle.getPosition().x + layer_num_text.getGlobalBounds().width+20), layer_data.getPosition().y+layer_data.getGlobalBounds().height + 40);
	//weight_data.setPosition((rectangle.getPosition().x + rectangle.getGlobalBounds().width/2 - weight_data.getGlobalBounds().width/2), layer_.getPosition().y+layer_data.getGlobalBounds().height + 40);

	weight_data.setFont(title_font);
	weight_data.setCharacterSize(25);
	weight_data.setString("- Weight Matrix for Layer 0:\n\n------------------------------\n");
	weight_data.setStyle(sf::Text::Bold);
	weight_data.setFillColor(sf::Color::White);
	weight_data.setPosition((rectangle.getPosition().x + rectangle.getGlobalBounds().width / 2 - weight_data.getGlobalBounds().width / 2), layer_num_text.getPosition().y);

	bool display_weight_data = false;

	bool display_all = false;
	// Setup initial variables for creating network

	int phase = 0;
	int layer_count = 0;
	int* node_count = NULL;
	int init_val_counter = 0;
	int layer_node_counter = 0;
	int init_weight_counter = 0;

	double* input = NULL;

	//int node_layer_counter = 0;

	int weight_rows = 0;
	int weight_cols = 0;
	double** weights = NULL;
	bool layer_weights_alloc = false;
	int conn_weight_row = 0;
	int conn_weight_col = 0;
	/*
	int cur_weight_row = 0;
	int cur_weight_col = 0;*/

	//	string func = "Sigmoid";

	// Prompt for layer count and allocate array
	//	cout << "How many layers will be in the network?: ";
	//	cin >> layer_count;
	//	node_count = new int[layer_count];
	//	cout << endl;

\
	//view.setSize(window.getSize().x,window.getSize().y);

	//window.setView(view);
	platform.setIcon(window.getSystemHandle());
	//window.setView(view);
		window.create(sf::VideoMode(win_x * screenScalingFactor, (win_y + 75) * screenScalingFactor), "Neural Netowrk Interactive Setup");





	sf::Vector2u resolution;
	// Rendering loop
	while (window.isOpen())
	{

		// Grab current event
		while (window.pollEvent(event))
		{

			// If click on exit, quit
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			// If text is entered, update string and go to render
			if (event.type == sf::Event::TextEntered)
			{
				// Verify inputis valid text, if so then add it
				if(isprint(event.text.unicode)){
					playerInput += event.text.unicode;
					playerText.setString(playerInput);
					playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - playerText.getGlobalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y - 50);

				}
				// If backspace is detected and string has input
				if(event.text.unicode == '\b' && (playerInput.getSize() > 0)){
					playerInput.erase(playerInput.getSize() - 1, 1);
					playerText.setString(playerInput);
					playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - playerText.getGlobalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y - 50);

				}
				//playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + 50);
				/*if (phase == 4)
				{
					playerText.setPosition(cur_text.getPosition().x + cur_text.getGlobalBounds().width + 20, cur_text.getPosition().y + (cur_text.getGlobalBounds().height / 2.25));
				}
				else
				{
					playerText.setPosition(cur_text.getPosition().x + cur_text.getGlobalBounds().width + 20, cur_text.getPosition().y - (cur_text.getGlobalBounds().height / 2));
				}*/
				// bottom pos playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + rectangle.getSize().y - 100);
				// middle pos playerText.setPosition((rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + (rectangle.getGlobalBounds().height/2));
			}
			// If backspace is entered, delete character and re-render
			/*if (event.key.code == sf::Keyboard::BackSpace)
			{
				playerInput = playerInput.substring(0, playerInput.getSize() - 1);
				//playerInput.toAnsiString().pop_back();

				playerText.setString(playerInput);

			}*/

			// Check for key press event to prevent multiple instances being detected
			if (event.type == sf::Event::KeyPressed)
			{
				// If escape event is dectected, exit
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				// If F# buttons are pressed, change resolution
				if (event.key.code == sf::Keyboard::F1)
				{
					win_x = 1280;
					win_y = 720;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F2)
				{
					win_x = 1920;
					win_y = 1080;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F3)
				{
					win_x = 1440;
					win_y = 900;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F4)
				{
					win_x = 800;
					win_y = 600;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F5)
				{
					win_x = 1024;
					win_y = 768;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F6)
				{
					win_x = 420;
					win_y = 480;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F7)
				{
					win_x = 1000;
					win_y = 1000;

					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F8)
				{
					win_x = 500;
					win_y = 350;



					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}
				if (event.key.code == sf::Keyboard::F9)
				{
					win_x = 200;
					win_y = 150;



					resolution.x = win_x;
					resolution.y = win_y;

					resize_window(&window, resolution);

					goto render_setup_screen;
				}

				// Phase 1 of questions
				if (phase == 0)
				{
					// If enter is pressed, save input and go to next question
					if (event.key.code == sf::Keyboard::Enter)
					{

						// Grab current input, convert it to string and then to int
						layer_count = stoi(playerInput.toWideString());
						node_count = new int[layer_count];

						// Update our info and set bool to display
						layer_num_text.setString(layer_num_text.getString() + to_string(layer_count) + "\n---------------------");
						layer_num_text.setPosition(rectangle.getPosition().x + 10, layer_num_text.getPosition().y);

						display_num_layers = true;

						// Clear out displayed input
						playerInput = "";
						playerText.setString("");

						// Update question string and proceed
						cur_text.setString("How many nodes are in Layer #0 (The input Layer)?");
						//cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), title.getPosition().y + cur_text.getGlobalBounds().height + 35);
						cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), cur_text.getPosition().y);

						// Update to next Phase
						phase++;
						layer_data.setPosition((rectangle.getPosition().x + 20), layer_num_text.getPosition().y + layer_num_text.getGlobalBounds().height + 10);
						goto render_setup_screen;
					}
				}
				// Phase 1 of questions
				if (phase == 1)
				{
					// If enter is pressed, save input and go to next question
					if (event.key.code == sf::Keyboard::Enter)
					{

						if (layer_node_counter < layer_count)
						{
							node_count[layer_node_counter] = stoi(playerInput.toWideString());

							// Clear out displayed input
							playerInput = "";
							playerText.setString("");
							cur_text.setString("How many nodes are in Layer #" + to_string(layer_node_counter + 1));
							cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), title.getPosition().y + cur_text.getGlobalBounds().height + 35);

							// Update string for displaying layer stats
							if (layer_node_counter == 0)
							{
								layer_data.setString(layer_data.getString() + to_string(node_count[layer_node_counter]) + " Nodes\n");
								display_layer_data = true;
							}

							if (layer_node_counter != layer_count && layer_node_counter != 0)
							{
								layer_data.setString(layer_data.getString() + "- Layer " + to_string(layer_node_counter) + ": " +  to_string(node_count[layer_node_counter]) + " Nodes\n");
							}

							layer_node_counter++;
							// Check if we are finished inputting node counts
							if (layer_node_counter == layer_count)
							{
								phase = 2;
								layer_data.setString(layer_data.getString() + "---------------------");
								input_data.setPosition((rectangle.getPosition().x + 20), layer_data.getPosition().y + layer_data.getGlobalBounds().height + 20);
								display_input_data = true;
								cur_text.setString("Please enter the initial input vector for L_0 (" + to_string(node_count[0]) + " nodes):");
								cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), cur_text.getPosition().y);
								input = new double[node_count[0]];
								playerInput = "";
								playerText.setString("");
							}

							goto render_setup_screen;
						}
					}
				}
				if (phase == 2)
				{

					if (event.key.code == sf::Keyboard::Enter)
					{
						if (init_val_counter < node_count[0])
						{
							input[init_val_counter] = stod(playerInput.toWideString());
							//cout << input[init_val_counter]  << endl;
							input_data.setString(input_data.getString() + "Input " + to_string(init_val_counter) + ": " + to_string(input[init_val_counter]) + "\n");
							display_input_data = true;
							playerInput = "";
							playerText.setString("");
							init_val_counter++;
						}
						if (init_val_counter == node_count[0])
						{
							phase = 3;
							input_data.setString(input_data.getString() + "\n---------------------");
							//cout << "Finished init vals!\n";
							playerInput = "";
							playerText.setString("");
							cur_text.setString("Please enter weights for Layer 0 (Connecting N_0 to N_0):");
							cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), cur_text.getPosition().y);

							display_weight_data = true;
						}
						goto render_setup_screen;
					}
				}
				// Get weight matrix for each layer
				if (phase == 3)
				{
					if (event.key.code == sf::Keyboard::Enter)
					{

						// Get weights for each layer
						if (init_weight_counter < layer_count - 1)
						{

							if (!layer_weights_alloc)
							{

								weight_rows = node_count[init_weight_counter];
								weight_cols = node_count[init_weight_counter + 1];

								weights = new double*[weight_rows];
								for (int j = 0; j < weight_rows; j++)
								{
									weights[j] = new double[weight_cols];
								}
								//printf("Allocating Layer %i matrix to size %i x %i\n", init_weight_counter, weight_rows, weight_cols);
								layer_weights_alloc = true;
							}
							//printf("Adding %f to position %i %i on layer %i !\n", stod(playerInput.toWideString()), conn_weight_row, conn_weight_col, init_weight_counter);
							weights[conn_weight_row][conn_weight_col] = stod(playerInput.toWideString());
							//cout << "! " << weights[conn_weight_row][conn_weight_col] << " !\n";

							// If we aren't finished adding colums to row, increment column count
							if (conn_weight_col != node_count[init_weight_counter + 1] - 1)
							{
								weight_data.setString(weight_data.getString() + "    " + to_string(weights[conn_weight_row][conn_weight_col]));
								conn_weight_col++;
							}
							// If we are finished grabbing colums for row, increment row and reset column
							else
							{
								weight_data.setString(weight_data.getString() + "    " + to_string(weights[conn_weight_row][conn_weight_col]) + "\n");
								conn_weight_col = 0;

								conn_weight_row++;
							}
							// Finished grabbing layer weights
							if (conn_weight_row == node_count[init_weight_counter])
							{

								conn_weight_row = 0;
								conn_weight_col = 0;
								net->net->add_layer(init_weight_counter, node_count[init_weight_counter], "Null", weights, weight_rows, weight_cols);

								weight_rows = 0;
								weight_cols = 0;
								layer_weights_alloc = false;
								init_weight_counter++;
								if (init_weight_counter != layer_count - 1)
								{
									weight_data.setString(weight_data.getString() + "Weight Matrix for Layer " + to_string(init_weight_counter) + ":\n\n------------------------------\n");
								}
							}
							if (init_weight_counter == layer_count - 1)
							{
								weights = NULL;
								weight_rows = 0;
								weight_cols = 0;
								net->net->add_layer(init_weight_counter, node_count[init_weight_counter], "Null", weights, weight_rows, weight_cols);
								phase = 4;
								// assign our inputs to input layer
								for (int i = 0; i < node_count[0]; i++)
								{
									net->net->input_layer->input[i] = input[i];
								}


								playerInput = "";
								playerText.setString("");
								cur_text.setString("Finished with weights! Please verify network data...\nPress 'y' to confirm and proceed, press 'n' to start again: ");
								cur_text.setPosition(cur_text.getPosition().x, cur_text.getPosition().y - 20);
								goto render_setup_screen;
							}
							//cur_text.setString("Please enter weights for Layer 0 (Connecting N0 to N0):");
							cur_text.setString("Please enter weights for Layer " + to_string(init_weight_counter) + " (Connecting N_" + to_string(conn_weight_row) + " to N_" + to_string(conn_weight_col) + "):");
							//weight_data.setString(weight_data.getString() + "     " + playerInput.toWideString() + ": \n");

							playerInput = "";
							playerText.setString("");
							//init_weight_counter++;
						}

						goto render_setup_screen;
					}
				}
				if (phase == 4)
				{
					if (event.key.code == sf::Keyboard::Enter)
					{
						cout << playerText.getString().toAnsiString() << endl;

						// If y is entered, accept network and move to next menu
						if(playerText.getString().toAnsiString() == "y"){
							net_loaded = true;
							playerInput = "";
							playerText.setString("");
							cur_text.setString("Network Acepted! Loading network...");
							cur_text.setPosition(cur_text.getGlobalBounds().width / 20, win_y / 8);
							display_player_text = false;

							// Draw setup page
							window.clear();
							window.draw(rectangle);
							window.draw(title);
							if (display_player_text)
							{
								window.draw(playerText);
							}
							window.draw(cur_text);
							if (display_num_layers || display_all)
							{
								window.draw(layer_num_text);
							}
							if (display_layer_data || display_all)
							{
								window.draw(layer_data);
							}
							if (display_input_data || display_all)
							{
								window.draw(input_data);
							}
							if (display_weight_data || display_all)
							{
								window.draw(weight_data);
							}
							window.display();
							sf::sleep(sf::milliseconds(500));
							window.close();
							display_title();
						}
						if(playerText.getString().toAnsiString() == "n"){
							net_loaded = false;
							playerInput = "";
							playerText.setString("");
							cur_text.setString("Network Declined! Clearing memory and building new network...");
							cur_text.setPosition(cur_text.getGlobalBounds().width / 20, win_y / 8);
							display_player_text = false;
							// Draw setup page
							window.clear();
							window.draw(rectangle);
							window.draw(title);
							if (display_player_text)
							{
								window.draw(playerText);
							}
							window.draw(cur_text);
							if (display_num_layers || display_all)
							{
								window.draw(layer_num_text);
							}
							if (display_layer_data || display_all)
							{
								window.draw(layer_data);
							}
							if (display_input_data || display_all)
							{
								window.draw(input_data);
							}
							if (display_weight_data || display_all)
							{

								window.draw(weight_data);
							}
							window.display();
							// Clear out NN layers from memory
							Layer* cur = net->net->input_layer;
							Layer* next = cur->next_layer;
							int i = 0;
							while(cur != NULL){
								cout << i << endl;
								cur->next_layer = NULL;
								cur->prev_layer = NULL;


								delete cur->input;
								delete cur->output;
								delete cur->weights;

								delete cur;

								cur = next;
								if(cur != NULL){
									next = cur->next_layer;
								}
							}
							net->net->cur_layer = NULL;
							net->net->last_layer = NULL;
							net->net->input_layer = NULL;
							net->net->layer_count = 0;

							sf::sleep(sf::milliseconds(500));
							window.close();
							display_int_setup();
						}


					}
				}
			}
		}
	render_setup_screen:
		// Draw setup page
		window.clear();
		window.draw(rectangle);
		window.draw(title);
		if (display_player_text)
		{
			window.draw(playerText);
		}
		window.draw(cur_text);
		if (display_num_layers || display_all)
		{
			window.draw(layer_num_text);
		}
		if (display_layer_data || display_all)
		{
			window.draw(layer_data);
		}
		if (display_input_data || display_all)
		{
			window.draw(input_data);
		}
		if (display_weight_data || display_all)
		{
			window.draw(weight_data);
		}
		window.display();
	}
}

// Display our title with interactive options
void NN_gui_setup::display_title()
{
	win_x = 900;
	win_y = 600;
	// Create font and text objects
	sf::Text title;
	sf::Text text;
	sf::Font title_font;
	sf::Text opt_text;
	sf::Text load_network;

	sf::Color c;

	sf::RectangleShape bg;
	sf::RectangleShape settings;






	sf::Texture bg_texture;
	sf::Texture settings_texture;

	bg_texture.loadFromFile("content/menu_bg.jpg");
	settings_texture.loadFromFile("content/settings.jpg");

	// Load font from file
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	// Set font attributes for option text
	opt_text.setFont(title_font);
	opt_text.setString("1: Load network info manually\n\n2: Load pre-made networks (for testing)");
	opt_text.setCharacterSize(19);
	opt_text.setFillColor(sf::Color::White);
	//opt_text.setPosition(text.getPosition().x + opt_text.getGlobalBounds().width/10 , text.getPosition().y + opt_text.getGlobalBounds().height);
	opt_text.setPosition(20, win_y - 2 * opt_text.getGlobalBounds().height + 70);

	//net_loaded = true;

	// Set font attributes for lower text
	load_network.setFont(title_font);
	load_network.setString("\n3: Process loaded network");
	load_network.setCharacterSize(19);
	if(net_loaded){
		//win_x += 70;
		extra_text = " (from input network)";
		c = sf::Color::White;
		load_network.setFillColor(c);
		load_network.setStyle(sf::Text::Bold);
		load_network.setString(load_network.getString() + extra_text);
		load_network.setPosition(20,opt_text.getPosition().y + opt_text.getGlobalBounds().height + 5);

	}
	else{
		extra_text = " (no network loaded...)";
		c = sf::Color(100,100,100,255);
		load_network.setFillColor(c);
		load_network.setStyle(sf::Text::Bold | sf::Text::StrikeThrough);
		load_network.setString(load_network.getString() + extra_text);
		load_network.setPosition(20,opt_text.getPosition().y + opt_text.getGlobalBounds().height + 5);

	}

	// Set font attributes for heading
	title.setFont(title_font);
	title.setString("Neural Network Graphical Interface");
	title.setCharacterSize(25);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width) / 2, win_y / 50);



	// Set font attributes for lower text
	text.setFont(title_font);
	text.setString("Please select from an option below:");
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(win_x / 2 - (text.getGlobalBounds().width / 2), title.getPosition().y + text.getGlobalBounds().height + 35);

	//bg.setFillColor(sf::Color::Transparent);
	//bg.setOutlineColor(sf::Color::White);
	//bg.setOutlineThickness(5);
	bg.setTexture(&bg_texture);
	bg.setPosition(10, text.getPosition().y + text.getGlobalBounds().height - 35);
	bg.setSize(sf::Vector2f(win_x - bg.getPosition().x*2 , load_network.getPosition().y - load_network.getGlobalBounds().height - opt_text.getGlobalBounds().height +35));

	settings.setTexture(&settings_texture);
	settings.setSize(sf::Vector2f(75,75));
	//settings.setPosition(win_x- settings.getGlobalBounds().width ,load_network.getPosition().y - settings.getGlobalBounds().height/2 - 10);
	settings.setPosition(text.getPosition().x + text.getGlobalBounds().width + 10 ,text.getPosition().y + text.getGlobalBounds().height/1.5);



	// ---------------------------------- Rendering Loop --------------------------------------
	// Setup and open Window
	// Use the screenScalingFactor and create window
	window.create(sf::VideoMode(win_x * screenScalingFactor, (win_y + 75) * screenScalingFactor), "Neural Netowrk GUI Interface");
	//resize_window(&window, sf::Vector2u(window.getSize().x, window.getSize().y+30));
	platform.setIcon(window.getSystemHandle());

	// While window is open, get current event (monitor for closed)
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			// If close event is dectected, exit
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			// If option 1 is detected
			if (event.key.code == sf::Keyboard::Num1)
			{
				window.close();
				display_int_setup();
			}

		}
		// Draw Title page
		window.clear();
		window.draw(bg);
		window.draw(settings);
		window.draw(title);
		window.draw(load_network);
		window.draw(text);
		//window.draw(playerText);
		window.draw(opt_text);
		window.display();
	}
}

void NN_gui_setup::resize_window(sf::Window* window, sf::Vector2u res){
	window->setSize(res);


}