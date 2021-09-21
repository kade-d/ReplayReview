#pragma once
#include "ControllerBase.h"

class HideHudController : public ControllerBase
{
public:
	HideHudController(BakkesMod::Plugin::BakkesModPlugin* p, HUD* h) {
		plugin = p;
		hud = h;
	}
	void OnHideHud(std::vector<std::string> params);
	vector<HUD::HUD_ITEM> GetHudItems();
};



