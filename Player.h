#pragma once

#include "SpriteBatch.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>
#include <list>
#include "Sprite.h"
#include "Buildings.h"

using namespace DirectX;
using namespace std;

static float INIT_GRAVITY = -30;
static float INIT_FPS = 10;

class Player
	: public Sprite
{
public:
	Player();
	Player(Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds);
	void Update(Buildings* buildings, float timeTotal, float timeDelta);
	void LoadTexture(ID3D11Device* d3dDevice);
	void Jump();
	void increaseSpeed(float multiplier);
	void reset();

	bool isJumping;

	float scale;

private:
	XMFLOAT2 Normalize(XMFLOAT2 vector);

	float gravity;
	XMFLOAT2 initialPosition;
	float initialSpeed;

	ID3D11ShaderResourceView* m_Texture;
};