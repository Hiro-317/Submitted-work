#include "GrapePlayerBombCollOperator.h"

#include "../../../../../../Common/Collider/SphereCollider.h"

GrapePlayerBombCollOperator::GrapePlayerBombCollOperator(
	COLLIDER_TAG COLL_TAG,
	short ATTACK_RATE_PERCENT,
	float ATTACK_COUNT_TIME,
	float ATTACK_START_TIME,
	const Vector3& playerPos, const Vector3& playerAngle,
	MSG_SENDER_ID operatorSenderId,
	const CharacterStats& playerStats
) :
	ActorBase(),
	COLL_TAG(COLL_TAG),
	ATTACK_RATE_PERCENT(ATTACK_RATE_PERCENT),
	ATTACK_COUNT_TIME(ATTACK_COUNT_TIME),
	ATTACK_START_TIME(ATTACK_START_TIME),
	playerPos(playerPos), playerAngle(playerAngle),
	operatorSenderId(operatorSenderId),
	playerStats(playerStats),
	isBlast(false),
	isAttackTargetFind(false),
	isInit(false),
	timeCounter(0)
{
}

void GrapePlayerBombCollOperator::Load(void)
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
	ColliderCreate(new SphereCollider(COLL_TAG, 300.0f));
	ColliderCreate(new SphereCollider(COLLIDER_TAG::PLAYER_COMMON, 100.0f));

	// 初期化処理
	SetJudge(false);
	isBlast = false;
	isAttackTargetFind = false;
	isInit = false;

	// 最初は描画しない
	SetIsDraw(false);

	// スキルのダメージ量の設定
	CreateAttackSkill(operatorSenderId, ATTACK_RATE_PERCENT, &playerStats, COLL_TAG);

	trans.Load("Character/Grape/Bomb");
	trans.scale = 0.2f;
}

void GrapePlayerBombCollOperator::Update(void)
{
	if (!isInit) { return; }

	// もし爆発していたら早期リターン
	if (isBlast) {
		CollOff();
		return;
	}

	// タイマースタート
	if (timeCounter > 0) {
		timeCounter--;
		SetIsDraw(true);
	}

	if (timeCounter < (ATTACK_COUNT_TIME - ATTACK_START_TIME)) {
		// 敵を探し始める
		SetIsEnemySerch(true);
	}

	// タイマーが終了するか、爆弾の範囲内に敵が入ったら爆発する
	if (timeCounter <= 0 || GetIsAttackTargetFind()) {
		CollOn();
		LocalBombSetEnd();
	}
}

void GrapePlayerBombCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (!Net::GetIns().IsHost()) { return; }

	if (ownTag == COLLIDER_TAG::PLAYER_COMMON) {
		// 攻撃の当たり判定
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:
			isAttackTargetFind = true;
			ColliderSerch(GetCollider(), COLLIDER_TAG::PLAYER_COMMON).back()->SetJudgeFlg(false);
			break;
		default:break;
		}
	}
	else {
		// 攻撃の当たり判定
		switch (other.GetTag())
		{
		case COLLIDER_TAG::BOSS:
		case COLLIDER_TAG::ENEMY:
		case COLLIDER_TAG::BOSS_DISTANCE:
			isBlast = true;
			LocalBombSetEnd();
			break;
		default:break;
		}
	}
}
