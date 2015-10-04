#include "pch.h"
#include "Buildings.h"

using namespace Windows::Graphics::Display;

Buildings::Buildings(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position,
	Windows::Foundation::Rect* screenBounds)
{
	//block = new Sprite(m_Texture, size, position, movementBounds);
	posX = position.x;	//probably want to get rid of these if I'm not going to use them...
	posY = position.y;
	this->movementBounds = new Windows::Foundation::Rect(*screenBounds);
	this->movementBounds->X -= size.x * 2 + 5;
	this->movementBounds->Width += size.x * 6 + 10;
	this->m_Texture = m_Texture;
	this->size = size;
	this->position = position;
	this->initialPosition = position;
	this->framesPerSecond = 1;
	Speed = INIT_SPEED;

	scale = DisplayProperties::LogicalDpi / 96.0f;

	//m_Texture = nullptr;
	generateGap = false;

	for (int i = 0; i < 7; i++) {
		Sprite* block = new Sprite(this->m_Texture, this->size, this->position, movementBounds, framesPerSecond, Speed);
		blocks.push_back(block);
		this->position.x += size.x;
	}
}

void Buildings::Draw(SpriteBatch* spriteBatch)
{
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Draw(spriteBatch);
	}
}

void Buildings::Update(float timeTotal, float timeDelta)
{
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->setVelocity(XMFLOAT2(-1, 0));
		Windows::Foundation::Rect* boundingBox = blocks[i]->getBoundingBox();
		if (boundingBox->X <= size.x*-1 ||
			boundingBox->X + boundingBox->Width >= movementBounds->X + movementBounds->Width) {
			delete blocks[i];
			blocks.erase(blocks.begin() + i);
		}
	}
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Update(timeTotal, timeDelta);
	}
	updateBuildings();	// must go after the sprite updates for smooth connection.
}

void Buildings::updateBuildings()
{
	//randomly decide when to create a gap in the buildings
	int number = rand() % 50;	//random number between 0 and 4

	if (number == 0 && !generateGap)
	{
		generateGap = true;
	}
	if (blocks.size() > 0) {
		Windows::Foundation::Rect* boundingBox = blocks[blocks.size() - 1]->getBoundingBox();
		XMFLOAT2 newPosition(boundingBox->X + boundingBox->Width, boundingBox->Y);
		if (generateGap) {
			newPosition.x += size.x * 4.75;	// TODO: Find a good number, and a good size for the building blocks
		}
		if (newPosition.x < movementBounds->X + movementBounds->Width - size.x) {
			Sprite* newBlock = new Sprite(m_Texture, size, newPosition, movementBounds, framesPerSecond, Speed);
			blocks.push_back(newBlock);
			generateGap = false;
		}
	}
}

bool Buildings::IntersectsWith(Sprite* sprite) {
	for (int i = 0; i < blocks.size(); i++) {
		Windows::Foundation::Rect* box = blocks[i]->getBoundingBox();
		Windows::Foundation::Rect box2 = *sprite->getBoundingBox();
		if (box->IntersectsWith(box2)) {
			return true;
		}
		//delete box;
	}
	return false;
}

//
void Buildings::increaseSpeed(float multiplier) {
	framesPerSecond *= multiplier;
	//Speed *= multiplier;
	for (Sprite* b : blocks) {
		b->setFPS(framesPerSecond);
		//b->setSpeed(Speed);
	}
}

void Buildings::reset() {
	Speed = INIT_SPEED;
	position = initialPosition;
	for (int i = 0; i < blocks.size(); i++) {
		delete blocks[i];
		blocks.erase(blocks.begin() + i);
	}
	for (int i = 0; i < 7; i++) {
		Sprite* block = new Sprite(this->m_Texture, this->size, this->position, movementBounds, framesPerSecond, Speed);
		blocks.push_back(block);
		this->position.x += size.x;
	}
}
