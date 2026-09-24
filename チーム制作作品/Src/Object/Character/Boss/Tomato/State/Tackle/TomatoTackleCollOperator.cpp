#include "TomatoTackleCollOperator.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"

TomatoTackleCollOperator::TomatoTackleCollOperator(
	const float TO_PLAYER_DISTANCE,
	MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats,
	const ParameterLoad& collParam
)
	:
	TO_PLAYER_DISTANCE(TO_PLAYER_DISTANCE + 5.0f),
	operatorSenderId(operatorSenderId),
	stats(stats),
	HEIGHT(collParam.GetParameter("Tackle", "Height")),
	SCALE(collParam.GetParameterToVector3("Tackle", "Scale")),
	CENTER(collParam.GetParameterToVector3("Tackle", "Center")* SCALE),
	ANGLE(collParam.GetParameterToVector3("Tackle", "Angle")),
	collBack(),
	collFront(),
	isDrawArea(false),
	rate(),
	stageHit()
{
}

void TomatoTackleCollOperator::Load(void)
{
	// 当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, TO_PLAYER_DISTANCE, 1000.0f)
	);

	// ステージ当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK_AREA, TO_PLAYER_DISTANCE, 1000.0f)
	);

	CreateAttackSkill(operatorSenderId, 150, &stats, COLLIDER_TAG::BOSS_ATTACK);

	SetJudge(true);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);

	collBack.Load("Range/LineRangeBack");
	collFront.Load("Range/LineRangeFront");

	collBack.pos = Vector3::Yonly(HEIGHT);
	collFront.pos = Vector3::Yonly(HEIGHT);

	collBack.scale = SCALE;
	collFront.scale = Vector3(SCALE.x, SCALE.y, 0.0f);

	collBack.centerDiff = CENTER;
	collFront.centerDiff = CENTER + Vector3::Yonly(1.0f);

	collBack.localAngle = ANGLE;
	collFront.localAngle = ANGLE;

	isDrawArea = false;
}

void TomatoTackleCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	// ステージに当たったか
	if (other.GetTag() == COLLIDER_TAG::STAGE && other.GetShape() == ColliderBase::SHAPE::XZ_CIRCLE) {

		stageHit = true;
	}
}

void TomatoTackleCollOperator::SubAlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collFront.Draw();
		collBack.Draw();
		SetUseLighting(true);
	}
}

void TomatoTackleCollOperator::SubRelease(void)
{
	collFront.Release();
	collBack.Release();
}
