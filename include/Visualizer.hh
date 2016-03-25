#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

void visualize_points_polygons(
	sf::Vector2u size,
	std::vector<sf::Vector2f> points,
	std::vector<std::vector<sf::Vector2f> > polygons
);

void visualize_polyline(
	sf::Vector2u size,
	std::vector<sf::Vector2f> polyline
);
