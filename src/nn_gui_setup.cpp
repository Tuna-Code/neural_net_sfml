#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include "nn_gui_setup.h"
#include <iostream>
#include <string>

using namespace std;

NN_gui_setup::NN_gui_setup(Net_Helper* net){
	// Assign our NN helper obj
	this->net = net;
	win_x = 1920;
	win_y = 1080;
	// Window dimensionts


	// in Windows at least, this must be called before creating the window
	screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());


}


// Open window and display interactive setup for node
void NN_gui_setup::display_int_setup(){
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
	if (!title_font.loadFromFile("fonts/akira.otf")){
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	// Set font attributes for heading
	title.setFont(title_font);
	title.setString("Neural Network Interactive Setup");
	title.setCharacterSize(25);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width/2) , win_y / 20);

	// Set font attributes for question
	cur_text.setFont(title_font);
	cur_text.setCharacterSize(22);
	cur_text.setString("How many layers are in the network?: ");
	cur_text.setFillColor(sf::Color::White);
	//cur_text.setStyle(sf::Text::Bold);
	cur_text.setPosition(cur_text.getGlobalBounds().width/20, win_y / 8);

	// Set attributes for rectangle (text box)

	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(20, cur_text.getPosition().y + cur_text.getGlobalBounds().height*2 + 20);
	rectangle.setSize(sf::Vector2f(win_x-40, win_y - rectangle.getPosition().y - cur_text.getPosition().y + cur_text.getGlobalBounds().height*2 + 80));

	// Set text attributes for input
	// middle pos playerText.setPosition((rectangle.getSize().x/2), rectangle.getPosition().y + (rectangle.getGlobalBounds().height/2));
	//playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + rectangle.getSize().y - 100); bottom pos
	playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + 20);
	playerText.setFont(title_font);
	playerText.setCharacterSize(30);
	playerText.setFillColor(sf::Color::White);
	bool display_player_text = true;

	// Set text attributes displaying input data
	layer_num_text.setPosition((rectangle.getPosition().x + 20), playerText.getPosition().y + 20);
	layer_num_text.setFont(title_font);
	layer_num_text.setCharacterSize(25);
	layer_num_text.setString("- Layers in Network: ");
	layer_num_text.setStyle(sf::Text::Bold);
	layer_num_text.setFillColor(sf::Color::White);
	bool display_num_layers = false;

	// Set text attributes displaying layer data
	layer_data.setPosition((rectangle.getPosition().x + 20), layer_num_text.getPosition().y+layer_num_text.getGlobalBounds().height + 40);
	layer_data.setFont(title_font);
	layer_data.setCharacterSize(25);
	layer_data.setString("- Layer 0:\n");
	layer_data.setStyle(sf::Text::Bold);
	layer_data.setFillColor(sf::Color::White);
	bool display_layer_data = false;

	// Set text attributes displaying initial data
	input_data.setPosition((rectangle.getPosition().x + 20), layer_data.getPosition().y+layer_data.getGlobalBounds().height + 40);
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
	weight_data.setPosition((rectangle.getPosition().x + rectangle.getGlobalBounds().width/2 - weight_data.getGlobalBounds().width/2), layer_num_text.getPosition().y);

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

	// While window is open, get current event (monitor for closed)
	window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");
	platform.setIcon(window.getSystemHandle());

	// Rendering loop
	while (window.isOpen()){


		// Grab current event
		while (window.pollEvent(event)){

			// If click on exit, quit
			if(event.type == sf::Event::Closed){
				window.close();
			}
			// If text is entered, update string and go to render
			if (event.type == sf::Event::TextEntered){
				playerInput += event.text.unicode;
				playerText.setString(playerInput);
				//playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + 50);
				if(phase == 4){
					playerText.setPosition(cur_text.getPosition().x + cur_text.getGlobalBounds().width + 20 , cur_text.getPosition().y + (cur_text.getGlobalBounds().height/2.25));
				}
				else{
					playerText.setPosition(cur_text.getPosition().x + cur_text.getGlobalBounds().width + 20 , cur_text.getPosition().y - (cur_text.getGlobalBounds().height/2));
				}
				// bottom pos playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + rectangle.getSize().y - 100);
				// middle pos playerText.setPosition((rectangle.getSize().x/2) - playerText.getGlobalBounds().width/2, rectangle.getPosition().y + (rectangle.getGlobalBounds().height/2));

			}
			// If backspace is entered, delete character and re-render
			if(event.key.code == sf::Keyboard::BackSpace){
				playerInput = playerInput.substring(0,playerInput.getSize()-1);
				playerText.setString(playerInput);


			}

			// Check for key press event to prevent multiple instances being detected
			if(event.type == sf::Event::KeyPressed){
				// If escape event is dectected, exit
				if (event.key.code == sf::Keyboard::Escape){
					window.close();
				}

				// Phase 1 of questions
				if(phase == 0){
					// If enter is pressed, save input and go to next question
					if(event.key.code == sf::Keyboard::Enter){

							// Grab current input, convert it to string and then to int
							layer_count = stoi(playerInput.toWideString());
							node_count = new int[layer_count];

							// Update our info and set bool to display
							layer_num_text.setString(layer_num_text.getString() + to_string(layer_count) + "\n\n---------------------");

							display_num_layers = true;

							// Clear out displayed input
							playerInput = "";
							playerText.setString("");

							// Update question string and proceed
							cur_text.setString("How many nodes are in Layer #0 (The input Layer)?");

							// Update to next Phase
							phase++;
							layer_data.setPosition((rectangle.getPosition().x + 20), layer_num_text.getPosition().y+layer_num_text.getGlobalBounds().height + 40);
							goto render_setup_screen;

					}
				}
				// Phase 1 of questions
				if(phase == 1){
					// If enter is pressed, save input and go to next question
					if(event.key.code == sf::Keyboard::Enter){

						if(layer_node_counter < layer_count){
							node_count[layer_node_counter] = stoi(playerInput.toWideString());


							// Clear out displayed input
							playerInput = "";
							playerText.setString("");
							cur_text.setString("How many nodes are in Layer #" + to_string(layer_node_counter+1));

							// Update string for displaying layer stats
							if(layer_node_counter == 0){
								layer_data.setString(layer_data.getString() + "     " + to_string(node_count[layer_node_counter]) + " Nodes\n");
								display_layer_data = true;
							}


							if(layer_node_counter != layer_count && layer_node_counter != 0){
								layer_data.setString(layer_data.getString() + "- Layer " + to_string(layer_node_counter) + "\n     " + to_string(node_count[layer_node_counter]) + " Nodes\n");
							}

							layer_node_counter++;
							// Check if we are finished inputting node counts
							if(layer_node_counter == layer_count){
								phase = 2;
								layer_data.setString(layer_data.getString() + "\n---------------------");
								input_data.setPosition((rectangle.getPosition().x + 20), layer_data.getPosition().y+layer_data.getGlobalBounds().height + 40);
								display_input_data = true;
								cur_text.setString("Please enter the initial vector for our input layer (" + to_string(node_count[0]) + " values):");
								input = new double[node_count[0]];
								playerInput = "";
								playerText.setString("");
							}

							goto render_setup_screen;
						}
					}
				}
				if(phase == 2){

					if(event.key.code == sf::Keyboard::Enter){
						if(init_val_counter < node_count[0]){
							input[init_val_counter] = stod(playerInput.toWideString());
							//cout << input[init_val_counter]  << endl;
							input_data.setString(input_data.getString() + "     Input " + to_string(init_val_counter) + ": " + to_string(input[init_val_counter]) + "\n");
							display_input_data = true;
							playerInput = "";
							playerText.setString("");
							init_val_counter++;

						}
						if(init_val_counter == node_count[0]){
							phase = 3;
							input_data.setString(input_data.getString() + "\n---------------------");
							//cout << "Finished init vals!\n";
							playerInput = "";
							playerText.setString("");
							cur_text.setString("Please enter weight matrix for Layer 0 (Connecting Node 0 to Node 0):");
							display_weight_data = true;
						}
						goto render_setup_screen;
					}

				}
				// Get weight matrix for each layer
				if(phase == 3){
					if(event.key.code == sf::Keyboard::Enter){

						// Get weights for each layer
						if(init_weight_counter < layer_count - 1){

							if(!layer_weights_alloc){


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
							if(conn_weight_col != node_count[init_weight_counter + 1] - 1){
								weight_data.setString(weight_data.getString() + "    " + to_string(weights[conn_weight_row][conn_weight_col]));
								conn_weight_col++;
							}
							// If we are finished grabbing colums for row, increment row and reset column
							else{
								weight_data.setString(weight_data.getString() + "    " + to_string(weights[conn_weight_row][conn_weight_col]) + "\n");
								conn_weight_col = 0;

								conn_weight_row++;
							}
							// Finished grabbing layer weights
							if(conn_weight_row == node_count[init_weight_counter]){

								conn_weight_row = 0;
								conn_weight_col = 0;
								net->net->add_layer(init_weight_counter, node_count[init_weight_counter], "Null", weights, weight_rows, weight_cols);


								weight_rows = 0;
								weight_cols = 0;
								layer_weights_alloc = false;
								init_weight_counter++;
								if(init_weight_counter != layer_count - 1){
									weight_data.setString(weight_data.getString() + "\n- Weight Matrix for Layer " + to_string(init_weight_counter) + ":\n\n------------------------------\n");

								}
							}
							if(init_weight_counter == layer_count - 1){
								weights = NULL;
								weight_rows = 0;
								weight_cols = 0;
								net->net->add_layer(init_weight_counter, node_count[init_weight_counter], "Null", weights, weight_rows, weight_cols);
								phase = 4;
								// assign our inputs to input layer
								for(int i = 0; i < node_count[0]; i++){
									net->net->input_layer->input[i] = input[i];
								}
								net->net->forward_prop();
								net->print_network();


								playerInput = "";
								playerText.setString("");
								cur_text.setString("Finished with weights! Please verify network data...\nPress 'y' to confirm and proceed, press 'n' to start again: ");
								cur_text.setPosition(cur_text.getPosition().x,cur_text.getPosition().y-20);
								goto render_setup_screen;

							}

							cur_text.setString("Please enter weight matrix for Layer " + to_string(init_weight_counter) + " (Connecting Node " + to_string(conn_weight_row) + " to Node " + to_string(conn_weight_col) + "):");
							//weight_data.setString(weight_data.getString() + "     " + playerInput.toWideString() + ": \n");


							playerInput = "";
							playerText.setString("");
							//init_weight_counter++;
						}

						goto render_setup_screen;
					}

				}
				if(phase == 4){


				}


			}

		}
		render_setup_screen:
			// Draw setup page
			window.clear();
			window.draw(rectangle);
			window.draw(title);
			if(display_player_text){
				window.draw(playerText);
			}
			window.draw(cur_text);
			if(display_num_layers || display_all){
				window.draw(layer_num_text);
			}
			if(display_layer_data || display_all){
				window.draw(layer_data);
			}
			if(display_input_data || display_all){
				window.draw(input_data);
			}
			if(display_weight_data || display_all){
				window.draw(weight_data);
			}
			window.display();
	}
}




// Display our title with interactive options
void NN_gui_setup::display_title(){
	// Create font and text objects
	sf::Text title;
	sf::Text text;
	sf::Font title_font;
	sf::Text opt_text;

	// Load font from file
	if (!title_font.loadFromFile("fonts/akira.otf")){
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}

	// Set font attributes for heading
	title.setFont(title_font);
	title.setString("Neural Network Interface");
	title.setCharacterSize(25);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width/2) , win_y / 10);

	// Set font attributes for lower text
	text.setFont(title_font);
	text.setString("Please select from an option below:");
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(win_x / 2 - (text.getGlobalBounds().width/2) , win_y / 6);

	// Set font attributes for option text
	opt_text.setFont(title_font);
	opt_text.setString("1: Manually enter network info\n\n\n2: Run pre-made networks (for testing)");
	opt_text.setCharacterSize(20);
	opt_text.setFillColor(sf::Color::White);
	//opt_text.setPosition(text.getPosition().x + opt_text.getGlobalBounds().width/10 , text.getPosition().y + opt_text.getGlobalBounds().height);
	opt_text.setPosition(20, win_y - 2*opt_text.getGlobalBounds().height);

// ---------------------------------- Rendering Loop --------------------------------------
	// Setup and open Window
	// Use the screenScalingFactor and create window
	window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");
	platform.setIcon(window.getSystemHandle());

	// While window is open, get current event (monitor for closed)
	while (window.isOpen()){
		while (window.pollEvent(event)){
			// If close event is dectected, exit
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape){
				window.close();
			}
			// If option 1 is detected
			if (event.key.code == sf::Keyboard::Num1){
				window.close();
				display_int_setup();
			}
		}
		// Draw Title page
		window.clear();
		window.draw(title);
		window.draw(text);
		//window.draw(playerText);
		window.draw(opt_text);
		window.display();
	}
}

