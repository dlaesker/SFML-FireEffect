# SFML-FireEffect
Here I provide a C++ SFML implementation of the classic Fire Effect. First, I replicated the work presented by Lode Vandevenne at his [website](https://lodev.org/cgtutor/fire.html). Thereafer, I provide an optimization over his approach. 

## Fire Effect
The "fire" lives within a N X M buffer where every location (i, j) is within [0, 255]. This buffer is used to index into a palette of colors later drawn onto the screen. Palette colors are generated using a hue, saturation, and lighteness (HSL) to RGB conversion as described at (https://en.wikipedia.org/wiki/HSL_and_HSV). A more detailed explanation is provided by Vandvenne in his website.  

![Fire Effect][fireEffect1.gif]
![Fire Effect Optimized][fireEffect2.gif]


