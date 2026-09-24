#include "ClearScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Net/NetWorkManager.h"

#include "../../Manager/Ranking/Ranking.h"

#include "../../Manager/Camera/FixedPoint/FixedPointCamera.h"

#include "../SceneManager/SceneManager.h"

#include "../ObjectUseDefine.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/LobbyStage/LobbyStage.h"
#include "../../Object/Clear/ClearCharaPreview/ClearCharaPreviewManager.h"

ClearScene::ClearScene() :
	SceneBase(),
	clearTextImage(-1),
	clearTimeFrameImage(-1),
	clearTime(SceneManager::GetIns().GetClearTime())
{
}

void ClearScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Clear");

	clearTextImage = LoadGraph("Data/Image/Clear/ClearTextImage.png");
	clearTimeFrameImage = LoadGraph("Data/Image/Clear/ClearTimeFrame.png");

	// オブジェクト生成
	ObjAdd(new SkyDome());
	ObjAdd(new LobbyStage());
	ObjAdd(new ClearCharaPreviewManager());
}

void ClearScene::SubPostInit(void)
{
	SetFogEnable(false);

	// 同期
	if (Net::GetIns().GetState() != Net::NetState::None && Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataGameTime(clearTime));
	}

	clearTime = SceneManager::GetIns().GetClearTime();
}

void ClearScene::SubPreUpdate(void)
{
	if (Net::GetIns().GetState() == Net::NetState::None) { return; }

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataGameTime>()) {
		clearTime = dataPtr->time;

		SceneManager::GetIns().SetClearTime(clearTime);

		delete dataPtr;
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

void ClearScene::SubPostUpdate(void)
{
	if (Net::GetIns().IsHost()) {
		if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down ||
			Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

			if (Net::GetIns().GetState() == Net::NetState::None) {
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);
			}
			else {
				Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby);
				Net::GetIns().ResumptionReceptionToHosting();
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::MultiLobby);
			}
			Snd::GetIns().Play("SystemButton");
		}
	}
	else {
		while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>()) {

			if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::ChangeSceneLobby) {
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::MultiLobby);
				Snd::GetIns().Play("SystemButton");
			}

			delete dataPtr;
		}
	}
}

void ClearScene::SubUiDraw(void)
{
	DrawRotaGraph3(App::SCREEN_SIZE_X_HALF, 0, 350, 0, 1, 1, 0, clearTextImage, true);

	DrawRotaGraph(250, App::SCREEN_SIZE_Y_HALF, 1, 0, clearTimeFrameImage, true);

	DrawFormatStringToHandle(165, 385, 0x000000, Font::GetIns().GetFont(FontKinds::MARUMINYA_50), "%.2f秒", clearTime);
}

void ClearScene::SubPreRelease(void)
{
	DeleteGraph(clearTimeFrameImage);
	DeleteGraph(clearTextImage);

	Ranking::GetIns().AddScore(SceneManager::GetIns().GetSelectBossType(), clearTime);
	Ranking::GetIns().Save();

	SetFogEnable(true);
}

void ClearScene::CreateCamera(void)
{
	// 定点カメラを生成する
	camera = new FixedPointCamera(FixedPointCamera::LookAt, Vector3::YZonly(4000.0f, -7000.0f), Vector3::Yonly(2500.0f));
}