#include"EndScene.h"

#include"../../../Utility/Utility.h"

#include"../../../Application/Application.h"
#include"../../SceneManager/SceneManager.h"
#include"../../../Manager/Input/KeyManager.h"
#include"../../../Manager/Sound/SoundManager.h"

EndScene::EndScene() :
	SceneBase(),

	nowSelect(),

	frameImage(),
	selectImage(),
	charaImage(),

	selectEasingCounter(),
	selectEasingRate(),

	charaEasingCounter(),
	charaEasingRotate()
{
}

void EndScene::SubPostLoad(void)
{

#pragma region ‰æ‘œ‚ğ“Ç‚İ‚Ş

	// ˜g
	frameImage = LoadGraph(FRAME_IMAGE_PATH.c_str());

	// ‘I‘ğˆ
	for (int i = 0; i < (int)SELECT::Max; i++) {
		selectImage[i][(int)true] = LoadGraph(SELECT_TO_SELECT_IMAGE_PATH[i].c_str());
		selectImage[i][(int)false] = LoadGraph(SELECT_TO_NOT_SELECT_IMAGE_PATH[i].c_str());
	}

	// ƒLƒƒƒ‰ƒNƒ^[
	charaImage = LoadGraph(CHARA_IMAGE_PATH.c_str());

#pragma endregion

}

void EndScene::SubPostInit(void)
{
	// Œ»İ‚Ì‘I‘ğó‘Ô‚ğ‰Šú‰»
	nowSelect = SELECT::No;

	selectEasingCounter = 0.0f;
	selectEasingRate = 0.0f;

	charaEasingCounter = 0.0f;
	charaEasingRotate = 0.0f;
}

void EndScene::SubPostUpdate(void)
{
	// –ß‚éƒ{ƒ^ƒ“‚Åƒ^ƒCƒgƒ‹‚É–ß‚é
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		Snd::GetIns().Play("SystemButton");
		SoundManager::GetIns().PausePlay();
		SceneManager::GetIns().PopScene();
		return;
	}

	// ‘I‘ğˆ‚É‚æ‚Á‚Ä•ªŠò
	switch (nowSelect) {

	case EndScene::SELECT::Yes: {
		// ‘I‘ğó‘ÔuYesv’†‚Ìˆ—

		// ‰Eƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç
		if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
			// Œø‰Ê‰¹
			Snd::GetIns().Play("SystemSelect");

			// ‘I‘ğó‘ÔuNov‚Ö
			nowSelect = EndScene::SELECT::No;
		}

		// Œˆ’èƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
			// Œø‰Ê‰¹
			Snd::GetIns().Play("SystemButton");

			// ƒQ[ƒ€‚ğI—¹
			App::GetIns().GameEnd();

			// ˆÈ~‚Íthis‚ªnullptr‚Ìˆ×I—¹
			return;
		}

		break;
	}

	case EndScene::SELECT::No: {
		// ‘I‘ğó‘ÔuNov’†‚Ìˆ—

		// ¶ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç
		if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
			// Œø‰Ê‰¹
			Snd::GetIns().Play("SystemSelect");

			// ‘I‘ğó‘ÔuYesv‚Ö
			nowSelect = EndScene::SELECT::Yes;
		}

		// Œˆ’èƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
			// Œø‰Ê‰¹
			Snd::GetIns().Play("SystemButton");

			// ’â~’†‚Ì‰¹º‚ğ‘S‚ÄÄ¶
			SoundManager::GetIns().PausePlay();

			// ‚±‚ÌƒV[ƒ“‚ğ”jŠüAƒ^ƒCƒgƒ‹‚É–ß‚é
			SceneManager::GetIns().PopScene();

			// ˆÈ~‚Íthis‚ªnullptr‚Ìˆ×I—¹
			return;
		}

		break;
	}

	}

	// ‰‰oXV``````````````````````````````

	selectEasingCounter += 0.1f;
	if (selectEasingCounter > 10000.0f) { selectEasingCounter = 0.0f; }
	selectEasingRate = 1.0f + (sinf(selectEasingCounter) + 1.0f) * 0.05f;

	charaEasingCounter += 0.05f;
	if (charaEasingCounter > 10000.0f) { charaEasingCounter = 0.0f; }
	charaEasingRotate = Deg2Rad(sinf(charaEasingCounter) * 5.0f);

	// ``````````````````````````````‰‰oXV
}

void EndScene::SubPreDraw(void)
{
	// ‰æ–Ê‘S‘Ì‚ğ”¼“§–¾•`‰æ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ‰æ‘œ•`‰æ‚Ìƒ‰ƒ€ƒ_ŠÖ”
	auto drawImage = [](int handle, const Vector2I& pos, float rate = 1.0f, float angle = 0.0f)->void {
		DrawRotaGraph(pos.x, pos.y, rate, angle, handle, true);
		};

	// ˜g‚ğ•`‰æ
	drawImage(frameImage, FRAME_POS);
	// ‘I‘ğˆ‚ğ•`‰æ
	for (int i = 0; i < (int)SELECT::Max; i++) {
		drawImage(
			selectImage[i][(int)((int)nowSelect == i)],
			SELECT_POS[i],
			((int)nowSelect == i) ? selectEasingRate : 1.0f
		);
	}
	// ƒCƒ[ƒWƒLƒƒƒ‰‚Ì•`‰æ
	drawImage(charaImage, CHARA_POS, 1.0f, charaEasingRotate);
}

void EndScene::SubPreRelease(void)
{

#pragma region ‰æ‘œ‚ğ‰ğ•ú

	// ˜g
	DeleteGraph(frameImage);

	// ‘I‘ğˆ
	for (int i = 0; i < (int)SELECT::Max; i++) {
		DeleteGraph(selectImage[i][(int)true]);
		DeleteGraph(selectImage[i][(int)false]);
	}

	// ƒLƒƒƒ‰ƒNƒ^[
	DeleteGraph(charaImage);

#pragma endregion

}