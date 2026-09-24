#pragma once

#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../CharacterStateBase.h"

#include "../../../../Common/Transform.h"

class OtherPlayerWatchState : public CharacterStateBase
{
public:
	OtherPlayerWatchState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		const std::vector<const Transform*>& playerTrans,
		const Vector3*& bossPos
	);

	~OtherPlayerWatchState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:
	//const int PLAYER_MAX = Net::GetIns().GetConnectStatus().EntryCount();

	// 座標の参照
	const std::vector<const Transform*>& playerTrans;

	const Vector3*& bossPos;
};

