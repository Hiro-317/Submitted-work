#include "CharaSelectPreviewManager.h"

#include "../../../Application/Application.h"

#include "../../../Scene/SceneManager/SceneManager.h"
#include "../../../Manager/Net/NetWorkManager.h"
#include "../../../Manager/Input/KeyManager.h"

#include "CharaSelectPreview/Orange/OrangeCharaSelectPreview.h"
#include "CharaSelectPreview/Tomato/TomatoCharaSelectPreview.h"
#include "CharaSelectPreview/Peach/PeachCharaSelectPreview.h"
#include "CharaSelectPreview/Grape/GrapeCharaSelectPreview.h"
#include "CharaSelectPreview/Unknow/UnknowCharaSelectPreview.h"

CharaSelectPreviewManager::CharaSelectPreviewManager(CHARA_TYPE selectCharacter) :
	charaPreview(nullptr),

	frameImage(-1), arrowImage(-1),
	enterImage{ -1, -1 },
	exitImage{ -1, -1 },

	easingCounter(0.0f), easingRate(0.0f)
{
	ChangeCharacter(selectCharacter);
}

void CharaSelectPreviewManager::Load(void)
{
#pragma region 画像の読み込み
	// 枠
	frameImage = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectFrame.png");

	// 矢印
	arrowImage = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectArrow.png");

	// 決定キー
	enterImage[(int)true] = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectEnterController.png");
	enterImage[(int)false] = LoadGraph("Data/Image/Lobby/CharaSelect/CharaSelectEnterKeyboard.png");

	// とじるキー
	exitImage[(int)true] = LoadGraph("Data/Image/Lobby/ExitController.png");
	exitImage[(int)false] = LoadGraph("Data/Image/Lobby/ExitKeyboard.png");
#pragma endregion
}

void CharaSelectPreviewManager::Init(void)
{
	// 演出用の変数を初期化
	easingCounter = easingRate = 0.0f;
}

void CharaSelectPreviewManager::Update(void)
{
	// 選択中のキャラタイプのプレビューを更新
	charaPreview->Update();

#pragma region イージング
	easingCounter += 0.08f;
	if (easingCounter > 100000.0f) { easingCounter = 0.0f; }
	easingRate += sinf(easingCounter) * 0.001f;
#pragma endregion
}

void CharaSelectPreviewManager::Draw(void)
{
	// 枠の描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF, 1, 0, frameImage, true);

	// 選択中のキャラタイプのプレビューの描画処理
	charaPreview->Draw();

	// 矢印の描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF, 1 + easingRate, 0, arrowImage, true);

	// 決定キーの描画
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y - 48, 1 + easingRate, 0, enterImage[(int)Key::GetIns().LastInputKinds()], true);

	// とじるキーの描画
	DrawRotaGraph(EXIT_IMAGE_POS.x, EXIT_IMAGE_POS.y, 1 + easingRate, 0, exitImage[(int)Key::GetIns().LastInputKinds()], true);
}

void CharaSelectPreviewManager::Release(void)
{
	// プレビューを破棄
	if (charaPreview) {
		charaPreview->Release();
		delete charaPreview;
		charaPreview = nullptr;
	}

	for (int& image : enterImage) { DeleteGraph(image); }

	DeleteGraph(arrowImage);
	DeleteGraph(frameImage);
}

void CharaSelectPreviewManager::ChangeCharacter(CHARA_TYPE select)
{
	// 範囲チェック
	if (select <= CHARA_TYPE::None || CHARA_TYPE::Max <= select) { return; }

	// 現在のプレビューを破棄
	if (charaPreview) {
		charaPreview->Release();
		delete charaPreview;
		charaPreview = nullptr;
	}

	// 新たに選択されたキャラのプレビューを生成
	switch (select) {
	case CHARA_TYPE::Orange: { charaPreview = new OrangeCharaSelectPreview(); break; }
	case CHARA_TYPE::Tomato: { charaPreview = new TomatoCharaSelectPreview(); break; }
	case CHARA_TYPE::Peach: { charaPreview = new PeachCharaSelectPreview(); break; }
	case CHARA_TYPE::Grape: { charaPreview = new GrapeCharaSelectPreview(); break; }
	default: { charaPreview = new UnknowCharaSelectPreview(); break; }
	}

	// 読み込み/初期化
	charaPreview->Load();
	charaPreview->Init();
}
