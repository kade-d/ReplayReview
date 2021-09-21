#pragma once
#include "ControllerBase.h"
#include "HUD.h"

class CameraFocusController : public ControllerBase
{
	public: 
		CameraFocusController(BakkesMod::Plugin::BakkesModPlugin* p, HUD* h) {
			plugin = p;
			hud = h;
		}

		void SetFocusedPlayer(ActorWrapper useless, void* params, string);
		void SetFlyCamNoTarget(CameraWrapper& camera);
		int GetNumOfPlayersFromFocusActors();
		void SetCamToPlayerViewFromUnknown(CameraWrapper& camera, std::string& selectedPlayer);
		void LoadPlayersInGame();
		void LoadFocusActorsFromPriWrappers(std::vector<PriWrapper> pris);
		std::vector<PriWrapper> SortPriWrappers(std::vector<PriWrapper> pris);
		std::vector<PriWrapper> SortPriWrappersByTeamNum(std::vector<PriWrapper> pris);
		std::vector<PriWrapper> SortPriWrappersAlphabetically(std::vector<PriWrapper> pris);
		std::vector<HUD::HUD_ITEM> CreateCameraHudItems(std::vector<PriWrapper> pris);
		std::string GetFocusedPlayer();
		void SendMouseInput(DWORD press, DWORD release);
		vector<HUD::HUD_ITEM> GetHudItems();
};

