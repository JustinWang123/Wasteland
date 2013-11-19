#include "MyEventReceiver.h"
#include "IrrWrapper.h"

////////////////////////////////////////////////////////////////////////////////
// EVENT RECEIVER METHODS: /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MyEventReceiver :: MyEventReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		key_is_down[i] = false;

	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		key_is_released[i] = false;

	lockMouse = false;
}

void MyEventReceiver :: Update()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		key_is_released[i] = false;

	mouse_wheel = 0;

}

bool MyEventReceiver :: OnEvent(const SEvent& event)
{
	// KEYBOARD EVENTS:
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.PressedDown == true)
		{
			key_is_down[event.KeyInput.Key] = true;
		}
		else
		{
			if(key_is_down[event.KeyInput.Key])
				key_is_released[event.KeyInput.Key] = true;

			key_is_down[event.KeyInput.Key] = false;
		}
	}


	// MOUSE EVENTS:
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if(event.MouseInput.isLeftPressed())
		{
			key_is_down[KEY_LBUTTON] = true;
		}
		else
		{
			if(key_is_down[KEY_LBUTTON])
				key_is_released[KEY_LBUTTON] = true;

			key_is_down[KEY_LBUTTON] = false;

		}

		if(event.MouseInput.isRightPressed())
		{
			key_is_down[KEY_RBUTTON] = true;
		}
		else
		{
			if(key_is_down[KEY_RBUTTON])
				key_is_released[KEY_RBUTTON] = true;

			key_is_down[KEY_RBUTTON] = false;
		}

		if(event.MouseInput.isMiddlePressed())
		{
			key_is_down[KEY_MBUTTON] = true;
		}
		else
		{
			if(key_is_down[KEY_MBUTTON])
				key_is_released[KEY_MBUTTON] = true;

			key_is_down[KEY_MBUTTON] = false;
		}

		mouse_wheel = event.MouseInput.Wheel;
		mouse_x = event.MouseInput.X;
		mouse_y = event.MouseInput.Y;
	}





	// GUI EVENTS:
	if (event.EventType == EET_GUI_EVENT)
	{
		gui_events[event.GUIEvent.EventType] = true;

		if(event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			button_clicked[event.GUIEvent.Caller->getID()] = true;
		}
	}

	return false;
}

bool MyEventReceiver :: is_key_down(EKEY_CODE keyCode) const
{
	return key_is_down[keyCode];
}

bool MyEventReceiver :: is_button_clicked(unsigned short gui_id)
{
	bool temp = button_clicked[gui_id];
	button_clicked[gui_id] = false;
	return temp;
}

bool MyEventReceiver :: get_gui_event(EGUI_EVENT_TYPE type)
{
	bool temp = gui_events[type];
	gui_events[type] = false;
	return temp;
}

unsigned short MyEventReceiver :: get_mouse_x()
{
	return mouse_x;
}

unsigned short MyEventReceiver :: get_mouse_y()
{
	return mouse_y;
}

