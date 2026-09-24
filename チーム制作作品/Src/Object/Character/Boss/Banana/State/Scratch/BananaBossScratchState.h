#pragma once

#include "../../../../CharacterStateBase.h"

#include "BananaScratchCollOperator.h"
#include "../../../../../../Common/Vector3.h"

class BananaBossScratchState : public CharacterStateBase
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
	BananaBossScratchState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		BananaScratchCollOperator* collOperator,
		const std::function<void(void)> UpdateFrame,
		const std::function<void(void)> PlayAttackAnim,
		const std::function<float(void)> GetAnimPlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~BananaBossScratchState()override = default;

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

	static constexpr float ATTACK_RATE = 0.5f;
	static constexpr float END_RATE = 0.8f;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// コリジョンオペレーターのポインタ
	BananaScratchCollOperator* collOperator;

	// 攻撃フレームの更新
	const std::function<void(void)> UpdateFrame;

	// アタックアニメーションの再生
	const std::function<void(void)> PlayAttackAnim;

	// アニメーションの再生割合取得
	const std::function<float(void)> GetAnimPlayRatio;

	// アニメーションが終わっているかどうか
	const std::function<bool(void)> IsAnimeEnd;

	// ステートを抜けたあとのステート
	const std::function<void(void)> DefaultChangeState;

	// クールタイムの設定
	const std::function<void(void)> SetCoolTime;

#pragma endregion

	// 一度だけ通すように
	bool first;
};