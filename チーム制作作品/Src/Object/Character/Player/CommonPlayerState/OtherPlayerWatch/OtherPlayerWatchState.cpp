#include "OtherPlayerWatchState.h"

#include "../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../../Manager/Camera/MultifuncCamera/MultifuncCamera.h"

#include "../../../../../Manager/Input/KeyManager.h"


OtherPlayerWatchState::OtherPlayerWatchState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState, 
	const std::vector<const Transform*>& playerTrans,
	const Vector3*& bossPos
) :
	CharacterStateBase(ownChangeState,isOwnState),
	playerTrans(playerTrans),
	bossPos(bossPos)
{
}

void OtherPlayerWatchState::Enter(void)
{
	dynamic_cast<MultifuncCamera*>(&CurrentCamera::Get())->ChangeModeFollowAuto(&playerTrans.front()->pos, bossPos);
}

void OtherPlayerWatchState::Update(void)
{

}

void OtherPlayerWatchState::Exit(void)
{
}
