#include "ToPlayerTargetUI.h"

ToPlayerTargetUI::ToPlayerTargetUI(const Vector2I& drawPos) :
	drawPos(drawPos)
{
}

void ToPlayerTargetUI::Load(void)
{
	UILoadImage("PlayerToNowSelect", 0, FILE_PATH_TYPE::PLAYER_HP);
}

void ToPlayerTargetUI::SubDraw(void)
{
	DrawRotaGraph(drawPos.x, drawPos.y, 1, 0, uiImages.front(), true);
}