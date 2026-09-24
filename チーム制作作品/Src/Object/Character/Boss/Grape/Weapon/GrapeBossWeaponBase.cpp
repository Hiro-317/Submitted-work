#include "GrapeBossWeaponBase.h"


GrapeBossWeaponBase::GrapeBossWeaponBase(int model, int front, int back)
	: ActorBase("Data/Parameter/Character/Boss/Grape/Weapons/"), collBack(), collFront()
{
	trans.Duplicate(model);
	collFront.model = front;
	collBack.model = back;
}

void GrapeBossWeaponBase::SubDraw(void)
{
	trans.Draw();

	SetUseLighting(false);
	collBack.Draw();
	collFront.Draw();
	SetUseLighting(true);
}
