#include "GrapeBoss.h"

#include "../../../../Application/Application.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Sound/SoundManager.h"

#include "Weapon/GrapeBossWeaponManager.h"
#include "Weapon/Straight/GrapeBossStraight.h"

#include "../../../Common/Collider/LineCollider.h"
#include "../../../Common/Collider/CapsuleCollider.h"
#include "../../../Common/Collider/XZCircleCollider.h"

#include "State/Idle/GrapeBossIdleState.h"
#include "State/Move/GrapeBossMoveState.h"
#include "State/KickDown/GrapeBossKickDownState.h"
#include "State/KickDown/GrapeKickDownCollOperator.h"
#include "State/Straight/GrapeBossStraightState.h"
#include "State/Stamp/GrapeBossStampState.h"
#include "State/Stamp/GrapeStampCollOperator.h"
#include "State/Single/GrapeBossSingleState.h"
#include "State/Stalker/GrapeBossStalkerState.h"
#include "State/Random/GrapeBossRandomState.h"
#include "State/Death/GrapeBossDeathState.h"

#include "../../../../Scene/Game/GameScene.h"

#include "../../../../Manager/Effect/EffectManager.h"

#include "../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../UI/HitUI/HitUI.h"


GrapeBoss::GrapeBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerDeath) :
	BossBase(
		"Data/Parameter/Character/Boss/Grape/",
		"Grape/GrapeModel",

		playerPos, playerDeath)
{

	coolTime = 120;
	Snd::GetIns().AddScene("GrapeBoss");
}

void GrapeBoss::PlayerLoad(void)
{
	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma region プレイヤーが抱える下位クラスを生成する

	subObjArray.push_back(new GrapeBossWeaponManager(operatorSenderId, characterStats));
	subObjArray.push_back(new GrapeKickDownCollOperator(operatorSenderId, characterStats));
	subObjArray.push_back(new GrapeStampCollOperator(operatorSenderId, characterStats));

#pragma endregion


#pragma region 状態設定

	AddState(
		static_cast<int>(STATE::IDLE),
		new GrapeBossIdleState(
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
			// 歩きアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::WALK, true); },
			// 移動への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::MOVE); },
			// 踏みつけへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); },
			// 投擲への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_B); },
			// スタンプへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_C); },
			// ひとつ追従への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_D); },
			// たくさん追従への状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_E); },
			// たくさんランダムへの状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_F); }
		)
	);
	AddState(
		static_cast<int>(STATE::MOVE),
		new GrapeBossMoveState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::MOVE); },
			// 移動量
			MOVE_SPEED,
			// 自分の座標と角度、プレイヤーの座標の読み取り
			trans.pos, trans.angle, playerPos,
			// ターゲット番号の取得
			[&]() { return targetNum; },
			// 走りアニメーションの再生
			[&]() { AnimePlay((int)ANIME_TYPE::RUN, true); },
			// 移動後攻撃に状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::ATTACK_A); }
		)
	);

	AddState(
		static_cast<int>(STATE::ATTACK_A),
		new GrapeBossKickDownState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_A); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_A); },
			// 自分の座標と角度とローカル角度
			trans.pos, trans.angle, MODEL_LOCAL_ROT,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::KickBomb),
			// モデルの拡大率
			MODEL_SCALE,
			// プレイヤーの座標の読み取り
			playerPos,
			// 当たり判定オペレータの参照
			SubObjSerch<GrapeKickDownCollOperator>(),
			// アニメーションの再生関数のポインタ
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICKDOWN, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = KICKDOWN_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_B),
		new GrapeBossStraightState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_B); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_B); },
			// 自分の座標と角度
			trans.pos, trans.angle,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::Straight),
			// プレイヤーの座標
			playerPos,
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::FORWARD_THROW, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = STRAIGHT_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_C),
		new GrapeBossStampState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_C); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_C); },
			// 自分の座標と角度
			trans.pos, trans.angle, isGround,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StampBomb),
			// プレイヤーの座標
			playerPos,
			SubObjSerch<GrapeStampCollOperator>(),
			// ジャンプ始めアニメーションのタイプ
			(int)ANIME_TYPE::JUMP_START,
			(int)ANIME_TYPE::STAMP,
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::JUMP_START, false); },
			[&]() { AnimePlay((int)ANIME_TYPE::JUMP_LOOP, true); },
			[&]() { AnimePlay((int)ANIME_TYPE::STAMP, false); },
			// アニメーションのスタンプタイプ
			[&]() { return GetAnimePlayType(); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// ボス自体のコライダの消滅
			[&]() { SetJudge(false); },
			// ボス自体のコライダの再生
			[&]() { SetJudge(true); },
			// クールタイムの設定
			[&]() { coolTime = STAMP_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_D),
		new GrapeBossSingleState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_D); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_D); },
			// プレイヤーの座標と角度
			playerPos,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::SingleBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = SINGLE_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_E),
		new GrapeBossStalkerState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_E); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_E); },
			// プレイヤーの座標と角度
			playerPos,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StalkerBomb),
			// プレイヤーのターゲット番号
			[&]() { return targetNum; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = STAMP_COOLTIME; }
		)
	);
	AddState(
		static_cast<int>(STATE::ATTACK_F),
		new GrapeBossRandomState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::ATTACK_F); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::ATTACK_F); },
			// 座標の参照
			trans.pos,
			// 攻撃の種類の情報
			SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::RandomBomb),
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::TOSS, false); },
			// アニメーションの再生割合を取得する関数のポインタ 
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { ChangeState((int)STATE::IDLE); },
			// クールタイムの設定
			[&]() { coolTime = RANDOM_COOLTIME; }
		)
	);

	AddState(
		static_cast<int>(STATE::DEATH),
		new GrapeBossDeathState(
			// 自分の状態に遷移する関数
			[&]() { state = static_cast<int>(STATE::DEATH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == static_cast<int>(STATE::DEATH); },
			// ボス死亡アニメーション
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH, false); },
			// アニメーションの全体時間
			[&]() { return GetAnimeTotalTime(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 死んだフラグを立てる
			[&]() {	SetIsDeath(true); }
		)
	);

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
			"","","",
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

void GrapeBoss::ReceptionUpdate(void)
{
	BossBase::ReceptionUpdate();

	// エリアの同期
	while (MsgDataBossAttackDraw* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAttackDraw>(operatorSenderId)) {

		// 受け取ったステートの描画
		switch (dataPtr->inform)
		{
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA:
		{
			SubObjSerch<GrapeKickDownCollOperator>()->SetPos(dataPtr->pos);
			SubObjSerch<GrapeKickDownCollOperator>()->SetScale(dataPtr->scale);
			break;
		}
		case MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC:
		{
			SubObjSerch<GrapeStampCollOperator>()->SetPos(dataPtr->pos);
			SubObjSerch<GrapeStampCollOperator>()->SetScale(dataPtr->scale);
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
			SubObjSerch<GrapeKickDownCollOperator>()->SetDrawArea(dataPtr->flg);
			break;
		}
		case MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC:
		{
			if (dataPtr->flg) {
				SubObjSerch<GrapeStampCollOperator>()->DrawAreaOn();
			}
			else {
				SubObjSerch<GrapeStampCollOperator>()->Off();
			}
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
	while (MsgDataBossBombInform* dataPtr = Net::GetIns().GetMsgData<MsgDataBossBombInform>(operatorSenderId)) {

		// 受け取ったボムの起動
		switch (dataPtr->inform)
		{
		case MsgDataBossBombInform::INFORM_TYPE::Straight:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::Straight)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			dynamic_cast<GrapeBossStraight*>(weapon.weaponIns)->SetStartDir(dataPtr->moveDir);
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::KickBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::KickBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::StampBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StampBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::SingleBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::SingleBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true; 
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::StalkerBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::StalkerBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true; 
			break;
		}
		case MsgDataBossBombInform::INFORM_TYPE::RandomBomb:
		{
			auto& weapon = SubObjSerch<GrapeBossWeaponManager>()->GetWeapons(WeaponType::RandomBomb)[dataPtr->index];
			weapon.weaponIns->SetStartPos(dataPtr->pos);
			weapon.weaponIns->SetViewPosCircle();
			weapon.live = true;
			break;
		}
		default:
			break;
		}

		delete dataPtr;
	}
}

void GrapeBoss::PlayDamage(void)
{
	AnimePlay((int)ANIME_TYPE::DAMAGE, false);
}
