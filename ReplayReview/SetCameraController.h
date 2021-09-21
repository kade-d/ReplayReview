#pragma once
#include "ControllerBase.h"

class SetCameraController : public ControllerBase
{
	public:
		SetCameraController(BakkesMod::Plugin::BakkesModPlugin* p, HUD* h) {
			plugin = p;
			hud = h;
		}

		void OnSetCameraSide(std::vector<std::string> params);

		vector<HUD::HUD_ITEM> GetHudItems();

};

