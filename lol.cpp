#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>

// Initialize some mutable modifiers
int max_shapes = 10;
int size_modder = 1;
int border_mod = 0;
int rotation_mod = 10;

class ModMenu {
private:
  struct Item {
  public:
    int* attrib_ptr;
    const int MAX;
    const int MIN;
    std::string label;
    sf::Time wasIncd;
    sf::Time wasDecd;
    Item(std::string l, int* att, int min, int max): label(l), attrib_ptr(att), MAX(max), MIN(min) {}
  };
  sf::Font font;
  sf::Text texbuf;
  sf::Clock clock;
  std::vector<Item> items;
  int hilit_index;
  int x, y, item_w, item_h;
public:
  ModMenu(int xp, int yp, int it_w, int it_h, sf::Font f, sf::Clock& c);
  void addItem(std::string label, int* att, int min, int max);
  void handleInput(sf::Event& event);
  void draw(sf::RenderWindow& window);
};

ModMenu::ModMenu(int xp, int yp, int it_w, int it_h, sf::Font f, sf::Clock& c)
  : hilit_index(0), x(xp), y(yp), font(f), item_w(it_w), item_h(it_h), clock(c) {

  texbuf.setFont(font);
  texbuf.setCharacterSize(16);
  texbuf.setColor(sf::Color::Black);
}

void ModMenu::addItem(std::string label, int* att, int min, int max) {
  items.push_back(Item(label, att, min, max));
}

void ModMenu::handleInput(sf::Event& event) {
  if (event.key.code == sf::Keyboard::Up) {
    if(--hilit_index < 0)
      hilit_index = items.size() - 1;
  }
  else if (event.key.code == sf::Keyboard::Down) {
    if(++hilit_index >= items.size())
      hilit_index = 0;
  }
  else if (event.key.code == sf::Keyboard::Left) {
    if (*(items[hilit_index].attrib_ptr) > items[hilit_index].MIN) {
      --*(items[hilit_index].attrib_ptr);
      items[hilit_index].wasDecd = clock.getElapsedTime();
    }
  }
  else if (event.key.code == sf::Keyboard::Right) {
    if (*(items[hilit_index].attrib_ptr) < items[hilit_index].MAX) {
      ++*(items[hilit_index].attrib_ptr);
      items[hilit_index].wasIncd = clock.getElapsedTime();
    }
  }
}

void ModMenu::draw(sf::RenderWindow& window) {
  int cur_y = y;
  sf::RectangleShape rect;
  rect.setOutlineThickness(1);
  rect.setOutlineColor(sf::Color::Black);
  
  for(std::vector<Item>::size_type i = 0; i < items.size(); ++i) {
    rect.setPosition(x,cur_y);
    rect.setSize(sf::Vector2f(item_w, item_h));
    // Draw the rectangle
    if (i == hilit_index)
      rect.setFillColor(sf::Color(255, 255, 255, 244));
    else
      rect.setFillColor(sf::Color(211,222,233,200));
    window.draw(rect);

    // Draw the arrows
    sf::ConvexShape arrow;
    arrow.setOutlineThickness(1);
    arrow.setOutlineColor(sf::Color::Black);
    if ((clock.getElapsedTime() - items[i].wasDecd).asSeconds() < 0.1) {
      arrow.setFillColor(sf::Color(255, 255, 255, 244));
    }
    else
      arrow.setFillColor(sf::Color(211,222,233,200));
    arrow.setPointCount(3);
    arrow.setPoint(0, sf::Vector2f(x - 10, cur_y + item_h/2));
    arrow.setPoint(1, sf::Vector2f(x, cur_y));
    arrow.setPoint(2, sf::Vector2f(x, cur_y + item_h));
    window.draw(arrow);

    if ((clock.getElapsedTime() - items[i].wasIncd).asSeconds() < 0.1) {
      arrow.setFillColor(sf::Color(255, 255, 255, 244));
    }
    else
      arrow.setFillColor(sf::Color(211,222,233,200));
    arrow.setPoint(0, sf::Vector2f(x + item_w, cur_y));
    arrow.setPoint(1, sf::Vector2f(x + item_w, cur_y + item_h));
    arrow.setPoint(2, sf::Vector2f(x + item_w + 10, cur_y + item_h/2));
    window.draw(arrow);

    // Draw the label
    texbuf.setString(items[i].label);
    int tw = texbuf.getLocalBounds().width;
    int th = texbuf.getLocalBounds().height;
    texbuf.setPosition(x + (item_w-tw)/2, cur_y);
    window.draw(texbuf);

    // Draw the value
    texbuf.setString(std::to_string(*items[i].attrib_ptr));
    rect.setSize(sf::Vector2f(texbuf.getLocalBounds().width + 8, item_h));
    rect.setFillColor(sf::Color(255, 255, 255, 244));    
    rect.setPosition(x + item_w + 14, cur_y);
    window.draw(rect);
    
    texbuf.setPosition(x + item_w + 14, cur_y);
    window.draw(texbuf);

    // Update the top position for the next value 'widget'
    cur_y += item_h + 2;
  }

  // Tell user to use arrow keys
  texbuf.setString("Use the arrow keys\nto navigate the menu");
  rect.setPosition(x, cur_y);
  rect.setSize(sf::Vector2f(texbuf.getLocalBounds().width + 8, texbuf.getLocalBounds().height + 8));
  rect.setFillColor(sf::Color(255, 255, 255, 244));    
  window.draw(rect);
  texbuf.setPosition(x, cur_y);
  window.draw(texbuf);
  
}

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
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Twinkly Shapes");
  window.setFramerateLimit(55);

  // Store our shapes here
  std::vector<sf::Shape*> shapes;

  // Load the font
  sf::Font font;
  if (!font.loadFromFile("CutiveMono-Regular.ttf")) {
    std::cout << "Error: Could not load font." << std::endl;
    window.close();
    return -1;
  }

  // Start the clock
  sf::Clock clock;

  // Set up the menu to change modifiers
  ModMenu mod_menu(24, 16, 128, 24, font, clock);
  mod_menu.addItem("Max Shapes", &max_shapes, 2, 99999);
  mod_menu.addItem("Size Mod", &size_modder, -25, 99999);
  mod_menu.addItem("Border Mod", &border_mod, -8, 99999);
  mod_menu.addItem("Rotation Mod", &rotation_mod, 1, 20);
  
  // Main loop
  while(window.isOpen()) {
    // Input and event handling
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
	window.close();

      if (event.type == sf::Event::KeyPressed) {
	if (event.key.code == sf::Keyboard::Escape)
	  window.close();
	else
	  mod_menu.handleInput(event);
      }
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
    // Draw the menu
    mod_menu.draw(window);
    
    window.display();
    
    // Shrink shapes and delete once they disappear
    for(std::vector<sf::Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
      sf::Vector2f scale = (*it)->getScale();
      // 1/10 chance to rotate slightly
      if(std::rand() % rotation_mod == 0)
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
