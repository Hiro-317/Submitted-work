#include "TomatoPlayerDeathState.h"

TomatoPlayerDeathState::TomatoPlayerDeathState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& pos, Vector3& angle,
	const std::function<void(void)> PlayerDeathSetting,
	const std::function<void(void)> PlayerIsDeathTrue,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	PlayerIsDeathTrue(PlayerIsDeathTrue),
	PlayerDeathSetting(PlayerDeathSetting),
	DefaultChangeState(DefaultChangeState),
	animeCounter(0.0f)
{
}

void TomatoPlayerDeathState::Enter(void)
{
	animeCounter = 30.0f;
	angle.x = 60.0f;
}

void TomatoPlayerDeathState::Update(void)
{
	pos.y += animeCounter;
	animeCounter -= 0.2f;
	angle.y -= 0.4f;

	if (animeCounter <= 0.0f) {
		PlayerDeathSetting();
		DefaultChangeState();
		animeCounter = 0.0f;
	}
}

void TomatoPlayerDeathState::Exit(void)
{
	animeCounter = 0.0f;
	PlayerIsDeathTrue();
}
