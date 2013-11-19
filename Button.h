#ifndef BUTTON_H
#define BUTTON_H

#include "IrrWrapper.h"

enum ButtonState
{
	BUTTON_STATE_UP,
	BUTTON_STATE_OVER,
	BUTTON_STATE_DOWN
};

class Button
{
public:
	Button();
	~Button(){}

	void Init(int screenXpos, int screenYpos, ITexture* setUpTex, ITexture* setOverTex, ITexture* setDownTex);
	void Update();
	void Draw();

	bool IsUp();
	bool IsOver();
	bool IsDown();
	bool IsClicked();

private:
	position2d<s32> screenPos;

	bool isClicked;
	ButtonState state;

	ITexture* upTex;
	ITexture* downTex;
	ITexture* overTex;

};

#endif
