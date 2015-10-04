#pragma once

#include <wrl\client.h>
#include <memory>

#include "Direct3DBase.h"
#include <directxmath.h>
#include <list>

#include "Audio.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "Sprite.h"
#include "Player.h"
#include "Buildings.h"
#include "Blockades.h"
#include "SpriteText.h"
#include "GameState.h"

using namespace DirectX;
using namespace Windows::Devices::Sensors;

// Width and height of the thing you want to draw to the screen.
static int BUILDING_HEIGHT = 250;
static int BUILDING_WIDTH = 150;
static int PLAYER_HEIGHT = 175.8f * 1.5;	// ehh...scale added here
static int MENU_SCREEN_HEIGHT = 1601 / 2;
static int MENU_SCREEN_WIDTH = 961 / 2;
static int INCREMENT = 10;	// For measuring the number of game loops before incrementing score by 1
static int SWIPE = 4;	// Number of frames of person swiping up that will register a "swipe"
static int SCORE_SPEED_INCREASE = 50;	// Incremental score where speed will increase
static int SCORE_SPEED_LIMIT = SCORE_SPEED_INCREASE * 50;	// Score where speed stops increasing
static float SPEED_MULTIPLIER = 1.25;

ref class Renderer sealed : public Direct3DBase
{
public:
	Renderer();
	Renderer(int highScore);

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	//void randomizeEnemies();
	void resetGame();
	
	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

	// Logic handled here because you can't have a "getGameState" function here, as it is a native type (Windows no likey)
	void HandlePressInput(Windows::UI::Input::PointerPoint^ currentPoint);	// called when pointer is down, alters state
	void HandleReleaseInput(Windows::UI::Input::PointerPoint^ currentPoint);	// called when pointer is released, alters state
	void HandleMoveInput(Windows::UI::Input::PointerPoint^ currentPoint);
	int getHighScore();
	void setHighScore(int newScore);

internal:
	Player* getPlayer();

private:
	int highScore;
	int score;
	int incrementCount;
	void displayScores();
	void HandleAudio(float timeTotal, float timeDelta);
	void speedUpGame();

	// Music stuff
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	//std::unique_ptr<DirectX::WaveBank> m_waveBank;
	std::unique_ptr<DirectX::SoundEffect> m_musicFile;
	std::unique_ptr<DirectX::SoundEffectInstance> m_music;

	uint32_t m_audioEvent;
	float m_audioTimerAcc;
	bool m_retryDefault;

	bool m_loadingComplete;
	GameState gameState;

	uint32 m_indexCount;

	unique_ptr<SpriteBatch> m_spriteBatch;

	Accelerometer^ m_accelerometer;
	AccelerometerReading^ m_accReading;

	float scale;

	Spritesheet* titleSheet;
	Spritesheet* pauseSheet;
	Sprite* titleScreen;
	Sprite* pauseScreen;

	Buildings* buildings;
	Sprite* blockades;
	Sprite* pauseButton;
	Spritesheet* playerSheet;
	Player* m_player;

	unique_ptr<SpriteFont> m_spriteFont;

	bool onButton(Sprite* button, XMFLOAT2 point);
	int swipeCounter;	// Assists in detecting a swipe
	XMFLOAT2 previousPoint;	// Also assists in detecting a swipe
};