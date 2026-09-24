#include "OrangePlayer.h"

#include "../../../../../Application/Application.h"

#include "../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../Manager/Font/FontManager.h"

#include "../../CommonPlayerState/Move/PlayerMoveState.h"
#include "../../CommonPlayerState/TripleAttack/PlayerTripleAttackState.h"
#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"
#include "../../CommonPlayerState/Dodge/PlayerDodgeState.h"
#include "../../CommonPlayerState/Damage/PlayerDamageState.h"
#include "../../CommonPlayerState/Death/PlayerDeathState.h"

#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"



OrangePlayer::OrangePlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(
		operatorSenderId,

		"OrangeParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Orange/",
		"Orange/OrangeModel")
{
}


void OrangePlayer::PlayerLoad(void)
{

#pragma region モデル

	// アニメーション～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// 各種アニメーションを登録する
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED_TABLE);

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

#pragma endregion

#pragma region プレイヤーが抱える下位クラスを生成する

	// 三弾攻撃の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerTripleAttackCollOperator(
			SKILL_1_TARGET_SERCH_RANGE,
			COLLIDER_TAG::PLAYER_ATTACK,
			SKILL_1_COLL_SIZE_TABLE,
			SKILL_1_COLL_LOCAL_POS,
			SKILL_1_ATTACK_RATE_PERCENT,
			trans.pos, trans.angle,
			operatorSenderId,
			characterStats
		)
	);

	// キック用の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerSimpleAttackCollOperator(
			SKILL_2_TARGET_SERCH_RANGE,
			SKILL_2_COLL_TAG,
			SKILL_2_COLL_SIZE_TABLE,
			SKILL_2_COLL_LOCAL_POS,
			trans.pos, trans.angle,
			SKILL_2_ATTACK_RATE_PERCENT,
			operatorSenderId,
			characterStats
		)
	);

#pragma endregion


#pragma region 状態設定

	// 移動状態を追加する
	AddState(
		(int)STATE::MOVE,
		new PlayerMoveState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::MOVE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::MOVE; },
			// 定数（加算移動量 / 移動量の最大値 / ダッシュの移動量倍率 / スタミナ量 / 加速減衰量）
			DASH_SPEED_RATE, DASH_STAMINA_MAX, ATTENUATION,
			// 参照（移動量 / 横軸加速度の最大値 / 角度）
			accelSum, ACCEL_MAX, trans.angle, characterStats,
			// アニメーションの再生関数のポインタ（待機 / 歩き / 走り）
			[&]() { AnimePlay((int)ANIME_TYPE::IDLE); },
			[&]() { AnimePlay((int)ANIME_TYPE::WALK); },
			[&]() { AnimePlay((int)ANIME_TYPE::RUN); }
		)
	);

	// 三弾攻撃状態用のコンテキスト構造体を定義して設定～～～～～～～～～～～
	PlayerTripleAttackStateContext stateSkill1Context;

	// キー
	stateSkill1Context.ATTACK_KEY = KEY_TYPE::PLAYER_SKILL_1;
	// クールタイム
	stateSkill1Context.COOL_TIME = SKILL_1_COOL_TIME;
	// 攻撃が次段に繋がる猶予時間
	stateSkill1Context.ATTACK_NEXT_STAGE_CONTINUE_TIME = SKILL_1_ATTACK_NEXT_STAGE_CONTINUE_TIME;
	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	stateSkill1Context.COLL_START_TIME = SKILL_1_COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	stateSkill1Context.COLL_END_TIME = SKILL_1_COLL_END_TIME;
	// 攻撃中の移動速度
	stateSkill1Context.ATTACK_MOVE_SPEED = SKILL_1_ATTACK_MOVE_SPEED;

	// 当たり判定のオペレーター
	stateSkill1Context.collOperator = SubObjSerch<PlayerTripleAttackCollOperator>();
	
	// 座標
	stateSkill1Context.pos = &trans.pos;
	// 角度
	stateSkill1Context.angle = &trans.angle;

	// 攻撃1～3段目アニメーションの再生関数のポインタ
	stateSkill1Context.PlayAttackAnimes = {
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH1,false); },
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH2,false); },
		[&]() { AnimePlay((int)ANIME_TYPE::PUNCH3,false); }
	};
	// アニメーションの再生割合を取得する関数のポインタ
	stateSkill1Context.GetAnimePlayRatio = std::bind(&OrangePlayer::GetAnimeRatio, this);
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	stateSkill1Context.IsAnimeEnd = std::bind(&OrangePlayer::IsAnimeEnd, this);

	// 攻撃終了後の状態遷移関数のポインタ
	stateSkill1Context.DefaultChangeState = [&]() { state = (int)STATE::MOVE; };

	// ～～～～～～～～～～～三弾攻撃状態用のコンテキスト構造体を定義して設定

	// 三段攻撃状態を追加する
	AddState(
		(int)STATE::SKILL_1,	
		new PlayerTripleAttackState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_1); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// コンテキスト構造体
			stateSkill1Context
		)
	);

	// 攻撃状態を追加する (キック)
	AddState(
		(int)STATE::SKILL_2,
		new PlayerSimpleAttackState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_2); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_2; },
			// 定数（使用するキー / クールタイム / 攻撃の判定を発生させる 開始 / 終了 時間（アニメーションの再生割合）/ 攻撃中の移動速度）
			KEY_TYPE::PLAYER_SKILL_2, SKILL_2_COOL_TIME, SKILL_2_COLL_START_TIME, SKILL_2_COLL_END_TIME, SKILL_2_ATTACK_MOVE_SPEED,
			// 当たり判定のオペレーター
			*SubObjSerch<PlayerSimpleAttackCollOperator>(),
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICK, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::SKILL_3,
		new PlayerDodgeState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::SKILL_3); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_3; },
			// 定数（使用するキー / クールタイム / 回避中の移動速度 / 無敵時間の 開始 / 終了時間（アニメーションの再生割合
			KEY_TYPE::PLAYER_SKILL_3, SKILL_3_COOL_TIME, SKILL_3_MOVE_SPEED,
			SKILL_3_INVI_START_TIME, SKILL_3_INVI_END_TIME,
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DODGE, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 無敵時間のセット関数
			std::bind(&OrangePlayer::SetInviCounter, this, std::placeholders::_1),
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::DAMAGE,
		new PlayerDamageState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::DAMAGE); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::DAMAGE; },
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DAMAGE, false); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 無敵時間のセット関数
			[&]() { SetInviCounter(DODGE_INVI_TIME); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// 死亡状態を追加する
	AddState(
		(int)STATE::DEATH,
		new PlayerDeathState(
			// 自分の状態に関する関数
			[&]() { ChangeState((int)STATE::DEATH); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::DEATH; },
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーション終了判定ゲット関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 死亡アニメーション再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::DEATH, false); },
			// 死亡後の設定関数のポインタ
			[&]() { PlayerDeathSetting(); },
			// 攻撃終了後の状態遷移関数のポインタ
			[&]() { Net::GetIns().GetConnectStatus().EntryCount() > 1 ? ChangeState((int)STATE::OTHER_WATCH) : ChangeState((int)STATE::MOVE);  }
		)
	);

	// 遷移条件の登録（before = 遷移元)(after = 遷移後）
	auto AddChangeStateCondition = [&](STATE before, STATE after)->void {
		GetStateIns((int)before).AddOtherStateCondition([this, after](void) { GetStateIns((int)after).OwnStateConditionUpdate(); });
		};

	// 移動状態 -> スキル1 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_1);
	// 移動状態 -> スキル2 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_2);
	// 移動中 -> スキル3の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_3);

#pragma endregion

#pragma region UIの登録と設定

	// HPUIの座標設定
	int number = 0;
	if (isOwnOperator) {
		number = 0;
	}
	else {
		// 操作者だけ一番上に表示、それ以外の人のHPは下に描画
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			if (Net::GetIns().GetSenderId() == (MSG_SENDER_ID)id) { continue; }
			number++;
			if (operatorSenderId == (MSG_SENDER_ID)id) { break; }
		}
	}

	// HPの登録
	ui_ArrayIns.emplace_back(
		new CharacterHpUI(
			characterStats.hp,
			characterStats.hpMax.Value(),
			HP_FRAME_IMAGE_NAME,
			HP_IMAGE_NAME,
			HP_LOST_IMAGE_NAME,
			"OrangeIcon",
			"P" + std::to_string((int)operatorSenderId + 1) + "Frame",
			"P" + std::to_string((int)operatorSenderId + 1) + "Back",
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS[number],
			FILE_PATH_TYPE::PLAYER_HP,
			""
		)
	);

	// 自分が操作者かどうか
	if (isOwnOperator) {

		// スタミナのUI登録
		ui_ArrayIns.emplace_back(
			new PlayerStaminaUI(
				dynamic_cast<PlayerMoveState*>(&GetStateIns((int)STATE::MOVE))->GetDashStamina(),
				DASH_STAMINA_MAX
			)
		);

		// スキル1のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL1_UI_DRAW_POS,
				dynamic_cast<PlayerTripleAttackState*>(&GetStateIns((int)STATE::SKILL_1))->GetCoolTimeCounter(),
				SKILL_1_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::YELLOW,
				"SkillSlotTripleAttack"
			)
		);

		// スキル2のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				dynamic_cast<PlayerSimpleAttackState*>(&GetStateIns((int)STATE::SKILL_2))->GetCoolTimeCounter(),
				SKILL_2_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::RED,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル3のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL3_UI_DRAW_POS,
				dynamic_cast<PlayerDodgeState*>(&GetStateIns((int)STATE::SKILL_3))->GetCoolTimeCounter(),
				SKILL_3_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::BLUE,
				"SkillSlotDodge"
			)
		);
	}

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));

#pragma endregion 
}

void OrangePlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	// 回避時の当たり判定処理(回避成功したら、ダメージ処理をスルーする)
	if (PlayerBase::DodgeOnCollision(STATE::SKILL_3, other)) { return; }

	// 敵の攻撃を受けた時のダメージ処理
	PlayerBase::OnCollision(ownTag, other, collisionPoint);
}

void OrangePlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(operatorSenderId)) {
		state = dataPtr->state;

		switch ((STATE)state) {
		case PlayerBase::STATE::SKILL_1: {
			SubObjSerch<PlayerTripleAttackCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_2: {
			SubObjSerch<PlayerSimpleAttackCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_3: {
			break;
		}
		case PlayerBase::STATE::DEATH: {
			PlayerDeathSetting();
			break;
		}

		default: { break; }
		}

		delete dataPtr;
	}

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(operatorSenderId)) {
		
		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_1:
		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_2:
		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_3: {
			// 三段攻撃
			if (dataPtr->isCollider) { SubObjSerch<PlayerTripleAttackCollOperator>()->CollOn((PLAYER_TRIPLE_ATTACK_STAGE)dataPtr->collKinds); }
			else { SubObjSerch<PlayerTripleAttackCollOperator>()->CollOff((PLAYER_TRIPLE_ATTACK_STAGE)dataPtr->collKinds); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack: {
			// キック
			if (dataPtr->isCollider) { SubObjSerch<PlayerSimpleAttackCollOperator>()->CollOn(); }
			else { SubObjSerch<PlayerSimpleAttackCollOperator>()->CollOff(); }
			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}
}

void OrangePlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}



