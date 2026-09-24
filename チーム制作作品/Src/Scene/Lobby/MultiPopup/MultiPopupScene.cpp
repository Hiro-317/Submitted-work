#include "MultiPopupScene.h"

#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Sound/SoundManager.h"
#include "../../../Manager/Net/NetWorkManager.h"
#include "../../../Manager/Font/FontManager.h"

#include "../../SceneManager/SceneManager.h"

#include "MatchingLoadingScene.h"
#include "Password/PasswordScene.h"

MultiPopupScene::MultiPopupScene() : 
	SceneBase(),

	select(SELECT::None),

	popupFrameImage(-1),
	selectImage{ -1, -1 },
	selectFrameImage(-1),
	selectArrowImage(-1),
	enterKeyImage{ -1,-1 },
	exitKeyImage{ -1, -1 },

	selectEasingCounter(0.0f),
	selectEasing(0.0f)
{
}

void MultiPopupScene::SubPostLoad(void)
{
#pragma region 画像を読み込む

	// ポップアップフレーム
	popupFrameImage = LoadGraph(POPUP_FRAME_PATH.c_str());

	// 選択肢
	for (int i = 0; i < (int)SELECT::Max; i++) {
		selectImage[i] = LoadGraph(SELECT_PATH[i].c_str());
	}

	// 選択中を示すフレーム
	selectFrameImage = LoadGraph(SELECT_FRAME_PATH.c_str());

	// 選択中を示す矢印
	selectArrowImage = LoadGraph(SELECT_ARROW_PATH.c_str());

	// 選択中のボタンの上に表示する決定キー
	enterKeyImage[(int)false] = LoadGraph(KEYBOARD_ENTER_KEY_PATH.c_str());
	enterKeyImage[(int)true] = LoadGraph(CONTROLLER_ENTER_KEY_PATH.c_str());

	passwordFrameImage = LoadGraph(PASSWORD_BACK_PATH.c_str());

	passwordButtonImage[(int)true] = LoadGraph(PASSWORD_CONTROLLER_BUTTON_PATH.c_str());
	passwordButtonImage[(int)false] = LoadGraph(PASSWORD_KEYBOARD_BUTTON_PATH.c_str());

	// 画面左上に配置するとじるキー
	exitKeyImage[(int)false] = LoadGraph(KEYBOARD_EXIT_KEY_PATH.c_str());
	exitKeyImage[(int)true] = LoadGraph(CONTROLLER_EXIT_KEY_PATH.c_str());

#pragma endregion
}

void MultiPopupScene::SubPostInit(void)
{
	// 選択の初期化
	select = SELECT::MakeRoom;

	// イージング変数の初期化
	selectEasingCounter = 0.0f;
	selectEasing = 0.0f;
}

void MultiPopupScene::SubPostUpdate(void)
{
	// タイトル画面に戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// 効果音
		Snd::GetIns().Play("SystemButton");

		// 自身を破棄する
		SceneManager::GetIns().PopScene();

		return;
	}

	// パスワード入力
	if (Key::GetIns().GetInfo(KEY_TYPE::MULTI_POP_UP_PASSWORD).down) {
		// 効果音
		Snd::GetIns().Play("SystemButton");

		// 専用のシーンを追加する
		SceneManager::GetIns().PushScene(std::make_unique<PasswordScene>());

		return;
	}

	// 左
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {

		// 効果音
		Snd::GetIns().Play("SystemSelect");

		// 1つ左へ（選択中の値を1つ減らす）
		select = (SELECT)((int)select - 1);
		// 範囲を超えていたら戻す
		if (select <= SELECT::None) { select = (SELECT)((int)SELECT::None + 1); }
	}
	// 右
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {

		// 効果音
		Snd::GetIns().Play("SystemSelect");

		// 1つ右へ（選択中の値を1つ増やす)）
		select = (SELECT)((int)select + 1);
		// 範囲を超えていたら戻す
		if (select >= SELECT::Max) { select = (SELECT)((int)SELECT::Max - 1); }
	}

	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		// 選択中での状態分け
		switch (select) {

		case MultiPopupScene::SELECT::MakeRoom: {	// 部屋をつくる

			// ホストとして接続受付開始（失敗したらスキップ）
			if (!Net::GetIns().StartHost()) { printfDx("失敗しました"); break; }

			clsDx();

			// シーンをマルチ用のロビーに移動させる
			SceneManager::GetIns().JumpSceneFade(SCENE_ID::MultiLobby);

			// 以降はthisがnullptrとなっているため終了
			return;
		}

		case MultiPopupScene::SELECT::JoinRoom: {	// 部屋をさがす

			// クライアントとして接続開始
			Net::GetIns().ConnectClient();

			clsDx();

			// マッチング中シーンを追加
			SceneManager::GetIns().PushScene(std::make_unique<MatchingLoadingScene>());

			// 以降はthisがnullptrとなっているため終了
			return;
		}

		default: { break; }	// 例外
		}
	}

#pragma region イージング
	selectEasingCounter += 0.08f;
	if (selectEasingCounter > 100000.0f) { selectEasingCounter = 0.0f; }
	selectEasing += sinf(selectEasingCounter) * 0.001f;
#pragma endregion

}

void MultiPopupScene::SubPostDraw(void)
{
	// 背景色
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポップアップフレーム
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF, 1, 0, popupFrameImage, true);

	static Vector2I tempPos = Vector2I();
	if (CheckHitKey(KEY_INPUT_UP) == 1) { tempPos.y--; }
	if (CheckHitKey(KEY_INPUT_DOWN) == 1) { tempPos.y++; }
	if (CheckHitKey(KEY_INPUT_LEFT) == 1) { tempPos.x--; }
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1) { tempPos.x++; }

	DrawRotaGraph(1100, 130, 1, 0, passwordFrameImage, true);
	DrawRotaGraph(1100, 130 + 60, 1, 0, passwordButtonImage[(int)Key::GetIns().LastInputKinds()], true);
	unsigned short providerPassword = Net::GetIns().GetAddressProviderPassword();
	if (providerPassword != 0) {
		DrawFormatStringToHandle(
			1060, 119,
			0x000000, Font::GetIns().GetFont(FontKinds::DELASUKO_GOTHIC_45),
			std::to_string(providerPassword).c_str()
		);
	}

	// 選択肢
	for (int i = 0; i < (int)SELECT::Max; i++) {
		DrawRotaGraph(SELECT_POS[i].x, SELECT_POS[i].y, (i == (int)select) ? 1 + selectEasing : 1, 0, selectImage[i], true);
	}

	// 選択中を示すフレーム
	DrawRotaGraph(SELECT_POS[(int)select].x, SELECT_POS[(int)select].y, 1 + selectEasing, 0, selectFrameImage, true);

	// 選択中を示す矢印
	DrawRotaGraph(SELECT_POS[(int)select].x, SELECT_POS[(int)select].y, 1 + selectEasing, 0, selectArrowImage, true);

	// 選択中のボタンの上に表示する決定キー
	DrawRotaGraph(
		SELECT_POS[(int)select].x + ENTER_KEY_LOCAL_POS.x,
		SELECT_POS[(int)select].y + ENTER_KEY_LOCAL_POS.y + (int)(selectEasing * ENTER_KEY_LOCAL_POS_EASING_RATE),
		0.75f + selectEasing, 0,
		enterKeyImage[(int)Key::GetIns().LastInputKinds()],
		true
	);

	// 画面左上に配置するとじるキー
	DrawRotaGraph(EXIT_KEY_POS.x, EXIT_KEY_POS.y, 1 + selectEasing, 0, exitKeyImage[(int)Key::GetIns().LastInputKinds()], true);

}

void MultiPopupScene::SubPreRelease(void)
{

#pragma region 画像を解放

	// ポップアップフレーム
	DeleteGraph(popupFrameImage);

	// 選択肢
	for (int& image : selectImage) { DeleteGraph(image); }

	// 選択中を示すフレーム
	DeleteGraph(selectFrameImage);

	// 選択中を示す矢印
	DeleteGraph(selectArrowImage);

	DeleteGraph(passwordFrameImage);
	DeleteGraph(passwordButtonImage[(int)true]);
	DeleteGraph(passwordButtonImage[(int)false]);

	// 選択中のボタンの上に表示する決定キー
	DeleteGraph(enterKeyImage[(int)true]);
	DeleteGraph(enterKeyImage[(int)false]);

#pragma endregion
}