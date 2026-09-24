#pragma once

#include <array>
#include <functional>

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/KeyManager.h"

#include "../../../../Common/Collider/ColliderBase.h"

class PlayerTripleAttackCollOperator;

// 攻撃の段階
enum class	PLAYER_TRIPLE_ATTACK_STAGE
{
	NON = -1,

	FIRST,
	SECOND,
	THIRD,

	MAX
};

// 三弾攻撃のステート用の定数をまとめて渡すための構造体
struct PlayerTripleAttackStateContext
{
	// 攻撃に使うキー
	KEY_TYPE ATTACK_KEY;

	// 攻撃そのもののクールタイム
	int COOL_TIME;

	// 攻撃が次段に繋がる猶予時間
	int ATTACK_NEXT_STAGE_CONTINUE_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_END_TIME;

	// 攻撃中の移動速度
	float ATTACK_MOVE_SPEED;

	// 当たり判定のオペレーターの参照
	PlayerTripleAttackCollOperator* collOperator;

	// 座標の参照
	Vector3* pos;
	// 角度の参照
	Vector3* angle;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	std::array<std::function<void(void)>, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> PlayAttackAnimes;
	// アニメーションの再生割合を取得する関数のポインタ
	std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	std::function<bool(void)> IsAnimeEnd;

	// 攻撃終了後の状態遷移関数のポインタ
	std::function<void(void)> DefaultChangeState;
};