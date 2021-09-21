#include "PlayerBoostController.h"
#include "HUD.h"


HUD::HUD_ITEM boostItem = { "B", "Boost" };
int boostItemIndex;

bool isShowingBoost = false;

void PlayerBoostController::OnDrawPlayerBoost(vector<string> params) {
	isShowingBoost = !isShowingBoost;
}

void PlayerBoostController::DrawPlayerBoost(CanvasWrapper canvas) {

	ReplayServerWrapper replayGame = this->plugin->gameWrapper->GetGameEventAsReplay();
	if (!replayGame.IsNull())
	{
		ArrayWrapper<CarWrapper> cars = replayGame.GetCars();
		Vector carLocations[8];

		canvas.SetColor(255, 255, 255, 255);
		for (int i = 0; i < cars.Count(); i++)
		{
			carLocations[i] = cars.Get(i).GetLocation();
			canvas.SetPosition(canvas.Project(carLocations[i]));
			if (!cars.Get(i).GetBoostComponent().IsNull())
			{
				int boostAmount = static_cast<int>(cars.Get(i).GetBoostComponent().GetCurrentBoostAmount() * 100);
				canvas.DrawString(std::to_string(boostAmount), 1.5, 1.5);
			}
		}
	}
}

void PlayerBoostController::Render(CanvasWrapper canvas) {
	if (isShowingBoost) {
		DrawPlayerBoost(canvas);
	}
}

vector<HUD::HUD_ITEM> PlayerBoostController::GetHudItems() {
	return { boostItem };
}
