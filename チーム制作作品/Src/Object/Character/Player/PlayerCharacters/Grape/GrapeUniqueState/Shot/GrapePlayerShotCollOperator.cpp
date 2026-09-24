#include "GrapePlayerShotCollOperator.h"

#include "../../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../../../Manager/Effect/EffectManager.h"

GrapePlayerShotCollOperator::GrapePlayerShotCollOperator(
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
	moveVec(Vector3()),
	lifeCounter(0),
	isAlive(false)
{
}

void GrapePlayerShotCollOperator::Load(void)
{
#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 衝突時の押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	// コライダー生成
	ColliderCreate(new SphereCollider(COLL_TAG, 50.0f));

	// 初期化処理
	SetJudge(false);

	// 最初は描画しない
	SetIsDraw(false);

	// スキルのダメージ量の設定
	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);

	// 弾のモデルをロード、スケールの設定
	trans.Load("Character/Grape/Bomb");
	trans.scale = 0.1f;

	// 発射地点を初期化、プレイヤーの足元から少し上から発射するようにする
	trans.pos.y = trans.pos.y + 30.0f;

	// カウンタを設定
	lifeCounter = LIFE_TIME;
}

void GrapePlayerShotCollOperator::Update(void)
{
	if (!isAlive) { return; }

	// 撃った瞬間から弾の生存時間を減らす
	if (lifeCounter > 0) {
		lifeCounter--;
	}

	// 生存時間がなくなるか、敵に当たったら弾を消す処理を行う
	if (lifeCounter <= 0) {
		LocalShotEnd();
		return;
	}

	// 弾を飛ばす処理
	if (isAlive) {
		trans.pos += moveVec * 40.0f;
		CollOn();
	}
}

void GrapePlayerShotCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (!Net::GetIns().IsHost()) { return; }

	// 攻撃の当たり判定
	switch (other.GetTag()) {

	case COLLIDER_TAG::ENEMY:
	case COLLIDER_TAG::BOSS_DISTANCE:
		LocalShotEnd();
		break;
	default:break;
	}
}

void GrapePlayerShotCollOperator::CollOn(void) {
	SetJudge(true);
	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerShot));
	}
}

void GrapePlayerShotCollOperator::CollOff(void) {
	SetJudge(false);
	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerShot));
	}
}

void GrapePlayerShotCollOperator::RemoteShotStart(const Vector3& pos, const Vector3& vec) {
	trans.pos = pos;
	moveVec = vec;
	SetIsDraw(true);
	lifeCounter = LIFE_TIME;
	isAlive = true;
}

void GrapePlayerShotCollOperator::RemoteShotEnd(const Vector3& pos) {
	lifeCounter = 0;
	CollOff();
	isAlive = false;
	EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_SMALL, pos);
	SetIsDraw(false);
}

void GrapePlayerShotCollOperator::LocalShotStart(const Vector3& pos, const Vector3& vec)
{
	RemoteShotStart(pos, vec);

	Net::GetIns().Send(MsgDataPlayerShotStart(pos, vec), operatorSenderId);
}

void GrapePlayerShotCollOperator::LocalShotEnd()
{
	RemoteShotEnd(trans.pos);

	Net::GetIns().Send(MsgDataPlayerShotEnd(trans.pos), operatorSenderId);
}
