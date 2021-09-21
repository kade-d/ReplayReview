#include "SetCameraController.h"

void SetCameraController::OnSetCameraSide(std::vector<std::string> params)
{
	CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
	ReplayServerWrapper replay = this->plugin->gameWrapper->GetGameEventAsReplay();

	if (!camera.IsNull())
	{
		if (params.size() > 1)
		{
			if (params[1] == "orange")
			{
				camera.SetFlyCamBallTargetMode();
				camera.SetFocusActor("");
				camera.SetLocation(Vector{ 0, 8000, 1800 });
				camera.SetRotation(Rotator{ -2200, -16380, 0 });
			}
			else if (params[1] == "blue")
			{
				camera.SetLocation(Vector{ 0, -8000, 1800 });
				camera.SetRotation(Rotator{ -2200, 16380, 0 });
			}
		}
	}
}


vector<HUD::HUD_ITEM> SetCameraController::GetHudItems() {
	return { };
}

