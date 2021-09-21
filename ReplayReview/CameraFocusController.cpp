#include "pch.h"
#include "CameraFocusController.h"
#include "HUD.h"

using namespace std;

string focusActors[8];

void CameraFocusController::SetFocusedPlayer(ActorWrapper useless, void* params, string)
{
	if (!this->plugin->gameWrapper->IsInReplay())
	{
		return;
	}

	struct FName {
		int index;
		int number;
	};

	struct handleKeyPressStruct {
		int controllerId;
		struct FName key;
		unsigned char eventType;
		float amountDepressed;
		bool bGamepad;
		bool returnValue;
	};

	handleKeyPressStruct* castParams = (handleKeyPressStruct*)params;

	int keyOneIndex = this->plugin->gameWrapper->GetFNameIndexByString("One");

	if (castParams->eventType == 0 //pressed down
		&& castParams->key.index >= keyOneIndex //greater than one
		&& castParams->key.index < keyOneIndex + GetNumOfPlayersFromFocusActors()) //less than 1 + num of players
	{
		// key is between 1 and 8 (on keyboard)

		int pressedKey = castParams->key.index - keyOneIndex;
		CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
		if (!camera.IsNull())
		{
			string selectedPlayer = focusActors[pressedKey];
			if (camera.GetCameraState() != "CameraState_BallCam_TA" && camera.GetCameraState() != "CameraState_Car_TA")
			{
				// only if camera state is not in player view
				SetCamToPlayerViewFromUnknown(camera, selectedPlayer);
			}
			else
			{
				camera.SetFocusActor(selectedPlayer);
			}
			
			
		}
	}
	else if (castParams->key.index == keyOneIndex + GetNumOfPlayersFromFocusActors()) {
		CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
		if (!camera.IsNull()) {
			SetFlyCamNoTarget(camera);
		}
	}
}

void CameraFocusController::SetFlyCamNoTarget(CameraWrapper& camera)
{
	camera.SetFlyCamBallTargetMode();
	SendMouseInput(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
}

int CameraFocusController::GetNumOfPlayersFromFocusActors() {
	int count = 0;
	for (int i = 0; i < sizeof(focusActors) / sizeof(focusActors[0]); i++) {
		if (!empty(focusActors[i])) {
			count += 1;
		}
	}
	return count;
}


void CameraFocusController::SetCamToPlayerViewFromUnknown(CameraWrapper& camera, std::string& selectedPlayer)
{
	camera.SetFlyCamBallTargetMode();
	camera.SetFocusActor(selectedPlayer);
	SendMouseInput(MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	SendMouseInput(MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	SendMouseInput(MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
}


void CameraFocusController::LoadPlayersInGame()
{

	ReplayServerWrapper replay = this->plugin->gameWrapper->GetGameEventAsReplay();
	if (!replay.IsNull())
	{
		ArrayWrapper<CarWrapper> cars = replay.GetCars();
		if (!cars.IsNull())
		{
			std::vector<PriWrapper> priWrappers = {};
			std::vector<HUD::HUD_ITEM> cameraHudItems = {};
			for (int i = 0; i < cars.Count(); i++)
			{
				CarWrapper car = cars.Get(i);
				if (car.IsNull())
				{
					continue;
				}
				PriWrapper pri = car.GetPRI();
				if (pri.IsNull())
				{
					continue;
				}
				priWrappers.push_back(pri);

			}

			priWrappers = SortPriWrappers(priWrappers);

			LoadFocusActorsFromPriWrappers(priWrappers);

			cameraHudItems = CreateCameraHudItems(priWrappers);

			hud->SetCameraHudItems(cameraHudItems);
		}
	}
}


void CameraFocusController::LoadFocusActorsFromPriWrappers(std::vector<PriWrapper> pris) {
	for (int i = 0; i < pris.size(); i++) {
		focusActors[i] = "Player_" + pris[i].GetUniqueIdWrapper().GetIdString();
	}
}


std::vector<PriWrapper> CameraFocusController::SortPriWrappers(std::vector<PriWrapper> pris)
{
	pris = SortPriWrappersAlphabetically(pris);
	pris = SortPriWrappersByTeamNum(pris);

	return pris;
}


std::vector<PriWrapper> CameraFocusController::SortPriWrappersByTeamNum(std::vector<PriWrapper> pris)
{
	std::vector<PriWrapper> tempPris = {};
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < pris.size(); j++)
		{
			PriWrapper pri = pris[j];
			if (pri.GetTeamNum() == i) {
				tempPris.push_back(pri);
			}
		}
	}
	return tempPris;
}


std::vector<PriWrapper> CameraFocusController::SortPriWrappersAlphabetically(std::vector<PriWrapper> pris)
{
	for (int i = 0; i < pris.size() - 1; i++)
	{
		for (int j = 0; j < pris.size() - i - 1; j++)
		{
			if (pris[j].GetPlayerName().ToString().compare(pris[j + 1].GetPlayerName().ToString()) > 0) { //j comes after j + 1 alphabetically
				PriWrapper temp = pris[j];
				pris[j] = pris[j + 1];
				pris[j + 1] = temp;
			}
		}
	}
	return pris;
}


std::vector<HUD::HUD_ITEM> CameraFocusController::CreateCameraHudItems(std::vector<PriWrapper> pris) {
	ReplayServerWrapper replay = this->plugin->gameWrapper->GetGameEventAsReplay();
	std::vector<HUD::HUD_ITEM> items = {};
	int i = 0;
	for (PriWrapper pri : pris) {
		LinearColor base = replay.GetTeams().Get(pri.GetTeamNum()).GetFontColor();

		CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
		if (!camera.IsNull()) {
			if (camera.GetFocusActor() == "Player_" + pri.GetUniqueIdWrapper().GetIdString()) {
				base = { 0, 0, 0, 1 };
			}
		}

		HUD::HUD_ITEM cameraHudItem = { std::to_string(i + 1), pri.GetPlayerName().ToString(),  pri.GetTeamNum() == 0, base, pri.GetTeamNum() };
		items.push_back(cameraHudItem);
		i++;
	}
	items.push_back({ to_string(GetNumOfPlayersFromFocusActors() + 1), "Fly", false, {.15, .15, .15, 1}, 3 });
	return items;
}


std::string CameraFocusController::GetFocusedPlayer() {
	CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
	if (!camera.IsNull())
	{
		PriWrapper specPRI = PriWrapper(reinterpret_cast<std::uintptr_t>(camera.GetViewTarget().PRI));
		if (!specPRI.IsNull())
		{
			return specPRI.GetPlayerName().ToString();
		}
	}
}


void CameraFocusController::SendMouseInput(DWORD press, DWORD release) {
	INPUT ip;

	ip.type = INPUT_MOUSE;

	ip.mi.dwFlags = press;
	SendInput(1, &ip, sizeof(INPUT));

	ip.mi.dwFlags = release;
	SendInput(1, &ip, sizeof(INPUT));
}


vector<HUD::HUD_ITEM> CameraFocusController::GetHudItems() {
	return {};
}
