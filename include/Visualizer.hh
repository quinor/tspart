#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

//for polygon each edge should be a pair of two consecutive vector elements
void visualize_points_polygons(
	sf::Vector2u size,
  int scale_factor,
	const std::vector<sf::Vector2f>& points,
	const std::vector<std::vector<sf::Vector2f> >& polygons
);

void visualize_polyline(
	sf::Vector2u size,
  int scale_factor,
	const std::vector<sf::Vector2f>& polyline
);
