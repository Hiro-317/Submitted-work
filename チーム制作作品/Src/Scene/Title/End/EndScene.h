#pragma once

#include <string>

#include "../../SceneBase.h"

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"

class EndScene : public SceneBase
{
public:
	EndScene();
	~EndScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPreDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	bool IsUseLoadingScreen(void)const { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

#pragma region 定数

	// 選択肢
	enum class SELECT { None = -1, Yes, No, Max };


	// 枠の画像パス
	const std::string FRAME_IMAGE_PATH = "Data/Image/Title/End/Frame.png";

	// 選択肢(選択中)の画像パス
	const std::string SELECT_TO_SELECT_IMAGE_PATH[(int)SELECT::Max] = {
		"Data/Image/Title/End/YesToSelect.png",
		"Data/Image/Title/End/NoToSelect.png",
	};
	// 選択肢(非選択中)の画像パス
	const std::string SELECT_TO_NOT_SELECT_IMAGE_PATH[(int)SELECT::Max] = {
		"Data/Image/Title/End/YesToNotSelect.png",
		"Data/Image/Title/End/NoToNotSelect.png",
	};

	// イメージキャラの画像パス
	const std::string CHARA_IMAGE_PATH = "Data/Image/Title/End/Chara.png";


	// 枠の座標
	const Vector2I FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// 選択肢の座標
	const Vector2I SELECT_POS[(int)SELECT::Max] = {
		FRAME_POS + Vector2I(-100, -130),
		FRAME_POS + Vector2I(100,  -130),
	};

	// イメージキャラの座標
	const Vector2I CHARA_POS = FRAME_POS + Vector2I(350, 200);

#pragma endregion

	// 現在の選択状態
	SELECT nowSelect;

	// 枠の画像ハンドル
	int frameImage;

	// 選択肢の画像ハンドル
	int selectImage[(int)SELECT::Max][2];

	// イメージキャラの画像ハンドル
	int charaImage;


	// 選択肢のイージングカウンター
	float selectEasingCounter;
	float selectEasingRate;

	// イメージキャラのイージングカウンター
	float charaEasingCounter;
	float charaEasingRotate;
};

