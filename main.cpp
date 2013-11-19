#include "IrrWrapper.h"
#include "GameManager.h"
#include "MapChunkInstance.h"
#include "Perameters.h"
#include "ItemManager.h"
#include "InstanceManager.h"
#include <iostream>
#include "MapManager.h"

int main()
{

	IRR->Init();
	GManager->Init(false);


	//We want to count the fps
	int fps = 0;
	int frameStartTime = IRR->device->getTimer()->getTime();
	int frameTimeDelta = 0;
    int fpsUpdate = 0;

    std::cout << "Starting Game Loop\n";

	while(IRR->device->run() && !IRR->receiver->key_is_down[KEY_ESCAPE])
		if(IRR->device->isWindowActive() )
        {
			GManager->Update();

			IRR->DrawText("FPS: " + IRR->ToString(fps), SCREEN_SIZE_X - 48, 10);
			IRR->EndScene();

			// Timing:
			frameTimeDelta = IRR->device->getTimer()->getTime() - frameStartTime;
			frameStartTime = IRR->device->getTimer()->getTime();
			IRR->SetTimeDelta((float)frameTimeDelta / 1000.0f);

			fpsUpdate++;
			if(fpsUpdate > 50)
			{
                fps = 1000/frameTimeDelta;
                fpsUpdate = 0;
			}
    	}
		else
			IRR->device->yield();

	//GManager->SaveWorld("World1");
	GManager->Destroy();
	IRR->Destroy();
	return 0;
}
