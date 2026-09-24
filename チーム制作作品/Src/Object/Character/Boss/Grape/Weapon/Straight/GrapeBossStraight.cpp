#include "GrapeBossStraight.h"

#include "../../../../../Common/Collider/SphereCollider.h"


GrapeBossStraight::GrapeBossStraight(int model, int front, int back)
	: GrapeBossWeaponBase(model, front, back)
{
	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS_ATTACK,
			GetParameter("Straight", "Radius") * ATTACK_SIZE
			)
	);
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS_ATTACK_AREA,
			GetParameter("Straight", "AreaRadius") * ATTACK_SIZE
			)
	);

	trans.scale = Vector3(ATTACK_SIZE);

	SetJudge(false);
	SetPushFlg(false);
	// 重力無視
	SetGravityFlg(false);
}

void GrapeBossStraight::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(operatorSenderId, 50, &stats);

	collBack.scale = Vector3::XZonly(ATTACK_SIZE, ATTACK_SIZE);
	collFront.scale = Vector3::Zonly(ATTACK_SIZE);
}

void GrapeBossStraight::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (ownTag == COLLIDER_TAG::BOSS_ATTACK_AREA) {

		end = true;
		SetJudge(false);
	}
}

void GrapeBossStraight::SubUpdate(void)
{
	// 進行方向に動かす
	trans.pos += moveDir * MOVE_SPEED;
}
