#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "HUD.h"

constexpr auto plugin_version = "0.2.0";

class ReplayReview: public BakkesMod::Plugin::BakkesModPlugin
{
	virtual void onLoad();
	virtual void onUnload();
	void Render(CanvasWrapper canvas);
	std::string menuTitle_ = "Replay Review Plugin";
};

