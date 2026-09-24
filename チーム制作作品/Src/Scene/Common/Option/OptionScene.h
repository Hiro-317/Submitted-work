#pragma once

#include <string>

#include "../../SceneBase.h"

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"

class OptionScene : public SceneBase
{
public:
	OptionScene();
	~OptionScene()override = default;

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

	// 選択肢
	enum class SELECT {
		None=-1,

		MasterVolume,
		BgmVolume,
		SeVolume,
		Exit,

		Max
	};

#pragma region 定数定義（各画像のパス）

	// オプションリソースまでのパス
	const std::string OPTION_RESORCE_PATH = "Data/Image/Option/";

	// 枠
	const std::string FRAME_IMAGE_PATH = "Frame.png";

	// マスター音量/BGM音量/SE音量 座標
	const std::string VOLUME_NAME_PATH[(int)SELECT::Max - 1] = {
		"MasterVolume.png",
		"BgmVolume.png",
		"SeVolume.png",
	};

	// ゲージ（マックス状態）
	const std::string VOLUME_GAUGE_MAX_IMAGE_PATH = "VolumeGaugeMax.png";
	// ゲージ（ゼロ状態）
	const std::string VOLUME_GAUGE_MIN_IMAGE_PATH = "VolumeGaugeMin.png";

	// ゲージの上に出すボタン
	const std::string SEEK_BUTTON_IMAGE_PATH = "SeekButton.png";

	// プラスボタン
	const std::string PLUS_BUTTON_IMAGE_PATH = "PlusButton.png";
	// マイナスボタン
	const std::string MINUS_BUTTON_IMAGE_PATH = "MinusButton.png";

	// 各ボタンの発光状態の共通先頭付加文字列
	const std::string PUSH_BUTTON_IMAGE_ADD_STRING = "Push";

	// とじるボタン
	const std::string EXIT_IMAGE_PATH = "Exit.png";

#pragma endregion

#pragma region 定数定義（各画像の 座標/サイズ）

	// 枠 座標
	const Vector2I FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// マスター音量/BGM音量/SE音量 座標
	const Vector2I VOLUME_NAME_POS[(int)SELECT::Max - 1] = {
		Vector2I(465, 280),
		Vector2I(465, 380),
		Vector2I(465, 480),
	};

	// ゲージ 座標
	const Vector2I VOLUME_GAUGE_LOCAL_POS = Vector2I(335, 0);
	// ゲージ サイズ
	const Vector2I VOLUME_GAUGE_SIZE = Vector2I(400, 40);

	// マイナスボタン 座標
	const Vector2I MINUS_BUTTON_LOCAL_POS = Vector2I(110, 0);
	// プラスボタン 座標
	const Vector2I PLUS_BUTTON_LOCAL_POS = Vector2I(560, 0);

	// プラス/マイナス ボタンのサイズ（縦横同一）
	const int BUTTON_SIZE = 50;

	// とじるボタン
	const Vector2I EXIT_POS = Vector2I(App::SCREEN_SIZE_X_HALF, 600);

#pragma endregion


	// 現在の選択状態
	SELECT nowSelect;


#pragma region 各画像ハンドル

	// 枠
	int frameImage;

	// マスター音量 / BGM音量 / SE音量
	int volumeNameImage[(int)SELECT::Max - 1];

	// ゲージ（マックス状態）
	int volumeGaugeMaxImage;
	// ゲージ（ゼロ状態）
	int volumeGaugeMinImage;

	// ゲージの上に出すボタン
	int seekButtonImage[2];

	// プラスボタン
	int plusButtonImage[2];
	// マイナスボタン
	int minusButtonImage[2];

	// とじるボタン
	int exitImage;

#pragma endregion

	bool isPushPlusButton;
	bool isPushMinusButton;

	float exitEasingCounter;
	float exitEasingRate;

	float GetVolumeRatio(int volumeSelect)const;
};