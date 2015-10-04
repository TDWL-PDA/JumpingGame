#include "pch.h"
#include "Goal.h"

using namespace Windows::Graphics::Display;

Goal::Goal()
{
}

Goal::Goal(Rect* movementBounds)
{
	scale = DisplayProperties::LogicalDpi / 96.0f;
	diameter = movementBounds->Width * scale / 5.0f;

	//Puts the goal in the lower left corner of the screen
	posX = movementBounds->Width * scale - diameter / 2.0f;
	posY = movementBounds->Height * scale - diameter / 2.0f;
}

void Goal::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(m_Texture, XMFLOAT2(posX, posY), nullptr, Colors::White, 0.0f, XMFLOAT2(250.0f, 250.0f), 
		XMFLOAT2(diameter / 500.0f, diameter / 500.0f), DirectX::SpriteEffects_None, 0.0f);
}

void Goal::LoadTexture(ID3D11Device* d3dDevice)
{
	CreateDDSTextureFromFile(d3dDevice, L"Assets/goal.dds", nullptr, &m_Texture, MAXSIZE_T);
}