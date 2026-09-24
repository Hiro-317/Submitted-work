#pragma once

#include "../../../../../../ActorBase.h"

#include "../../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../../Common/Collider/XZCircleCollider.h"


class BananaBossBanadrill : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	BananaBossBanadrill(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats
	);
	~BananaBossBanadrill()override = default;

	void Load(void)override;

	void CollSet(bool flg) { SetJudge(flg); }
	void Off(void) { isDrawArea = false; SetJudge(false); }

	void SetScale(const float& scale) { collFront.scale = Vector3(SCALE * scale, 1.0f, SCALE * scale); }

	void SetBanameranPos(const Vector3& pos) { banadrill.pos = pos; }
	void SetBanameranRot(const Vector3& rot) { banadrill.angle = rot; }

	// 攻撃範囲の描画設定
	void SetDrawArea(bool flg) { isDrawArea = flg; }

	bool GetIsGround(void) { return isGround; }

	void Update(void) override;
	void AlphaDraw(void) override;

private:

	// バナメランのサイズ
	const float SIZE = 3.0f;

	// コライダーの半径
	const float RADIUS = 8.0f * SIZE;
	const float ATTACK_RADIUS = RADIUS + 25.0f;

	// コライダー描画の拡大率
	const float SCALE = ATTACK_RADIUS / 100.0f;

	// コライダー描画の座標
	const float HEIGHT = 37.0f;
	const float ATTACK_HEIGHT = 1000.0f;

	// 攻撃持続時間
	const int ATTACK_DURATION = 30;

#pragma region 受け取る参照変数

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& stats;

#pragma endregion

	Transform collBack;
	Transform collFront;

	Transform banadrill;

	// 攻撃範囲描画フラグ
	bool isDrawArea;

	// 攻撃時間
	int cnt;
};