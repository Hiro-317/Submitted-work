#include "GrapeBossDeathState.h"

#include "../../../../../../Scene/Game/GameScene.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"


GrapeBossDeathState::GrapeBossDeathState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::function<void(void)> DeathAnim,
	const std::function<float(void)> AnimeTotalTime,
	const std::function<bool(void)> IsAnimEnd,
	const std::function<void(void)> IsDeath
)
	:CharacterStateBase(ownChangeState, isOwnState),
	DeathAnim(DeathAnim), AnimeTotalTime(AnimeTotalTime),
	IsAnimEnd(IsAnimEnd), IsDeath(IsDeath)
{
}

void GrapeBossDeathState::Enter(void)
{
	DeathAnim();
	changeTime = ((int)AnimeTotalTime() + DEATH_LAST_TIME);
	GameScene::Shake(ShakeKinds::DIAG, ShakeSize::BIG, changeTime);
	cnt = 0;
}

void GrapeBossDeathState::Update(void)
{
	// アニメーションが終わっていたらシーン移行
	if (cnt >= changeTime) {
		Net::GetIns().Send(MsgDataBossIsDeath(true));
		IsDeath();
	}
	// チェンジ用のカウント
	cnt++;
}

void GrapeBossDeathState::Exit(void)
{
}

void GrapeBossDeathState::AlwaysUpdate(void)
{
}
