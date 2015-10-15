#include "pch.h"
#include "SpriteText.h"

namespace SpriteText
{
	wchar_t const* numToWchar_t(int num)
	{
		switch (num)
		{
		case 0:
			return L"0";
			break;
		case 1:
			return L"1";
			break;
		case 2:
			return L"2";
			break;
		case 3:
			return L"3";
			break;
		case 4:
			return L"4";
			break;
		case 5:
			return L"5";
			break;
		case 6:
			return L"6";
			break;
		case 7:
			return L"7";
			break;
		case 8:
			return L"8";
			break;
		case 9:
			return L"9";
			break;
		default:
			return L"X";
			break;
		}

	}

	wchar_t const* letterToWchar_t(char letter)
	{
		return L"X";
	}

	void SpriteText::displayNum(SpriteBatch* spriteBatch, SpriteFont* spriteFont, int myScore, XMFLOAT2 position, float scale, int zeroFill)
	{
		int numDigits = getNumDigits(myScore);
		wchar_t const* num;
		float* stringlength;
		float tempScore;

		for (int i = 0; i < numDigits; i++)
		{
			tempScore = myScore % 10;
			num = SpriteText::numToWchar_t(tempScore);
			stringlength = spriteFont->MeasureString(num).n128_f32;	// scale the stringlength properly
			*stringlength *= scale;

			spriteFont->DrawString(spriteBatch, num, position,
				Colors::White, 0.0f, XMFLOAT2(*stringlength / 2.0f, 0.0f), scale, DirectX::SpriteEffects_None, 0.0f);
			position.x -= *stringlength;

			myScore /= 10;
		}

		// check if zero fill is needed
		for (int i = zeroFill - numDigits; i > 0; i--)
		{
			stringlength = spriteFont->MeasureString(L"0").n128_f32;
			*stringlength *= scale;

			spriteFont->DrawString(spriteBatch, L"0", position,
				Colors::White, 0.0f, XMFLOAT2(*stringlength / 2.0f, 0.0f), scale, DirectX::SpriteEffects_None, 0.0f);
			position.x -= *stringlength;
		}
	}

	int SpriteText::getNumDigits(int num)
	{
		if (num == 0) return 1;
		int counter = 0;
		if (num < 0) counter = 1; // remove this line if '-' counts as a digit
		while (num)
		{
			num /= 10;
			counter++;
		}
		return counter;
	}
}