#include "pch.h"
#include "CrosshairController.h"
#include "HUD.h"


bool isShowingCrosshair = false;
int crosshairWidth = 2;
int crosshairLength = 8;


HUD::HUD_ITEM crosshairItem = { "X", "Crosshair" };
int crosshairItemIndex;


void CrosshairController::OnShowCrosshair(vector<string> params) {
	isShowingCrosshair = !isShowingCrosshair;
}

void CrosshairController::ShowCrosshair(CanvasWrapper canvas) {
	
	canvas.SetColor(255, 255, 255, 255);

	Vector2 center = canvas.GetSize() / 2.0;

	Vector2 top = Vector2{ center.X,  center.Y + crosshairLength };
	Vector2 bottom = Vector2{ center.X,  center.Y - crosshairLength };
	canvas.DrawLine(bottom, top, 2);

	Vector2 right = Vector2{ center.X + crosshairLength,  center.Y };
	Vector2 left = Vector2{ center.X - crosshairLength,  center.Y };

	canvas.DrawLine(left, right, crosshairWidth);
}

void CrosshairController::Render(CanvasWrapper canvas) {
	if (isShowingCrosshair) {
		ShowCrosshair(canvas);
	}
}

vector<HUD::HUD_ITEM> CrosshairController::GetHudItems() {
	return {crosshairItem};
}
