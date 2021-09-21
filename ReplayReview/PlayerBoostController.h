#pragma once
#include "ControllerBase.h"
class PlayerBoostController : public ControllerBase
{
	public:
		PlayerBoostController(BakkesMod::Plugin::BakkesModPlugin* p, HUD* h) {
			plugin = p;
			hud = h;
		}
		void OnDrawPlayerBoost(vector<string> params);
		void DrawPlayerBoost(CanvasWrapper canvas);
		void Render(CanvasWrapper canvas);
		vector<HUD::HUD_ITEM> GetHudItems();
};

