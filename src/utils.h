/* Utility header for various helper structs and functions
*/
#pragma once
//---------------- Includes ----------------
#include "Platform/Platform.hpp"				// SFML include for OS platform
#include "nnet.h"								// Neural net object
#include "layer.h"								// Individual layer object in net
#include "net_helper.h"							// Helper class for various net functions
#include "nn_gui_setup.h"						// GUI Setup (main menu and loading functions)
#include "nn_display.h"							// Display functions (show loaded net)
#include <iostream>
#include <math.h>
#include <string>
#include <thread>
#include <vector>
#include "boost/filesystem.hpp"					// Boost lib for filesystem I/O simplicity
#include <unistd.h>

//---------------- Defines ----------------
#define PI 3.14159265

//------- Util Function Declarations -------

string display_file_select_load(int pos_x, int pos_y, string extension);		// Display a window for which net to load and return its path



//---------------- Simple Structs ----------------
// Button struct for GUI portion (grouping text and rect together for simplicity)
struct Button
{
	sf::Text text;
	sf::RectangleShape rect;
	sf::Font font;

	Button(std::string button_text, int x, int y){
		font.loadFromFile("fonts/akira.otf");
		text.setFont(font);
		text.setString(button_text);
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::Black);
		text.setPosition(x, y);

		rect.setPosition(text.getPosition().x, text.getPosition().y);
		rect.setSize(sf::Vector2f(text.getGlobalBounds().width + 5, text.getGlobalBounds().height*2));
		rect.setFillColor(sf::Color(128,128,128));
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);
	}
};


