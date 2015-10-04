/*
GameStates

Enum holding all potential game states.

This enum will aid in the production of the main FSM (finite state machine) to be made in the main file.
Basically, only one state will be asserted at a time, and that will decide what objects get updated and whatnot.

authors: Patrick Sobolewski
*/
#pragma once

enum class GameState
{
	Initial,
	MainMenu,
	HighScoreDisplay,
	InGameActive,
	InGamePaused,
	GameOver,
};