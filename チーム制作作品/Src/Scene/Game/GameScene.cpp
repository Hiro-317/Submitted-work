#include "GameScene.h"

#include <cmath>

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Camera/MultifuncCamera/MultifuncCamera.h"
#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Effect/EffectManager.h"

#include "../../scene/SceneManager/SceneManager.h"

#include "../ObjectUseDefine.h"

#include "Pause/GamePause.h"

#include "../Common/GameDebugScene.h"

#include "../../Object/Character/Player/PlayerManager/PlayerManager.h"

#include "../../Object/Character/Boss/Tomato/TomatoBoss.h"
#include "../../Object/Character/Boss/Grape/GrapeBoss.h"
#include "../../Object/Character/Boss/Banana/BananaBoss.h"

#include "../../Object/Stage/TomatoBoss/TomatoBossStage.h"
#include "../../Object/Stage/GrapeBoss/GrapeBossStage.h"
#include "../../Object/Stage/BananaBoss/BananaBossStage.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/CapsuleDebugObject.h"


GameScene::GameScene() :
	SceneBase(),
	bossInsRef(nullptr),
	playerInsRef(nullptr),
	focusFlg(false),
	time(0.0f)
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	// 選択ボスを取得
	BOSS_TYPE selectBossType = SceneManager::GetIns().GetSelectBossType();

	// ステージを生成
	switch (selectBossType) {
	case BOSS_TYPE::Tomato: { ObjAdd(new TomatoBossStage()); break; }
	case BOSS_TYPE::Grape: { ObjAdd(new GrapeBossStage()); break; }
	case BOSS_TYPE::Banana: { ObjAdd(new BananaBossStage()); break; }
	default: { break; }
	}

	// プレイヤーを生成
	ObjAdd(new PlayerManager());
	playerInsRef = ObjSerch<PlayerManager>(objects);

	// 接続されているプレイヤー数座標を取得する
	std::vector<const Vector3*> pos;
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		pos.emplace_back(&ObjSerch<PlayerManager>(objects)->GetPlayerIns((MSG_SENDER_ID)id)->GetTrans().pos);
	}
	// 接続されているプレイヤー数生存判定を取得する
	std::vector<const bool*> live;
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		live.emplace_back(&ObjSerch<PlayerManager>(objects)->GetPlayerIns((MSG_SENDER_ID)id)->GetIsDeath());
	}

	// ボスを生成
	switch (selectBossType) {
	case BOSS_TYPE::Tomato: { ObjAdd(new TomatoBoss(pos, live)); break; }
	case BOSS_TYPE::Grape: { ObjAdd(new GrapeBoss(pos, live)); break; }
	case BOSS_TYPE::Banana: { ObjAdd(new BananaBoss(pos, live)); break; }
	default: { break; }
	}
	bossInsRef = ObjSerch<BossBase>(objects);

	// プレイヤーにボスの座標をわたす
	ObjSerch<PlayerManager>(objects)->SetBossPos(&ObjSerch<BossBase>(objects)->GetTrans().pos);
}

void GameScene::SubPostInit(void)
{
	focusFlg = false;

	SoundManager::GetIns().Play("Battle");

	time = 0.0f;
}

void GameScene::SubPreUpdate(void)
{
	if (Net::GetIns().GetState() == Net::NetState::None) { return; }

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataGameTime>()) {
		time = dataPtr->time;

		SceneManager::GetIns().SetClearTime(time);

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>(MSG_SENDER_ID::None, true)) {
		SceneManager::GetIns().PushScene(std::make_unique<GamePause>(dataPtr->header.senderId));
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

void GameScene::SubPostUpdate(void)
{
	Vector3 pos = Vector3();

	// 注目するか
	if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_FOCUS).down) {
		focusFlg = !focusFlg;
		if (focusFlg) {
			dynamic_cast<MultifuncCamera*>(camera)->ChangeModeFollowAuto(
				&playerInsRef->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
				&bossInsRef->GetTrans().pos);
		}
		else {
			dynamic_cast<MultifuncCamera*>(camera)->ChangeModeFollowYaw(
				&playerInsRef->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
				Vector3::YZonly(250, -550),
				playerInsRef->GetPlayerIns(Net::GetIns().GetSenderId())->GetInterestPos(),
				Deg2Rad(4.0f)
			);
		}
	}

#pragma region 遷移判定（ポーズも含む）
	// ポーズ判定
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		Net::GetIns().EventInformSend(MsgDataSystemInform::INFORM_TYPE::PushGamePause);
		EffectManager::GetIns()->PauseEffect();
		SceneManager::GetIns().PushScene(std::make_unique<GamePause>(Net::GetIns().GetSenderId()));
		return;
	}

	// ゲームクリア判定
	if (bossInsRef->GetIsDeath()) {

		// 同期
		if (Net::GetIns().GetState() != Net::NetState::None && Net::GetIns().IsHost()) { Net::GetIns().Send(MsgDataGameTime(time)); }
		SceneManager::GetIns().SetClearTime(time);


		// シーン切り替え時に前シーンのエフェクトを残さない
		if (EffectManager::GetIns() != nullptr) { EffectManager::GetIns()->StopEffectAll(); }

		switch (SceneManager::GetIns().GetSelectBossType())
		{
		case BOSS_TYPE::Tomato:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Clear, FADE_TYPE::DEFAULT, 90, 0xe33434, 0x000000);
			break;
		case BOSS_TYPE::Grape:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Clear, FADE_TYPE::DEFAULT, 90, 0x9338e8, 0x000000);
			break;
		case BOSS_TYPE::Banana:
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Clear, FADE_TYPE::DEFAULT, 90, 0xfcf80a, 0x000000);
			break;
		default:
			break;
		}
		return;
	}

	// ゲームオーバー判定
	if (ObjSerch<PlayerManager>(objects)->IsPlayerAllDeath()) {

		// シーン切り替え時に前シーンのエフェクトを残さない
		if (EffectManager::GetIns() != nullptr) { EffectManager::GetIns()->StopEffectAll(); }

		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameOver, FADE_TYPE::DEFAULT, 90, 0xffffff, 0x000000);
		return;
	}

#if _DEBUG
	// シーンを再読み込み
	if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_RELOAD).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Game);

		// シーン切り替え時に前シーンのエフェクトを残さない
		if (EffectManager::GetIns() != nullptr) { EffectManager::GetIns()->StopEffectAll(); }

		return;
	}
#endif // _DEBUG

	EffectManager::GetIns()->UpdateEffect();
	EffectManager::GetIns()->ReceptionUpdate();
	UpdateEffekseer3D();

#pragma endregion

#pragma region 時間計測

	// 時間更新
	time += FRAME_TIME;

	// 同期
	if (Net::GetIns().GetState() != Net::NetState::None&&Net::GetIns().IsHost()) {
		if ((char)time % MULTI_TIME_SEND_COUNT == 0) { Net::GetIns().Send(MsgDataGameTime(time)); }
	}
#pragma endregion
}

void GameScene::SubUiDraw(void)
{
	DrawString(0, 0, std::to_string(time).c_str(), 0xffffff);
}

void GameScene::SubPostRelease(void)
{
	Net::GetIns().ReceptionDataReset();
}


void GameScene::CreateCamera(void)
{
	camera = new MultifuncCamera(
		false,
		&ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetTrans().pos,
		Vector3::YZonly(250, -550),
		ObjSerch<PlayerManager>(objects)->GetPlayerIns(Net::GetIns().GetSenderId())->GetInterestPos(),
		Deg2Rad(4.0f)
	);
}
