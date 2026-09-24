#include "BananaBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Idle/BananaBossIdleState.h"
#include "State/Scratch/BananaBossScratchState.h"
#include "State/Scratch/BananaScratchCollOperator.h"
#include "State/Fire/BananaBossFireState.h"
#include "State/Fire/BananaFireCollOperator.h"
#include "State/Boomerang/BananaBossBoomerangState.h"
#include "State/Boomerang/BananaBossBanamerang.h"
#include "State/Death/BananaBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"


BananaBoss::BananaBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerLive) :
	BossBase(
		"Data/Parameter/Character/Boss/Banagon/",
		"Banana/Banagon",

		playerPos, playerLive)
{

	coolTime = 120;
}

void BananaBoss::PlayerLoad(void)
{
	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma region プレイヤーが抱える下位クラスを生成する

	StartFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "LowerArm.R")));
	EndFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R")));

	subObjArray.push_back(new BananaScratchCollOperator(operatorSenderId, characterStats, INIT_POS, StartFrame, EndFrame));
	subObjArray.push_back(new BananaFireCollOperator(operatorSenderId, characterStats));
	subObjArray.push_back(new BananaBossBanamerang(operatorSenderId, characterStats));

	AddState(
		static_cast<int>(STATE::IDLE),
		new BananaBossIdleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::IDLE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::IDLE); },
			// 自分の座標、プレイヤーの座標の読み取り
			trans.pos, trans.angle, playerPos,
			// クールタイム
			[&]() { return coolTime; },
			// ターゲット番号の取得
			[&]() { return targetNum; },
			// アイドルアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); },
			// ひっかきへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// 吐く攻撃への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// バナメランへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); }//,
			//// ひとつ追従への状態遷移関数のポインタ
			//[&]() { ChangeState((int)STATE::ATTACK_D); },
			//// たくさん追従への状態遷移関数のポインタ
			//[&]() { ChangeState((int)STATE::ATTACK_E); },
			//// たくさんランダムへの状態遷移関数のポインタ
			//[&]() { ChangeState((int)STATE::ATTACK_F); }
		)
	);

	AddState(
		static_cast<int>(STATE::ATTACK_A),
		new BananaBossScratchState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_A); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_A); },
			// コライダへのポインタ
			SubObjSerch<BananaScratchCollOperator>(),
			// フレーム座標の更新
			[&]() {
				StartFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "LowerArm.R")));
				EndFrame = Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R")));
			},
			// 攻撃アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = SCRATCH_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_B),
		new BananaBossFireState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_B); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_B); },
			// コライダへのポインタ
			SubObjSerch<BananaFireCollOperator>(),
			// 攻撃アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::BREATH, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = SCRATCH_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_C),
		new BananaBossBoomerangState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_C); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_C); },
			// コライダへのポインタ
			SubObjSerch<BananaBossBanamerang>(),
			// 手の座標
			[&]() { return Vector3(MV1GetFramePosition(trans.model, MV1SearchFrame(trans.model, "Ring.01.R"))); },
			// 溜めアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH_START, false); },
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH_LOOP, true); },
			// 攻撃アニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::SCRATCH_END, false); },
			// 攻撃後のアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE, true); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = SCRATCH_COOLTIME; }
		)
	);

	AddState(
		static_cast<int>(STATE::DEATH),
		new BananaBossDeathState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::DEATH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::DEATH); },
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH_FALL, false); },
			// 死んだフラグ3を立てる
			[&]() {	SetIsDeath(true); }
		)
	);

#pragma endregion


#pragma region 状態設定

#pragma endregion


#pragma region UI生成
	// HPバー生成
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			characterStats.hp,
			characterStats.hpMax.Value(),
			HP_FRAME_IMAGE_NAME,
			HP_IMAGE_NAME,
			HP_LOST_IMAGE_NAME,
			"", "", "",
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS,
			FILE_PATH_TYPE::BOSS_HP,
			""
		)
	);

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
#pragma endregion

}

void BananaBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			SubObjSerch<BananaScratchCollOperator>()->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB:
		{
			SubObjSerch<BananaFireCollOperator>()->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			SubObjSerch<BananaBossBanamerang>()->SetScale(dataPtr->scale);
			SubObjSerch<BananaBossBanamerang>()->SetBanameranPos(dataPtr->pos);
			SubObjSerch<BananaBossBanamerang>()->SetBanameranRot(Vector3::Yonly(dataPtr->angle));
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossAttackDrawFlg* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDrawFlg>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA:
		{
			if (dataPtr->flg) {
				SubObjSerch<BananaScratchCollOperator>()->SetDrawArea(dataPtr->flg);
			}
			else {
				SubObjSerch<BananaScratchCollOperator>()->Off();
			}
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB:
		{
			if (dataPtr->flg) {
				SubObjSerch<BananaFireCollOperator>()->SetDrawArea(dataPtr->flg);
			}
			else {
				SubObjSerch<BananaFireCollOperator>()->Off();
			}
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			if (dataPtr->flg) {
				SubObjSerch<BananaBossBanamerang>()->SetDrawArea(dataPtr->flg);
			}
			else {
				SubObjSerch<BananaBossBanamerang>()->Off();
			}			
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
}

void BananaBoss::CharacterInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;
	trans.angle = INIT_ROT;

	// 重力無効
	ActorBase::SetGravityFlg(false);

	// 初期状態を移動状態にする
	ChangeState(static_cast<int>(STATE::IDLE));

	for (ActorBase*& c : subObjArray) { c->Init(); }
}

void BananaBoss::PlayDamage(void)
{
	AnimePlay((int)ANIME_TYPE::DAMAGE, false);
}
