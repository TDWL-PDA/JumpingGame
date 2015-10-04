#pragma once

#include "SpriteBatch.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>
#include <list>
#include "Sprite.h"

using namespace DirectX;
using namespace std;

class Blockades : public Sprite
{
public:
	Blockades(XMFLOAT2 size, XMFLOAT2 position,
		Windows::Foundation::Rect* movementBounds, int dividers[]);

	bool keepJumping;

	float posX;
	float posY;
	float scale;
};

