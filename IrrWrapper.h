/*
IRRLICHT WRAPPER:

The purpose of the irrlicht wrapper is to hide all the low level details involved with rendering the game world, handling collision,
handling input and drawing the HUD.  The irrlicht wrapper will also contain methods for converting common data types as well as
certain math functions.

DUTIES:
Initiate all irrlicht systems
Handle Input
Render Scene
Clean Up
*/

#ifndef IRR_WRAPPER
#define IRR_WRAPPER

#include <string>
#include <sstream>

#include <irrlicht.h>
#include "MyEventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//const int SCREEN_SIZE_X = 1280;
//const int SCREEN_SIZE_Y = 720;

const int SCREEN_SIZE_X = 640;
const int SCREEN_SIZE_Y = 360;

class IrrWrapper
{
	public:
		static IrrWrapper* Instance();

		// General Methods:
		void 					Init();
		void 					Destroy();
		void					EndScene();

		// Text Methods:
		void					DrawText(std::string text, int x, int y);
		void					DrawText(std::string text, int x, int y, SColor c);

        // Texture Methods:
        void                    DrawTexture(ITexture* tex, int x, int y);


		// Utility Methods:
		std::string 			ToString(double d);
		std::string 			ToString(int i);
		int 					GetAngle(double xpos1, double ypos1, double xpos2, double ypos2);
		float					WrapValue(float angle);
		vector2di				GetScreenCoordinates(vector3df worldPosition);
		vector3df				RotateVectorAboutVector(vector3df v_toBeRotated, vector3df v_rotateAbout, float a);
		vector3df				GetEulerFromBasis(vector3df v_heading, vector3df v_up, vector3df v_tangent);
		int						ShortestAngle(float from, float to);
		vector3df				GetWorldCoords(vector2df screenPos);

		// All important subsystems are public so that they can be easily accessed:
		IrrlichtDevice* 		device;
		IVideoDriver* 			driver;
		ISceneManager* 			sceneMgr;
		IGUIEnvironment* 		gui;
		MyEventReceiver* 		receiver;
		ISceneCollisionManager* collisionMgr;
		ICameraSceneNode* 		camera;

		int						GetTime() {return device->getTimer()->getRealTime();}
		float					GetTimeDelta();
		void					SetTimeDelta(float setTimeDelta);

private:
		IGUIFont* 				font;
		float					timeDelta;
};

#define	IRR IrrWrapper::Instance()

#endif
