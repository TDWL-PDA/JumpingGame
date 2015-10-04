#include "pch.h"
#include "Renderer.h"
#include <time.h>
#include <iostream>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Graphics::Display;

Renderer::Renderer() :
	m_loadingComplete(false),
	m_indexCount(0)
{
	gameState = GameState::Initial;

	scale = DisplayProperties::LogicalDpi / 96.0f;
	swipeCounter = 0;
	score = 0;
	incrementCount = 0;
	highScore = 0; // To be replaced with memory call
	previousPoint = XMFLOAT2(0, 0);

	srand((unsigned)time(0));
}

Renderer::Renderer(int highScore) :
	m_loadingComplete(false),
	m_indexCount(0)
{
	gameState = GameState::Initial;

	scale = DisplayProperties::LogicalDpi / 96.0f;
	swipeCounter = 0;
	score = 0;
	incrementCount = 0;
	this->highScore = highScore;
	previousPoint = XMFLOAT2(0, 0);

	srand((unsigned)time(0));
}


void Renderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();

	m_spriteBatch = unique_ptr<SpriteBatch>(new 
		DirectX::SpriteBatch(m_d3dContext.Get()));

	m_spriteFont = unique_ptr<SpriteFont>(new SpriteFont(m_d3dDevice.Get(), L"Assets/deluxe.spritefont"));

	//seed the clock
	srand((int)time(NULL));
}

void Renderer::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();

	scale = DisplayProperties::LogicalDpi / 96.0f;
	m_movementBounds.Height *= scale;
	m_movementBounds.Width *= scale;

	//XMFLOAT2 defaultSizeOfScreen(960, 1600);

	//Create the title screen
	ID3D11ShaderResourceView* m_titleScreenTexture = nullptr;

	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/jumpingTitleSprite.dds", nullptr, &m_titleScreenTexture, MAXSIZE_T);
	float scaleX = m_movementBounds.Width / MENU_SCREEN_WIDTH;
	float scaleY = m_movementBounds.Height / MENU_SCREEN_HEIGHT;
	scale = scaleX > scaleY ? scaleX : scaleY;
	titleSheet = new Spritesheet(XMFLOAT2(MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT), 4, MENU_SCREEN_WIDTH*2);
	titleScreen = new Sprite(titleSheet, m_titleScreenTexture, XMFLOAT2(0, 0), &m_movementBounds, 0, 6, scale);

	//Create the pause screen
	ID3D11ShaderResourceView* m_pauseScreenTexture = nullptr;
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/jumpingPauseScreen.dds", nullptr, &m_pauseScreenTexture, MAXSIZE_T);
	pauseSheet = new Spritesheet(XMFLOAT2(MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT), 4, MENU_SCREEN_WIDTH*2);
	pauseScreen = new Sprite(pauseSheet, m_pauseScreenTexture, XMFLOAT2(0, 0), &m_movementBounds, 0, 6, scale);

	//Create the player
	XMFLOAT2 positionOfPlayer(50, m_movementBounds.Height - BUILDING_HEIGHT - PLAYER_HEIGHT + 5);
	vector<XMFLOAT2> sizeOfPlayer(4);
	vector<int> playerFrames(4);
	for (int i = 0; i < 4; i++)
	{
		sizeOfPlayer[i] = XMFLOAT2(218.0f / 2, 1758.0f / 10);
		playerFrames[i] = 4;
	}
	playerFrames[0] = 6;
	playerFrames[3] = 6;
	playerSheet = new Spritesheet(sizeOfPlayer, 4, playerFrames, 218);
	m_player = new Player(playerSheet, positionOfPlayer, &m_movementBounds);
	m_player->LoadTexture(m_d3dDevice.Get());

	//Create the buildings
	ID3D11ShaderResourceView* m_buildingsTexture = nullptr;
	XMFLOAT2 positionOfBuildings(0, m_movementBounds.Height - BUILDING_HEIGHT);
	XMFLOAT2 sizeOfBuildings(BUILDING_WIDTH, BUILDING_HEIGHT);

	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/block.dds", nullptr, &m_buildingsTexture, MAXSIZE_T);

	buildings = new Buildings(m_buildingsTexture, sizeOfBuildings, positionOfBuildings, &m_movementBounds);

	//Create the blockades
	ID3D11ShaderResourceView* m_blockadesTexture = nullptr;
	XMFLOAT2 positionOfBlockades(m_movementBounds.Width - 150, 0);
	XMFLOAT2 sizeOfBlockades(150, 300);

	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/blockade.dds", nullptr, &m_blockadesTexture, MAXSIZE_T);

	blockades = new Sprite(m_blockadesTexture, sizeOfBlockades, positionOfBlockades, &m_movementBounds);

	//Create the pause button
	ID3D11ShaderResourceView* m_pauseTexture = nullptr;
	XMFLOAT2 positionOfPause(10, 10);
	XMFLOAT2 sizeOfButton(32, 38);
	scale = 2.0;

	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/pauseButton.dds", nullptr, &m_pauseTexture, MAXSIZE_T);

	pauseButton = new Sprite(m_pauseTexture, sizeOfButton, positionOfPause, &m_movementBounds, scale);

	// Audio
	// Create DirectXTK for Audio objects 
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG 
	eflags = eflags | AudioEngine_Debug;
#endif 
	m_audEngine.reset(new AudioEngine(eflags));

	m_audioEvent = 0;
	m_audioTimerAcc = 10.f;
	m_retryDefault = false;

	//m_waveBank.reset(new WaveBank(m_audEngine.get(), L"assets\\adpcmdroid.xwb"));
	m_musicFile.reset(new SoundEffect(m_audEngine.get(), L"Assets/SpaceDolphins.wav"));
	m_music = m_musicFile->CreateInstance();
}

void Renderer::HandleAudio(float timeTotal, float timeDelta)
{
	m_audioTimerAcc -= timeDelta;
	if (m_audioTimerAcc < 0)
	{
		if (m_retryDefault)
		{
			m_retryDefault = false;
			if (m_audEngine->Reset())
			{
				// Restart looping audio 
				m_music->Play(true);
			}
		}
		else
		{
			// Unused atm
			m_audioTimerAcc = 4.f;

			//m_waveBank->Play(m_audioEvent++);

			if (m_audioEvent >= 11)
				m_audioEvent = 0;
		}
	}

	if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
	{
		// Setup a retry in 1 second (can happen when transitioning to and from a BlueTooth audio connection) 
		m_audioTimerAcc = 1.f;
		m_retryDefault = true;
	}
}

void Renderer::Update(float timeTotal, float timeDelta)
{
	HandleAudio(timeTotal, timeDelta);
	switch (gameState) {
	case GameState::InGameActive:
		m_player->Update(buildings, timeTotal, timeDelta);
		buildings->Update(timeTotal, timeDelta);

		// Collision manager
		// TODO: Add reset option;
		// TODO: Make a not shitty transition
		//if (!m_player->isJumping && !buildings->IntersectsWith(m_player)) {
		//	gameState = GameState::Initial;
		//	m_player->Jump();
		//}

		if (m_player->getBoundingBox()->Y < 0)	//for this game, only need to make sure player doesn't go off of top of screen (b/c movement is limited)
			m_player->getBoundingBox()->Y = 0;
		// Else if you're dead
		else if (abs(m_player->getBoundingBox()->Y + m_player->getBoundingBox()->Height - m_movementBounds.Height) <= 25) {	// TODO: Find way to get accuracy better
			resetGame();
		}

		// Update score
		incrementCount++;
		if (incrementCount >= INCREMENT) {
			score++;
			if (score % SCORE_SPEED_INCREASE == 0 && score < SCORE_SPEED_LIMIT) {
				speedUpGame();
			}
			incrementCount = 0;
		}
		break;
	case GameState::InGamePaused:
		pauseScreen->Update(timeTotal, timeDelta);
		break;
	case GameState::Initial:
		titleScreen->Update(timeTotal, timeDelta);
	default:
		break;
	}
}

void Renderer::Render()
{
	const float bg_color[] = { 245.0f / 255.0f, 241.0 / 255.0f, 196.0f / 255.0f, 1.000f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		bg_color
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	m_spriteBatch->Begin();

	float* stringlength = m_spriteFont->MeasureString(L"Tap to start!").n128_f32;

	switch (gameState) {
	case GameState::InGameActive:
		m_player->Draw(m_spriteBatch.get());
		buildings->Draw(m_spriteBatch.get());
		//blockades->Draw(m_spriteBatch.get());
		pauseButton->Draw(m_spriteBatch.get());
		displayScores();
		break;
	case GameState::InGamePaused:
		pauseScreen->Draw(m_spriteBatch.get());
		break;
	case GameState::Initial:
		titleScreen->Draw(m_spriteBatch.get());
		m_spriteFont->DrawString(m_spriteBatch.get(), L"Tap to start!",
			XMFLOAT2(m_movementBounds.Width / 2.0f, m_movementBounds.Height / (5.0f / 2.0f)),
			Colors::Gold, 0.0f, XMFLOAT2(*stringlength / 2.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
		break;
	default:
		break;
	}

	m_spriteBatch->End();
}

void Renderer::HandlePressInput(Windows::UI::Input::PointerPoint^ currentPoint)
{
	XMFLOAT2 vectorPoint = XMFLOAT2(currentPoint->RawPosition.X, currentPoint->RawPosition.Y);

	switch (gameState) {
	case GameState::InGameActive:
		//check for pause
		if (onButton(pauseButton, vectorPoint)) {
			gameState = GameState::InGamePaused;
		}
		break;
	case GameState::InGamePaused:
		gameState = GameState::InGameActive;
		break;
	default:
		break;
	}
}

void Renderer::HandleReleaseInput(Windows::UI::Input::PointerPoint^ currentPoint)
{
	XMFLOAT2 vectorPoint = XMFLOAT2(currentPoint->RawPosition.X, currentPoint->RawPosition.Y);

	switch (gameState) {
	case GameState::InGameActive:
		if (swipeCounter >= SWIPE) {
			swipeCounter = 0;
			getPlayer()->Jump();
		}
		break;
	case GameState::InGamePaused:
		break;
	case GameState::Initial:
		gameState = GameState::InGameActive;
		m_music->Play(true);
	}
}

void Renderer::HandleMoveInput(Windows::UI::Input::PointerPoint^ currentPoint)
{
	XMFLOAT2 vectorPoint = XMFLOAT2(currentPoint->RawPosition.X, currentPoint->RawPosition.Y);

	switch (gameState) {
	case GameState::InGameActive:
		if (vectorPoint.y < previousPoint.y) {	// You're moving up
			swipeCounter++;
		}
		else swipeCounter = 0;
		break;
	default:
		break;
	}

	previousPoint = vectorPoint;
}

void Renderer::displayScores()
{
	if (score > highScore)
		highScore = score;

	float* digitLength = m_spriteFont->MeasureString(L"0").n128_f32;
	XMFLOAT2 position = XMFLOAT2(m_movementBounds.Width - m_movementBounds.Width / 20.0f, m_movementBounds.Height / 20.0f + pauseButton->getHeight() + 20);	// replace 10 with variable
	float *stringlength = m_spriteFont->MeasureString(L"Score").n128_f32;
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Score",
		position, Colors::Black, 0.0f, XMFLOAT2(*stringlength, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);

	position.y += 32;
	SpriteText::displayNum(m_spriteBatch.get(), m_spriteFont.get(), score, position, 0);
	position.x = m_movementBounds.Width / 20;
	position.y -= 32;
	m_spriteFont->DrawString(m_spriteBatch.get(), L"High Score",
		position, Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
	for (int i = 0; i < SpriteText::getNumDigits(highScore) - 1; i++)
		position.x += *digitLength;
	position.y += 32;
	SpriteText::displayNum(m_spriteBatch.get(), m_spriteFont.get(), highScore, position, 0);
}

void Renderer::speedUpGame() {
	float multiplier = SPEED_MULTIPLIER;
	m_player->increaseSpeed(multiplier);
	buildings->increaseSpeed(multiplier);
}

bool Renderer::onButton(Sprite* button, XMFLOAT2 pointer)
{
	Point point = Point(pointer.x, pointer.y);
	if (button->getBoundingBox()->Contains(point))
		return true;
	return false;
}

void Renderer::resetGame()
{
	buildings->reset();
	m_player->reset();
	//m_player->getBoundingBox()->X = 50.0f;
	//m_player->getBoundingBox()->Y = 50.0f;
	score = 0;
	gameState = GameState::Initial;
}

Player* Renderer::getPlayer()
{
	return m_player;
}

int Renderer::getHighScore() { return highScore; }
void Renderer::setHighScore(int newScore) { highScore = newScore; }