#include "GrapeKickDownCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

GrapeKickDownCollOperator::GrapeKickDownCollOperator(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
)	:
	operatorSenderId(operatorSenderId),
	stats(stats),
	collBack(),
	collFront()
{
}

void GrapeKickDownCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, RADIUS, 1000.0f)
	);

	CreateAttackSkill(operatorSenderId, 75, &stats, COLLIDER_TAG::BOSS_ATTACK);

	SetJudge(false);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT + 1.0f);

	collBack.scale = SCALE;
	collFront.scale = 0.0f;

	isDrawArea = false;
}

void GrapeKickDownCollOperator::SubUpdate(void)
{
	collBack.pos = Vector3(trans.pos.x ,HEIGHT, trans.pos.z);
	collFront.pos = Vector3(trans.pos.x, HEIGHT, trans.pos.z);
}

void GrapeKickDownCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}

void GrapeKickDownCollOperator::SubRelease(void)
{
	collBack.Release();
	collFront.Release();
}
