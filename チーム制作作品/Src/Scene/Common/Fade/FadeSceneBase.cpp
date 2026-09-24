#include "FadeSceneBase.h"

#include <algorithm>

FadeSceneBase::FadeSceneBase(unsigned short FADE_TIME) : SceneBase(),
	fadeTime((std::max)(static_cast<unsigned short>(1), FADE_TIME)),
	fadeCounter(0),
	state(STATE::FADE_OUT)
{
}

void FadeSceneBase::StartFadeIn(void)
{
	if (state != STATE::WAIT) { return; }

	fadeCounter = fadeTime;
	state = STATE::FADE_IN;
}

float FadeSceneBase::GetFadeRatio(void)const
{
	if (state == STATE::WAIT) { return 1.0f; }
	if (state == STATE::END) { return 0.0f; }

	return static_cast<float>(fadeCounter) / static_cast<float>(fadeTime);
}

void FadeSceneBase::SubPostUpdate(void)
{
	SubFadeUpdate();

	switch (state)
	{
	case STATE::FADE_OUT:
		if (++fadeCounter >= fadeTime)
		{
			fadeCounter = fadeTime;
			state = STATE::WAIT;
		}
		break;

	case STATE::WAIT:
		break;

	case STATE::FADE_IN:
		if (fadeCounter > 0) { fadeCounter--; }
		if (fadeCounter == 0) { state = STATE::END; }
		break;

	case STATE::END:
	default:
		break;
	}
}

void FadeSceneBase::SubUiDraw(void)
{
	SubFadeDraw(GetFadeRatio());
}
