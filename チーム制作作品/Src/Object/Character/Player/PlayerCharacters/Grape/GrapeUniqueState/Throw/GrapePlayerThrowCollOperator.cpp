#include "GrapePlayerThrowCollOperator.h"

#include "../../../../../../../Manager/Effect/EffectManager.h"
#include "../../../../../../Common/Collider/SphereCollider.h"

GrapePlayerThrowCollOperator::GrapePlayerThrowCollOperator(
	COLLIDER_TAG COLL_TAG,
	const short ATTACK_RATE_PERCENT,
	const Vector3& playerPos, const Vector3& playerAngle,
	MSG_SENDER_ID operatorSenderId,
	const CharacterStats& playerStats
) :
	ActorBase(),
	COLL_TAG(COLL_TAG),
	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	playerPos(playerPos), playerAngle(playerAngle),
	operatorSenderId(operatorSenderId),
	playerStats(playerStats),
	targetVec(Vector3()),
	gravity(0),
	bouncePower(0.0f),
	isBlast(false)
{
}

void GrapePlayerThrowCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(true);
#pragma endregion

	// コライダー生成
	ColliderCreate(new SphereCollider(COLL_TAG, 50.0f));

	// 初期化処理
	SetJudge(false);
	isBlast = false;

	// 最初は描画しない
	SetIsDraw(false);

	// スキルのダメージ量の設定
	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);

	trans.Load("Character/Grape/Bomb");
	trans.scale = 0.5f;
}

void GrapePlayerThrowCollOperator::SubUpdate()
{
	// ヒットしたら消える
	if (isBlast) {
		CollOff();
		SetIsDraw(false);
		return;
	}

	// 重力を加える
	gravity -= 0.5f;

	if (!isBlast) {
		CollOn();

		// 投げた爆弾が地面についたら、前回より跳躍力を弱めながら
		// バウンドしていく
		if (isGround) {
			bouncePower /= 1.5f;
			gravity = bouncePower;

			// 跳躍力がある程度減少したら、爆発させる
			if (bouncePower <= 0.1f ) {
				ColliderSerch<SphereCollider>(GetCollider(), COLL_TAG).back()->SetRadius(300.0f);
				bouncePower = 0.0f;
				LocalThrowBombEnd();
				return;
			}
		}
		else {
			// 前方向へ移動
			trans.pos += targetVec * 10.0f;
		}

		// 縦方向へ移動
		trans.pos.y += gravity;
	}
}

void GrapePlayerThrowCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (!Net::GetIns().IsHost()) { return; }
	// 攻撃の当たり判定
	switch (other.GetTag())
	{
	case COLLIDER_TAG::BOSS:
	case COLLIDER_TAG::ENEMY:
	case COLLIDER_TAG::BOSS_DISTANCE:
		LocalThrowBombEnd();
		break;

	default:break;
	}
}


void GrapePlayerThrowCollOperator::RemoteThrowBombStart(const Vector3& pos, const Vector3& vec)
{
	ColliderSerch<SphereCollider>(GetCollider(), COLL_TAG).back()->SetRadius(100.0f);

	targetVec = vec;
	trans.pos = pos;
	bouncePower = 10.0f;
	gravity = bouncePower;

	isBlast = false;
	SetIsDraw(true);
	SetPushFlg(true);
	CollOff();
}

void GrapePlayerThrowCollOperator::RemoteThrowBombEnd(void)
{
	if (isBlast) { return; }
	isBlast = true;
	EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_SMALL, trans.pos);
	SetIsDraw(false);
}

void GrapePlayerThrowCollOperator::LocalThrowBombStart(const Vector3& pos, const Vector3& vec)
{
	RemoteThrowBombStart(pos, vec);

	Net::GetIns().Send(MsgDataGrapePlayerBombThrowStart(trans.pos, vec), operatorSenderId);
}

void GrapePlayerThrowCollOperator::LocalThrowBombEnd(void)
{
	RemoteThrowBombEnd();

	Net::GetIns().Send(MsgDataGrapePlayerBombThrowEnd(), operatorSenderId);
}
