#pragma once

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/KeyManager.h"

class PlayerDodgeState : public CharacterStateBase
{
public:
	PlayerDodgeState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		KEY_TYPE DODGE_KEY,
		int COOL_TIME, float DODGE_MOVE_SPEED, 
		float INVI_START_TIME, float INVI_END_TIME,
		Vector3& pos, Vector3& angle,
		const std::function<void(void)> PlayDodgeAnime,
		const std::function<float(void)> GetAnimePlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(unsigned char)> SetInviCounter,
		const std::function<void(void)> DefaultChangeState
	);
	~PlayerDodgeState()override = default;
	void OwnStateConditionUpdate(void)override;
	void Enter(void)override;
	void Update(void)override;
	void Exit(void)override;
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 回避に使用するキー
	const KEY_TYPE DODGE_KEY;

	// 回避のクールタイム
	const int COOL_TIME;

	// 回避の移動速度
	const float DODGE_MOVE_SPEED;

	// 回避の無敵時間（アニメーションの再生割合）
	const float INVI_START_TIME;

	// 回避の無敵時間終了（アニメーションの再生割合）
	const float INVI_END_TIME;
#pragma	endregion 

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayDodgeAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

	// 無敵カウンターをセットする関数のポインタ
	const std::function<void(unsigned char)> SetInviCounter;
#pragma endregion

};