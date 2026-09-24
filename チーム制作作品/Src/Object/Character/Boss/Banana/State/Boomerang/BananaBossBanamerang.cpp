#include "BananaBossBanamerang.h"


BananaBossBanamerang::BananaBossBanamerang(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
) :
	operatorSenderId(operatorSenderId),
	stats(stats)
{
	banamerang.Load("Character/Banana/Banamerang");
	banamerang.scale = SIZE;
}

void BananaBossBanamerang::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, RADIUS, 120.0f)
	);

	CreateAttackSkill(operatorSenderId, 85, &stats, COLLIDER_TAG::BOSS_ATTACK);
	SetPushFlg(false);
	SetJudge(false);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetTransformPtr(&banamerang);

	collBack.Load("Range/CircleHalfRangeBack");
	collFront.Load("Range/CircleHalfRangeFront");

	collBack.pos = POS;
	collFront.pos = POS;

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	collBack.localAngle = Vector3::Yonly(Deg2Rad(180.0f));
	collFront.localAngle = Vector3::Yonly(Deg2Rad(180.0f));

	isDrawArea = false;
}

void BananaBossBanamerang::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		banamerang.Draw();
		SetUseLighting(true);
	}
}
