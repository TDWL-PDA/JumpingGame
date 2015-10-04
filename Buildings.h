#pragma once

#include "SpriteBatch.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>
#include <list>
#include "Sprite.h"

using namespace DirectX;
using namespace std;

static float INIT_SPEED = 750;
//static float INIT_FPS = 1;

class Buildings
{
public:
	Buildings(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position,
		Windows::Foundation::Rect* movementBounds);

	void Update(float timeTotal, float timeDelta);
	void Draw(SpriteBatch* spriteBatch);
	bool IntersectsWith(Sprite* sprite);
	void increaseSpeed(float multiplier);
	void reset();

	float posX;
	float posY;
	float scale;

private:
	//Sprite* block;
	vector<Sprite*> blocks;
	float framesPerSecond;
	bool generateGap;
	Windows::Foundation::Rect* movementBounds;
	ID3D11ShaderResourceView *m_Texture;
	XMFLOAT2 size, position, initialPosition;
	float Speed;

	void updateBuildings();
};

