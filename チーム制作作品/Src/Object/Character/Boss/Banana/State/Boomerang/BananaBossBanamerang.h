#pragma once

#include "../../../../../ActorBase.h"

#include "../../../../../Common/Collider/XZCircleCollider.h"


class BananaBossBanamerang : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	BananaBossBanamerang(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats
	);
	~BananaBossBanamerang()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }
	void Off(void) { isDrawArea = false; SetJudge(false); }

	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE.x * scale, 1.0f, SCALE.z * scale); }

	void SetBanameranPos(const Vector3& pos) { banamerang.pos = pos; }
	void SetBanameranRot(const Vector3& rot) { banamerang.angle = rot; }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

private:

	// バナメランのサイズ
	const float SIZE = 8.75f;

	// コライダーの半径
	const float RADIUS = 575.0f;

	// コライダー描画の拡大率
	const Vector3 SCALE = Vector3(11.5f, 1.0f, 12.2f);

	// コライダー描画の座標
	const Vector3 POS = Vector3(0.0f, 37.0f, 1325.0f);

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

#pragma endregion

	void SubAlphaDraw(void) override;

	Transform collBack;
	Transform collFront;

	Transform banamerang;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};