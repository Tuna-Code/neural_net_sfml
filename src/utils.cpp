#include "utils.h"


using namespace std;


/* Utility Function: Open window and display list of files
   Input: Pointer to net object
   Output: string out loaded net (+ loads net into provided pointer)
*/
string display_file_select_load(int pos_x, int pos_y, string extension){

	// Initial resolition
	double x = 400;
	double y = 600;

	int spacer = 10;
	int max_font_size = 28;
	sf::Text title;
	util::Platform platform;
	sf::RenderWindow window;

	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());


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

	boost::filesystem::path p(data_dir);
	boost::filesystem::directory_iterator end_itr;
	sf::Event event;


	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
	{
		if (is_regular_file(itr->path()))
		{
			//string current_file = itr->path().string();
			if(itr->path().extension().string() == extension){
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
	window.create(sf::VideoMode((largest_x + spacer) * screenScalingFactor, (start_y) * screenScalingFactor), "");
	window.setPosition(sf::Vector2(pos_x, pos_y));

	// ---------------------------------------- Main Open Loop --------------------------------------------------

	while (window.isOpen()){

		while (window.pollEvent(event)){
			// If closing
			if (event.type == sf::Event::Closed){
				window.close();
				return "";
			}

			else if(event.type == sf::Event::KeyPressed){

				if(event.key.code == sf::Keyboard::Escape){
					window.close();
					return "";
				}

			}

			else if(event.type == sf::Event::MouseButtonPressed){
				for(int i = 0; i < net_list.size(); i++){
					// Check if clicked on Layer title
					if(net_rect[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
						cout << "Clicked on: " << net_list[i] << endl;

						/*manual_net_loaded = false;
						file_net_loaded = true;
						net->load_from_file(net_list[i]);
						file_window.close();
						*/return net_list[i];


					}
				}

			}


		}





		window.clear();
		window.draw(title);
		for(int i = 0; i < net_list.size(); i++){
			window.draw(net_rect[i]);
			window.draw(net_text[i]);
		}
		window.display();

	}



}