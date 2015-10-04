#include "pch.h"
#include "Enemy.h"

// Initialize static texture member
ID3D11ShaderResourceView* Enemy::m_Texture = nullptr;

using namespace Windows::Graphics::Display;

//Enemy::Enemy() : Sprite(m_Texture, size, position, movementBounds)
//{
//}

Enemy::Enemy(float x, float y, Windows::Foundation::Rect* movementBounds)
{
	posX = x;
	posY = y;

	scale = DisplayProperties::LogicalDpi / 96.0f;

	// Enemy diameter = 1/5th of screen width
	diameter = movementBounds->Width * scale / 5.0f;

	// Randomize starting rotation angle
	rotation = (float)rand() / ((float)RAND_MAX / 360.0f);
}

void Enemy::Draw(SpriteBatch* spritebatch)
{
	spritebatch->Draw(Enemy::m_Texture, XMFLOAT2(posX, posY), nullptr, Colors::White, XMConvertToRadians(rotation), XMFLOAT2(250.0f, 250.0f), XMFLOAT2(diameter / 500.0f, diameter / 500.0f), DirectX::SpriteEffects_None, 0.0f);

	rotation += 1.0f;
	if (rotation > 360.0f)
		rotation = 0.0f;
}

void Enemy::LoadTexture(ID3D11Device* d3dDevice)
{
	CreateDDSTextureFromFile(d3dDevice, L"Assets/enemy.dds", nullptr, &Enemy::m_Texture, MAXSIZE_T);
}
