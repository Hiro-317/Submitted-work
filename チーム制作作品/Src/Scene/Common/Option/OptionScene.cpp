#include "OptionScene.h"

#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Sound/SoundManager.h"

#include "../../SceneManager/SceneManager.h"

OptionScene::OptionScene() :
	SceneBase(),

	nowSelect(SELECT::None),

	frameImage(),
	volumeNameImage(),
	volumeGaugeMaxImage(),
	volumeGaugeMinImage(),
	seekButtonImage(),
	plusButtonImage(),
	minusButtonImage(),
	exitImage(),

	isPushPlusButton(false),
	isPushMinusButton(false),

	exitEasingCounter(0.0f),
	exitEasingRate(0.0f)
{
}

void OptionScene::SubPostLoad(void)
{
#pragma region äeâÊëúÇÃì«Ç›çûÇ›

	// âÊëúì«Ç›çûÇ›ÇÃÉâÉÄÉ_ä÷êî
	auto loadOptionImage = [&](std::string name)->int { return LoadGraph((OPTION_RESORCE_PATH + name).c_str()); };

	// òg
	frameImage = loadOptionImage(FRAME_IMAGE_PATH.c_str());

	// É}ÉXÉ^Å[âπó  / BGMâπó  / SEâπó 
	for (int i = 0; i < (int)SELECT::Max - 1; i++) { volumeNameImage[i] = loadOptionImage(VOLUME_NAME_PATH[i].c_str()); }

	// ÉQÅ[ÉWÅiÉ}ÉbÉNÉXèÛë‘Åj
	volumeGaugeMaxImage = loadOptionImage(VOLUME_GAUGE_MAX_IMAGE_PATH.c_str());
	// ÉQÅ[ÉWÅiÉ[ÉçèÛë‘Åj
	volumeGaugeMinImage = loadOptionImage(VOLUME_GAUGE_MIN_IMAGE_PATH.c_str());

	// ÉQÅ[ÉWÇÃè„Ç…èoÇ∑É{É^Éì
	seekButtonImage[(int)true] = loadOptionImage((PUSH_BUTTON_IMAGE_ADD_STRING + SEEK_BUTTON_IMAGE_PATH).c_str());
	seekButtonImage[(int)false] = loadOptionImage(SEEK_BUTTON_IMAGE_PATH.c_str());

	// ÉvÉâÉXÉ{É^Éì
	plusButtonImage[(int)true] = loadOptionImage((PUSH_BUTTON_IMAGE_ADD_STRING + PLUS_BUTTON_IMAGE_PATH).c_str());
	plusButtonImage[(int)false] = loadOptionImage(PLUS_BUTTON_IMAGE_PATH.c_str());
	// É}ÉCÉiÉXÉ{É^Éì
	minusButtonImage[(int)true] = loadOptionImage((PUSH_BUTTON_IMAGE_ADD_STRING + MINUS_BUTTON_IMAGE_PATH).c_str());
	minusButtonImage[(int)false] = loadOptionImage(MINUS_BUTTON_IMAGE_PATH.c_str());

	// Ç∆Ç∂ÇÈÉ{É^Éì
	exitImage = loadOptionImage(EXIT_IMAGE_PATH.c_str());

#pragma endregion

}

void OptionScene::SubPostInit(void)
{
	// åªç›ÇÃëIëèÛë‘Çèâä˙âª
	nowSelect = (SELECT)((int)SELECT::None + 1);
}

void OptionScene::SubPostUpdate(void)
{
	isPushMinusButton = false;
	isPushPlusButton = false;

#pragma region ëÄçÏ
	// ÉQÅ[ÉÄèIóπèàóùÅ`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// å¯â âπ
		Snd::GetIns().Play("SystemSelect");

		// é©êgÇÃÉVÅ[ÉìÇîjä¸Ç∑ÇÈ
		SceneManager::GetIns().PopScene();

		// à»ç~ÇÕthisÇ™nullptrÇ∆Ç»Ç¡ÇƒÇ¢ÇÈÇΩÇﬂèIóπ
		return;
	}
	// Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`ÉQÅ[ÉÄèIóπèàóù

	// ëÄçÏÇ…ÇÊÇ¡ëIëéàÇïœÇ¶ÇÈÅ`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {

		// 1Ç¬ëOÇÃÉ^ÉCÉvÇ÷
		nowSelect = (SELECT)((int)nowSelect - 1);

		// îÕàÕäOÇæÇ¡ÇΩÇÁãtÇÃí[Ç÷
		if (nowSelect <= SELECT::None) { nowSelect = (SELECT)((int)SELECT::Max - 1); }
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {

		// 1Ç¬éüÇÃÉ^ÉCÉvÇ÷
		nowSelect = (SELECT)((int)nowSelect + 1);

		// îÕàÕäOÇæÇ¡ÇΩÇÁãtÇÃí[Ç÷
		if (nowSelect >= SELECT::Max) { nowSelect = (SELECT)((int)SELECT::None + 1); }
	}
	// Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`ëÄçÏÇ…ÇÊÇ¡ëIëéàÇïœÇ¶ÇÈ

	// åªç›ÇÃëIëèÛë‘Ç…âûÇ∂Çƒï™äÚÅ`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
	switch (nowSelect) {

	case OptionScene::SELECT::MasterVolume: {
		// É}ÉXÉ^Å[É{ÉäÉÖÅ[ÉÄå∏éZ
		if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).now) { Snd::GetIns().AddMasterVolume(-2); isPushMinusButton = true; }
		// É}ÉXÉ^Å[É{ÉäÉÖÅ[ÉÄâ¡éZ
		if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).now) { Snd::GetIns().AddMasterVolume(2); isPushPlusButton = true; }
		break;
	}

	case OptionScene::SELECT::BgmVolume: {
		// BGMÉ{ÉäÉÖÅ[ÉÄå∏éZ
		if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).now) { Snd::GetIns().AddBgmVolume(-0.01f); isPushMinusButton = true; }
		// BGMÉ{ÉäÉÖÅ[ÉÄâ¡éZ
		if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).now) { Snd::GetIns().AddBgmVolume(0.01f); isPushPlusButton = true; }
		break;
	}

	case OptionScene::SELECT::SeVolume: {
		// SEÉ{ÉäÉÖÅ[ÉÄå∏éZ
		if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).now) { Snd::GetIns().AddSeVolume(-0.01f); isPushMinusButton = true; }
		// SEÉ{ÉäÉÖÅ[ÉÄâ¡éZ
		if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).now) { Snd::GetIns().AddSeVolume(0.01f); isPushPlusButton = true; }
		break;
	}

	case OptionScene::SELECT::Exit: {
		// èIóπ
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
			// å¯â âπ
			Snd::GetIns().Play("SystemSelect");

			// é©êgÇÃÉVÅ[ÉìÇîjä¸Ç∑ÇÈ
			SceneManager::GetIns().PopScene();

			// à»ç~ÇÕthisÇ™nullptrÇ∆Ç»Ç¡ÇƒÇ¢ÇÈÇΩÇﬂèIóπ
			return;
		}
		break;
	}

	default: { break; }
	}
	// Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`åªç›ÇÃëIëèÛë‘Ç…âûÇ∂Çƒï™äÚ
#pragma endregion

	exitEasingCounter += 0.1f;
	if (exitEasingCounter > 100000.0f) { exitEasingCounter = 0.0f; }
	exitEasingRate = 1.0f + ((sinf(exitEasingCounter) + 1.0f) * 0.05f);

}

void OptionScene::SubPreDraw(void)
{
	// âÊëúï`âÊÇÃÉâÉÄÉ_ä÷êî
	auto drawImage = [](int handle, const Vector2I& pos, float rate = 1.0f, float angle = 0.0f)->void {
		DrawRotaGraph(pos.x, pos.y, rate, angle, handle, true);
		};

	// âÊñ ëSëÃÇîºìßñæï`âÊ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#pragma region äeâÊëúÇÃï`âÊ

	// òg
	drawImage(frameImage, FRAME_POS);

	// É}ÉXÉ^Å[âπó  / BGMâπó  / SEâπó 
	for (int i = 0; i < (int)SELECT::Max - 1; i++) {
		const Vector2I& pos = VOLUME_NAME_POS[i];

		// å©èoÇµ
		drawImage(volumeNameImage[i], pos);

		// âπó ÇÃäÑçá
		float volumeRatio = GetVolumeRatio(i);

		// ÉQÅ[ÉW
		DrawRectGraph(
			(pos.x + VOLUME_GAUGE_LOCAL_POS.x) - (VOLUME_GAUGE_SIZE.x / 2),
			(pos.y + VOLUME_GAUGE_LOCAL_POS.y) - (VOLUME_GAUGE_SIZE.y / 2),
			0, 0,
			(int)((float)VOLUME_GAUGE_SIZE.x * volumeRatio), VOLUME_GAUGE_SIZE.y,
			volumeGaugeMaxImage,
			true
		);
		DrawRectGraph(
			(pos.x + VOLUME_GAUGE_LOCAL_POS.x) - (VOLUME_GAUGE_SIZE.x / 2) + (int)((float)VOLUME_GAUGE_SIZE.x * volumeRatio),
			(pos.y + VOLUME_GAUGE_LOCAL_POS.y) - (VOLUME_GAUGE_SIZE.y / 2),
			(int)((float)VOLUME_GAUGE_SIZE.x * volumeRatio), 0,
			(int)((float)VOLUME_GAUGE_SIZE.x * (1.0f - volumeRatio)), VOLUME_GAUGE_SIZE.y,
			volumeGaugeMinImage,
			true
		);

		// ÉQÅ[ÉWÇÃè„Ç…èoÇ∑É{É^Éì
		Vector2I seekButtonPos = (pos + VOLUME_GAUGE_LOCAL_POS);
		seekButtonPos.x -= (VOLUME_GAUGE_SIZE.x - BUTTON_SIZE) / 2;
		seekButtonPos.x += (int)((float)(VOLUME_GAUGE_SIZE.x - BUTTON_SIZE) * volumeRatio);
		drawImage(seekButtonImage[(int)(nowSelect == (SELECT)i)], seekButtonPos);
	}

	if (nowSelect != SELECT::Exit) {
		// É}ÉCÉiÉXÉ{É^Éì
		drawImage(minusButtonImage[isPushMinusButton], VOLUME_NAME_POS[(int)nowSelect] + MINUS_BUTTON_LOCAL_POS);
		// ÉvÉâÉXÉ{É^Éì
		drawImage(plusButtonImage[isPushPlusButton], VOLUME_NAME_POS[(int)nowSelect] + PLUS_BUTTON_LOCAL_POS);
	}

	// Ç∆Ç∂ÇÈÉ{É^Éì
	drawImage(exitImage, EXIT_POS, (nowSelect == SELECT::Exit) ? exitEasingRate : 1.0f);

#pragma endregion

}

void OptionScene::SubPreRelease(void)
{
#pragma region äeâÊëúÇÃâï˙

	// òg
	DeleteGraph(frameImage);

	// É}ÉXÉ^Å[âπó  / BGMâπó  / SEâπó 
	for (int i = 0; i < (int)SELECT::Max - 1; i++) { DeleteGraph(volumeNameImage[i]); }

	// ÉQÅ[ÉWÅiÉ}ÉbÉNÉXèÛë‘Åj
	DeleteGraph(volumeGaugeMaxImage);
	// ÉQÅ[ÉWÅiÉ[ÉçèÛë‘Åj
	DeleteGraph(volumeGaugeMinImage);

	// ÉQÅ[ÉWÇÃè„Ç…èoÇ∑É{É^Éì
	DeleteGraph(seekButtonImage[(int)true]);
	DeleteGraph(seekButtonImage[(int)false]);

	// ÉvÉâÉXÉ{É^Éì
	DeleteGraph(plusButtonImage[(int)true]);
	DeleteGraph(plusButtonImage[(int)false]);
	// É}ÉCÉiÉXÉ{É^Éì
	DeleteGraph(minusButtonImage[(int)true]);
	DeleteGraph(minusButtonImage[(int)false]);

	// Ç∆Ç∂ÇÈÉ{É^Éì
	DeleteGraph(exitImage);

#pragma endregion
}

float OptionScene::GetVolumeRatio(int volumeSelect) const
{
	switch ((SELECT)volumeSelect) {

	case OptionScene::SELECT::MasterVolume: { return Snd::GetIns().MasterVolumeRatio(); }

	case OptionScene::SELECT::BgmVolume: { return Snd::GetIns().BgmVolume(); }

	case OptionScene::SELECT::SeVolume: { return Snd::GetIns().SeVolume(); }

	default: { break; }
	}
}
