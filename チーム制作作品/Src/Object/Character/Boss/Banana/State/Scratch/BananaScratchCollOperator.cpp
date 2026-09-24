#include "BananaScratchCollOperator.h"


BananaScratchCollOperator::BananaScratchCollOperator(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats,
	const Vector3& pos,
	const Vector3& StartPos,
	const Vector3& EndPos
) :
	operatorSenderId(operatorSenderId),
	stats(stats),
	pos(pos),
	StartPos(StartPos), EndPos(EndPos),
	collBack(),	collFront()
{
}

void BananaScratchCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::BOSS_ATTACK, StartPos, EndPos, RADIUS)
	);

	CreateAttackSkill(operatorSenderId, 75, &stats, COLLIDER_TAG::BOSS_ATTACK);
	SetPushFlg(false);
	SetJudge(false);

	collBack.centerDiff = Vector3::XZonly(pos.x, pos.z) + DIFF;
	collFront.centerDiff = Vector3::XZonly(pos.x, pos.z) + DIFF;

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
}

void BananaScratchCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}
