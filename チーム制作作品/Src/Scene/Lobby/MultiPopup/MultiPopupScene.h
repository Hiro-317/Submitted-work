#pragma once

#include <string>

#include "../../SceneBase.h"

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"

class MultiPopupScene : public SceneBase
{
public:
	MultiPopupScene();
	~MultiPopupScene()override = default;

private:

#pragma region å—vŠÖ”Ä’è‹`

	// “Ç‚İ‚İ
	void SubPostLoad(void)override;

	// ‰Šú‰»
	void SubPostInit(void)override;

	// XV
	void SubPostUpdate(void)override;

	// •`‰æ
	void SubPostDraw(void)override;

	// I—¹
	void SubPreRelease(void);

#pragma endregion

	bool IsUseLoadingScreen(void)const override { return false; }

	// “–‚½‚è”»’èŠÇ—‚Íg—p‚µ‚È‚¢
	bool UseCollisionManager(void)const override { return false; }

#pragma region ’è”’è‹`
	
	enum class SELECT {
		None = -1,

		MakeRoom,	// •”‰®‚ğ‚Â‚­‚é
		JoinRoom,	// •”‰®‚ğ‚³‚ª‚·

		Max
	};

	// ‰æ‘œ‚ÌƒpƒX````````````````````````````

	// ƒfƒBƒŒƒNƒgƒŠ
	const std::string DIRECTORY_PATH = "Data/Image/Lobby/";
	
	// ƒ|ƒbƒvƒAƒbƒvƒtƒŒ[ƒ€
	const std::string POPUP_FRAME_PATH = DIRECTORY_PATH + "MultiPopup/PopupFrame.png";

	// ‘I‘ğˆ
	const std::string SELECT_PATH[(int)SELECT::Max] = {
		DIRECTORY_PATH + "MultiPopup/MakeRoom.png",
		DIRECTORY_PATH + "MultiPopup/JoinRoom.png",
	};

	// ‘I‘ğ’†‚ğ¦‚·ƒtƒŒ[ƒ€
	const std::string SELECT_FRAME_PATH = DIRECTORY_PATH + "MultiPopup/SelectFrame.png";

	// ‘I‘ğ’†‚ğ¦‚·–îˆó
	const std::string SELECT_ARROW_PATH = DIRECTORY_PATH + "MultiPopup/SelectArrow.png";

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[
	const std::string CONTROLLER_ENTER_KEY_PATH = DIRECTORY_PATH + "NowSelectController.png";
	const std::string KEYBOARD_ENTER_KEY_PATH = DIRECTORY_PATH + "NowSelectKeyboard.png";

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[
	const std::string CONTROLLER_EXIT_KEY_PATH = DIRECTORY_PATH + "ExitController.png";
	const std::string KEYBOARD_EXIT_KEY_PATH = DIRECTORY_PATH + "ExitKeyboard.png";

	// “ü—Í’†‚Ì‚ ‚¢‚±‚Æ‚Î‚ğ‘‚­‚½‚ß‚Ì”wŒi
	const std::string PASSWORD_BACK_PATH = DIRECTORY_PATH + "MultiPopup/PasswordFrame.png";

	// ‚ ‚¢‚±‚Æ‚Î“ü—Í‚Ìƒ{ƒ^ƒ“‚ÌƒCƒ‰ƒXƒg(ƒRƒ“ƒgƒ[ƒ‰[)
	const std::string PASSWORD_CONTROLLER_BUTTON_PATH = DIRECTORY_PATH + "MultiPopup/PasswordControllerButton.png";
	// ‚ ‚¢‚±‚Æ‚Î“ü—Í‚Ìƒ{ƒ^ƒ“‚ÌƒCƒ‰ƒXƒg(ƒL[ƒ{[ƒh)
	const std::string PASSWORD_KEYBOARD_BUTTON_PATH = DIRECTORY_PATH + "MultiPopup/PasswordKeyboardButton.png";

	// ````````````````````````````‰æ‘œ‚ÌƒpƒX

	// Še‰æ‘œ‚Ì•`‰æÀ•W`````````````````````````
	
	// ƒ|ƒbƒvƒAƒbƒvƒtƒŒ[ƒ€
	const Vector2I POPUP_FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// ‘I‘ğˆ
	const Vector2I SELECT_POS[(int)SELECT::Max] = {
		Vector2I(510,App::SCREEN_SIZE_Y_HALF + 40),
		Vector2I(930,App::SCREEN_SIZE_Y_HALF + 40),
	};

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[‚Ì‘Š‘ÎÀ•W
	const Vector2I ENTER_KEY_LOCAL_POS = Vector2I(0, 195);
	const float ENTER_KEY_LOCAL_POS_EASING_RATE = 100.0f;

	// ‰æ–Ê¶ã‚É”z’u‚·‚é‚Æ‚¶‚éƒL[
	const Vector2I EXIT_KEY_POS = Vector2I(288, 131);

	// “ü—Í’†‚Ì‚ ‚¢‚±‚Æ‚Î‚Ì”wŒi
	const Vector2I PASSWORD_BACK_POS = Vector2I();
	// “ü—Í’†‚Ì‚ ‚¢‚±‚Æ‚Î
	const Vector2I PASSWORD_POS = Vector2I();

	// ‚ ‚¢‚±‚Æ‚Î“ü—Í‚Ìƒ{ƒ^ƒ“‚ÌƒCƒ‰ƒXƒg
	const Vector2I PASSWORD_BUTTON_POS = Vector2I();

	// `````````````````````````Še‰æ‘œ‚Ì•`‰æÀ•W

#pragma endregion

	// ‘I‘ğ’†
	SELECT select;

	// ‰æ‘œƒnƒ“ƒhƒ‹``````````````````

	// ƒ|ƒbƒvƒAƒbƒvƒtƒŒ[ƒ€
	int popupFrameImage;

	// ‘I‘ğˆ
	int selectImage[(int)SELECT::Max];

	// ‘I‘ğ’†‚ğ¦‚·ƒtƒŒ[ƒ€
	int selectFrameImage;

	// ‘I‘ğ’†‚ğ¦‚·–îˆó
	int selectArrowImage;

	// ‘I‘ğ’†‚Ìƒ{ƒ^ƒ“‚Ìã‚É•\¦‚·‚éŒˆ’èƒL[
	int enterKeyImage[2];

	int passwordFrameImage;
	int passwordButtonImage[2];

	// ‰æ–Ê¶ã‚É”z’u‚·‚é‚Æ‚¶‚éƒL[
	int exitKeyImage[2];

	// ``````````````````‰æ‘œƒnƒ“ƒhƒ‹

	// ‘I‘ğ’†‚ÌƒCƒ[ƒWŒQ‚ğ“®‚©‚·ƒJƒEƒ“ƒ^[
	float selectEasingCounter;
	float selectEasing;
};