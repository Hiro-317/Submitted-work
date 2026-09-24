#pragma once

#include "../../../../../ActorBase.h"

struct CharacterStats;

class GrapeKickDownCollOperator : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	GrapeKickDownCollOperator(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats
	);
	~GrapeKickDownCollOperator()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }

	void SetPos(const Vector3& pos) { trans.pos = pos; }
	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE.x * scale, 1.0f, SCALE.z * scale); }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// 半径
	const float RADIUS = 300.0f;

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