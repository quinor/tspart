#pragma once

#include "API.hh"
#include "Image.hh"
#include "Points.hh"
#include "General.hh"


template<typename... Ts>
class Graph : public General, public Ts...
{};
