#pragma once
#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/KeyManager.h"

#include "PlayerSingleModifierCollOperator.h"

class PlayerSingleModifierState : public CharacterStateBase
{
public:

	PlayerSingleModifierState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		PlayerSingleModifierCollOperator& collOperator,
		float COLL_START_TIME, KEY_TYPE HEAL_KEY,
		int COOL_TIME,
		const std::function<void(void)> PlayHealAnime,
		const std::function<float(void)> GetAnimePlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState
	);
	~PlayerSingleModifierState() = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

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

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const float COLL_START_TIME;

	// 遷移するためのキー
	const KEY_TYPE HEAL_KEY;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerSingleModifierCollOperator& collOperator;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayHealAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;
	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion

	bool isModifierEnd;
};