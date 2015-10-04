#include "pch.h"
#include "Player.h"

using namespace Windows::Graphics::Display;

Player::Player(Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds) : 
	Sprite(spritesheet, m_Texture, position, movementBounds, 1000, 10, 1.5)
{
	isJumping = false;
	gravity = INIT_GRAVITY;

	//scale = DisplayProperties::LogicalDpi / 96.0f;
	initialPosition = position;
	initialSpeed = 1000;
	m_Texture = nullptr;
}

void Player::LoadTexture(ID3D11Device* d3dDevice)
{
	CreateDDSTextureFromFile(d3dDevice,
		L"Assets/suitman_all.dds", nullptr, &m_Texture, MAXSIZE_T);

	Sprite::m_Texture = m_Texture;
}

void Player::Update(Buildings* buildings, float timeTotal, float timeDelta)
{
	Sprite::Update(timeTotal, timeDelta);
	if (isJumping) {
		Speed += gravity;
		if (getAnimation() == 1 && animationPlayedOnce) {
			setAnimation(2);
			//setVelocity(XMFLOAT2(0, 0));
		}
		else if (getAnimation() == 2) {
			if (buildings->IntersectsWith(this))
			{
				if (position.y >= initialPosition.y) {
					position = initialPosition;
					setVelocity(XMFLOAT2(0, 0));
					setAnimation(3);
					isJumping = false;
				}
			}
			//else if (position.y >= initialPosition.y) {	// TODO: Indicate that player is dead
			//	isJumping = false;
			//}
		}
	}
	else if (getAnimation() == 3 && animationPlayedOnce) {
		setAnimation(0);
		isJumping = false;
		setVelocity(XMFLOAT2(0, 0));
	}
	else if (!buildings->IntersectsWith(this)) {	// Falling
		setVelocity(XMFLOAT2(0, 1));
		Speed = initialSpeed;
	}
}

void Player::Jump() {
	if (!isJumping) {
		Speed = initialSpeed;
		setAnimation(1);
		setVelocity(XMFLOAT2(0, -1));
		isJumping = true;
	}
}

XMFLOAT2 Player::Normalize(XMFLOAT2 vector)
{
	int magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));		//normalize the vector velocity
	vector.x = vector.x / magnitude;
	vector.y = vector.y / magnitude;

	return vector;
}

// If all goes well, you should only need to adjust FPS
void Player::increaseSpeed(float multiplier) {
	//Speed *= multiplier;
	//setSpeed(Speed);
	setFPS(getFPS()*multiplier);
	//gravity *= multiplier;
}

void Player::reset() {
	position = initialPosition;
	setFPS(INIT_FPS);
	setVelocity(XMFLOAT2(0, 0));
	Speed = initialSpeed;
	setAnimation(0);
	gravity = INIT_GRAVITY;
	isJumping = false;
}