#pragma once

#include "SpriteBatch.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>

using namespace DirectX;
using namespace std;
using namespace Windows::Foundation;

class Goal
{
public:
	Goal();
	Goal(Rect* movementBounds);
	void Draw(SpriteBatch* spriteBatch);
	void LoadTexture(ID3D11Device* d3dDevice);
	float posX;
	float posY;
	float diameter;

private:
	ID3D11ShaderResourceView* m_Texture;

	float scale;

};
