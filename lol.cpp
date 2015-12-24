#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>

// Initialize some mutable modifiers
int max_shapes = 10;
int size_modder = 1;
int border_mod = 0;

// Adds a random shape to a vector<Shape*>
// Delete the added pointer when done
void shape_adder1(std::vector<sf::Shape*>& shapes) {
  int r = std::rand() % 10;
  sf::Shape* s;

  // Create a circle
  if (r == 0) {
    s = new sf::CircleShape(std::rand() % 50 + size_modder);
    s->setPosition(std::rand() % 1280, std::rand() % 720);
    s->setFillColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
    s->setOutlineThickness(std::rand() % 15 + border_mod);
    s->setOutlineColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
    // Rotate to change the origin point
    // so circles shrink into a random direction
    s->rotate(std::rand() % 360);
    shapes.push_back(s);
  }
  // Create a rectangle
  else if(r == 1) {
    s = new sf::RectangleShape(sf::Vector2f(std::rand() % 75 + size_modder,
					    std::rand() % 75 + size_modder));
    s->setPosition(std::rand() % 1280, std::rand() % 720);
    s->setFillColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
    s->setOutlineThickness(std::rand() % 15 + border_mod);
    s->setOutlineColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
    s->rotate(std::rand() % 360);
    shapes.push_back(s);
  }
  // Create a 3~10gon
  else {
    s = new sf::CircleShape(std::rand() % 50 + size_modder, r+1);
    s->setPosition(std::rand() % 1280, std::rand() % 720);
    s->setFillColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
    s->setOutlineThickness(std::rand() % 15 + border_mod);
    s->setOutlineColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
    s->rotate(std::rand() % 360);
    shapes.push_back(s);
  }
}

int main(int argc, char** argv) {

  // Create window and set framerate
  sf::RenderWindow window(sf::VideoMode(1280, 720), "LOL");
  window.setFramerateLimit(55);

  // Store our shapes here
  std::vector<sf::Shape*> shapes;

  // Main loop
  while(window.isOpen()) {
    // Input and event handling
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
	window.close();
      // Increase and decrease the max number of shapes that can be in the vector with up and down keys
      // In reality, the shapes are deleted too fast to ever reach max_shapes
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) std::cout << ++max_shapes << std::endl;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
	if (max_shapes > 2)
	  std::cout << --max_shapes << std::endl;
      }
      // Modify the size of shapes with left and right keys
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) --size_modder;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ++size_modder;

      // Modify size of borders with A and D
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) --border_mod;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ++border_mod;

      // Print the number of instances in shapes
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) std::cout << shapes.size() << std::endl;
      
      
    }

    // Add new shapes
    int diff = max_shapes - shapes.size();
    if (diff > 0) {
      // Crude way to add more shapes to get closer to max_shapes
      for(int i = 0; i < diff/2+1; ++i)
	shape_adder1(shapes);
    }

    // Clear screen
    window.clear(sf::Color::Black);
    // Draw shapes
    for(std::vector<sf::Shape*>::size_type i = 0; i < shapes.size(); ++i) {
      window.draw(*shapes[i]);
    }
    window.display();
    // Shrink shapes and delete once they disappear
    for(std::vector<sf::Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
      sf::Vector2f scale = (*it)->getScale();
      // 1/10 chance to rotate slightly
      if(std::rand() % 10 == 0)
	(*it)->rotate(std::rand()%15);
      // Decreases scale by subtrahend of 0.1 for both x and y
      (*it)->setScale(scale.x - 0.1, scale.y - 0.1);
      // If the shape is invisible (scale (0,0)) then delete it to free memory
      if(scale.x <= 0 && scale.y <= 0) {
	delete (*it);
	shapes.erase(it);
      }
    }
  }
  
  return 0;
}
