#include "LobbyScene.h"

#include "../../pch.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Camera/FixedPoint/FixedPointCamera.h"
#include "../../Manager/Camera/CameraEvent/StartCameraEvent/StartCameraEvent.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Sound/SoundManager.h"

#include "../../Scene/SceneManager/SceneManager.h"

#include "../ObjectUseDefine.h"

#include "CharaSelect/LobbyCharaSelectScene.h"
#include "MultiPopup/MultiPopupScene.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/LobbyStage/LobbyStage.h"
#include "../../Object/Lobby/LobbyCharaPreview/LobbyCharaPreviewManager.h"


LobbyScene::LobbyScene() :
	SceneBase(),

	choice(),

	boardImage(-1),

	choiceButtonImage(),

	arrowImage(-1),

	enterKeyImage()
{
}

void LobbyScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Lobby");

	if (SceneManager::GetIns().GetSelectBossType() == BOSS_TYPE::None) {
		SceneManager::GetIns().SetSelectBossType((BOSS_TYPE)((int)BOSS_TYPE::None + 1));
	}

	// オブジェクト生成
	ObjAdd(new SkyDome());
	ObjAdd(new LobbyStage());
	ObjAdd(new LobbyCharaPreviewManager());

#pragma region 各画像の読み込み

	// 画面上部のボードの画像
	boardImage = LoadGraph((IMAGE_DATA_FILE_DIR + "LobbyBoard.png").c_str());

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// 名前
		const std::string IMAGE_NAME = IMAGE_DATA_FILE_DIR + CHOICE_BUTTON_IMAGE_NAME[choiceIndex];

		// 選択時
		choiceButtonImage[choiceIndex][(int)true] = LoadGraph((IMAGE_NAME + CHOICE_BUTTON_IMAGE_DECORATION + ".png").c_str());

		// 非選択時
		choiceButtonImage[choiceIndex][(int)false] = LoadGraph((IMAGE_NAME + NOT_CHOICE_BUTTON_IMAGE_DECORATION + ".png").c_str());
	}

	// 選択中の矢印
	arrowImage = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectArrow.png").c_str());

	// 選択中の決定キー
	enterKeyImage[(int)false] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectKeyboard.png").c_str());
	enterKeyImage[(int)true] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectController.png").c_str());

#pragma endregion
}

void LobbyScene::SubPostInit(void)
{
	Snd::GetIns().Play("Lobby");

	SetFogEnable(false);

	choice = CHOICE::CharaChange;

	Net::GetIns().Disconnection();
}

void LobbyScene::SubPostUpdate(void)
{
	// タイトル画面に戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		Snd::GetIns().Play("SystemButton");
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

	// 左
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		Snd::GetIns().Play("SystemSelect");

		choice = (CHOICE)((int)choice - 1);
		if (choice <= CHOICE::None) { choice = (CHOICE)((int)CHOICE::None + 1); }
	}
	// 右
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		Snd::GetIns().Play("SystemSelect");

		choice = (CHOICE)((int)choice + 1);
		if (choice >= CHOICE::Max) { choice = (CHOICE)((int)CHOICE::Max - 1); }
	}


	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		Snd::GetIns().Play("SystemButton");

		// 選択肢ごとの処理
		switch (choice) {

		case LobbyScene::CHOICE::None: { break; }	// 選択肢なし（ありえないはず）

		case LobbyScene::CHOICE::Exit: {	// 退出

			// ネットワーク切断
			Net::GetIns().Disconnection();

			// タイトル画面に戻る
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);

			break;
		}

		case LobbyScene::CHOICE::Multi: {	// マルチ

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(std::make_unique<MultiPopupScene>());

			break;
		}

		case LobbyScene::CHOICE::CharaChange: {	// キャラ変更

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(
				std::make_unique<LobbyCharaSelectScene>(
					// キャラ変更シーンから戻ってきたときに、プレビューを更新
					[&]() { ObjSerch<LobbyCharaPreviewManager>(objects)->ReloadChara(); }
				)
			);

			break;
		}

		case LobbyScene::CHOICE::Enter: {	// 出撃

			// ゲームシーンに遷移
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::BossSelect);

			break;
		}
		}
	}
}

void LobbyScene::SubPostDraw(void)
{
	// ボードの描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, 115, 1, 0, boardImage, true);

	// 選択肢の描画
	for (int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(CHOICE_BUTTON_POS[i].x, CHOICE_BUTTON_POS[i].y, 1, 0, choiceButtonImage[i][(int)((CHOICE)i == choice)], true);
	}
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y, 1, 0, arrowImage, true);
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y - 75, 0.3f, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);

}

void LobbyScene::SubPreRelease(void)
{
	// 画像の解放
	DeleteGraph(enterKeyImage[0]);
	DeleteGraph(enterKeyImage[1]);
	DeleteGraph(arrowImage);
	for (int* handlePair : choiceButtonImage) {
		DeleteGraph(handlePair[0]);
		DeleteGraph(handlePair[1]);
	}
	DeleteGraph(boardImage);

	SetFogEnable(true);
}


void LobbyScene::CreateCamera(void)
{
	// 定点カメラを生成する
	camera = new FixedPointCamera(FixedPointCamera::LookAt, Vector3::YZonly(750.0f, -2000.0f), Vector3::Yonly(150.0f));

	// イベントカメラを生成
	camera->StartEvent(new StartCameraEvent(*camera, Vector3(0.0f, 1500.0f, -5000.0f), Vector3::Yonly(Deg2Rad(-30.0f)), 0.0f, 200.0f));
}