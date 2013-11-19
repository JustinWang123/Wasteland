#include "Button.h"

Button :: Button()
{
	isClicked = false;
	state = BUTTON_STATE_UP;
}

void Button :: Init(int screenXpos, int screenYpos, ITexture* setUpTex, ITexture* setOverTex, ITexture* setDownTex)
{
	screenPos.X = screenXpos;
	screenPos.Y = screenYpos;

	upTex = setUpTex;
	downTex = setDownTex;
	overTex = setOverTex;

}

void Button :: Update()
{
	if(IRR->receiver->mouse_x > screenPos.X
	&& IRR->receiver->mouse_y > screenPos.Y
	&& IRR->receiver->mouse_x < screenPos.X + upTex->getOriginalSize().Width
	&& IRR->receiver->mouse_y < screenPos.Y + upTex->getOriginalSize().Height)
	{
		if(IRR->receiver->key_is_down[KEY_LBUTTON])
		{
			if(state == BUTTON_STATE_OVER)
			{
				isClicked = true;
			}

			state = BUTTON_STATE_DOWN;
		}
		else
		{
			state = BUTTON_STATE_OVER;
		}
	}
	else
	{
		state = BUTTON_STATE_UP;
	}
}

void Button :: Draw()
{
	if(state == BUTTON_STATE_UP)
	{
		IRR->driver->draw2DImage(upTex, screenPos, rect<s32>(0,0,upTex->getOriginalSize().Width,upTex->getOriginalSize().Height), 0, SColor(255,255,255,255), true);	
	}
	else if(state == BUTTON_STATE_DOWN)
	{
		IRR->driver->draw2DImage(downTex, screenPos, rect<s32>(0,0,downTex->getOriginalSize().Width,upTex->getOriginalSize().Height), 0, SColor(255,255,255,255), true);	
	}
	else if(state == BUTTON_STATE_OVER)
	{
		IRR->driver->draw2DImage(overTex, screenPos, rect<s32>(0,0,overTex->getOriginalSize().Width,upTex->getOriginalSize().Height), 0, SColor(255,255,255,255), true);	
	}
}

bool Button :: IsUp()
{
	return state == BUTTON_STATE_UP;
}

bool Button :: IsOver()
{
	return state == BUTTON_STATE_OVER;
}

bool Button :: IsDown()
{
	return state == BUTTON_STATE_DOWN;
}

bool Button :: IsClicked()
{
	if(isClicked)
	{
		isClicked = false;
		return true;
	}
	else
	{
		return false;
	}
}
