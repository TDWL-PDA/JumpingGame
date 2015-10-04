#include "pch.h"
#include "Blockades.h"

using namespace Windows::Graphics::Display;

Blockades::Blockades(XMFLOAT2 size, XMFLOAT2 position,
	Windows::Foundation::Rect* movementBounds, int dividers[]) : Sprite(m_Texture, size, position, movementBounds, 10, 2, 12, dividers)
{
	posX = position.x;	//probably want to get rid of these if I'm not going to use them...
	posY = position.y;
	keepJumping = false;
	Speed = 300;

	scale = DisplayProperties::LogicalDpi / 96.0f;

	m_Texture = nullptr;
}
