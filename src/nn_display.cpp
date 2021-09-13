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
	sf::Text title;
	sf::Font title_font;


	window.create(sf::VideoMode(win_x * screenScalingFactor, win_y * screenScalingFactor), "Neural Netowrk GUI Interface");

	platform.setIcon(window.getSystemHandle());
	cout << "Displaying Network...\n";
	net->net->forward_prop();
	//net->print_network();

	int l2_size = net->net->input_layer->next_layer->next_layer->num_nodes;
	//cout << l2_size;
	// Setup layer 2 circles
	sf::CircleShape* l2 = NULL;

	l2 = new sf::CircleShape[l2_size];

	for(int i = 1; i <= l2_size; i++){
		l2[i-1].setRadius(20*i);
		l2[i-1].setPosition(200*i,200*i);
		l2[i-1].setFillColor(sf::Color::Black);
		l2[i-1].setOutlineThickness(2);
		l2[i-1].setOutlineColor(sf::Color::White);

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



	while (window.isOpen()){


		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}

		}



	window.clear();

	for(int i = 0; i < l2_size; i++){

		window.draw(l2[i]);
	}
	window.draw(title);
	window.display();
	}

}