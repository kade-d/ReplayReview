#include "HideHudController.h"

HUD::HUD_ITEM hideHudItem = { "O", "Hide HUD", true };
int hudItemIndex;


void HideHudController::OnHideHud(std::vector<std::string> params)
{
	bool hidden = hud->Hide();

	if (hidden) {
		hideHudItem.name = "Show HUD";
		hud->ChangeHUDItem(hudItemIndex, hideHudItem);
	}
	else {
		hideHudItem.name = "Hide HUD";
		hud->ChangeHUDItem(hudItemIndex, hideHudItem);
	}

}


vector<HUD::HUD_ITEM> HideHudController::GetHudItems() {
	return { hideHudItem };
}

