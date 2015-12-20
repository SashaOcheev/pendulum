#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

const sf::Vector2i WIN_SIZE = { 800, 600 };
const int ANTIALIASING_LEVEL = 8;

const struct
{
	float VELOCITY = 0.1f;
	float TOUCH = 25.f;
	float MAX = 45.f;
	float BOOST = 0.01f;
} ANGLE;

const struct
{
	std::string FILE = "images/gear.png";
	sf::Vector2f CENTER = { 75.f, 75.f };
	sf::Vector2f POSITION_1 = { 200.f, 300.f };
	float ANGLE_1 = 0.f;
	sf::Vector2f DISTANCE = { 137.f, 0.f };
	float ANGLES_DIFFERENT = 15.f;
	sf::Vector2f POSITION_2 = POSITION_1 + DISTANCE;
	float ANGLE_2 = ANGLE_1 + ANGLES_DIFFERENT;
} GEARS;

const struct
{
	sf::Vector2f POSITION = { 450, 160 };
	std::vector<sf::Vector2f> LEFT_HAMMER = { { 0, 0 },{ -100, 50 },{ 0, 30 } };
	std::vector<sf::Vector2f> RIGHT_HAMMER = { { 0, 0 },{ 100, 50 },{ 0, 30 } };
	std::vector<sf::Vector2f> BOTTOM = { { 0, 0 },{ -30, 300 },{ 30, 300 } };
} PENDULUM;

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

	void updateSprite(float rotation)
	{
		angle += rotation;
		sprite.setRotation(angle);
	}
};
////////////////////////////////////////////////////////////

//      pendulum       /////////////////////////////////////

sf::ConvexShape setConvex(std::vector<sf::Vector2f> dots, sf::Vector2f position, float angle)
{
	sf::ConvexShape convex;
	convex.setPointCount(dots.size());
	for (int i = 0; i < dots.size(); i++)
		convex.setPoint(i, dots[i]);
	convex.setOrigin(0.f, 0.f);
	convex.setPosition(position);
	convex.setFillColor(sf::Color::Black);
	convex.setRotation(angle);
	return convex;
}

struct Pendulum
{
	sf::ConvexShape left_hammer;
	sf::ConvexShape right_hammer;
	sf::ConvexShape bottom;
	float angle;

	Pendulum()
	{
		angle = ANGLE.MAX;
		left_hammer = setConvex(PENDULUM.LEFT_HAMMER, PENDULUM.POSITION, angle);
		right_hammer = setConvex(PENDULUM.RIGHT_HAMMER, PENDULUM.POSITION, angle);
		bottom = setConvex(PENDULUM.BOTTOM, PENDULUM.POSITION, angle);
	}

	void update(float rotation)
	{
		angle += rotation;
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

//     velocity        /////////////////////////////////////
struct Velocity
{
	float boost;
	float gear_velocity;
	float pendulum_velocity;
	
	Velocity()
	{
		boost = -ANGLE.BOOST;
		gear_velocity = ANGLE.VELOCITY;
		pendulum_velocity = 0.f;
	}

	void update(float pendulum_angle)
	{
		float new_angle = pendulum_angle + pendulum_velocity * pendulum_velocity / 2 / boost;
		if (abs(new_angle) >= ANGLE.MAX)
			boost = -boost;

		if (pendulum_velocity > 0)
		{
			if (pendulum_angle >= ANGLE.TOUCH)
				gear_velocity = -ANGLE.VELOCITY;
		}
		if (pendulum_velocity < 0)
		{
			if (pendulum_angle <= ANGLE.TOUCH)
				gear_velocity = ANGLE.VELOCITY;
		}
		pendulum_velocity += boost;
	}
};
////////////////////////////////////////////////////////////

//       shapes        /////////////////////////////////////
struct Init
{
	Sprite_init gear1;
	Sprite_init gear2;
	Pendulum pendulum;
	Velocity velocity;

	Init()
	{
		gear1.setSprite(GEARS.FILE, GEARS.POSITION_1, GEARS.CENTER, GEARS.ANGLE_1);
		gear2.setSprite(GEARS.FILE, GEARS.POSITION_2, GEARS.CENTER, GEARS.ANGLE_2);
		gear1.sprite.setColor(sf::Color::Blue);
		gear2.sprite.setColor(sf::Color::Red);
	}


	void update()
	{
		gear1.updateSprite(ANGLE.VELOCITY);
		gear2.updateSprite(-ANGLE.VELOCITY);
		pendulum.update(0.f);
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

		init.update();
		init.drawShapes(window);

		window.display();
	}
	return 0;
}