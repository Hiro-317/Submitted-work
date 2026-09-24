#include "MatchingLoadingScene.h"

#include "../../../Application/Application.h"

#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Net/NetWorkManager.h"

#include "../../SceneManager/SceneManager.h"

MatchingLoadingScene::MatchingLoadingScene():
	SceneBase(),

	waitCounter(0)
{
}

void MatchingLoadingScene::SubPostInit(void)
{
	// 接続待ち時間を設定
	waitCounter = MATCHING_WAIT_TIME;
}

void MatchingLoadingScene::SubPostUpdate(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		// 接続状況をリセットする
		Net::GetIns().Disconnection();

		// このシーンを破棄して、戻る
		SceneManager::GetIns().PopScene();

		return;
	}

	// 接続できたかどうか
	if (Net::GetIns().GetState() == Net::NetState::Connected) {

		// 接続完了したため、マルチ用のロビーに移動する
		SceneManager::GetIns().JumpSceneFade(SCENE_ID::MultiLobby);

		return;
	}

	// 待ち時間計測
	if (--waitCounter <= 0) {

		// 接続状況をリセットする
		Net::GetIns().Disconnection();

		// このシーンを破棄して、戻る
		SceneManager::GetIns().PopScene();

		return;
	}
}

void MatchingLoadingScene::SubPostDraw(void)
{
	// 画面全体を暗めに
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Vector2I ScreenSizeQuarter = Vector2I(App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y) / 20;

	// 中央のポップアップを描画
	DrawBox(
		App::SCREEN_SIZE_X_HALF - (32 * 11 / 2), App::SCREEN_SIZE_Y_HALF - 20,
		App::SCREEN_SIZE_X_HALF + (32 * 11 / 2), App::SCREEN_SIZE_Y_HALF + 20,
		0x000000, true
	);

	SetFontSize(32);
	DrawString(App::SCREEN_SIZE_X_HALF- (32 * 10 / 2), App::SCREEN_SIZE_Y_HALF-16, "マッチング中・・・", 0xffffff);
	SetFontSize(16);
}