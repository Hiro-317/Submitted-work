#pragma once

#include "../../../../CharacterStateBase.h"

#include "BananaBossBanamerang.h"
#include "../../../../../../Common/Vector3.h"

class BananaBossBoomerangState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="collOperator">攻撃オペレーターの参照</param>
	/// <param name="PlayAttackAnim">攻撃アニメーションの再生</param>
	/// <param name="GetAnimPlayRatio">攻撃アニメーションの再生割合取得</param>
	/// <param name="IsAnimeEnd">アニメーションが終了しているか</param>
	/// <param name="DefaultChangeState">攻撃後遷移ステート</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	BananaBossBoomerangState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		BananaBossBanamerang* collOperator,
		const std::function<Vector3(void)> HandPos,
		const std::function<void(void)> PlayStartAnim,
		const std::function<void(void)> PlayLoopAnim,
		const std::function<void(void)> PlayAttackAnim,
		const std::function<void(void)> PlayIdleAnim,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~BananaBossBoomerangState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 再生割合
	static constexpr float ATTACK_RATE = 0.35f;
	static constexpr float EFFECT_RATE = 0.32f;

	// バナメランの開始座標
	const Vector3 POS = Vector3(-575.0f, 100.0f, 1325.0f);
	const float DIFF = POS.z / 2.0f;

	// 攻撃の溜め時間
	const float START_TIME = 60.0f;

	// 攻撃をやめる時間
	const float END_TIME = 180.0f;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// コリジョンオペレーターのポインタ
	BananaBossBanamerang* collOperator;

	// 手の位置
	const std::function<Vector3(void)> HandPos;

	// 溜めアニメーションの再生
	const std::function<void(void)> PlayStartAnim;
	const std::function<void(void)> PlayLoopAnim;

	// 攻撃アニメーションの再生
	const std::function<void(void)> PlayAttackAnim;

	// 攻撃後アニメーションの再生
	const std::function<void(void)> PlayIdleAnim;

	// アニメーションが終わっているかどうか
	const std::function<bool(void)> IsAnimeEnd;

	// ステートを抜けたあとのステート
	const std::function<void(void)> DefaultChangeState;

	// クールタイムの設定
	const std::function<void(void)> SetCoolTime;

#pragma endregion

	// 一度だけ通すように
	bool first;
	bool startAnim;

	// 攻撃のカウント
	float cnt;

};