// Gordian by Daniel Luna

#pragma once

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/Time.hpp"

namespace Gordian
{


/// Interface for a renderable class.
/// Renderable objects can be drawn with BlendTime.
class IRenderable
{

public:

	virtual ~IRenderable() {}

	virtual void Render(sf::Time BlendTime, sf::RenderTarget& Target, sf::RenderStates States) const = 0;

};


};
