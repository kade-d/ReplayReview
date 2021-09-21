#pragma once
#include "pch.h"
#include "HUD.h"

using namespace std;

class ControllerBase
{
	public:
		BakkesMod::Plugin::BakkesModPlugin* plugin;
		HUD* hud;

		ControllerBase() {
			
		}

		virtual vector<HUD::HUD_ITEM> GetHudItems() = 0;

};

