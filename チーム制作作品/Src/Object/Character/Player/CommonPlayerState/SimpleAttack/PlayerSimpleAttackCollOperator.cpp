#include "PlayerSimpleAttackCollOperator.h"

#include "../../../../Common/Collider/SphereCollider.h"

PlayerSimpleAttackCollOperator::PlayerSimpleAttackCollOperator(
	float FIND_ATTACK_TARGET_RANGE,
	COLLIDER_TAG COLL_TAG, float COLL_SIZE,
	const Vector3& COLL_LOCAL_POS,

	const Vector3& playerPos,
	const Vector3& playerAngle,

	const short ATTACK_RATE_PERCENT,
	MSG_SENDER_ID operatorSenderId,
	const CharacterStats& playerStats
) :
	ActorBase(),
	FIND_ATTACK_TARGET_RANGE(FIND_ATTACK_TARGET_RANGE),
	COLL_TAG(COLL_TAG), COLL_SIZE(COLL_SIZE),
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerPos(playerPos), playerAngle(playerAngle),

	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),

	operatorSenderId(operatorSenderId),
	playerStats(playerStats),

	isFindAttackTarget(false),
	targetPos(nullptr),
	isHit(false)
{
}

void PlayerSimpleAttackCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// 当たり判定情報を生成する
	ColliderCreate(new SphereCollider(COLL_TAG, COLL_SIZE, COLL_LOCAL_POS));
	ColliderCreate(new SphereCollider(COLLIDER_TAG::PLAYER_COMMON, FIND_ATTACK_TARGET_RANGE));
	SetJudge(false);

	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);
}

void PlayerSimpleAttackCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (ownTag == COLLIDER_TAG::PLAYER_COMMON) {
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:
			isFindAttackTarget = true;
			targetPos = &other.GetTransform().pos;
			ColliderSerch(GetCollider(), COLLIDER_TAG::PLAYER_COMMON).back()->SetJudgeFlg(false);
			break;
		default:break;
		}
	}
	else if(ownTag == COLL_TAG) {
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:
			isHit = true;
			break;
		default:break;
		}
	}
}

void PlayerSimpleAttackCollOperator::SubUpdate(void)
{
	trans.pos = playerPos;
	trans.angle = playerAngle;
	ColliderSerch(GetCollider(), COLLIDER_TAG::PLAYER_COMMON).back()->SetJudgeFlg(false);
}
