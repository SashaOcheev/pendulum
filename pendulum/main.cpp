#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

const sf::Vector2i WIN_SIZE = { 800, 600 };
const int ANTIALIASING_LEVEL = 8;
const float ANGLE_VELOCITY = 0.1f;
const sf::Vector2f PENDULUM_POSITION = { 480, 160 };

const struct
{
	const std::string FILE = "images/gear.png";
	const sf::Vector2f CENTER = { 75.f, 75.f };
	const sf::Vector2f POSITION_1 = { 200.f, 300.f };
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

//      pendulum       /////////////////////////////////////

sf::ConvexShape setConvex(std::vector<sf::Vector2f> dots, sf::Vector2f position)
{
	sf::ConvexShape convex;
	convex.setPointCount(dots.size());
	for (int i = 0; i < dots.size(); i++)
		convex.setPoint(i, dots[i]);
	convex.setOrigin(0.f, 0.f);
	convex.setPosition(position);
	convex.setFillColor(sf::Color::Black);
	return convex;
}

struct Pendulum
{
	sf::ConvexShape left_hammer;
	sf::ConvexShape right_hammer;
	sf::ConvexShape bottom;

	Pendulum() {}

	void set(sf::Vector2f position)
	{
		std::vector<sf::Vector2f> left_hammer_dots = { {0, 0}, { -100, 50 }, { 0, 30 } };
		std::vector<sf::Vector2f> right_hammer_dots = { { 0, 0 }, { 100, 50 }, { 0, 30 } };
		std::vector<sf::Vector2f> bottom_dots = { { 0, 0 }, { -30, 300 },{ 30, 300 } };
		left_hammer = setConvex(left_hammer_dots, position);
		right_hammer = setConvex(right_hammer_dots, position);
		bottom = setConvex(bottom_dots, position);
	}

	void update(float angle)
	{
		left_hammer.setRotation(angle);
		right_hammer.setRotation(angle);
		bottom.setRotation(angle);
	}

	void draw(sf::RenderWindow &window)
	{
		window.draw(left_hammer);
		window.draw(right_hammer);
		window.draw(bottom);
	}
};
////////////////////////////////////////////////////////////

//       shapes        /////////////////////////////////////
struct Init
{
	Sprite_init gear1;
	Sprite_init gear2;
	Pendulum pendulum;

	Init()
	{
		gear1.setSprite(GEARS.FILE, GEARS.POSITION_1, GEARS.CENTER, GEARS.ANGLE_1);
		gear2.setSprite(GEARS.FILE, GEARS.POSITION_2, GEARS.CENTER, GEARS.ANGLE_2);
		gear1.sprite.setColor(sf::Color::Blue);
		gear2.sprite.setColor(sf::Color::Red);
		pendulum.set(PENDULUM_POSITION);
	}

	void updateShapes()
	{
		gear1.updateSprite(sf::Vector2f(0.f, 0.f), ANGLE_VELOCITY);
		gear2.updateSprite(sf::Vector2f(0.f, 0.f), -ANGLE_VELOCITY);
		pendulum.update(ANGLE_VELOCITY);
	}

	void drawShapes(sf::RenderWindow &window)
	{
		pendulum.draw(window);
		window.draw(gear1.sprite);
		window.draw(gear2.sprite);
	}
};
////////////////////////////////////////////////////////////

int main()
{
	Init init;

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

		init.updateShapes();
		init.drawShapes(window);

		window.display();
	}
	return 0;
}