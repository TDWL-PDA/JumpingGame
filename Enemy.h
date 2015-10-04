#pragma once

#include "SpriteBatch.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>
#include "Sprite.h"

using namespace DirectX;

class Enemy
{
public:
	Enemy();
	Enemy(float x, float y,
		Windows::Foundation::Rect* movementBounds);
	static void LoadTexture(ID3D11Device* d3dDevice);
	void Draw(SpriteBatch* spriteBatch);

	static ID3D11ShaderResourceView* m_Texture;
	float posX, posY, diameter;

private:
	float rotation, scale;
};

