#include "pch.h"
#include "HUD.h"

Vector2 boxSize = Vector2{ 110, 40 };
Vector2 cameraBoxSize = Vector2{ 110, 20 };

const int SPACING = 20;
float textScale = 1;
bool hidden = false;

std::vector<HUD::HUD_ITEM> hudItems = {};
std::vector<HUD::HUD_ITEM> cameraItems;


int HUD::AddHUDItem(HUD_ITEM item) {
	items.push_back(item);
	return items.size() - 1;
}

int HUD::AddHUDItems(std::vector<HUD_ITEM> items) {
	for (int i = 0; i < items.size(); i++) {
		hudItems.push_back(items[i]);
	}
	return items.size() - 1;
}

void HUD::ClearHudItems() {
	hudItems = {};
}

void HUD::SetCameraHudItems(std::vector<HUD::HUD_ITEM> items) {
	cameraItems = items;
}

void HUD::ChangeHUDItem(int index, HUD_ITEM newItem) {
	hudItems[index] = newItem;
}

bool HUD::Hide() {
	hidden = !hidden;
	return hidden;
}

void HUD::DrawHUDItems(CanvasWrapper canvas) {
	for (int i = 0; i < hudItems.size(); ++i) {

		Vector2 startPos = Vector2{ (boxSize.X * i) + (SPACING * i) + SPACING, 25};

		if (hidden) {
			//only draw special hud items
			if (!hudItems[i].pinned) {
				continue;
			}
		}


		if (hudItems[i].pinned) {
			//pin items to right of window
			startPos.X = canvas.GetSize().X - 150;
		}
		
		canvas.SetColor(0, 0, 0, 160);
		canvas.SetPosition(startPos);
		canvas.FillBox(boxSize);
		canvas.SetColor(255, 255, 255, 255);
		
		Vector2F stringSize = canvas.GetStringSize(hudItems[i].name + " (" + hudItems[i].key + ")", textScale, textScale);
		Vector2 stringPos;
		stringPos.X = startPos.X + ((boxSize.X - stringSize.X) / 2);
		stringPos.Y = startPos.Y + ((boxSize.Y - stringSize.Y) / 2);
		canvas.SetPosition(stringPos);
		canvas.DrawString(hudItems[i].name + " (" + hudItems[i].key + ")", textScale, textScale);
	}
	
	if (!hidden) 
	{
		DrawCameraHudItems(canvas);
	}
	
}

void HUD::DrawCameraHudItems(CanvasWrapper& canvas)
{

	int offset = GetOffset(canvas);

	Vector2F startPos = Vector2F{ (float)(canvas.GetSize().X / 2) - offset, (float)canvas.GetSize().Y - 30 };

	for (int i = 0; i < cameraItems.size(); ++i) 
	{
		std::string playerName = cameraItems[i].name + " (" + cameraItems[i].key + ")";

		Vector2F stringSize = canvas.GetStringSize(playerName, textScale, textScale);
		Vector2F boxSize = stringSize + 10;
		
		Vector2F boxPos;
		Vector2F stringPos;

		boxPos = startPos;
		stringPos = startPos;
		startPos.X += boxSize.X + SPACING;

		boxPos -= 5;
		
		cameraItems[i].color.A = .8;
		canvas.SetColor(cameraItems[i].color * 255);
		canvas.SetPosition(boxPos);
		canvas.FillBox(boxSize);

		canvas.SetPosition(stringPos);
		canvas.SetColor(255, 255, 255, 255);
		canvas.DrawString(playerName, textScale, textScale);
		
	}
}

// get offset so we can center the camera hud items
int HUD::GetOffset(CanvasWrapper& canvas)
{
	int offset = 0;
	for (int i = 0; i < cameraItems.size(); ++i)
	{
		std::string playerName = cameraItems[i].name + " (" + cameraItems[i].key + ")";
		Vector2F stringSize = canvas.GetStringSize(playerName, textScale, textScale);
		Vector2F boxSize = stringSize + 10;
		if (cameraItems[i].team == 0) {
			if (i == 0) {
				offset += boxSize.X;
			}
			else {
				offset += boxSize.X + SPACING;
			}
		}
	}
	offset += SPACING / 4;
	return offset;
}



