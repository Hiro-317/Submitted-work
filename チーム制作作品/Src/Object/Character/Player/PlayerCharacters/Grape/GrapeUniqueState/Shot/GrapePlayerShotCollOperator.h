#pragma once
#include "../../../../../../ActorBase.h"

struct CharacterStats;

class GrapePlayerShotCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_TAG">コライダーのタグ</param>
	/// <param name="ATTACK_RATE_PERCENT">ショットのダメージ倍率</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerAngle">プレイヤーの角度</param>
	/// <param name="operatorSenderId">送信ID</param>
	/// <param name="playerStats">プレイヤーのステータス</param>
	GrapePlayerShotCollOperator(
		COLLIDER_TAG COLL_TAG,
		const short ATTACK_RATE_PERCENT,
		const Vector3& playerPos, const Vector3& playerAngle,
		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);

	// デストラクタ
	~GrapePlayerShotCollOperator()override = default;

	// ロード
	void Load(void)override;
	// 更新処理
	void Update(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 攻撃の判定を発生させる
	void CollOn(void);
	// 攻撃の判定を消す
	void CollOff(void);

	// 受信処理時、弾を発射するときに呼び出す処理
	void RemoteShotStart(const Vector3& pos, const Vector3& vec);
	// 受信処理時、弾が爆散したときに呼び出す処理
	void RemoteShotEnd(const Vector3& pos);

	// 弾を発射するときに呼び出す処理
	void LocalShotStart(const Vector3& pos, const Vector3& vec);
private:

#pragma region 定数

	// 攻撃倍率
	const short ATTACK_RATE_PERCENT;

	// 攻撃の段階ごとのタグ
	COLLIDER_TAG COLL_TAG;

	const short LIFE_TIME = 40;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーの座標
	const Vector3& playerPos;
	// プレイヤーの向き
	const Vector3& playerAngle;

	// 送信ID
	const MSG_SENDER_ID operatorSenderId;

	// プレイヤーのステータス
	const CharacterStats& playerStats;
#pragma endregion

	// 投げる対象（ターゲット）
	Vector3 moveVec;

	// 弾の生存時間
	short lifeCounter;

	// 弾が生存しているかどうか
	bool isAlive;

	// 弾が爆散したときに呼び出す処理
	void LocalShotEnd();
};


