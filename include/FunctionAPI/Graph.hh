#pragma once

#include "API.hh"
#include "ImageAPI.hh"
#include "PointsAPI.hh"


template<typename... Ts>
class Graph : public API, public Ts...
{};
