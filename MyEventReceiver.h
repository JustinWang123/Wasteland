#ifndef EVENT_REC
#define EVENT_REC

#include <string>
#include <sstream>

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const unsigned short  MAX_BUTTONS = 100;
const unsigned short  MAX_GUI_EVENTS = EGET_COUNT;

class MyEventReceiver : public IEventReceiver
{
	public:
		MyEventReceiver();
		void  Update();
		virtual bool OnEvent(const SEvent& event);
		virtual bool is_key_down(EKEY_CODE keyCode) const;
		virtual bool is_button_clicked(unsigned short  gui_id);
		virtual bool get_gui_event(EGUI_EVENT_TYPE type);
		virtual unsigned short  get_mouse_x();
		virtual unsigned short  get_mouse_y();

		bool lockMouse;
        bool key_is_down[KEY_KEY_CODES_COUNT]; // Holds the state of all keyboard and mouse keys
		bool key_is_released[KEY_KEY_CODES_COUNT];
		bool button_clicked[MAX_BUTTONS]; // Used to determine if a GUI button has been clicked
		bool gui_events[MAX_GUI_EVENTS]; // Used to determine if a GUI event has occured
		unsigned short mouse_x;
		unsigned short  mouse_y;
		short  mouse_wheel;
};
#endif
