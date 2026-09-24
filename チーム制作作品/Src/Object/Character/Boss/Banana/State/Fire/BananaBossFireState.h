#pragma once

#include "../../../../CharacterStateBase.h"

#include "BananaFireCollOperator.h"
#include "../../../../../../Common/Vector3.h"

class BananaBossFireState : public CharacterStateBase
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
	BananaBossFireState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		BananaFireCollOperator* collOperator,
		const std::function<void(void)> PlayAttackAnim,
		const std::function<float(void)> GetAnimPlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~BananaBossFireState()override = default;

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

	// エフェクトの再生位置
	const Vector3 EFFECT_POS = Vector3(0.0f, 280.0f, 1325.0f);

#pragma endregion

#pragma region 受け取る参照変数・関数

	// コリジョンオペレーターのポインタ
	BananaFireCollOperator* collOperator;

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
	bool effect;

	// 攻撃持続のカウント
	int cnt;

};