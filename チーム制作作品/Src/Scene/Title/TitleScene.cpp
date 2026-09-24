#include "TitleScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../SceneManager/SceneManager.h"

#include "../Common/Option/OptionScene.h"
#include "End/EndScene.h"

TitleScene::TitleScene():
	SceneBase(),

	nowSelect(SELECT::None),

	frameImage(),
	backImage(),
	logoImage(),
	sunImage(),
	balloonImage(),
	flyingBoatImage(),
	cloudImage(),
	banagonImage(),
	selectImage(),
	charaImage(),

	sunEasingCounter(),
	sunEasingRotate(),

	balloonEasingCounter(),
	balloonEasingRotate(),

	flyingBoatEasingCounter(),
	flyingBoatEasingRotate(),

	charaEasingCounter(),
	charaEasingRotate(),

	cloudPos(),
	cloudVec(),

	selectEasingCounter(),
	selectEasingRate(),

	logoEasingCounter(),
	logoEasingRate()
{
}

void TitleScene::SubPostLoad(void)
{
	// 音声のシーン切り替え
	Snd::GetIns().ChangeScene("Title");

#pragma region 画像の読み込み

	// 画像読み込みのラムダ関数
	auto loadTitleImage = [&](std::string name)->int { return LoadGraph((TITLE_RESORCE_FILE_PATH + name).c_str()); };

	// 枠
	frameImage = loadTitleImage(FRAME_IMAGE_PATH);
	// 背景
	backImage = loadTitleImage(BACK_IMAGE_PATH);

	// タイトルロゴ
	logoImage = loadTitleImage(LOGO_IMAGE_PATH);

	// 太陽
	sunImage = loadTitleImage(SUN_IMAGE_PATH);
	// 気球
	balloonImage = loadTitleImage(BALLOON_IMAGE_PATH);
	// 飛行船
	flyingBoatImage = loadTitleImage(FLYING_BOAT_IMAGE_PATH);
	// 雲
	cloudImage = loadTitleImage(CLOUD_IMAGE_PATH);
	// バナゴン
	banagonImage = loadTitleImage(BANAGON_IMAGE_PATH);

	// 選択肢
	for (int i = 0; i < (int)SELECT::Max; i++) { selectImage[i] = loadTitleImage(SELECT_IMAGE_PATH[i]); }

	// キャラクター
	for (int i = 0; i < (int)CHARA::Max; i++) { charaImage[i] = loadTitleImage(CHARA_IMAGE_PATH[i]); }

#pragma endregion
}

void TitleScene::SubPostInit(void)
{
	Net::GetIns().Disconnection();

	// 現在選択中の選択肢を初期化
	nowSelect = (SELECT)((int)SELECT::None + 1);

	// 雲の座標の初期化
	for (int i = 0; i < CLOUD_NUM; i++) { 
		cloudPos[i] = CLOUD_INIT_POS[i]; 
		cloudVec[i].x = GetRand(CLOUD_MOVE_MAX_SPEED * 2) - CLOUD_MOVE_MAX_SPEED;
		if (cloudVec[i].x == 0) { cloudVec[i].x++; }
		if (i == CLOUD_NUM - 1) { cloudVec[i].x *= BANAGON_MOVE_SPEED_RATE; }
	}

	// 演出更新
	EasingUpdate();
}
void TitleScene::SubPostUpdate(void)
{
#pragma region 操作/進行

	// ゲーム終了処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// 再生中の音声を一時停止
		Snd::GetIns().Pause();
		// 効果音
 		Snd::GetIns().Play("SystemSelect");

		// ゲーム終了用のイベントシーンを挿入
		SceneManager::GetIns().PushScene(std::make_unique<EndScene>());

		return;
	}

	// 操作によって選択肢を変える
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		// 1つ前のタイプへ
		nowSelect = (SELECT)((int)nowSelect - 1);
		// 範囲外だったら逆の端へ
		if (nowSelect <= SELECT::None) { nowSelect = (SELECT)((int)SELECT::Max - 1); }

		// 演出を初期化
		selectEasingCounter = 0.0f;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		// 1つ次のタイプへ
		nowSelect = (SELECT)((int)nowSelect + 1);
		// 範囲外だったら逆の端へ
		if (nowSelect >= SELECT::Max) { nowSelect = (SELECT)((int)SELECT::None + 1); }

		// 演出を初期化
		selectEasingCounter = 0.0f;
	}

	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		// 現在の選択状態に応じて分岐
		switch (nowSelect) {

		case TitleScene::SELECT::Play: {
			// ゲームスタート

			// 効果音
			Snd::GetIns().Play("SystemSelect");

			// ロビー画面へ
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);

			break;
		}

		case TitleScene::SELECT::Option: {
			// オプション

			// 効果音
			Snd::GetIns().Play("SystemSelect");

			// 専用のシーンを追加
			SceneManager::GetIns().PushScene(std::make_unique<OptionScene>());

			break;
		}

		case TitleScene::SELECT::Exit: {
			// ゲーム終了

			// 再生中の音声を一時停止
			Snd::GetIns().Pause();
			// 効果音
			Snd::GetIns().Play("SystemSelect");

			// 専用のシーンを追加
			SceneManager::GetIns().PushScene(std::make_unique<EndScene>());

			break;
		}

		default: { break; }
		}
	}
#pragma endregion

	// 演出更新
	EasingUpdate();
}

void TitleScene::SubPostDraw(void)
{
	// 画像描画のラムダ関数
	auto drawImage = [](int handle, const Vector2I& pos, float rate = 1.0f, float angle = 0.0f, bool reverse = false)->void {
		DrawRotaGraph(pos.x, pos.y, rate, angle, handle, true, reverse);
		};

#pragma region 各画像の描画
	// 背景
	drawImage(backImage, BACK_POS);

	// 太陽
	drawImage(sunImage, SUN_POS, 1.0f, sunEasingRotate);
	// 雲
	for (int i = 0; i < CLOUD_NUM; i++) {
		drawImage((i == CLOUD_NUM - 1) ? banagonImage : cloudImage, cloudPos[i], 1.0f, 0.0f, cloudVec[i].x > 0.0f);
	}
	// 気球
	drawImage(balloonImage, BALLOON_POS, 1.0f, balloonEasingRotate);
	// 飛行船
	drawImage(flyingBoatImage, FLYING_BOAT_POS, 1.0f, flyingBoatEasingRotate);

	// キャラ
	for (int i = 0; i < (int)CHARA::Max; i++) { drawImage(charaImage[i], CHARA_POS[i], 1.0f, charaEasingRotate[i]); }

	// 非選択中の選択肢
	for (int i = 0; i < (int)SELECT::Max; i++) {
		if (nowSelect == (SELECT)i) { continue; }
		drawImage(selectImage[i], SELECT_POS[i]); 
	}
	// 選択中のものを最後に描画する（その際演出も加える）
	drawImage(selectImage[(int)nowSelect], SELECT_POS[(int)nowSelect], selectEasingRate);

	// ロゴ
	drawImage(logoImage, LOGO_POS, logoEasingRate);

	// 枠
	drawImage(frameImage, FRAME_POS);
#pragma endregion
}
void TitleScene::SubPreRelease(void)
{
#pragma region 各画像の解放

	// 枠
	DeleteGraph(frameImage);
	// 背景
	DeleteGraph(backImage);

	// タイトルロゴ
	DeleteGraph(logoImage);

	// 太陽
	DeleteGraph(sunImage);
	// 気球
	DeleteGraph(balloonImage);
	// 飛行船
	DeleteGraph(flyingBoatImage);
	// 雲
	DeleteGraph(cloudImage);
	// バナゴン
	DeleteGraph(banagonImage);

	// 選択肢
	for (int& image : selectImage) { DeleteGraph(image); }
	// キャラクター
	for (int& image : charaImage) { DeleteGraph(image); }

#pragma endregion

}

void TitleScene::EasingUpdate(void)
{
	// カウンター更新のラムダ関数
	auto easingCounterUpdate = [](float& easingCounter, float add)->void {
		easingCounter += add;
		if (easingCounter > 100000.0f) { easingCounter = 0.0f; }
		if (easingCounter < -100000.0f) { easingCounter = 0.0f; }
		};

	// 太陽
	easingCounterUpdate(sunEasingCounter, 0.5f);
	sunEasingRotate = Deg2Rad(sunEasingCounter);

	// 雲
	for (int i = 0; i < CLOUD_NUM; i++) {
		cloudPos[i] += cloudVec[i];

		// 方向転換（右に）
		if (cloudPos[i].x < -App::SCREEN_SIZE_X_HALF / 2) {
			cloudVec[i].x = GetRand(CLOUD_MOVE_MAX_SPEED) + 1;
			if (i == CLOUD_NUM - 1) { cloudVec[i].x *= BANAGON_MOVE_SPEED_RATE; }
		}
		// 方向転換（左に）
		else if (cloudPos[i].x > App::SCREEN_SIZE_X + App::SCREEN_SIZE_X_HALF / 2) {
			cloudVec[i].x = -(GetRand(CLOUD_MOVE_MAX_SPEED) + 1);
			if (i == CLOUD_NUM - 1) { cloudVec[i].x *= BANAGON_MOVE_SPEED_RATE; }
		}
	}

	// 気球
	easingCounterUpdate(balloonEasingCounter, 0.05f);
	balloonEasingRotate = Deg2Rad(sinf(balloonEasingCounter) * 5.0f);

	// 飛行船
	easingCounterUpdate(flyingBoatEasingCounter, -0.05f);
	flyingBoatEasingRotate = Deg2Rad(sinf(flyingBoatEasingCounter) * 5.0f);

	// キャラ
	easingCounterUpdate(charaEasingCounter, 0.05f);
	charaEasingRotate[(int)CHARA::Grape] = Deg2Rad(cosf(charaEasingCounter) * 4.0f);
	charaEasingRotate[(int)CHARA::Tomato] = Deg2Rad(sinf(charaEasingCounter) * 4.0f);
	charaEasingRotate[(int)CHARA::Peach] = Deg2Rad(cosf(charaEasingCounter) * 8.0f);
	charaEasingRotate[(int)CHARA::Orange] = Deg2Rad(sinf(charaEasingCounter) * 8.0f);

	// 選択肢
	easingCounterUpdate(selectEasingCounter, 0.1f);
	selectEasingRate = 1.0f + ((sinf(selectEasingCounter) + 1.0f) * 0.1f);

	// ロゴ
	easingCounterUpdate(logoEasingCounter, 0.03f);
	logoEasingRate = 1.0f + ((sinf(logoEasingCounter) + 1.0f) * 0.12f);
}