#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <stdio.h>

/* ********************** *
 * Author: Denis Laesker  *
 ************************ */

/* Description:
 This file provides a code optimization on the Fire Effect presented by Lode Vandevenne at https://lodev.org/cgtutor/fire.html. Here, I represent the fire buffer as a single-dimensional array (as opposed as using a 2D array). I also provide a time complexity reduction for computing the values in the fire buffer and drawing the individual pixels from O(n*m) to ~O(n + m).
 */

#define FRAME_RATE  	20
#define WIN_HEIGHT		128
#define WIN_WIDTH 		640
#define SIZE					(WIN_HEIGHT * WIN_WIDTH)
#define WIN_BG_COLOR  sf::Color::White

#define IX(x, y)(x + y*WIN_WIDTH) // To access array elements

sf::Uint32 fire[SIZE]; 					// Buffer to contain the fire...
std::vector<sf::Color> palette;	// ...and this will contain the colors

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

int main(){
	srand(time(NULL));
	int h = WIN_HEIGHT, w = WIN_WIDTH; // Easier to type :)
	
	// SFML setup
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	
	sf::RenderWindow window(sf::VideoMode(w, h), "FIRE!", sf::Style::Default, settings);
	
	window.setFramerateLimit(FRAME_RATE);
	
	// Initializing the fire buffer to all zeroes
	for(int i = 0; i < SIZE; ++i) fire[0] = 0;
	
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
		for(int x = 0; x < w; x++)
			fire[(h - 1)*w + x] = abs(32768 + rand()) % 256;
	
		int loc1,loc2,loc3,loc4;
		loc1 = loc2 = loc3 = loc4 = 0;
		for(int z = 0; z < SIZE; ++z){
			int y = z / w; // Row
			int x = z % w; // Col
			
			loc1 = w*((y + 1) % h) + (x - 1 + w) % w;
			loc2 = w*((y + 2) % h) + x % w;
			loc3 = w*((y + 1) % h) + (x + 1) % w;
			loc4 = w*((y + 3) % h) + x % w;
			
			fire[IX(x,y)] = ((fire[loc1] + fire[loc2] + fire[loc3] + fire[loc4]) * 64) / 257;
		}
	
		window.clear(WIN_BG_COLOR);
		
		for(int z = 0; z < SIZE; ++z){
			sf::RectangleShape pixel;
			pixel.setSize({ 1.f, 1.f });
			
			int y = z / w; // Row
			int x = z % w; // Col
			
			pixel.setFillColor({ 	palette[fire[IX(x,y)]].r,
														palette[fire[IX(x,y)]].g,
														palette[fire[IX(x,y)]].b });
			
			pixel.setPosition(sf::Vector2f(x, y));
			window.draw(pixel);
		}
		window.display();
	}
	return 0;
}
