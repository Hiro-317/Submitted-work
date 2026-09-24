#pragma once

#include <string>

#include "../../../SceneBase.h"

#include "../../../../Application/Application.h"

#include "../../../../Common/Vector2.h"

#include <functional>

// パスワードの桁数
static int PASSWORD_NUMBER_OF_DIGITS = 3;

class PasswordScene : public SceneBase
{
public:
	PasswordScene();
	~PasswordScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPostDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	bool IsUseLoadingScreen(void)const override { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	// テンキー
	enum class NUMBER_KEY {

		None = -1,

		// 0～9
		Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,

		// 消去
		Delete,

		// 決定
		Enter,

		Max
	};

#pragma region 定数定義（各画像のパス）

	// タイトルリソースファイルまでのパス
	const std::string PASSWORD_RESORCE_FILE_PATH = "Data/Image/Lobby/MultiPopup/Password/";

	// 枠
	const std::string FRAME_IMAGE_PATH = "Frame.png";

	// パススペース
	const std::string PASS_SPACE_IMAGE_PATH = "PassSpace.png";

	// テンキー(数字)
	const std::string NUMBER_KEY_IMAGE_FRONT_PATH = "NumberKey";
	const std::string NUMBER_KEY_IMAGE_BACK_PATH = ".png";
	// テンキー(数字)の選択中画像の最初につく名前
	const std::string SELECT_NUMBER_KEY_IMAGE_FRONT_PATH = "Select";

	// テンキー(消去)
	const std::string DELETE_IMAGE_PATH = "Delete.png";
	// テンキー(決定)
	const std::string ENTER_IMAGE_PATH = "Enter.png";

	// 閉じるボタン
	const std::string EXIT_CONTROLLER_IMAGE_PATH = "Data/Image/Lobby/ExitController.png";
	const std::string EXIT_KEYBOARD_IMAGE_PATH = "Data/Image/Lobby/ExitKeyboard.png";

#pragma endregion

#pragma region 定数定義（各画像の 座標/初期座標）

	// 枠
	const Vector2I FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// パススペース
	const Vector2I PASS_SPACE_POS = Vector2I(App::SCREEN_SIZE_X_HALF, 210);

	// テンキーの親座標
	const Vector2I NUMBER_KEY_MASTER_POS = Vector2I(App::SCREEN_SIZE_X_HALF, 440);

	// テンキー
	const Vector2I NUMBER_KEY_LOCAL_POS[(int)NUMBER_KEY::Max] = {
		//「0」
		Vector2I(0,240),
		//「1」
		Vector2I(-130,-120),
		//「2」
		Vector2I(0,-120),
		//「3」
		Vector2I(130,-120),
		//「4」
		Vector2I(-130,0),
		//「5」
		Vector2I(0,0),
		//「6」
		Vector2I(130,0),
		//「7」
		Vector2I(-130,120),
		//「8」
		Vector2I(0,120),
		//「9」
		Vector2I(130,120),

		//「消去」
		Vector2I(280,-70),
		//「決定」
		Vector2I(280,70),
	};

	// 閉じるボタン
	const Vector2I EXIT_POS = Vector2I(App::SCREEN_SIZE_X_HALF - 350, App::SCREEN_SIZE_Y_HALF - 300);
#pragma endregion

	// 現在の入力中のパスキー
	std::string nowTypingPass;

#pragma region 各画像ハンドル
	// 枠
	int frameImage;

	// パススペース
	int passSpaceImage;

	// テンキー数字
	int numberKeyImage[(int)NUMBER_KEY::Max][2];

	// 閉じるボタン
	int exitImage[2];
#pragma endregion

	// 現在なにを選択しているか
	NUMBER_KEY nowNumberKey;

	// テンキーの操作
	std::function<void(void)> NumberKeyOperator[(int)NUMBER_KEY::Max];

#pragma region 各キーを選択中の操作

	// 「0」キーを選択しているとき
	void SelectZeroKeyOperator(void);

	// 「1」キーを選択しているとき
	void SelectOneKeyOperator(void);

	// 「2」キーを選択しているとき
	void SelectTwoKeyOperator(void);

	// 「3」キーを選択しているとき
	void SelectThreeKeyOperator(void);

	// 「4」キーを選択しているとき
	void SelectFourKeyOperator(void);

	// 「5」キーを選択しているとき
	void SelectFiveKeyOperator(void);

	// 「6」キーを選択しているとき
	void SelectSixKeyOperator(void);

	// 「7」キーを選択しているとき
	void SelectSevenKeyOperator(void);

	// 「8」キーを選択しているとき
	void SelectEightKeyOperator(void);

	// 「9」キーを選択しているとき
	void SelectNineKeyOperator(void);

	// 「Delete」キーを選択しているとき
	void SelectDeleteKeyOperator(void);

	// 「Enter」キーを選択しているとき
	void SelectEnterKeyOperator(void);

#pragma endregion

	// 数値を入力
	void TypePassNumber(int number) {
		nowTypingPass += std::to_string(number);
		if (nowTypingPass.size() > PASSWORD_NUMBER_OF_DIGITS) { 
			nowTypingPass.pop_back();
		}
	}
	// 数値を消去
	void DeletePassNumber(void) {
		if (nowTypingPass.size() > 0) {
			nowTypingPass.pop_back();
		}
	}
};