#include "ReplayReview.h"
#include "HUD.h"
#include "ControllerBase.h"
#include "PlayLineController.h"
#include "CrosshairController.h"
#include "PlayerBoostController.h"
#include "SetCameraController.h"
#include "CameraFocusController.h"
#include "HideHudController.h"
#include <math.h>
#include <RenderingTools.h>
#include <map>

using namespace std;

HUD hud = HUD();

vector<std::shared_ptr<ControllerBase>> controllers = {};


BAKKESMOD_PLUGIN(ReplayReview, "Review RL Replays", plugin_version, PLUGINTYPE_REPLAY)

std::shared_ptr<PlayLineController> playLineController;
std::shared_ptr<CrosshairController> crosshairController;
std::shared_ptr<PlayerBoostController> playerBoostController;
std::shared_ptr<SetCameraController> setCameraController;
std::shared_ptr<CameraFocusController> cameraFocusController;
std::shared_ptr<HideHudController> hideHudController;


void ReplayReview::onLoad()
{
	playLineController = make_shared<PlayLineController>(this, &hud);
	crosshairController = make_shared<CrosshairController>(this, &hud);
	playerBoostController = make_shared<PlayerBoostController>(this, &hud);
	setCameraController = make_shared<SetCameraController>(this, &hud);
	cameraFocusController = make_shared<CameraFocusController>(this, &hud);
	hideHudController = make_shared<HideHudController>(this, &hud);

	controllers.push_back(playLineController);
	controllers.push_back(crosshairController);
	controllers.push_back(playerBoostController);
	controllers.push_back(setCameraController);
	controllers.push_back(cameraFocusController);
	controllers.push_back(hideHudController);
	
	cvarManager->registerNotifier("draw_line", std::bind(&PlayLineController::OnDrawPlayLine, playLineController, std::placeholders::_1), "Starts/stops drawing play line", PERMISSION_REPLAY);
	cvarManager->registerNotifier("clear_line", std::bind(&PlayLineController::OnClearPlayLine, playLineController, std::placeholders::_1), "Clears the play line", PERMISSION_REPLAY);
	cvarManager->registerNotifier("show_player_boost", std::bind(&PlayerBoostController::OnDrawPlayerBoost, playerBoostController, std::placeholders::_1), "Shows player boost amounts", PERMISSION_REPLAY);
	cvarManager->registerNotifier("show_crosshair", std::bind(&CrosshairController::OnShowCrosshair, crosshairController, std::placeholders::_1), "Shows a crosshair for drawing play lines", PERMISSION_REPLAY);
	cvarManager->registerNotifier("set_camera_side", std::bind(&SetCameraController::OnSetCameraSide, setCameraController, std::placeholders::_1), "Sets camera position to blue/orange side", PERMISSION_REPLAY);
	cvarManager->registerNotifier("hide_hud", std::bind(&HideHudController::OnHideHud, hideHudController, std::placeholders::_1), "Shows/hides the hud", PERMISSION_REPLAY);
	
	cvarManager->setBind("B", "show_player_boost");
	cvarManager->setBind("X", "show_crosshair");
	cvarManager->setBind("V", "draw_line");
	cvarManager->setBind("C", "clear_line");
	cvarManager->setBind("O", "hide_hud");
	
	gameWrapper->RegisterDrawable(bind(&ReplayReview::Render, this, std::placeholders::_1));

	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameViewportClient_TA.HandleKeyPress", std::bind(&CameraFocusController::SetFocusedPlayer, cameraFocusController, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	gameWrapper->HookEventPost("Function TAGame.Replay_TA.Tick", std::bind(&CameraFocusController::LoadPlayersInGame, cameraFocusController));
	
}

void ReplayReview::Render(CanvasWrapper canvas) {
	if (gameWrapper->IsInReplay()) 
	{
		hud.ClearHudItems();
		for (int i = 0; i < controllers.size(); i++) {
			hud.AddHUDItems(controllers[i]->GetHudItems());
		}

		hud.DrawHUDItems(canvas);
		
		playLineController->Render(canvas);
		crosshairController->Render(canvas);
		playerBoostController->Render(canvas);
		
	}
}

void ReplayReview::onUnload()
{

}