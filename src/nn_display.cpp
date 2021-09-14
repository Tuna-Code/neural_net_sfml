#include "Platform/Platform.hpp"
#include "layer.h"
#include "net_helper.h"
#include "nnet.h"
#include <iostream>
#include <string>
#include "nn_display.h"

using namespace std;

NN_Display::NN_Display(Net_Helper* net){
	this->net = net;

	win_x = 1920;
	win_y = 1080;
	// Window dimensionts

	// in Windows at least, this must be called before creating the window
	screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
}

void NN_Display::display_net(){
	// ---------------------------------------- SETUP ------------------------------

	int layer_count = 10;//net->net->layer_count;
	int layer_font_size = 25;
	sf::Text title;
	sf::Text menu;
	sf::Font title_font;
	sf::Text* layer_title = NULL;

	layer_title = new sf::Text[layer_count];

	int spacer = 20;

	if (!title_font.loadFromFile("fonts/akira.otf"))
	{
		cout << "FONT NOT FOUND!!\n";
		exit(EXIT_FAILURE);
	}
		// Set font attributes for heading
	title.setFont(title_font);
	title.setString("Neural Network Visualizer");
	title.setCharacterSize(30);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	title.setPosition(win_x / 2 - (title.getGlobalBounds().width) / 2, win_y / 50);

	menu.setFont(title_font);
	menu.setString("Options");
	menu.setCharacterSize(15);
	menu.setFillColor(sf::Color::White);
	menu.setStyle(sf::Text::Bold | sf::Text::Underlined);
	menu.setPosition(20,title.getPosition().y + menu.getGlobalBounds().height/2);



	sf::Vertex menu_line[] =
	{
    sf::Vertex(sf::Vector2f(menu.getPosition().x + menu.getGlobalBounds().width + spacer, 0)),
    sf::Vertex(sf::Vector2f(menu.getPosition().x + menu.getGlobalBounds().width + spacer, win_y))
	};


	window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");

	platform.setIcon(window.getSystemHandle());
	cout << "Displaying Network...\n";
	net->net->forward_prop();
	//net->print_network();

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
	layer_title[0].setPosition(op_x, op_y);


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
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}

		}



	window.clear();


	for(int i = 0; i < layer_count; i++){
		window.draw(layer_title[i]);
	}

	window.draw(title);
	window.draw(menu);
	window.draw(menu_line, 2, sf::Lines);
	window.display();
	}

}