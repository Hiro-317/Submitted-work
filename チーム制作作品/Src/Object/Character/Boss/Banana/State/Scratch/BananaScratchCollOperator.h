#pragma once

#include "../../../../../ActorBase.h"

#include "../../../../../Common/Collider/CapsuleCollider.h"


class BananaScratchCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	BananaScratchCollOperator(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats,
		const Vector3& pos,
		const Vector3& StartPos,
		const Vector3& EndPos
	);
	~BananaScratchCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }
	void Off(void) { isDrawArea = false; SetJudge(false); }

	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE.x * scale, 1.0f, SCALE.z * scale); }

	// 攻撃フレームの更新
	void SetColliderFrame(void) { 
		auto coll = dynamic_cast<CapsuleCollider*>(ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back());
		coll->SetStartPos(StartPos); coll->SetEndPos(EndPos);
	}

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// コライダー描画の高さ
	const float HEIGHT = 37.0f;

	// コライダーの半径
	const float RADIUS = 150.0f;

	// コライダー描画の拡大率
	const Vector3 SCALE = Vector3(4.75f, 1.0f, 3.0f);

	// コライダーの誤差
	const Vector3 DIFF = Vector3(-160.0f, 0.0f, -875.0f);

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

	const Vector3& StartPos;

	const Vector3& EndPos;

	const Vector3& pos;

#pragma endregion

	void SubAlphaDraw(void) override;

	Transform collBack;
	Transform collFront;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};