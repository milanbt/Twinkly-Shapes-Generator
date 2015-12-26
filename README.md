# Twinkly-Shapes-Generator
Generates random shrinking shapes

- Need to have SFML installed
- Tested on Debian Stretch, Jessie, and Mac OS X 10.11.2

I like to set size_modder, border_mod, and rotation_mod to their minimums and increase max shapes. It looks like some kind of  twinkling stars.

To compile: c++ lol.cpp -lsfml-graphics -lsfml-window -lsfml-system -o lol.out

On Mac OS X: c++ -F /Library/Frameworks lol.cpp -framework SFML -framework sfml-graphics\
       	     -framework sfml-window -framework sfml-system -o lol.out

To run: ./lol.out

