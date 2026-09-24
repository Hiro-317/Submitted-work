#include "PasswordScene.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Input/KeyManager.h"

#include "../../../../Manager/Font/FontManager.h"

#include "../../../SceneManager/SceneManager.h"

PasswordScene::PasswordScene() :
	nowTypingPass(),
	nowNumberKey(NUMBER_KEY::None),
	NumberKeyOperator()
{
}

void PasswordScene::SubPostLoad(void)
{
	// 現在のあいことばを取得
	unsigned short nowPassword = Net::GetIns().GetAddressProviderPassword();

	// 設定済みならそれを現在入力中のパスキーに入れる
	nowTypingPass = (nowPassword != 0) ? std::to_string(nowPassword) : "";

#pragma region 各画像を読み込み
	// 画像読み込みのラムダ関数
	auto loadPasswordImage = [&](std::string name)->int { return LoadGraph((PASSWORD_RESORCE_FILE_PATH + name).c_str()); };

	// 枠
	frameImage = loadPasswordImage(FRAME_IMAGE_PATH);

	// パススペース
	passSpaceImage = loadPasswordImage(PASS_SPACE_IMAGE_PATH);

	// テンキー(数字)
	for (int i = 0; i < (int)NUMBER_KEY::Max - 2; i++) {
		numberKeyImage[i][(int)true] = loadPasswordImage(SELECT_NUMBER_KEY_IMAGE_FRONT_PATH + NUMBER_KEY_IMAGE_FRONT_PATH + std::to_string(i) + NUMBER_KEY_IMAGE_BACK_PATH);
		numberKeyImage[i][(int)false] = loadPasswordImage(NUMBER_KEY_IMAGE_FRONT_PATH + std::to_string(i) + NUMBER_KEY_IMAGE_BACK_PATH);
	}

	// テンキー(消去)
	numberKeyImage[(int)NUMBER_KEY::Delete][(int)true] = loadPasswordImage(SELECT_NUMBER_KEY_IMAGE_FRONT_PATH + DELETE_IMAGE_PATH);
	numberKeyImage[(int)NUMBER_KEY::Delete][(int)false] = loadPasswordImage(DELETE_IMAGE_PATH);

	// テンキー(決定)
	numberKeyImage[(int)NUMBER_KEY::Enter][(int)true] = loadPasswordImage(SELECT_NUMBER_KEY_IMAGE_FRONT_PATH + ENTER_IMAGE_PATH);
	numberKeyImage[(int)NUMBER_KEY::Enter][(int)false] = loadPasswordImage(ENTER_IMAGE_PATH);

	// 閉じるボタン
	exitImage[(int)true] = LoadGraph(EXIT_CONTROLLER_IMAGE_PATH.c_str());
	exitImage[(int)false] = LoadGraph(EXIT_KEYBOARD_IMAGE_PATH.c_str());

#pragma endregion

#pragma region テンキーの操作の紐づけ

	// 「0」
	NumberKeyOperator[(int)NUMBER_KEY::Zero] = std::bind(&PasswordScene::SelectZeroKeyOperator, this);
	// 「1」
	NumberKeyOperator[(int)NUMBER_KEY::One] = std::bind(&PasswordScene::SelectOneKeyOperator, this);
	// 「2」
	NumberKeyOperator[(int)NUMBER_KEY::Two] = std::bind(&PasswordScene::SelectTwoKeyOperator, this);
	// 「3」
	NumberKeyOperator[(int)NUMBER_KEY::Three] = std::bind(&PasswordScene::SelectThreeKeyOperator, this);
	// 「4」
	NumberKeyOperator[(int)NUMBER_KEY::Four] = std::bind(&PasswordScene::SelectFourKeyOperator, this);
	// 「5」
	NumberKeyOperator[(int)NUMBER_KEY::Five] = std::bind(&PasswordScene::SelectFiveKeyOperator, this);
	// 「6」
	NumberKeyOperator[(int)NUMBER_KEY::Six] = std::bind(&PasswordScene::SelectSixKeyOperator, this);
	// 「7」
	NumberKeyOperator[(int)NUMBER_KEY::Seven] = std::bind(&PasswordScene::SelectSevenKeyOperator, this);
	// 「8」
	NumberKeyOperator[(int)NUMBER_KEY::Eight] = std::bind(&PasswordScene::SelectEightKeyOperator, this);
	// 「9」
	NumberKeyOperator[(int)NUMBER_KEY::Nine] = std::bind(&PasswordScene::SelectNineKeyOperator, this);
	// 「Delete」
	NumberKeyOperator[(int)NUMBER_KEY::Delete] = std::bind(&PasswordScene::SelectDeleteKeyOperator, this);
	// 「Enter」
	NumberKeyOperator[(int)NUMBER_KEY::Enter] = std::bind(&PasswordScene::SelectEnterKeyOperator, this);

#pragma endregion

}

void PasswordScene::SubPostInit(void)
{
	nowNumberKey = NUMBER_KEY::One;
}

void PasswordScene::SubPostUpdate(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {
		SceneManager::GetIns().PopScene();
		return;
	}
	// 操作
	NumberKeyOperator[(int)nowNumberKey]();
}

void PasswordScene::SubPostDraw(void)
{
	// 画面全体を半透明描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 画像描画のラムダ関数
	auto drawImage = [](int handle, const Vector2I& pos, float rate = 1.0f, float angle = 0.0f)->void {
		DrawRotaGraph(pos.x, pos.y, rate, angle, handle, true);
		};

#pragma region 各画像の描画

	// 枠
	drawImage(frameImage, FRAME_POS);

	// パススペース
	drawImage(passSpaceImage, PASS_SPACE_POS);
	for (int i = 0; i < nowTypingPass.size(); i++) {
		std::string c(1, nowTypingPass.at(i));
		int defaultPosX = PASS_SPACE_POS.x - 150;
		DrawFormatStringToHandle(defaultPosX + (i * 130), PASS_SPACE_POS.y - 32, 0x000000, Font::GetIns().GetFont(FontKinds::DELASUKO_GOTHIC_64), c.c_str());
	}

	// テンキー
	for (int i = 0; i < (int)NUMBER_KEY::Max; i++) {
		drawImage(numberKeyImage[i][(int)nowNumberKey == i], NUMBER_KEY_MASTER_POS + NUMBER_KEY_LOCAL_POS[i]);
	}

	// 閉じるボタン
	drawImage(exitImage[(int)Key::GetIns().LastInputKinds()], EXIT_POS);

#pragma endregion
}

void PasswordScene::SubPreRelease(void)
{
#pragma region 各画像の解放
	// 枠
	DeleteGraph(frameImage);

	// パススペース
	DeleteGraph(passSpaceImage);

	// テンキー
	for (int i = 0; i < (int)NUMBER_KEY::Max; i++) {
		DeleteGraph(numberKeyImage[i][(int)true]);
		DeleteGraph(numberKeyImage[i][(int)false]);
	}

	// 閉じるボタン
	DeleteGraph(exitImage[(int)true]);
	DeleteGraph(exitImage[(int)false]);
#pragma endregion
}

#pragma region テンキーの操作

void PasswordScene::SelectZeroKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Eight;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Seven;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Nine;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down && !nowTypingPass.empty()) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectOneKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Four;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Two;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectTwoKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Five;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::One;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Three;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectThreeKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Six;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Two;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Delete;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectFourKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::One;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Seven;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Five;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectFiveKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Two;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Eight;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Four;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Six;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectSixKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Three;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Nine;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Five;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Enter;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectSevenKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Four;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Zero;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Eight;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectEightKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Five;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Zero;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Seven;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Nine;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectNineKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Six;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Zero;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Eight;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		nowNumberKey = NUMBER_KEY::Enter;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) { TypePassNumber((int)nowNumberKey); }
}

void PasswordScene::SelectDeleteKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {
		nowNumberKey = NUMBER_KEY::Enter;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Three;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		DeletePassNumber();
	}
}

void PasswordScene::SelectEnterKeyOperator(void)
{
	if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {
		nowNumberKey = NUMBER_KEY::Delete;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		nowNumberKey = NUMBER_KEY::Nine;
	}

	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		if (nowTypingPass.size() > 0) {
			Net::GetIns().SetAddressProviderPassword(std::stoi(nowTypingPass));
		}
		else {
			Net::GetIns().SetAddressProviderPassword(0);
		}
		SceneManager::GetIns().PopScene();
		return;
	}
}

#pragma endregion