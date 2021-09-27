#include "utils.h"

#define PI 3.14159265


using namespace std;



// NN_Display constructor (takes Net_Helper previously loaded w/network)
NN_Display::NN_Display(Net_Helper* net){

	// Update our objects network pointer to the one provided
	this->net = net;

	// Set initial resolution
	win_x = 1440;
	win_y = 900;
	//win_x = 1920;
	//win_y = 1080;
	// Set max font size (TODO: Change this based on resolution)
	max_font_size = 28;

	// Grab scaling factors for display (required here on Winows)
	screenScalingFactor = platform.getScreenScalingFactor(side_menu_window.getSystemHandle());

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

	label_spacer = 10;
	button_spacer = 20;
	spacer = 10;

	close_net_disp = false;
	close_layer_disp = false;
	close_node_disp = false;



}

// Function for minimal display (for large nets)
void NN_Display::display_side_menu(){
	bool net_loaded = false;
	NN_gui_setup* setup = new NN_gui_setup(net);
	// Set initial window size
	int win_x = 300;
	bool disp_controls = false;
	// Vector containing our button objects
	vector<Button> option_button_list;
	vector<Button> control_button_list;
	// Vector containing our button strings
	vector<string> option_string_list;
	vector<string> control_string_list;

	// GUI Spacers


	// Window label text
	sf::Text title;
	sf::Text net_control;

	// Load font
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		std::exit(EXIT_FAILURE);
	}

	// Set font attributes for main heading
	title.setFont(title_font);
	title.setString("Options...");
	title.setCharacterSize(max_font_size * 0.6);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(label_spacer, label_spacer);

	net_control.setFont(title_font);
	net_control.setString("Net Controls...");
	net_control.setCharacterSize(max_font_size * 0.6);
	net_control.setFillColor(sf::Color::White);
	net_control.setStyle(sf::Text::Bold | sf::Text::Underlined);


	// Add our button texts
	option_string_list.push_back("Select + Load Net");
	option_string_list.push_back("Clear Memory");
	option_string_list.push_back("Main Menu");

	control_string_list.push_back("Load Inputs");
	control_string_list.push_back("Load Training Data");
	control_string_list.push_back("-> Prop.");
	control_string_list.push_back("<- Prop.");
	control_string_list.push_back("Reset (Init Vals)");
	control_string_list.push_back("Display Net (Data)");
	control_string_list.push_back("Display Net (Visual)");



	//control_string_list.push_back("Display Weights (All)");

	// Set initial Y position for button
	int cur_y = title.getPosition().y + title.getGlobalBounds().height + button_spacer;
	// Temp button pointer
	Button* temp = NULL;
	// Our current max_y value (for window size)
	int max_y = 0;

	// Loop through each string we set and generate button struct + increment Y position
	for(int i = 0; i < option_string_list.size(); i++){
		temp = new Button(option_string_list[i],title.getPosition().x, cur_y);
		cur_y = temp->rect.getPosition().y + temp->rect.getGlobalBounds().height + button_spacer;
		option_button_list.push_back(*temp);
		max_y = temp->rect.getPosition().y + temp->rect.getGlobalBounds().height + button_spacer;

	}
	net_control.setPosition(label_spacer, max_y);
	cur_y = net_control.getPosition().y + net_control.getGlobalBounds().height + button_spacer;

	for(int i = 0; i < control_string_list.size(); i++){
		temp = new Button(control_string_list[i],title.getPosition().x, cur_y);
		cur_y = temp->rect.getPosition().y + temp->rect.getGlobalBounds().height + button_spacer;
		control_button_list.push_back(*temp);
		max_y = temp->rect.getPosition().y + temp->rect.getGlobalBounds().height + button_spacer;
	}



	// Create our window object + set position
	side_menu_window.create(sf::VideoMode(win_x * screenScalingFactor, max_y * screenScalingFactor), "Neural Netowrk GUI Interface");
	side_menu_window.setPosition(sf::Vector2(10, 10));
	platform.setIcon(side_menu_window.getSystemHandle());
	sf::Color button_color(sf::Color::Black);
// ---------------------------------------- Main window loop------------------------------------------
	// If user loaded network on homescreen, spwan thread and display
	if(net->net->input_layer != NULL){
			t0 = new std::thread(display_net, this);
			net_loaded = true;
	}
	// Main window open loop
	while (side_menu_window.isOpen()){
		if(net_loaded){
			button_color = sf::Color(128, 128,128);
		}
		else{
			button_color = sf::Color(55, 55, 55);
		}
		// Get current event
		while (side_menu_window.pollEvent(side_menu_event)){

			// If user closes
			if (side_menu_event.type == sf::Event::Closed){
				side_menu_window.close();
				return;
			}
			// If user hits presses a key
			else if(side_menu_event.type == sf::Event::KeyPressed){
				// If escape is pressed
				if(side_menu_event.key.code == sf::Keyboard::Escape){
					side_menu_window.close();
					return;
				}
			}
			// If user clicks
			else if(side_menu_event.type == sf::Event::MouseButtonPressed){
				// Loop through out buttons and see which was clocked
				for(int i = 0; i < option_button_list.size(); i++){
					if(option_button_list[i].rect.getGlobalBounds().contains(side_menu_window.mapPixelToCoords(sf::Mouse::getPosition(side_menu_window)))){

						// Button 0 (Select + load network)
						if(i == 0){
							// Get path to network to load
							string new_net = display_file_select_load(side_menu_window.getPosition().x + side_menu_window.getSize().x + spacer, side_menu_window.getPosition().y  + spacer, ".top");

							// If no network is loaded yet
							if(net->net->input_layer == NULL){
								// Verify file path was returned and load net
								if(new_net != ""){
									net->load_from_file(new_net);
									net_loaded = true;
									// Spawn new thread and display
									t0 = new std::thread(display_net, this);
								}
							}
							// If network is already loaded
							else{


								if(node_data_window.isOpen()){
									close_node_disp = true;

									t2->join();
								}
								if(layer_data_window.isOpen()){
									close_layer_disp = true;
									t1->join();
								}
								if(net_disp_window.isOpen()){
									close_net_disp = true;
									t0->join();
								}

								// Clear out network and release memory
								net->delete_network();
								// Load newly chosen network
								net->load_from_file(new_net);
								net_loaded = true;
								// Spawn new thread and display net
								t0 = new std::thread(display_net, this);
							}
						}
						if(i == 1){


							if(node_data_window.isOpen()){
								close_node_disp = true;

								t2->join();
							}
							if(layer_data_window.isOpen()){
								close_layer_disp = true;
								t1->join();
							}
							if(net_disp_window.isOpen()){
								close_net_disp = true;
								t0->join();
							}


							net->delete_network();
							net_loaded = false;

						}
						if(i == 2){
							if(node_data_window.isOpen()){
								close_node_disp = true;

								t2->join();
							}
							if(layer_data_window.isOpen()){
								close_layer_disp = true;
								t1->join();
							}
							if(net_disp_window.isOpen()){
								close_net_disp = true;
								t0->join();
							}


							if(net_loaded){
								net->delete_network();
								net_loaded = false;
							}
							side_menu_window.close();
							setup->display_title();


						}

					}
				}
				for(int i = 0; i < control_button_list.size(); i++){
					if(control_button_list[i].rect.getGlobalBounds().contains(side_menu_window.mapPixelToCoords(sf::Mouse::getPosition(side_menu_window))) && net_loaded){

						if(i == 1){
							if(!net_disp_window.isOpen() && (net->net->input_layer != NULL)){
								t0 = new std::thread(display_net, this);
							}

						}
						if(i == 2){
							net->net->forward_prop();
							update_stats_window = true;
							update_node_window = true;

						}
						if(i == 4){
							net->reset_network();
							update_stats_window = true;
							update_node_window =true;
						}

						if(i == 6){
							if(!net_disp_window.isOpen()){
								t0 = new std::thread(display_net, this);
							}
						}
						//if(i == 6){
						//	display_weight_text[net->net->layer_count - 1][0] = !display_weight_text[net->net->layer_count - 1][0];

						//}
					}

				}
			}
		}

// -------------------------------------- Main Display Sequence --------------------------------------

		// Clear window
		side_menu_window.clear();
		// Draw our title text
		side_menu_window.draw(title);
		// Loop through button vector and draw to screen
		for(int i = 0; i < option_button_list.size(); i++){
			side_menu_window.draw(option_button_list[i].rect);
			side_menu_window.draw(option_button_list[i].text);
		}
		//if(net->net->input_layer != NULL){
		side_menu_window.draw(net_control);
		for(int i = 0; i < control_button_list.size(); i++){
			control_button_list[i].rect.setFillColor(button_color);
			side_menu_window.draw(control_button_list[i].rect);
			side_menu_window.draw(control_button_list[i].text);
		}
		//}


		// Display buffer
		side_menu_window.display();
	}
	return;
}



































// Main function to display loaded network visually and dynamically
void NN_Display::display_net(){
	disp_layer = -1;
	// Verify we have a net loaded into our pointer, otherwise return with error
	if(net->net->input_layer == NULL){
		cout << "No net loaded!";
		return;
	}




// ---------------------------------------- Initial Display Setup ------------------------------------------
	// Temp layer pointer used throughout app
	Layer* l = NULL;
	// How many pixels for spacers

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
	sf::Text delete_text;
	sf::Text display_weights_text;

	// 2d Text array for titles of our layer serctions
	sf::Text* layer_title = NULL;
	layer_title = new sf::Text[layer_count];

	// Rectangle shapes for menu buttons
	sf::RectangleShape forward_rect;
	sf::RectangleShape reset_rect;
	sf::RectangleShape delete_rect;
	sf::RectangleShape display_weights_rect;

	// Object for our text font and then load. Exit if not found
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		std::exit(EXIT_FAILURE);
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
	options.setString("Options    ");
	options.setCharacterSize(max_font_size * .6667);
	options.setFillColor(sf::Color::White);
	options.setStyle(sf::Text::Bold | sf::Text::Underlined);
	options.setPosition(spacer/1.5,title.getPosition().y + options.getGlobalBounds().height/2);

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
	forward_rect.setSize(sf::Vector2f(forward_text.getGlobalBounds().width, forward_text.getGlobalBounds().height*2));
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

	// Set attributes for display weights button
	display_weights_text.setFont(title_font);
	display_weights_text.setString("Disp. Weights");
	display_weights_text.setCharacterSize(max_font_size * .5);
	display_weights_text.setFillColor(sf::Color::Black);
	display_weights_text.setPosition(options.getPosition().x,reset_rect.getPosition().y +spacer*2.5 + reset_rect.getGlobalBounds().height/2);

	display_weights_rect.setPosition(display_weights_text.getPosition().x, display_weights_text.getPosition().y);
	display_weights_rect.setSize(sf::Vector2f(display_weights_text.getGlobalBounds().width + spacer, display_weights_text.getGlobalBounds().height*2));
	display_weights_rect.setFillColor(bg_button);
	display_weights_rect.setOutlineColor(sf::Color::White);
	display_weights_rect.setOutlineThickness(1);

	// Set attributes for delete button
	delete_text.setFont(title_font);
	delete_text.setString("Clear Memory");
	delete_text.setCharacterSize(max_font_size * .5);
	delete_text.setFillColor(sf::Color::Black);
	delete_text.setPosition(options.getPosition().x,win_y - delete_text.getGlobalBounds().height*2 - spacer);

	delete_rect.setPosition(delete_text.getPosition().x, delete_text.getPosition().y);
	delete_rect.setSize(sf::Vector2f(delete_text.getGlobalBounds().width + spacer, delete_text.getGlobalBounds().height*2));
	delete_rect.setFillColor(bg_button);
	delete_rect.setOutlineColor(sf::Color::White);
	delete_rect.setOutlineThickness(1);

	// Create window and set position to far left and set icon
	net_disp_window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");
	net_disp_window.setPosition(sf::Vector2((int) side_menu_window.getPosition().x + (int) side_menu_window.getSize().x + spacer, side_menu_window.getPosition().y));

	platform.setIcon(net_disp_window.getSystemHandle());

// ---------------------------------------- Generate Layer Titles and Positions ----------------------------

	// Generate initial title and position for input layer heading
	layer_title[0].setString("Layer: " + to_string(0));
	layer_title[0].setFont(title_font);
	layer_title[0].setCharacterSize(layer_font_size);
	layer_title[0].setFillColor(sf::Color::White);
	layer_title[0].setStyle(sf::Text::Bold | sf::Text::Underlined);
	//layer_title[0].setPosition((options.getPosition().x + options.getGlobalBounds().width + spacer*2) + layer_title[0].getGlobalBounds().width/2, (title.getPosition().y + title.getGlobalBounds().height  + spacer*2));
	layer_title[0].setPosition((win_x/layer_count)/2 - layer_title[0].getGlobalBounds().width/2, (title.getPosition().y + title.getGlobalBounds().height  + spacer*2));

	// For each remaining layer (1 to end) generate it's title and set position based on previous layer title
	for(int i = 1; i < layer_count; i++){
		layer_title[i].setString("Layer: " + to_string(i));
		layer_title[i].setFont(title_font);
		layer_title[i].setCharacterSize(layer_font_size);
		layer_title[i].setFillColor(sf::Color::White);
		layer_title[i].setStyle(sf::Text::Bold | sf::Text::Underlined);
		//layer_title[i].setPosition(layer_title[i-1].getPosition().x + ((win_x - (options.getPosition().x + options.getGlobalBounds().width + spacer*2)) / layer_count) + spacer, (title.getPosition().y + title.getGlobalBounds().height  + spacer*2));
		layer_title[i].setPosition(layer_title[i-1].getPosition().x + (win_x  / layer_count), (title.getPosition().y + title.getGlobalBounds().height  + spacer*2));

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


//---------------------- Generate our text object array for displaying weights -----------------------------

	// Allocate text object array and select desired layer for testing
	sf::Text** weight_text = NULL;
	weight_text = new sf::Text*[net->net->layer_count-1];

	// Vertex objects to hold start and end of current line for simplifing logic
	sf::Vertex line_start;
	sf::Vertex line_end;
	// What line in layer are we currently on
	int line_counter;
	// Pixel gab (how far text is above line once rotated)
	int gap = 5;
	// Variables for computing text positions based on line data
	int beg_x;
	int end_x;
	int beg_y;
	int end_y;
	double x_diff;
	double y_diff;
	double mid_x;
	double mid_y;
	double angle;
	int num_weights;

	// Allocate our bool array (entry [i][j] is for layer i, line j descending, last entry is display all nodes in layer, final entry is display all layers)
	display_weight_text = NULL;
	display_weight_text = new bool*[net->net->layer_count];



	// Start on our input layer
	l = net->net->input_layer;

	// Iterate through each layer and generate weight text objects
	for(int x = 0; x < net->net->layer_count - 1; x++){

		// Reset line counter to 0 and compute how many weight lines are on this layer
		line_counter = 0;
		num_weights = l->num_nodes*l->next_layer->num_nodes;

		// Allocate our weight array of text objects
		weight_text[x] = new sf::Text[num_weights];
		// Allocate our bool array for displaying text data
		display_weight_text[x] = new bool[num_weights + 1];

		// Loop through each node in our current layer and compute text data connectiong to each node in next layer
		for(int i = 0; i < l->num_nodes; i++){
			for(int j = 0; j < l->next_layer->num_nodes; j++){

				// Set display value for this weight to false initially
				display_weight_text[x][line_counter] = false;

				// For our current node pair, grab the weight lines beginning and end vertex
				line_start = test_lines[x][line_counter][0];
				line_end = test_lines[x][line_counter][1];

				// Set text display attributes and text origin to center
				weight_text[x][line_counter].setFont(title_font);
				weight_text[x][line_counter].setCharacterSize(12);
				weight_text[x][line_counter].setFillColor(sf::Color::White);
				weight_text[x][line_counter].setStyle(sf::Text::Bold);
				weight_text[x][line_counter].setString(to_string(l->weights[i][j]));
				weight_text[x][line_counter].setOrigin(weight_text[x][line_counter].getGlobalBounds().width/2, weight_text[x][line_counter].getGlobalBounds().height/2);

				// Grab beginning and end coords of current line
				beg_x = line_start.position.x;
				end_x = line_end.position.x;
				beg_y = line_start.position.y;
				end_y = line_end.position.y;

				// Compute differences in x and y for start and end verticies
				x_diff = end_x - beg_x;
				y_diff = beg_y - end_y;

				// Compute angel needed to rotate (inverse tangent) then convert from radians to degrees
				angle = atan(y_diff/x_diff) * (180.0 / PI);

				// Compute middle of line position + add gap for visibility
				mid_x = beg_x + (x_diff/2);
				mid_y = beg_y - (y_diff/2) - weight_text[x][line_counter].getGlobalBounds().height - gap;

				// Set text to middle of line + gap and rotate
				weight_text[x][line_counter].setPosition(mid_x ,mid_y);
				weight_text[x][line_counter].setRotation(-angle);

				// Increment to the next line and continue
				line_counter++;
			}
		}
		// Move to next layer
		l = l->next_layer;
		// Allocate final bool in layer for display all layer
		display_weight_text[x][line_counter+1] = false;
	}
	// Allocate last layer (for display all)
	display_weight_text[net->net->layer_count - 1] = new bool[1];
	display_weight_text[net->net->layer_count - 1][0] = false;


	Layer* weight_layer = NULL;
	int cur_num_weights = 0;
// ---------------------------------------- Open Main Loop --------------------------------------------------
	// Window open loop
	while (net_disp_window.isOpen()){
		if(close_net_disp){
			close_net_disp = false;
			net_disp_window.close();
			return;

		}

		// Grab event
		while (net_disp_window.pollEvent(net_disp_event)){

			// If closing
			if (net_disp_event.type == sf::Event::Closed)
			{
				net_disp_window.close();

				disp_layer = -1;
				return;
			}

			else if(net_disp_event.type == sf::Event::KeyPressed){
				if(net_disp_event.key.code == sf::Keyboard::Escape){
					net_disp_window.close();

					disp_layer = -1;
					return;
				}
			}
			// If clicked, check if clicked on objects
			else if(net_disp_event.type == sf::Event::MouseButtonPressed){

				// Iterate through to see if layer had a clicked object
				l = net->net->input_layer;
				for(int i = 0; i < layer_count; i++){
					// Check if clicked on Layer title
					if(layer_title[i].getGlobalBounds().contains(net_disp_window.mapPixelToCoords(sf::Mouse::getPosition(net_disp_window)))){

							// If currently displayed layer ISN'T the one we chose (to avoid redundant pointer update)
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
									weight_layer = net->net->input_layer;
									for(int j = 0; j < net->net->layer_count -1; j++){
										cur_num_weights = weight_layer->num_nodes*weight_layer->next_layer->num_nodes;
										display_weight_text[j][cur_num_weights+1] = false;
										weight_layer = weight_layer->next_layer;
									}
								}
								else{
									output_layer = false;
									// Set bool for displaying all layer weights to true for this layer
									weight_layer = net->net->input_layer;
									for(int j = 0; j < net->net->layer_count -1; j++){
										cur_num_weights = weight_layer->num_nodes*weight_layer->next_layer->num_nodes;
										if(j == i){
											display_weight_text[j][cur_num_weights+1] = true;
										}
										else{
											display_weight_text[j][cur_num_weights+1] = false;
										}
										weight_layer = weight_layer->next_layer;
									}
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
						if(nodes[i][j].getGlobalBounds().contains(net_disp_window.mapPixelToCoords(sf::Mouse::getPosition(net_disp_window)))){

							if((cur_node_disp_layer != i) || (cur_node_disp_node != j || !node_window_open)){
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
				if(forward_rect.getGlobalBounds().contains(net_disp_window.mapPixelToCoords(sf::Mouse::getPosition(net_disp_window)))){
					net->net->forward_prop();
					update_stats_window = true;
					update_node_window = true;

				}

				// If reset is clicked
				if(reset_rect.getGlobalBounds().contains(net_disp_window.mapPixelToCoords(sf::Mouse::getPosition(net_disp_window)))){
					net->reset_network();
					update_stats_window = true;
					update_node_window =true;
				}

				// If display weights is clicked
				if(display_weights_rect.getGlobalBounds().contains(net_disp_window.mapPixelToCoords(sf::Mouse::getPosition(net_disp_window)))){
					display_weight_text[net->net->layer_count - 1][0] = !display_weight_text[net->net->layer_count - 1][0];
				}

				// If delete is clicked
				if(delete_rect.getGlobalBounds().contains(net_disp_window.mapPixelToCoords(sf::Mouse::getPosition(net_disp_window)))){
					net->delete_network();
					net_disp_window.close();
					return;
					//goto start_display;

					//display_net();

				}

			}

		}

// ------- Display all of our generated items ---------

	// Clear current frame
	net_disp_window.clear();

	// Loop through our layers and draw their headings
	for(int i = 0; i < layer_count; i++){
		net_disp_window.draw(layer_title[i]);
	}

	// Loop through each layer and draw nodes in layer
	l = net->net->input_layer;
	for(int i = 0; i < layer_count; i++){
		// Draw node j from layer i
		for(int j=0; j < l->num_nodes; j++){
			net_disp_window.draw(nodes[i][j]);
		}
		l = l->next_layer;
	}

	// Draw our title text(s)
	net_disp_window.draw(title);
	//net_disp_window.draw(options);

	// Draw our button rectangles (done before text)
	//net_disp_window.draw(forward_rect);
	//net_disp_window.draw(reset_rect);
	//net_disp_window.draw(display_weights_rect);
	//net_disp_window.draw(delete_rect);

	// Draw our button text
	//net_disp_window.draw(forward_text);
	//net_disp_window.draw(reset_text);
	//net_disp_window.draw(display_weights_text);
	//net_disp_window.draw(delete_text);

	// Draw line dividing options and display sections
	//net_disp_window.draw(options_line, 2, sf::Lines);

	// Loop through each layer (except output) and draw our connecting lines from 3d array
	l = net->net->input_layer;
	for(int i = 0; i < layer_count-1; i++){
		// Each layer contains nodes*next_nodes lines to connect them
		for(int j = 0; j < l->num_nodes*l->next_layer->num_nodes; j++){
			// Draw line j from layer i
			net_disp_window.draw(test_lines[i][j], 2, sf::Lines);
		}
		l = l->next_layer;
	}

	// Loop through our text array for displaying weight values
	// If display all bool is set

	if(display_weight_text[net->net->layer_count - 1][0]){
		l = net->net->input_layer;
		for(int i = 0; i < net->net->layer_count - 1; i++){
			for(int j = 0; j < l->num_nodes*l->next_layer->num_nodes; j++){
				net_disp_window.draw(weight_text[i][j]);
			}
			l = l->next_layer;
		}
	}
	// If display all isn't set, check for display layer or node
	else{

		// Check for each layers bool display value
		l = net->net->input_layer;
		for(int i = 0; i < net->net->layer_count - 1; i++){
			if(display_weight_text[i][l->num_nodes*l->next_layer->num_nodes + 1]){
				for(int j = 0; j < l->num_nodes*l->next_layer->num_nodes; j++){
					net_disp_window.draw(weight_text[i][j]);
				}
			}
			l = l->next_layer;
		}
	}

	// Display our window
	net_disp_window.display();
	}
	return;
}



// Function to open window displaying stats of selected node. Called in new thread
void NN_Display::display_node_stats(){
	cur_node_disp_layer = -1;
	cur_node_disp_node = -1;

	double x = 500;
	double y = 300;



	sf::Text title;
	sf::Text node_data;

	Layer* layer = net->net->input_layer;
	// Object for our text font and then load. Exit if not found
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf")){
		cout << "FONT NOT FOUND!!\n";
		std::exit(EXIT_FAILURE);
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
	node_data_window.setPosition(sf::Vector2((int)layer_data_window.getPosition().x, (int)layer_data_window.getPosition().y + (int)layer_data_window.getSize().y + spacer*3));

	// ---------------------------------------- Main Open Loop --------------------------------------------------
	int layer_num = 0;
	int node_num = 0;
	while (node_data_window.isOpen()){

		if(close_node_disp){
			close_node_disp = false;
			node_window_open = false;
			update_node_window = true;
			node_data_window.close();
			return;
		}
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
			if (node_data_event.type == sf::Event::Closed)
			{
				node_data_window.close();
				node_window_open = false;
				return;

			}
			else if(node_data_event.type == sf::Event::KeyPressed){
				if(node_data_event.key.code == sf::Keyboard::Escape){
					node_data_window.close();
					node_window_open = false;
					return;
				 }
			}

		}


		node_data_window.clear();

		node_data_window.draw(title);
		node_data_window.draw(node_data);
		node_data_window.display();
	}
	return;


}
// Function to open window displaying stats of selected layer (from layer_ptr). Called in new thread
void NN_Display::display_layer_stats(){
	cur_node_disp_layer = -1;
	cur_node_disp_node = -1;
	disp_layer = -1;
// ---------------------------------------- Initial Setuo ---------------------------------------------------

	// Set initial resolution
	double x = 600;
	double y = 700;

	// Set pixel spacer
	int spacer = 20;

	// Background color for Buttons (gray-ish);
	sf::Color bg_button(128, 128, 128);

	// Text objects for displaying layer info
	sf::Text title;
	sf::Text layer_stats;
	sf::Text weight_title;
	sf::Text weight_sub;
	sf::Text weight_stats;

	// Text objects for buttons
	sf::Text display_weights_text;

	// Rect shape objects for buttons
	sf::RectangleShape display_weights_rect;

	// Font object used in window. Fail if unable to load
	sf::Font title_font;
	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		std::exit(EXIT_FAILURE);
	}

	// Create new window and set to right of main window
	layer_data_window.create(sf::VideoMode(x * screenScalingFactor, y * screenScalingFactor), "");
	layer_data_window.setPosition(sf::Vector2((int)net_disp_window.getPosition().x + (int)net_disp_window.getSize().x + 10, (int)net_disp_window.getPosition().y));

	// Set attributes for display weights button
	display_weights_text.setFont(title_font);
	display_weights_text.setString("Disp. Weights");
	display_weights_text.setCharacterSize(max_font_size * .5);
	display_weights_text.setFillColor(sf::Color::Black);
	display_weights_text.setPosition(spacer/2, y - display_weights_text.getGlobalBounds().height - spacer);

	display_weights_rect.setPosition(display_weights_text.getPosition().x, display_weights_text.getPosition().y);
	display_weights_rect.setSize(sf::Vector2f(display_weights_text.getGlobalBounds().width + spacer, display_weights_text.getGlobalBounds().height*2));
	display_weights_rect.setFillColor(bg_button);
	display_weights_rect.setOutlineColor(sf::Color::White);
	display_weights_rect.setOutlineThickness(1);

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

		if(close_layer_disp){
			close_layer_disp = false;
			stats_window_open = false;
			update_stats_window = true;
			layer_data_window.close();
			return;
		}
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
			layer_stats.setString(layer_stats.getString() + "* Layer Output Vector: \n----------\n" + temp + "\n\n----------------------------------\n");

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
				weight_title.setString("* Output layer - No Weights...\n----------");
				weight_title.setPosition(title.getPosition().x, layer_stats.getPosition().y + layer_stats.getGlobalBounds().height + spacer);
			}
			update_stats_window = false;
		}

		// Get current events in window
		while (layer_data_window.pollEvent(layer_data_event)){

			// If closing, close and update object that window is closed
			if (layer_data_event.type == sf::Event::Closed)
			{
				layer_data_window.close();
				stats_window_open = false;
				return;
			}

			else if (layer_data_event.type == sf::Event::KeyPressed)
			{
				if(layer_data_event.key.code == sf::Keyboard::Escape){
					layer_data_window.close();
					stats_window_open = false;
					return;
				}

			}


			// If clicked, check if clicked on objects
			else if(layer_data_event.type == sf::Event::MouseButtonPressed){

				// Check if our display weights button was clicked
				if(display_weights_rect.getGlobalBounds().contains(layer_data_window.mapPixelToCoords(sf::Mouse::getPosition(layer_data_window)))){
					if(!output_layer){
						display_weight_text[layer_ptr->num][layer_ptr->num_nodes*layer_ptr->next_layer->num_nodes + 1] = !display_weight_text[layer_ptr->num][layer_ptr->num_nodes*layer_ptr->next_layer->num_nodes + 1];
					}
				}
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

			// Display our button rectangles
			layer_data_window.draw(display_weights_rect);
			// Display our button text
			layer_data_window.draw(display_weights_text);
		}

		// Dislay window with drawn objects
		layer_data_window.display();
	}
	return;

}