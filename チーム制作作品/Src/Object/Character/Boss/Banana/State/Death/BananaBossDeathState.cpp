#include "BananaBossDeathState.h"

#include "../../../../../../Scene/Game/GameScene.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"


BananaBossDeathState::BananaBossDeathState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::function<void(void)>& PlayDeath,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(ownChangeState, isOwnState),
	PlayDeath(PlayDeath),
	IsDeath(IsDeath)
{
}

void BananaBossDeathState::Enter(void)
{
	GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, CHANGE_COUNT);
	PlayDeath();
}

void BananaBossDeathState::Update(void)
{
	// アニメーションが終わっていたらシーン移行
	if (cnt >= CHANGE_COUNT) {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
	// チェンジ用のカウント
	cnt++;
}

void BananaBossDeathState::Exit(void)
{
}

void BananaBossDeathState::AlwaysUpdate(void)
{
}
