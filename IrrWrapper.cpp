#include "IrrWrapper.h"
#include "MyEventReceiver.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include "Perameters.h"

using namespace std;


IrrWrapper* IrrWrapper :: Instance()
{
	static IrrWrapper instance;
	return &instance;
}

void IrrWrapper :: Init()
{
	receiver = new MyEventReceiver();

	// Setup irrlicht devices:
	SIrrlichtCreationParameters parameters;
	parameters.AntiAlias = 0; // 1
	parameters.DriverType = EDT_OPENGL;
	parameters.WindowSize = dimension2d<u32>(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	parameters.Bits = 16; //32
	parameters.Fullscreen = false;
	parameters.Stencilbuffer = false;
	parameters.Vsync = false;
	parameters.EventReceiver = receiver;

	device = createDeviceEx(parameters);

    driver = 			device->getVideoDriver();
    sceneMgr = 			device->getSceneManager();
    gui = 				device->getGUIEnvironment();
	collisionMgr =		sceneMgr->getSceneCollisionManager();

	IGUISkin* skin = gui->getSkin();
	font = gui->getFont("tex/font.bmp");
	skin->setFont(font);

	// Setup camera:
	camera = IrrWrapper::sceneMgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0), 0);
	camera->setNearValue(0.01f);
}

void IrrWrapper :: Destroy()
{
	delete receiver;
	device->drop();
}



void IrrWrapper :: EndScene()
{

	receiver->Update();
	gui->drawAll();
	driver->endScene();
	driver->beginScene(true, true, fogColor);
	sceneMgr->drawAll();
}

void IrrWrapper :: DrawText(std::string text, int x, int y)
{
	IrrWrapper::font->draw(text.c_str(), rect<s32>(x,y,640,480),SColor(255,255,255,255));

}

void IrrWrapper :: DrawText(std::string text, int x, int y, SColor c)
{
	IrrWrapper::font->draw(text.c_str(), rect<s32>(x,y,640,480), c);

}

void IrrWrapper :: DrawTexture(ITexture* tex, int x, int y)
{
    IRR->driver->draw2DImage(tex, vector2di(x,y), rect<s32>(0,0,tex->getOriginalSize().Width,tex->getOriginalSize().Height), 0, SColor(255,255,255,255), true);
}





////////////////////////////////////////////////////////////////////////////////
// UTILITY METHODS: ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string IrrWrapper :: ToString(double d)
{
	stringstream out;
	out << d;
	return out.str();
}

std::string IrrWrapper :: ToString(int i)
{
	stringstream out;
	out << i;
	return out.str();
}

int IrrWrapper :: GetAngle(double xpos1, double ypos1, double xpos2, double ypos2)
{
	int angle = 0;

	if(ypos2 < ypos1)
		angle = (int)(90 - atan((double)(xpos2 - xpos1) / (double)(ypos1 - ypos2) ) * 180/PI);
	else if(ypos2 > ypos1)
		angle = (int)(270 - atan((double)(xpos2 - xpos1) / (double)(ypos1 - ypos2) ) * 180/PI);
	else
	{
		if(xpos2 > xpos1)
			angle = 0;
		else
			angle = 180;
	}

	return (int)WrapValue(angle + 90);
}

float IrrWrapper :: WrapValue(float angle)
{
	if(angle > 360)
		angle = angle - 360;
	else if(angle < 0)
		angle = 360 + angle;

	return angle;
}

vector2di IrrWrapper :: GetScreenCoordinates(vector3df worldPosition)
{
	return collisionMgr->getScreenCoordinatesFrom3DPosition(worldPosition);
}






vector3df IrrWrapper :: RotateVectorAboutVector(vector3df v, vector3df l, float a)
{
	float m_L[3][3] ={{   0,   l.Z,  -l.Y},
					 {-l.Z,	    0,   l.X},
					 { l.Y,  -l.X,	   0}};




	float m_LxL[3][3] = {{0,0,0},
					   {0,0,0},
					   {0,0,0}};

	for(int r = 0; r < 3; r++)
		for(int c = 0; c < 3; c++)
			for(int i = 0; i < 3; i++)
				m_LxL[r][c] += (m_L[r][i] * m_L[i][c]);


	for(int r = 0; r < 3; r++)
		for(int c = 0; c < 3; c++)
			m_LxL[r][c] *= ( (1 - cos(a))/l.getDistanceFromSQ(vector3df(0,0,0)));


	float m_L2[3][3]= {{0,0,0},
					 {0,0,0},
					 {0,0,0}};

	for(int r = 0; r < 3; r++)
		for(int c = 0; c < 3; c++)
			m_L2[r][c] = m_L[r][c] * sin(a)/l.getDistanceFrom(vector3df(0,0,0));

	float m_I[3][3] = {{1,0,0},
					 {0,1,0},
					 {0,0,1}};

	float m_final[3][3]= {{0,0,0},
					    {0,0,0},
					    {0,0,0}};

	for(int r = 0; r < 3; r++)
		for(int c = 0; c < 3; c++)
			m_final[r][c] = m_I[r][c] + m_L2[r][c] + m_LxL[r][c];

	vector3df v_output(0,0,0);

	v_output.X =	v.X * m_final[0][0] +
					v.Y * m_final[1][0] +
					v.Z * m_final[2][0];

	v_output.Y =	v.X * m_final[0][1] +
					v.Y * m_final[1][1] +
					v.Z * m_final[2][1];

	v_output.Z =	v.X * m_final[0][2] +
					v.Y * m_final[1][2] +
					v.Z * m_final[2][2];

	return v_output;


}

vector3df IrrWrapper :: GetEulerFromBasis(vector3df v_heading, vector3df v_up, vector3df v_tangent)
{
	matrix4 m;
	m[0] = v_heading.X;
	m[1] = v_heading.Y;
	m[2] = v_heading.Z;
	m[3] = 0;

	m[4] = v_tangent.X;
	m[5] = v_tangent.Y;
	m[6] = v_tangent.Z;
	m[7] = 0;

	m[8] = v_up.X;
	m[9] = v_up.Y;
	m[10] = v_up.Z;
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;

	return m.getRotationDegrees();
}

int	IrrWrapper :: ShortestAngle(float from, float to)
{
	int posDistance;

	if(to > from)
	{
		posDistance = to - from;
	}
	else
	{
		posDistance = (360 - from) + to;
	}

	int negDistance;

	if(to > from)
	{
		negDistance = from + (360 - to);
	}
	else
	{
		negDistance = from - to;
	}

	if(abs(posDistance) < abs(negDistance))
	{
		return posDistance;
	}
	else
	{
		return -negDistance;
	}
}

vector3df IrrWrapper :: GetWorldCoords(vector2df screenPos)
{
	vector3df worldPosition;
	line3df ray = collisionMgr->getRayFromScreenCoordinates(position2di(screenPos.X, screenPos.Y) );
	plane3df plane(vector3df(0, 0, -1), 0);
	plane.getIntersectionWithLine(ray.start, ray.end, worldPosition);
	worldPosition.Z = 0;

	return worldPosition;
}

float IrrWrapper :: GetTimeDelta()
{
	return timeDelta;
}

void IrrWrapper :: SetTimeDelta(float setTimeDelta)
{
	if(setTimeDelta < 0.1f)
		timeDelta = setTimeDelta;
	else
		timeDelta = 0.1f;
}
