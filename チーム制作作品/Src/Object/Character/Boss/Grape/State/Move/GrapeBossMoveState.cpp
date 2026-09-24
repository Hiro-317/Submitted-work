#include "GrapeBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossMoveState::GrapeBossMoveState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED,
	Vector3& pos, Vector3& angle, const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayRunAnime,
	const std::function<void(void)> KickDownChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED),
	pos(pos), angle(angle), playerPos(playerPos),
	GetTarget(GetTarget),
	PlayRunAnime(PlayRunAnime),
	KickDownChangeState(KickDownChangeState)
{
	
}

void GrapeBossMoveState::Enter(void)
{
	target = GetTarget();
	PlayRunAnime();
}

void GrapeBossMoveState::Update(void)
{
	// 回転の更新
	moveDir = (*playerPos.at(target) - pos).Normalized();
	angle.y = atan2f(moveDir.x, moveDir.z);

	// 位置の更新
	pos += moveDir * MOVE_SPEED;

	if ((*playerPos.at(target) - pos).Length() <= 450.0f) {
		KickDownChangeState();
	}
}

void GrapeBossMoveState::Exit(void)
{
}

void GrapeBossMoveState::AlwaysUpdate(void)
{
}
