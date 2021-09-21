#include "pch.h"
#include "PlayLineController.h"
#include "HUD.h"
#include <RenderingTools.h>

vector<vector<Vector>> playLines = {};
vector<Vector> currentPlayLine = {};
bool isDrawingPlayLines = false;

HUD::HUD_ITEM playLineItem = { "V", "Draw" };
HUD::HUD_ITEM clearPlayLineItem = { "C", "Clear" };


void PlayLineController::OnDrawPlayLine(vector<string> params)
{
	if (isDrawingPlayLines)
	{
		playLines.push_back(currentPlayLine);
		currentPlayLine = {};
	}
	isDrawingPlayLines = !isDrawingPlayLines;
}


void PlayLineController::DrawPlayLine(CanvasWrapper canvas) 
{

	CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
	ReplayServerWrapper replay = this->plugin->gameWrapper->GetGameEventAsReplay();

	if (!replay.IsNull() && !camera.IsNull()) {
		if (isDrawingPlayLines) {

			Vector cameraPoint = camera.GetLocation();
			Vector cameraRot = RotatorToVector(camera.GetRotation()).getNormalized();

			try {
				Vector intersection = RayBoxIntersection(cameraPoint, cameraRot, Vector{ -4096, -5120, 50 }, Vector{ 4096, 5120, 2044 });
				currentPlayLine.push_back(intersection);
			}
			catch (const char* msg) {

			}
		}
	}
}


Vector PlayLineController::Get3DIntersectionVector(Vector rayOrigin, Vector rayDirection, Vector planePoint, Vector planeNormal) {
	double rayPlaneDirection = Vector().dot(rayDirection, planeNormal); //tests if ray and plane are parallel
	if (rayPlaneDirection != 0)
	{
		//line and plane intersect
		double scalar = Vector().dot(planePoint - rayOrigin, planeNormal) / Vector().dot(rayDirection, planeNormal); //aka 't' in many examples
		if (scalar > 0)
		{
			// surface is 'in front of' ray
			return rayOrigin + rayDirection * scalar; //intersection point
		}
	}
	throw "No intersection Present";
}


Vector PlayLineController::RayBoxIntersection(Vector rayOrigin, Vector rayDirection, Vector minAABB, Vector maxAABB) {
	float distance = CalculateDistanceToIntersection(rayOrigin, rayDirection, minAABB, maxAABB);
	if (distance == INFINITY) {
		throw "No Intersection Present";
	}

	Vector intersectionPoint = { 0, 0, 0 };

	intersectionPoint.X = GetIntersectionPoint(rayOrigin.X, rayDirection.X, distance);
	intersectionPoint.Y = GetIntersectionPoint(rayOrigin.Y, rayDirection.Y, distance);
	intersectionPoint.Z = GetIntersectionPoint(rayOrigin.Z, rayDirection.Z, distance);

	return intersectionPoint;
}


float PlayLineController::CalculateDistanceToIntersection(Vector& rayOrigin, Vector& rayDirection, Vector& minAABB, Vector& maxAABB)
{
	float low = -INFINITY;
	float high = INFINITY;

	float tLowX = (minAABB.X - rayOrigin.X) / rayDirection.X;
	float tHighX = (maxAABB.X - rayOrigin.X) / rayDirection.X;

	float tLowY = (minAABB.Y - rayOrigin.Y) / rayDirection.Y;
	float tHighY = (maxAABB.Y - rayOrigin.Y) / rayDirection.Y;

	float tLowZ = (minAABB.Z - rayOrigin.Z) / rayDirection.Z;
	float tHighZ = (maxAABB.Z - rayOrigin.Z) / rayDirection.Z;

	CheckDim(tLowX, tHighX, low, high);
	CheckDim(tLowY, tHighY, low, high);
	CheckDim(tLowZ, tHighZ, low, high);

	if (low > high) {
		return INFINITY;
	}
	if (low < 0) {
		return high;
	}
	return low;

}


float PlayLineController::GetIntersectionPoint(float dimRayOrigin, float dimRayDirection, float distance) {
	return dimRayOrigin + (dimRayDirection * distance);
}


void PlayLineController::CheckDim(float& tLow, float& tHigh, float& low, float& high)
{
	if (tLow > tHigh) {
		swap(tLow, tHigh);
	}
	if (tHigh < low || tLow > high) {
		throw "No intersection present";
	}
	if (tLow > low) {
		low = tLow;
	}
	if (tHigh < high) {
		high = tHigh;
	}
}


void PlayLineController::OnClearPlayLine(vector<string> params) {
	playLines.clear();
}


void PlayLineController::ShowPlayLines(CanvasWrapper& canvas)
{
	CameraWrapper camera = this->plugin->gameWrapper->GetCamera();
	if (camera.IsNull()) {
		return;
	}

	canvas.SetColor(255, 255, 0, 255);

	if (playLines.size() > 0) {
		for (int i = 0; i < playLines.size(); i++) {
			if (playLines[i].size() > 0) {
				for (int j = 0; j < playLines[i].size() - 1; j++) {

					RT::Frustum frustum = RT::Frustum(canvas, camera);
					RT::Line line = RT::Line(playLines[i][j], playLines[i][j + 1]);

					line.DrawWithinFrustum(canvas, frustum);
				}
			}
		}
	}
	//current play line may not have been added to play lines yet (bc it might not be finished) but we still want to draw it
	//i.e. this is the line the user is currently drawing.
	if (currentPlayLine.size() > 0) {
		for (int k = 0; k < currentPlayLine.size() - 1; k++) {
			RT::Frustum frustum = RT::Frustum(canvas, camera);
			RT::Line line = RT::Line(currentPlayLine[k], currentPlayLine[k + 1]);

			line.DrawWithinFrustum(canvas, frustum);
		}
	}
}

void PlayLineController::Render(CanvasWrapper canvas) {
	if (isDrawingPlayLines) {
		DrawPlayLine(canvas);
	}
	ShowPlayLines(canvas);
}

vector<HUD::HUD_ITEM> PlayLineController::GetHudItems() {
	return {playLineItem, clearPlayLineItem};
}
