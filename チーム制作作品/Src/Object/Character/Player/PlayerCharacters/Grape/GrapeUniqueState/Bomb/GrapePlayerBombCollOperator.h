#pragma once
#include "../../../../../../ActorBase.h"
#include "../../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../../Manager/Effect/EffectManager.h"

#include "../../../../../../Common/Collider/ColliderBase.h"

struct CharacterStats;

class GrapePlayerBombCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_TAG">コライダーに着けるタグ</param>
	/// <param name="SKILL_2_ATTACK_RATE_PERCENT">攻撃倍率</param>
	/// <param name="playerPos">プレイヤー座標</param>
	/// <param name="playerAngle">プレイヤーアングルram>
	/// <param name="operatorSenderId"></param>
	/// <param name="playerStats">プレイヤーのステータス情報</param>
	GrapePlayerBombCollOperator(
		COLLIDER_TAG COLL_TAG,
		short ATTACK_RATE_PERCENT,
		float ATTACK_COUNT_TIME,
		float ATTACK_START_TIME,
		const Vector3& playerPos, const Vector3& playerAngle,
		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);
	~GrapePlayerBombCollOperator()override = default;

	// ロード
	void Load(void)override;
	// 更新処理
	void Update(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 攻撃の判定を発生させる
	void CollOn(void) {
		if (!isBlast) ColliderSerch(GetCollider(), COLL_TAG).back()->SetJudgeFlg(true);
		SetIsEnemySerch(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerBomb));
		}
	}

	// 攻撃の判定を消す
	void CollOff(void) {
		ColliderSerch(GetCollider(), COLL_TAG).back()->SetJudgeFlg(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerBomb));
		}
	}

	// 敵を探す
	void SetIsEnemySerch(bool isJudge) {
		if (isBlast) { return; }
		ColliderSerch(GetCollider(), COLLIDER_TAG::PLAYER_COMMON).back()->SetJudgeFlg(isJudge);
	}

	void PlayEffect(void) { EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_BIG, trans.pos); }

	// 攻撃対象を見つけたかどうか
	const bool GetIsAttackTargetFind(void) { return isAttackTargetFind; }

	// ボム設置時に呼び出す処理
	void RemoteBombSetStart(const Vector3& pos) {
		// すべて初期化
		CollOff();
		SetIsDraw(true);
		SetIsEnemySerch(false);

		trans.pos = pos;
		isBlast = false;
		isAttackTargetFind = false;
		timeCounter = ATTACK_COUNT_TIME;

		isInit = true;
	}

	void RemoteBombSetEnd(void) {
		SetIsDraw(false);
		PlayEffect();
		isBlast = true;
	}

	void LocalBombSetStart(const Vector3& pos) {

		RemoteBombSetStart(pos);

		Net::GetIns().Send(MsgDataGrapePlayerBombStart(pos), operatorSenderId);
	}

	void LocalBombSetEnd(void) {

		RemoteBombSetEnd();

		Net::GetIns().Send(MsgDataGrapePlayerBombEnd(), operatorSenderId);
	}
private:

#pragma region 定数

	// 三段攻撃の攻撃倍率
	const short ATTACK_RATE_PERCENT;

	// 攻撃の段階ごとのタグ
	COLLIDER_TAG COLL_TAG;

	// 爆発するまでのカウント時間
	const float ATTACK_COUNT_TIME;

	// 爆弾待機時間
	const float ATTACK_START_TIME;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーの座標
	const Vector3& playerPos;
	// プレイヤーの向き
	const Vector3& playerAngle;

	const MSG_SENDER_ID operatorSenderId;

	// プレイヤーのステータス
	const CharacterStats& playerStats;
#pragma endregion

	// 攻撃のヒット管理のフラグ
	bool isBlast;

	bool isAttackTargetFind;

	bool isInit;

	// 終了までのカウント用
	short timeCounter;


};