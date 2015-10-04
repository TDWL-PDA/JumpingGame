#pragma once
//#include <map>
#include "Direct3DBase.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"

using namespace DirectX;

namespace SpriteText
{
	wchar_t const* numToWchar_t(int num);
	wchar_t const* letterToWchar_t(char letter);

	void displayNum(SpriteBatch* spriteBatch, SpriteFont* spriteFont, int myScore, XMFLOAT2 position, int zeroFill);
	int getNumDigits(int num);
}

//using namespace std;
//
//class SpriteText
//{
//public:
//	SpriteText();
//
//
//
//private:
//	// TODO: Implement this class in a dictionary style,
//	// so that it contains a single static dictionary that
//	// other classes can access.
//
//	//map<const int, wchar_t const*> numDict = {
//	//	{ 0, L"0" }, { 1, L"1" }, { 2, L"2" },
//	//	{ 3, L"3" }, { 4, L"4" }, { 5, L"5" },
//	//	{ 6, L"6" }, { 7, L"7" }, { 8, L"8" },
//	//	{ 9, L"9" }
//	//};
//	//map<const char, wchar_t const*> charDict;
//};

