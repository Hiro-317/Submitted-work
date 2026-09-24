#include "GrapePlayer.h"

#include "../../CommonPlayerState/Move/PlayerMoveState.h"
#include "../../CommonPlayerState/Damage/PlayerDamageState.h"
#include "../../CommonPlayerState/Death/PlayerDeathState.h"
#include "GrapeUniqueState/Bomb/GrapePlayerBombState.h"
#include "GrapeUniqueState/Shot/GrapePlayerShotState.h"
#include "GrapeUniqueState/Throw/GrapePlayerThrowState.h"

#include "GrapeUniqueState/Bomb/GrapePlayerBombCollOperator.h"
#include "GrapeUniqueState/Shot/GrapePlayerShotCollOperator.h"
#include "GrapeUniqueState/Throw/GrapePlayerThrowCollOperator.h"

#include "../../../../UI/PlayerStaminaUI/PlayerStaminaUI.h"
#include "../../../../UI/CharacterHpUI/CharacterHpUI.h"
#include "../../../../UI/PlayerSkillUI/PlayerSkillUI.h"
#include "../../../../UI/HitUI/HitUI.h"

GrapePlayer::GrapePlayer(MSG_SENDER_ID operatorSenderId) :
	PlayerBase(operatorSenderId,

		"GrapeParameter",
		"PlayerHP",
		"PlayerAttackPower",
		"PlayerDefensePower",
		"PlayerMoveSpeed",

		"Data/Parameter/Character/Player/Grape/",
		"Grape/GrapeModel"
	)
{
}

void GrapePlayer::PlayerLoad(void)
{
#pragma region 下位オブジェクトの生成

	// アニメーションコントローラーを生成する
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::MAX, ANIME_SPEED);

	// 巨砲の生成
	subObjArray.emplace_back(
		new GrapePlayerShotCollOperator(
			COLLIDER_TAG::PLAYER_ATTACK,
			SKILL1_ATTACK_RATE,
			trans.pos, trans.angle,
			operatorSenderId,
			characterStats
		)
	);

	// BUDOOOON!の生成
	subObjArray.emplace_back(
		new GrapePlayerThrowCollOperator(
			COLLIDER_TAG::PLAYER_ATTACK,
			SKILL2_ATTACK_RATE,
			trans.pos, trans.angle,
			operatorSenderId,
			characterStats
		)
	);

	// ポリフェノールインパクトの生成
	subObjArray.emplace_back(
		new GrapePlayerBombCollOperator(
			COLLIDER_TAG::PLAYER_ATTACK,
			SKILL3_ATTACK_RATE,
			// 爆発するまでの時間
			ATTACK_COUNT_TIME,
			// 爆弾の待機時間
			ATTACK_START_TIME,
			trans.pos, trans.angle,
			operatorSenderId,
			characterStats
		)
	);

#pragma endregion 

#pragma region 状態設定

	AddState(
		(int)STATE::SKILL_1,
		new GrapePlayerShotState(
			[&]() { ChangeState((int)STATE::SKILL_1); },
			[&]() { return state == (int)STATE::SKILL_1; },
			*SubObjSerch<GrapePlayerShotCollOperator>(),
			SKILL1_COOL_TIME,
			trans.pos, trans.angle,
			bossPos,
			[&]() { AnimePlay((int)ANIME_TYPE::THROW, false); },
			[&]() { return IsAnimeEnd(); },
			[&]() { return GetAnimeRatio(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	AddState(
		(int)STATE::SKILL_2,
		new GrapePlayerThrowState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_2); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_2; },
			// 当たり判定管理クラスのポインタ
			*SubObjSerch<GrapePlayerThrowCollOperator>(),
			// クールタイム
			SKILL2_COOL_TIME,
			// 座標 / 角度
			trans.pos, trans.angle,
			// ボスの座標
			bossPos,
			// アニメーション再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::THROW, false); },
			// アニメーション終了管理フラグのポインタ
			[&]() { return IsAnimeEnd(); },
			// アニメーション再生割合ゲット関数のポインタ
			[&]() { return GetAnimeRatio(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

	// 設置型爆弾
	AddState(
		(int)STATE::SKILL_3,
		new GrapePlayerBombState(
			// 自分の状態に遷移する関数
			[&]() { ChangeState((int)STATE::SKILL_3); },
			// 自分の状態かどうかを返す関数
			[&]() { return state == (int)STATE::SKILL_3; },
			// 設置爆弾の
			*SubObjSerch<GrapePlayerBombCollOperator>(),
			// クールタイム
			SKILL3_COOL_TIME,
			// プレイヤーの座標
			trans.pos,
			// アニメーション再生関数のポインタ
			[&]() { AnimePlay((int)ANIME_TYPE::KICK_DOWN, false); },
			// アニメーション終了フラグゲット関数のポインタ
			[&]() { return IsAnimeEnd(); },
			// 攻撃終了後の状態遷移関数のポインタ (今回は移動状態に遷移するようにする）
			[&]() { ChangeState((int)STATE::MOVE); }
		)
	);

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
			"GrapeIcon",
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
				dynamic_cast<GrapePlayerShotState*>(&GetStateIns((int)STATE::SKILL_1))->GetCoolTimeCounter(),
				SKILL1_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::YELLOW,
				"SkillSlotShot"
			)
		);

		// スキル2のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL2_UI_DRAW_POS,
				dynamic_cast<GrapePlayerThrowState*>(&GetStateIns((int)STATE::SKILL_2))->GetCoolTimeCounter(),
				SKILL2_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::PURPLE,
				"SkillSlotGrapeBombThrow"
			)
		);

		// スキル3のUI
		ui_ArrayIns.emplace_back(
			new PlayerSkillUI(
				SKILL3_UI_DRAW_POS,
				dynamic_cast<GrapePlayerBombState*>(&GetStateIns((int)STATE::SKILL_3))->GetCoolTimeCounter(),
				SKILL3_COOL_TIME,
				PlayerSkillUI::SKILL_UI_COLOR::PURPLE,
				"SkillSlotGrapeBombPut"
			)
		);
	}

	// ミスUIの生成
	ui_ArrayIns.emplace_back(new HitUI(trans.pos));
}

void GrapePlayer::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	// 敵の攻撃を受けた時のダメージ処理
	PlayerBase::OnCollision(ownTag, other, collisionPoint);
}

void GrapePlayer::ReceptionUpdate(void)
{
	PlayerBase::ReceptionUpdate();

	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerCollOperator>(operatorSenderId)) {

		switch (dataPtr->collKinds) {

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerBomb: {
			// 通常攻撃
			if (dataPtr->isCollider) { SubObjSerch<GrapePlayerBombCollOperator>()->CollOn(); }
			else { SubObjSerch<GrapePlayerBombCollOperator>()->CollOff(); }
			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerThrow: {
			// スタンプ
			if (dataPtr->isCollider) { SubObjSerch<GrapePlayerThrowCollOperator>()->CollOn(); }
			else { SubObjSerch<GrapePlayerThrowCollOperator>()->CollOff(); }

			break;
		}

		case MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerShot: {
			// 回復
			if (dataPtr->isCollider) { SubObjSerch<GrapePlayerShotCollOperator>()->CollOn(); }
			else { SubObjSerch<GrapePlayerShotCollOperator>()->CollOff(); }
			break;
		}

		default: { break; }	// 例外
		}

		delete dataPtr;
	}

	while (MsgDataPlayerState* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerState>(operatorSenderId)) {
		state = dataPtr->state;

		switch ((STATE)state) {
		case PlayerBase::STATE::DEATH: {
			PlayerDeathSetting();
			break;
		}

		default: { break; }
		}

		delete dataPtr;
	}

	while (MsgDataPlayerShotStart* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerShotStart>(operatorSenderId)) {
		
		// グレープショットの発射の瞬間で行う処理
		SubObjSerch<GrapePlayerShotCollOperator>()->RemoteShotStart(dataPtr->pos, dataPtr->vec);
		
		delete dataPtr;
	}

	while (MsgDataPlayerShotEnd* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerShotEnd>(operatorSenderId)) {

		// グレープショットの発射終了の瞬間で行う処理
		SubObjSerch<GrapePlayerShotCollOperator>()->RemoteShotEnd(dataPtr->pos);

		delete dataPtr;
	}

	while (MsgDataGrapePlayerBombStart* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerBombStart>(operatorSenderId)) {

		// グレープショットの発射の瞬間で行う処理
		SubObjSerch<GrapePlayerBombCollOperator>()->RemoteBombSetStart(dataPtr->pos);

		delete dataPtr;
	}

	while (MsgDataGrapePlayerBombEnd* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerBombEnd>(operatorSenderId)) {

		// グレープショットの発射終了の瞬間で行う処理
		SubObjSerch<GrapePlayerBombCollOperator>()->RemoteBombSetEnd();

		delete dataPtr;
	}

	while (MsgDataGrapePlayerBombThrowStart* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerBombThrowStart>(operatorSenderId)) {

		// グレープショットの発射終了の瞬間で行う処理
		SubObjSerch<GrapePlayerThrowCollOperator>()->RemoteThrowBombStart(dataPtr->pos, dataPtr->vec);

		delete dataPtr;
	}

	while (MsgDataGrapePlayerBombThrowEnd* dataPtr = Net::GetIns().GetMsgData<MsgDataGrapePlayerBombThrowEnd>(operatorSenderId)) {

		// グレープショットの発射終了の瞬間で行う処理
		SubObjSerch<GrapePlayerThrowCollOperator>()->RemoteThrowBombEnd();

		delete dataPtr;
	}
}

void GrapePlayer::SendUpdate(void)
{
	PlayerBase::SendUpdate();
}