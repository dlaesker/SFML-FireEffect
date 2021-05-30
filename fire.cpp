#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <stdio.h>

/* ********************** *
 * Author: Denis Laesker  *
 ************************ */

/* Description:
 This is an implementation of the Fire Effect inspired by the work by Lode Vandevenne at https://lodev.org/cgtutor/fire.html. This file simply replicates his work using SFML. However, I eliminate the need for a secondary drawing buffer. For an optimized implementation of the effect, consult the source code in "fireOptimized.cpp."
 */

#define FRAME_RATE  	20
#define WIN_WIDTH 		640
#define WIN_HEIGHT		128
#define WIN_BG_COLOR  sf::Color::White

sf::Uint32 fire[WIN_HEIGHT][WIN_WIDTH];	// Buffer to contain the fire...
std::vector<sf::Color> palette;					// ...and this will contain the colors

// Derived from -> https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB
sf::Color HSLtoRGB(int hue, int saturation, int lightness){
	int chroma = (1 - abs(2*lightness - 1)) * saturation;
	int _hue = hue / 60;
	
	int X = chroma * (1 - abs(_hue % (2 - 1)));
	
	int m = lightness - chroma / 2;
	
	if((_hue >= 0) && (_hue < 1))	return sf::Color(chroma + m, X + m, 0 + m);
	if((_hue >= 1) && (_hue < 2)) return sf::Color(X + m, chroma + m, 0 + m);
	if((_hue >= 2) && (_hue < 3)) return sf::Color(0 + m, chroma + m, X + m);
	if((_hue >= 3) && (_hue < 4)) return sf::Color(0 + m, X + m, chroma + m);
	if((_hue >= 4) && (_hue < 5)) return sf::Color(X + m, 0 + m, chroma + m);
	if((_hue >= 5) && (_hue < 6)) return sf::Color(chroma + m, 0 + m, X + m);
	return sf::Color(0 + m, 0 + m, 0 + m);
}

int main(int argc, char *argv[]){
	/* SFML setup */
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	
	sf::RenderWindow window(
		sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "FIRE!", sf::Style::Default, settings
	);
	
	window.setFramerateLimit(FRAME_RATE);

	// Initialize the fire buffer to zero
	for(int y = 0; y < WIN_HEIGHT; y++){
		for(int x = 0; x < WIN_WIDTH; x++){
			fire[y][x] = 0;
		}
	}

  // Generate the palette
  for(int x = 0; x < 256; x++){
    // HSLtoRGB is used to generate colors:
    // Hue goes from 0 to 85: red to yellow
    // Saturation is always the maximum: 255
    // Lightness is 0..255 for x=0..128, and 255 for x=128..255
		palette.push_back(HSLtoRGB(x / 3, 255, std::min(255, x * 2)));
  }
	
	sf::Event event;
	while(window.isOpen()){
		while(window.pollEvent(event)){ // Poll for events
			if(event.type == sf::Event::Closed)
						window.close();
			
			if(	(event.type == sf::Event::KeyPressed) &&
					(event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		
		// Randomize the bottom row of the fire buffer
		for(int x = 0; x < WIN_WIDTH; x++)
			fire[WIN_HEIGHT - 1][x] = abs(32768 + rand()) % 256;
			
		//do the fire calculations for every pixel, from top to bottom
		for(int y = 0; y < WIN_HEIGHT - 1; y++){
			for(int x = 0; x < WIN_WIDTH; x++){
				fire[y][x] =
        ((fire[(y + 1) % WIN_HEIGHT][(x - 1 + WIN_WIDTH) % WIN_WIDTH]
        + fire[(y + 2) % WIN_HEIGHT][(x) % WIN_WIDTH]
        + fire[(y + 1) % WIN_HEIGHT][(x + 1) % WIN_WIDTH]
        + fire[(y + 3) % WIN_HEIGHT][(x) % WIN_WIDTH])
        * 64) / 257;
			}
		}
		
		window.clear(WIN_BG_COLOR);
		
		// Draw fire!
		for(int y = 0; y < WIN_HEIGHT; y++){
			for(int x = 0; x < WIN_WIDTH; x++){
				// Rectangles to represent every pixel on the window.
				sf::RectangleShape pixel;
				pixel.setSize({ 1.f, 1.f });
				pixel.setFillColor({ 	palette[fire[y][x]].r,
															palette[fire[y][x]].g,
															palette[fire[y][x]].b });
	
				pixel.setPosition(sf::Vector2f(x, y));
				window.draw(pixel);
			}
		}
		window.display();
	}
}
