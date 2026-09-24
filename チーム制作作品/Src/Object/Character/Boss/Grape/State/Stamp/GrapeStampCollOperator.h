#pragma once

#include "../../../../../ActorBase.h"

#include "../../../../../Common/Collider/ColliderBase.h"

struct CharacterStats;

class GrapeStampCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	GrapeStampCollOperator(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats
	);
	~GrapeStampCollOperator()override = default;

	void Load(void)override;
	Vector3 GetAttackPos(void) { return trans.pos; }

	void SetPos(const Vector3& pos) { trans.pos = pos; }
	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE.x * scale, 1.0f, SCALE.z * scale); }

	// 攻撃範囲の描画設定
	void DrawAreaOn(void) { isDrawArea = true; ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetJudgeFlg(true); }
	void CollOn(void) { ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(true); }
	void Off(void) { SetJudge(false); isDrawArea = false; }

private:

	// 半径
	const float RADIUS = 500.0f;

	// コライダー描画の高さ
	const float HEIGHT = 37.0f;

	// コライダー描画の拡大率
	const Vector3 SCALE = Vector3(RADIUS / 100.0f, 1.0f, RADIUS / 100.0f);

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

#pragma endregion

	void SubUpdate(void) override;
	void SubAlphaDraw(void) override;
	void SubRelease(void) override;

	Transform collBack;
	Transform collFront;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};