#pragma once

#include "../../../../../ActorBase.h"

#include "../../../../../Common/Collider/SphereCollider.h"


class BananaFireCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	BananaFireCollOperator(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats
	);
	~BananaFireCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }
	void Off(void) { isDrawArea = false; SetJudge(false); }

	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE * scale, 1.0f, SCALE * scale); }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// コライダーの半径
	const float RADIUS = 500.0f;

	// コライダー描画の拡大率
	const float SCALE = RADIUS / 100.0f;

	// コライダーの誤差
	const Vector3 DIFF = Vector3(-160.0f, 0.0f, -875.0f);

	// コライダーの座標
	const Vector3 POS = Vector3(0.0f, 37.0f, 500.0f);

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

#pragma endregion

	void SubAlphaDraw(void) override;

	Transform collBack;
	Transform collFront;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};