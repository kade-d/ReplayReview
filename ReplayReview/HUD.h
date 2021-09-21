#pragma once


class HUD
{
	public:
		struct HUD_ITEM {
			std::string key;
			std::string name;
			bool pinned = false;
			LinearColor color = { 0, 0, 0, .8 };
			int team = 0;
		};
		int AddHUDItem(HUD_ITEM item);
		int AddHUDItems(std::vector<HUD_ITEM> items);
		void ClearHudItems();
		void SetCameraHudItems(std::vector<HUD::HUD_ITEM> items);
		void ChangeHUDItem(int index, HUD_ITEM newItem);
		void DrawHUDItems(CanvasWrapper canvas);
		void DrawCameraHudItems(CanvasWrapper& canvas);
		int GetOffset(CanvasWrapper& canvas);
		bool Hide();

	private:
		std::vector<HUD::HUD_ITEM> items;
};

