#include "GamePause.h"

#include "../../../Utility/Utility.h"

#include "../../../Application/Application.h"
#include "../../SceneManager/SceneManager.h"
#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Sound/SoundManager.h"
#include "../../../Manager/Net/NetWorkManager.h"
#include "../../../Manager/Effect/EffectManager.h"

GamePause::GamePause(MSG_SENDER_ID operatorSenderId) :
	SceneBase(),

	img(),
	nowSelect(SELECT::YES),

	operatorSenderId(operatorSenderId),
	isOperator(operatorSenderId == MSG_SENDER_ID::None ? true : operatorSenderId == Net::GetIns().GetSenderId())
{
}

void GamePause::SubPostLoad(void)
{
	img[(int)SELECT::YES] = LoadImg("Data/Image/Title/End/Yes.png");
	img[(int)SELECT::NO] = LoadImg("Data/Image/Title/End/No.png");
}

void GamePause::SubPostInit(void)
{
	nowSelect = SELECT::YES;
	SoundManager::GetIns().Pause();
	Snd::GetIns().Play("SystemButton");
	Key::GetIns().SetMouseFixed(false);
}

void GamePause::SubPostUpdate(void)
{
	// 自分がポーズ画面を開いたとき
	if (isOperator) {

		if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

			Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::PopGamePause);

			Snd::GetIns().PausePlay();
			Snd::GetIns().Play("SystemButton");
			SceneManager::GetIns().PopScene();
			Key::GetIns().SetMouseFixed(true);
			EffectManager::GetIns()->PlayEffect();
			return;
		}

		switch (nowSelect) {

		case GamePause::SELECT::YES:
			if (Key::GetIns().GetInfo(KEY_TYPE::DOWN).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePauseChoicesSwitch);

				nowSelect = GamePause::SELECT::NO;
				Snd::GetIns().Play("SystemSelect");
			}

			if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::JumpSceneBossSelect);

				Snd::GetIns().Play("SystemButton");

				EffectManager::GetIns()->StopEffectAll();
				SceneManager::GetIns().JumpSceneFade(SCENE_ID::BossSelect);
				return;
			}

			break;

		case GamePause::SELECT::NO:

			if (Key::GetIns().GetInfo(KEY_TYPE::UP).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::GamePauseChoicesSwitch);

				nowSelect = GamePause::SELECT::YES;
				Snd::GetIns().Play("SystemSelect");
			}

			if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::PopGamePause);

				Snd::GetIns().PausePlay();
				Snd::GetIns().Play("SystemButton");
				SceneManager::GetIns().PopScene();
				Key::GetIns().SetMouseFixed(true);
				EffectManager::GetIns()->PlayEffect();
				return;
			}

			break;
		}

	}
	// 自分以外がポーズ画面を開いたとき(受信処理のみ)
	else {

		while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>(MSG_SENDER_ID::None, true)) {

			switch (dataPtr->inform) {
			case MsgDataSystemInform::INFORM_TYPE::None: { break; }

			case MsgDataSystemInform::INFORM_TYPE::JumpSceneBossSelect: {
				Snd::GetIns().Play("SystemButton");
				EffectManager::GetIns()->StopEffectAll();
				SceneManager::GetIns().JumpSceneFade(SCENE_ID::BossSelect);
				break;
			}
			case MsgDataSystemInform::INFORM_TYPE::PopGamePause: {
				Snd::GetIns().PausePlay();
				Snd::GetIns().Play("SystemButton");
				SceneManager::GetIns().PopScene();
				Key::GetIns().SetMouseFixed(true);
				EffectManager::GetIns()->PlayEffect();
				break;
			}
			case MsgDataSystemInform::INFORM_TYPE::GamePauseChoicesSwitch: {
				Snd::GetIns().Play("SystemSelect");
				if (nowSelect == SELECT::YES) { nowSelect = SELECT::NO; }
				else if (nowSelect == SELECT::NO) { nowSelect = SELECT::YES; }

				break;
			}
			default: { break; }
			}

			delete dataPtr;
		}

	}

	// 切断 の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataConnectInform>()) {

		if (dataPtr->inform == MsgDataConnectInform::INFORM_TYPE::Disconnect) {
			Net::GetIns().Disconnection();
			SceneManager::GetIns().JumpSceneFade(SCENE_ID::Lobby);
		}

		delete dataPtr;
	}
}

void GamePause::SubPreDraw(void)
{
	int xx = Application::SCREEN_SIZE_X;
	int yy = Application::SCREEN_SIZE_Y;
	int x = xx / 2;
	int y = yy / 2;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, xx, yy, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawRotaGraph(x, y, 1, 0, img[(int)nowSelect], true);
}

void GamePause::SubPreRelease(void)
{
	for (auto& id : img) { DeleteGraph(id); }
}