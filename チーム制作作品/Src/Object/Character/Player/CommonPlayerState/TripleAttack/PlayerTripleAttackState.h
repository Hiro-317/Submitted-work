#pragma once

#include <array>

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/KeyManager.h"

#include "PlayerTripleAttackCollOperator.h"

#include "PlayerTripleAttackStDefine.h"

class PlayerTripleAttackState : public CharacterStateBase
{
public:

	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移させる関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="stateContext">コンテキスト構造体</param>
	PlayerTripleAttackState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,

		PlayerTripleAttackStateContext& stateContext
	);
	~PlayerTripleAttackState()override = default;

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

	// 攻撃に使うキー
	KEY_TYPE ATTACK_KEY;

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃が次段に繋がる猶予時間
	const int ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_END_TIME;

	// 攻撃中の移動速度
	const float ATTACK_MOVE_SPEED;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerTripleAttackCollOperator& collOperator;

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	const std::array<std::function<void(void)>, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> PlayAttackAnimes;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion

	// 現状の攻撃の段階
	PLAYER_TRIPLE_ATTACK_STAGE attackStage;


	// 攻撃が次段に繋がるかの猶予時間のカウンター
	int attackNextStageContinueTimeCounter;
};