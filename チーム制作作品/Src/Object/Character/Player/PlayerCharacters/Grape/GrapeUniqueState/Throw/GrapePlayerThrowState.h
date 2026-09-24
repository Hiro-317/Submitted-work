#pragma once
#include "../../../../../CharacterStateBase.h"

#include "GrapePlayerThrowCollOperator.h"

class GrapePlayerThrowState : public CharacterStateBase
{
public:

	// コンストラクタ
	GrapePlayerThrowState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		GrapePlayerThrowCollOperator& collOperator,
		const int COOL_TIME,
		Vector3& pos, Vector3& angle,
		const Vector3*& bossPos,
		const std::function<void(void)> PlayAnime,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<float(void)> AnimeRatio,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~GrapePlayerThrowState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	void Update(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:
#pragma region 定数定義

	// クールタイム
	const int COOL_TIME;

#pragma endregion 

#pragma region 変数定義

	// 座標の参照
	Vector3& pos;

	Vector3& angle;

	const Vector3*& bossPos;

	GrapePlayerThrowCollOperator& collOperator;

	const std::function<void(void)> PlayAnime;
	const std::function<bool(void)> IsAnimeEnd;
	const std::function<float(void)> AnimeRatio;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion 

	Vector3 moveVec;

	bool isThrow;
};