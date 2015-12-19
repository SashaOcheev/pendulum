#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

const sf::Vector2i WIN_SIZE = { 800, 600 };
const int ANTIALIASING_LEVEL = 8;
const float ANGLE_VELOCITY = 0.1f;

const struct
{
	const std::string FILE = "images/gear.png";
	const sf::Vector2f CENTER = { 75.f, 75.f };
	const sf::Vector2f POSITION_1 = { 300.f, 400.f };
	const float ANGLE_1 = 0.f;
	const sf::Vector2f DISTANCE = { 137.f, 0.f };
	const float ANGLES_DIFFERENT = 15.f;
	const sf::Vector2f POSITION_2 = POSITION_1 + DISTANCE;
	const float ANGLE_2 = ANGLE_1 + ANGLES_DIFFERENT;
}GEARS;

//       sprite       //////////////////////////////////////
struct Sprite_init
{
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	float angle;

	Sprite_init(){}

	void setSprite(std::string texture_file, sf::Vector2f pos, sf::Vector2f origin, float ang)
	{
		texture.loadFromFile(texture_file);
		position = pos;
		angle = ang;

		sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setOrigin(origin);
		sprite.setRotation(angle);
	}

	void updateSprite(sf::Vector2f move, float rotation)
	{
		position.x += move.x;
		position.y += move.y;
		angle += rotation;
		sprite.setPosition(position);
		sprite.setRotation(angle);
	}
};
////////////////////////////////////////////////////////////

int main()
{
	Sprite_init gear1;
	Sprite_init gear2;
	gear1.setSprite(GEARS.FILE, GEARS.POSITION_1, GEARS.CENTER, GEARS.ANGLE_1);
	gear2.setSprite(GEARS.FILE, GEARS.POSITION_2, GEARS.CENTER, GEARS.ANGLE_2);
	gear2.sprite.setColor(sf::Color::Red);


	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTIALIASING_LEVEL;
	sf::RenderWindow window(sf::VideoMode(WIN_SIZE.x, WIN_SIZE.y), "Pendulum", sf::Style::Default, settings);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::White);

		gear1.updateSprite(sf::Vector2f(0.f, 0.f), ANGLE_VELOCITY);
		gear2.updateSprite(sf::Vector2f(0.f, 0.f), -ANGLE_VELOCITY);
		window.draw(gear1.sprite);
		window.draw(gear2.sprite);

		window.display();
		

	}
	return 0;
}