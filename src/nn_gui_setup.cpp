#include "nn_gui_setup.h"
#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nn_display.h"
#include "nnet.h"
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include <vector>1

using namespace boost::filesystem;
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
	extra_text = " ";
	manual_net_loaded = false;
	file_net_loaded = false;

	display = new NN_Display(net);
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
	opt_text.setString("1: Load network & inputs from file\n\n2: Load simple network info (manual)");
	opt_text.setCharacterSize(19);
	opt_text.setFillColor(sf::Color::White);
	//opt_text.setPosition(text.getPosition().x + opt_text.getGlobalBounds().width/10 , text.getPosition().y + opt_text.getGlobalBounds().height);
	opt_text.setPosition(20, win_y - 2 * opt_text.getGlobalBounds().height + 70);

	//net_loaded = true;

	// Set font attributes for lower text

	load_network.setFont(title_font);
	load_network.setString("\n3: Process loaded network (no network loaded...)");
	load_network.setCharacterSize(19);
	c = sf::Color(100, 100, 100, 255);
	load_network.setFillColor(c);
	load_network.setStyle(sf::Text::Bold | sf::Text::StrikeThrough);
	load_network.setString(load_network.getString() + extra_text);
	load_network.setPosition(20, opt_text.getPosition().y + opt_text.getGlobalBounds().height + 5);


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
	bg.setSize(sf::Vector2f(win_x - bg.getPosition().x * 2 + 50, (load_network.getPosition().y - load_network.getGlobalBounds().height) - opt_text.getGlobalBounds().height + 35));

	settings.setTexture(&settings_texture);
	settings.setSize(sf::Vector2f(75, 75));
	//settings.setPosition(win_x- settings.getGlobalBounds().width ,load_network.getPosition().y - settings.getGlobalBounds().height/2 - 10);
	settings.setPosition(text.getPosition().x + text.getGlobalBounds().width + 10, text.getPosition().y + text.getGlobalBounds().height / 1.5);

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
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{

				if (settings.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					window.close();
					display->display_side_menu();
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{

				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}

				// If option 1 is detected
				if (event.key.code == sf::Keyboard::Num1)
				{
					//path dir(".");
					t1 = new std::thread(&NN_gui_setup::display_file_select, this);
					// Set bools to update text that we loaded a network


					//window.close();
					//display_title();
					// Load our network from file
					//net->load_from_file();
				}
				// If option 2 is detected
				if (event.key.code == sf::Keyboard::Num2)
				{
					// Close window and go to interactive input
					window.close();
					display_int_setup();
				}
				// If option 3 is detected
				if (event.key.code == sf::Keyboard::Num3)
				{
					if (net->net->input_layer != NULL)
					{
						// Close window and start display of loaded network
						window.close();
						//display->display_net();
						display->display_side_menu();
					}
					else{
						cout << "NO NET LOADED!\n";
					}
				}
			}
		}


		if (manual_net_loaded)
		{
			//win_x += 70;
			load_network.setString("\n3: Process loaded network (from manual input)");
			extra_text = " (from manual input)";
			load_network.setFillColor(sf::Color::White);
			load_network.setStyle(sf::Text::Bold);
		}
		else if (file_net_loaded)
		{
			load_network.setString("\n3: Process loaded network (from file input)");
			extra_text = " (from manual input)";
			load_network.setFillColor(sf::Color::White);
			load_network.setStyle(sf::Text::Bold);
		}
		else
		{
		load_network.setString("\n3: Process loaded network (no network loaded...)");
		load_network.setCharacterSize(19);
		load_network.setFillColor(sf::Color(100, 100, 100, 255));
		load_network.setStyle(sf::Text::Bold | sf::Text::StrikeThrough);
		load_network.setString(load_network.getString() + extra_text);
		load_network.setPosition(20, opt_text.getPosition().y + opt_text.getGlobalBounds().height + 5);

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

void NN_gui_setup::display_file_select(){
	double x = 400;
	double y = 600;

	int spacer = 10;
	int max_font_size = 28;
	sf::Text title;

	sf::Text* net_text = NULL;
	sf::RectangleShape* net_rect = NULL;
	// Background color for Buttons (gray-ish);
	sf::Color bg_button(128, 128, 128);
	// Object for our text font and then load. Exit if not found
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		std::exit(EXIT_FAILURE);
	}

	string data_dir = "data";
	vector <string> net_list;

	path p(data_dir);
	directory_iterator end_itr;

	for (directory_iterator itr(p); itr != end_itr; ++itr)
	{
		if (is_regular_file(itr->path()))
		{
			//string current_file = itr->path().string();
			if(itr->path().extension().string() == ".top"){
				net_list.push_back(itr->path().string());
			}
			//cout << itr->path().extension().string();
			//cout << current_file << endl;
		}
	}
	title.setFont(title_font);
	title.setString("Please select a network...");
	title.setCharacterSize(max_font_size * 0.55);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Underlined);
	title.setPosition(spacer, spacer);

	net_text = new sf::Text[net_list.size()];
	net_rect = new sf::RectangleShape[net_list.size()];
	int start_x = 2*spacer;
	int start_y = title.getPosition().y + title.getGlobalBounds().height + 2.5*spacer;
	int largest_x = title.getPosition().x + title.getGlobalBounds().width;

	for(int i = 0; i < net_list.size(); i++){
		cout << net_list[i] << endl;
		net_text[i].setFont(title_font);
		net_text[i].setString(net_list[i]);
		net_text[i].setCharacterSize(max_font_size * .55);
		net_text[i].setFillColor(sf::Color::Black);
		net_text[i].setPosition(start_x,start_y);

		net_rect[i].setPosition(net_text[i].getPosition().x, net_text[i].getPosition().y);
		net_rect[i].setSize(sf::Vector2f(net_text[i].getGlobalBounds().width + spacer/2, net_text[i].getGlobalBounds().height*2));
		net_rect[i].setFillColor(bg_button);
		net_rect[i].setOutlineColor(sf::Color::White);
		net_rect[i].setOutlineThickness(1);
		if(net_rect[i].getPosition().x + net_rect[i].getGlobalBounds().width > largest_x){
			largest_x = net_rect[i].getPosition().x + net_rect[i].getGlobalBounds().width;
		}
		start_y += net_text[i].getGlobalBounds().height + 3*spacer;
	}








	// Create new window and set to right of main window
	file_window.create(sf::VideoMode((largest_x + spacer) * screenScalingFactor, (start_y) * screenScalingFactor), "");
	//file_window.setPosition(sf::Vector2((int)layer_data_window.getPosition().x, (int)layer_data_window.getPosition().y + (int)layer_data_window.getSize().y + spacer*3));

	// ---------------------------------------- Main Open Loop --------------------------------------------------

	while (file_window.isOpen()){

		while (file_window.pollEvent(file_event)){
			// If closing
			if (file_event.type == sf::Event::Closed){
				file_window.close();
				return;
			}

			else if(file_event.type == sf::Event::KeyPressed){

				if(file_event.key.code == sf::Keyboard::Escape){
					file_window.close();
					return;
				}

			}

			else if(file_event.type == sf::Event::MouseButtonPressed){
				for(int i = 0; i < net_list.size(); i++){
					// Check if clicked on Layer title
					if(net_rect[i].getGlobalBounds().contains(file_window.mapPixelToCoords(sf::Mouse::getPosition(file_window)))){
						cout << "Clicked on: " << net_list[i] << endl;
						manual_net_loaded = false;
						file_net_loaded = true;
						net->load_from_file(net_list[i]);
						file_window.close();
						return;


					}
				}

			}


		}





		file_window.clear();
		file_window.draw(title);
		for(int i = 0; i < net_list.size(); i++){
			file_window.draw(net_rect[i]);
			file_window.draw(net_text[i]);
		}
		file_window.display();

	}






}
















void NN_gui_setup::resize_window(sf::Window* window, sf::Vector2u res)
{
	window->setSize(res);
}

// Open window and display interactive setup for node
void NN_gui_setup::display_int_setup()
{
	if(net->net->input_layer != NULL){
		net->delete_network();
	}
	win_x = 800;
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
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width / 2), win_y / 50);

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
	playerText.setCharacterSize(50);
	playerText.setFillColor(sf::Color::White);
	bool display_player_text = true;

	// Set text attributes displaying input data
	layer_num_text.setFont(title_font);
	layer_num_text.setCharacterSize(25);
	layer_num_text.setString("Layer Count: ");
	layer_num_text.setStyle(sf::Text::Bold);
	layer_num_text.setFillColor(sf::Color::White);
	layer_num_text.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - layer_num_text.getGlobalBounds().width / 2, rectangle.getPosition().y + 20);
	bool display_num_layers = false;

	// Set text attributes displaying layer data
	layer_data.setPosition((rectangle.getPosition().x + 20), layer_num_text.getPosition().y + layer_num_text.getGlobalBounds().height - 20);
	layer_data.setFont(title_font);
	layer_data.setCharacterSize(25);
	layer_data.setString("Layer Node Count:\n[");
	layer_data.setStyle(sf::Text::Bold);
	layer_data.setFillColor(sf::Color::White);
	bool display_layer_data = false;

	// Set text attributes displaying initial data
	input_data.setPosition((rectangle.getPosition().x + 20), layer_data.getPosition().y + layer_data.getGlobalBounds().height + 40);
	input_data.setFont(title_font);
	input_data.setCharacterSize(25);
	input_data.setString("Initial Values:\n");
	input_data.setStyle(sf::Text::Bold);
	input_data.setFillColor(sf::Color::White);
	bool display_input_data = false;

	// Set text attributes displaying initial data
	//weight_data.setPosition((rectangle.getPosition().x + layer_num_text.getGlobalBounds().width+20), layer_data.getPosition().y+layer_data.getGlobalBounds().height + 40);
	//weight_data.setPosition((rectangle.getPosition().x + rectangle.getGlobalBounds().width/2 - weight_data.getGlobalBounds().width/2), layer_.getPosition().y+layer_data.getGlobalBounds().height + 40);

	weight_data.setFont(title_font);
	weight_data.setCharacterSize(20);
	weight_data.setString(" Weight Matrix (Layer 0):\n-------------------------\n");
	weight_data.setStyle(sf::Text::Bold);
	weight_data.setFillColor(sf::Color::White);
	//weight_data.setPosition((rectangle.getPosition().x + rectangle.getGlobalBounds().width / 2 - weight_data.getGlobalBounds().width / 2), layer_num_text.getPosition().y);
	weight_data.setPosition(layer_data.getPosition().x, layer_data.getPosition().y);

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
				if (isprint(event.text.unicode))
				{
					playerInput += event.text.unicode;
					playerText.setString(playerInput);
					playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - playerText.getGlobalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y - 75);
				}
				// If backspace is detected and string has input
				if (event.text.unicode == '\b' && (playerInput.getSize() > 0))
				{
					playerInput.erase(playerInput.getSize() - 1, 1);
					playerText.setString(playerInput);
					playerText.setPosition(rectangle.getPosition().x + (rectangle.getSize().x / 2) - playerText.getGlobalBounds().width / 2, rectangle.getPosition().y + rectangle.getSize().y - 75);
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
								layer_data.setString(layer_data.getString() + to_string(node_count[layer_node_counter]));
								display_layer_data = true;
							}

							if (layer_node_counter != layer_count && layer_node_counter != 0)
							{
								layer_data.setString(layer_data.getString() + ", " + to_string(node_count[layer_node_counter]));
							}

							layer_node_counter++;
							// Check if we are finished inputting node counts
							if (layer_node_counter == layer_count)
							{
								phase = 2;
								layer_data.setString(layer_data.getString() + "]\n---------------------");
								input_data.setPosition((rectangle.getPosition().x + 20), layer_data.getPosition().y + layer_data.getGlobalBounds().height + 20);

								cur_text.setString("Please enter the initial input vector for L0 (" + to_string(node_count[0]) + " nodes):");
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
							input_data.setString(input_data.getString() + "N" + to_string(init_val_counter) + ": " + to_string(input[init_val_counter]) + "\n");
							display_input_data = true;
							playerInput = "";
							playerText.setString("");
							init_val_counter++;
						}
						if (init_val_counter == node_count[0])
						{
							phase = 3;
							input_data.setString(input_data.getString() + "---------------------");
							//cout << "Finished init vals!\n";
							playerInput = "";
							playerText.setString("");
							cur_text.setString("Please enter weights for Layer 0 (Connecting N0 to N0):");
							cur_text.setPosition(win_x / 2 - (cur_text.getGlobalBounds().width / 2), cur_text.getPosition().y);

							display_weight_data = true;
							display_layer_data = false;
							display_num_layers = false;
							display_input_data = false;
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
								if (conn_weight_col != 0)
								{
									weight_data.setString(weight_data.getString() + " " + to_string(weights[conn_weight_row][conn_weight_col]));
								}
								else
								{
									weight_data.setString(weight_data.getString() + to_string(weights[conn_weight_row][conn_weight_col]));
								}
								conn_weight_col++;
							}
							// If we are finished grabbing colums for row, increment row and reset column
							else
							{
								weight_data.setString(weight_data.getString() + " " + to_string(weights[conn_weight_row][conn_weight_col]) + "\n");
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
									weight_data.setString("Weight Matrix (Layer " + to_string(init_weight_counter) + "):\n------------------------------\n");
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
							cur_text.setString("Please enter weights for Layer " + to_string(init_weight_counter) + " (Connecting N" + to_string(conn_weight_row) + " to N" + to_string(conn_weight_col) + "):");
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
						if (playerText.getString().toAnsiString() == "y")
						{
							manual_net_loaded = true;
							file_net_loaded = false;
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
						if (playerText.getString().toAnsiString() == "n")
						{
							manual_net_loaded = false;
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
							net->delete_network();

							//sf::sleep(sf::milliseconds(500));
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
