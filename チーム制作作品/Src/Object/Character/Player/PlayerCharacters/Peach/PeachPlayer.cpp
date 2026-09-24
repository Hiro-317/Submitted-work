#include "PeachPlayer.h"

#include "../../CommonPlayerState/Move/PlayerMoveState.h"
#include "../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackState.h"
#include "../../CommonPlayerState/SingleModifier/PlayerSingleModifierState.h"
#include "../../CommonPlayerState/Damage/PlayerDamageState.h"
#include "../../CommonPlayerState/Death/PlayerDeathState.h"

#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../../UI/ToPlayerTargetUI/ToPlayerTargetUI.h"
#include "../../../../UI/HitUI/HitUI.h"

PeachPlayer::PeachPlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(operatorSenderId,

		"PeachParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Peach/",
		"Peach/PeachModel"
	)
{
}

void PeachPlayer::PlayerLoad(void)
{
	// 影を消す（消さなかったら、変な色合いになるので）
	MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
#pragma region 下位オブジェクトの生成
	
	// パンチ用の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerSimpleAttackCollOperator(
			SKILL_1_TARGET_SERCH_RANGE,
			SKILL_1_COLL_TAG,
			SKILL_1_COLL_SIZE_TABLE,
			SKILL_1_COLL_LOCAL_POS,
			trans.pos, trans.angle,
			SKILL_1_ATTACK_RATE_PERCENT,
			operatorSenderId,
			characterStats
		)
	);

	// 回復用の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerSingleModifierCollOperator(
			COLLIDER_TAG::PLAYER_HEAL,
			operatorSenderId,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerHeal,
			targetPlayerPos,
			100
		)
	);

	// 回復用の当たり判定オペレーターを生成する
	subObjArray.emplace_back(
		new PlayerSingleModifierCollOperator(
			COLLIDER_TAG::PLAYER_BUFF,
			operatorSenderId,
			MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerBuff,
			targetPlayerPos,
			80,
			150,
			ModifierType::PeachPlayerSkillBuff
		)
	);

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED);
	
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


	// 攻撃状態を追加する (キック)
	AddState(
		(int)STATE::SKILL_1,
		new PlayerSimpleAttackState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_1); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_1; },
			// 定数（使用するキー / クールタイム / 攻撃の判定を発生させる 開始 / 終了 時間（アニメーションの再生割合）/ 攻撃中の移動速度）
			KEY_TYPE::PLAYER_SKILL_1, SKILL_1_COOL_TIME, SKILL_1_COLL_START_TIME, SKILL_1_COLL_END_TIME, SKILL_1_ATTACK_MOVE_SPEED,
			// 当たり判定のオペレーター
			*SubObjSerch<PlayerSimpleAttackCollOperator>(),
			// 座標 / 角度
			trans.pos, trans.angle,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::PUNCH, false); },
			// アニメーションの再生割合を取得する関数のポインタ / アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return GetAnimeRatio(); }, [&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::SKILL_2,
		new PlayerSingleModifierState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_2); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_2; },
			*SubObjSerch<PlayerSingleModifierCollOperator>(),
			0.3f, KEY_TYPE::PLAYER_SKILL_2, 600,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::HEAL, false); },
			// アニメーションの再生割合のゲット関数ポインタ
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::SKILL_3,
		new PlayerSingleModifierState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_3); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_3; },
			*SubObjSerch<PlayerSingleModifierCollOperator>(1),
			0.3f, KEY_TYPE::PLAYER_SKILL_3, 60,
			// アニメーションの再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::HEAL, false); },
			// アニメーションの再生割合のゲット関数ポインタ
			[&]() { return GetAnimeRatio(); },
			// アニメーションの終了フラグを取得する関数のポインタ
			[&]() { return IsAnimeEnd(); },
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

	state;

	// 遷移条件の登録（before = 遷移元)(after = 遷移後）
	auto AddChangeStateCondition = [&](STATE before, STATE after)->void {
		GetStateIns((int)before).AddOtherStateCondition([this, after](void) { GetStateIns((int)after).OwnStateConditionUpdate(); });
		};

	//// 移動状態 -> スキル1 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_1);
	//// 移動状態 -> スキル2 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_2);
	//// 移動状態 -> スキル3 の遷移を登録
	AddChangeStateCondition(STATE::MOVE, STATE::SKILL_3);

#pragma endregion 

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
			"PeachIcon",
			"P" + std::to_string((int)operatorSenderId + 1) + "Frame",
			"P" + std::to_string((int)operatorSenderId + 1) + "Back",
			HP_IMAGE_SIZE,
			HP_GAUGE_OFFSET,
			HP_UI_POS[number],
			FILE_PATH_TYPE::PLAYER_HP,
			""
		)
	);

	// このプレイヤーの操作者だけ通す
	if (isOwnOperator)
	{
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
				dynamic_cast<PlayerSimpleAttackState*>(&GetStateIns((int)STATE::SKILL_1))->GetCoolTimeCounter(),
				SKILL_1_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::YELLOW,
				"SkillSlotSimpleAttack"
			)
		);

		// スキル2のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				dynamic_cast<PlayerSingleModifierState*>(&GetStateIns((int)STATE::SKILL_2))->GetCoolTimeCounter(),
				600,
				PlayerSkillUI::SKILL_UI_COLOR::GREEN,
				"SkillSlotHeal"
			)
		);

		// スキル3のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL3_UI_DRAW_POS,
				dynamic_cast<PlayerSingleModifierState*>(&GetStateIns((int)STATE::SKILL_3))->GetCoolTimeCounter(),
				60,
				PlayerSkillUI::SKILL_UI_COLOR::INDIGO,
				"SkillSlotMoveSpeedBuff"
			)
		);

		ui_ArrayIns.emplace_back(new ToPlayerTargetUI(targetIconPos));
	}

	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
}

void PeachPlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	// 敵の攻撃を受けた時のダメージ処理
	PlayerBase::OnCollision(ownTag, other, collisionPoint);
}

void PeachPlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(operatorSenderId)) {

		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack: {
			// 通常攻撃
			if (dataPtr->isCollider) { SubObjSerch<PlayerSimpleAttackCollOperator>()->CollOn(); }
			else { SubObjSerch<PlayerSimpleAttackCollOperator>()->CollOff(); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerHeal: {
			// 回復
			if (dataPtr->isCollider) { SubObjSerch<PlayerSingleModifierCollOperator>()->CollOn(); }
			else { SubObjSerch<PlayerSingleModifierCollOperator>()->CollOff(); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::PeachPlayerBuff: {
			// スタンプ
			if (dataPtr->isCollider) { SubObjSerch<PlayerSingleModifierCollOperator>(1)->CollOn(); }
			else { SubObjSerch<PlayerSingleModifierCollOperator>(1)->CollOff(); }

			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(operatorSenderId)) {
		state = dataPtr->state;

		switch ((STATE)state) {
		case PlayerBase::STATE::SKILL_1: {
			SubObjSerch<PlayerSimpleAttackCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_2: {
			SubObjSerch<PlayerSingleModifierCollOperator>()->ResetIsHit();
			break;
		}
		case PlayerBase::STATE::SKILL_3: {
			SubObjSerch<PlayerSingleModifierCollOperator>(1)->ResetIsHit();
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
}

void PeachPlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}
