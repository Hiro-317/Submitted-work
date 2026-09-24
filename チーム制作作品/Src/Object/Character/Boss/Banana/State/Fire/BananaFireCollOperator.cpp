#include "BananaFireCollOperator.h"


BananaFireCollOperator::BananaFireCollOperator(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
) :
	operatorSenderId(operatorSenderId),
	stats(stats)
{
}

void BananaFireCollOperator::Load(void)
{
	// プレイヤー当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(COLLIDER_TAG::BOSS_ATTACK, RADIUS)
	);

	CreateAttackSkill(operatorSenderId, 100, &stats, COLLIDER_TAG::BOSS_ATTACK);
	SetPushFlg(false);
	SetJudge(false);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetTransformPos(POS);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
}

void BananaFireCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		SetUseLighting(true);
	}
}
