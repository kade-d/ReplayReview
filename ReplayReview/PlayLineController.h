#include "ControllerBase.h"

class PlayLineController : public ControllerBase {

	public:
		PlayLineController(BakkesMod::Plugin::BakkesModPlugin* p, HUD* h) {
			plugin = p;
			hud = h;
		}

		void DrawPlayLine(CanvasWrapper canvas);
		Vector Get3DIntersectionVector(Vector rayOrigin, Vector rayDirection, Vector planePoint, Vector planeNormal);
		void OnDrawPlayLine(vector<string> params);
		Vector RayBoxIntersection(Vector rayOrigin, Vector rayDirection, Vector minAABB, Vector maxAABB);
		float CalculateDistanceToIntersection(Vector& rayOrigin, Vector& rayDirection, Vector& minAABB, Vector& maxAABB);
		float GetIntersectionPoint(float dimRayOrigin, float dimRayDirection, float distance);
		void CheckDim(float& tLow, float& tHigh, float& low, float& high);
		void OnClearPlayLine(vector<string> params);
		void ShowPlayLines(CanvasWrapper& canvas);
		void Render(CanvasWrapper canvas);
		vector<HUD::HUD_ITEM> GetHudItems();
};