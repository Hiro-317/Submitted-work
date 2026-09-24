#pragma once

#include "../../../../../CharacterStateBase.h"

class TomatoPlayerDamageState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own
	/// ">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="INVI_TIME">無敵時間</param>
	/// <param name="PlayDamageAnime">アニメーション再生関数</param>
	/// <param name="IsAnimeEnd">アニメーションが終了したかどうかを返す関数</param>
	/// <param name="DefaultChangeState">次の状態に遷移する関数</param>
	TomatoPlayerDamageState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos,
		const std::function<void(void)> InviStart,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~TomatoPlayerDamageState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;


private:

#pragma region 変数
	// プレイヤーの座標
	Vector3& angle;
#pragma endregion

#pragma region 関数ポインタ
	// 無敵時間のセット関数
	const std::function<void(void)> InviStart;

	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion

};
