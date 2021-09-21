#pragma once
#include "ControllerBase.h"


class CrosshairController : public ControllerBase
{
	public:
		CrosshairController(BakkesMod::Plugin::BakkesModPlugin* p, HUD* h) {
			plugin = p;
			hud = h;
		}

		void OnShowCrosshair(vector<string> params);

		void ShowCrosshair(CanvasWrapper canvas);

		void Render(CanvasWrapper canvas);

		vector<HUD::HUD_ITEM> GetHudItems();

};

