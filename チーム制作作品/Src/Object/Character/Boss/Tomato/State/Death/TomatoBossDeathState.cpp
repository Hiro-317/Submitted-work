#include "TomatoBossDeathState.h"

#include "../../../../../../Scene/Game/GameScene.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"


TomatoBossDeathState::TomatoBossDeathState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& scale, Vector3& angle,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(ownChangeState, isOwnState),
	scale(scale), angle(angle), defaultScale(defaultScale),
	IsDeath(IsDeath)
{
}

void TomatoBossDeathState::Enter(void)
{
	GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, CHANGE_COUNT);
}

void TomatoBossDeathState::Update(void)
{
	// âÒì]ÇµÇ»Ç™ÇÁè¨Ç≥Ç≠Ç≥ÇπÇÈ
	if (cnt <= CHANGE_COUNT) {

		cnt++;
		angle.y += ROTATION_POW;
		scale *= SCALE_POW;
	}
	else {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
}

void TomatoBossDeathState::Exit(void)
{
}

void TomatoBossDeathState::AlwaysUpdate(void)
{
}
