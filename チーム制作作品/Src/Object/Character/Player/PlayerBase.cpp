#include "PlayerBase.h"

#include "../../../Manager/Font/FontManager.h"
#include "../../../Manager/Effect/EffectManager.h"

#include "CommonPlayerState/OtherPlayerWatch/OtherPlayerWatchState.h"
#include "CommonPlayerState/Move/PlayerMoveState.h"

#include "../../UI/HitUI/HitUI.h"

#include "../../Common/Collider/LineCollider.h"
#include "../../Common/Collider/CapsuleCollider.h"


PlayerBase::PlayerBase(

	short HP_MAX,
	short ATTACK_POWER,
	short DEFENSE_POWER,
	short SPEED_POWER,

	std::string modelPath
) :
	CharacterBase(
		HP_MAX,
		ATTACK_POWER,
		DEFENSE_POWER,
		SPEED_POWER),
	otherPlayerTrans()
{
	trans.Load(("Character/" + modelPath).c_str());
	this->operatorSenderId = operatorSenderId;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
}

PlayerBase::PlayerBase(
	MSG_SENDER_ID operatorSenderId,

	const std::string fileName,
	const std::string hpParameterID,
	const std::string attackPowerParameterID,
	const std::string defensePowerParameterID,
	const std::string moveSpeedParameterID,
	const std::string& parameterPath,

	std::string modelPath
) :
	CharacterBase(
		fileName,
		hpParameterID,
		attackPowerParameterID,
		defensePowerParameterID,
		moveSpeedParameterID,
		parameterPath
	),
	otherPlayerTrans(),
	bossPos(nullptr)
{
	trans.Load(("Character/" + modelPath).c_str());

	this->operatorSenderId = operatorSenderId;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
}

void PlayerBase::CharacterLoad(void)
{
	PlayerLoad();

	if(Net::GetIns().GetConnectStatus().EntryCount() != 1) {
		// 観戦モード
		AddState(
			(int)STATE::OTHER_WATCH,
			new OtherPlayerWatchState(
				// 自分の状態に遷移する関数
				[&]() { ChangeState((int)STATE::OTHER_WATCH); },
				// 自分の状態かどうかを返す関数
				[&]() { return state == (int)STATE::OTHER_WATCH; },
				// 他プレイヤーの座標
				otherPlayerTrans,
				// ボスの座標
				bossPos
			)
		);
	}

#pragma region モデル
	// モデルを読み込む

	// モデルのスケールを設定
	trans.scale = MODEL_SCALE;

	// モデルの中心点のズレ
	trans.centerDiff = MODEL_CENTER_DIFF;

	// 角度のズレ(ローカル回転)
	trans.localAngle = MODEL_LOCAL_ROT;

#pragma endregion

#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

	// 押し出す力の大きさを設定する
	SetPushWeight(COLLISION_PUSH_WEIGHT);

	// 無敵中に赤点滅を表示しない
	SetInviEffectFlg(false);

#pragma endregion

#pragma region 当たり判定情報設定

	// 当たり判定を生成する（線分コライダー）
	ColliderCreate(new LineCollider(COLLIDER_TAG::PLAYER, LINE_COLLIDER_START_POS, LINE_COLLIDER_END_POS));

	// 当たり判定を生成する（カプセルコライダー）
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::PLAYER,
			CAPSULE_COLLIDER_START_POS, CAPSULE_COLLIDER_END_POS,
			CAPSULE_COLLIDER_RADIUS
		)
	);

#pragma endregion

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
}

void PlayerBase::CharacterInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS + Vector3::Xonly(150.0f) * (float)operatorSenderId;

	// 初期状態を移動状態にする
	state = (int)STATE::MOVE;

	for (ActorBase*& c : subObjArray) { c->Init(); }

	TargetPlayerNext();
}

void PlayerBase::CharacterUpdate(void)
{
	// 自身が死亡していたら更新処理は行わない
	if (GetIsDeath()) { return; }

	interestPos = trans.pos + INTEREST_POS;

	// 下位クラスの更新処理
	for (ActorBase*& c : subObjArray) { c->Update(); }

	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_TO_PLAYER_TARGET).down) {
		TargetPlayerNext();
	}

	// HPがゼロ以下になったら死亡状態に遷移
	if (characterStats.hp <= 0 && state != (int)STATE::DEATH) {
		ChangeState((int)STATE::DEATH);
	}

#ifdef _DEBUG		// クールタイム用
	//if (CheckHitKey(KEY_INPUT_0)) {
	//	short damage = 10;
	//	characterStats.hp -= damage;
	//	// プレイヤーが受けるダメージ値を、クライアント側に送信
	//	Net::GetIns().Send(MsgDataPlayerDamage(damage), operatorSenderId);
	//}
#endif // _DEBUG
}

void PlayerBase::CharacterRemoteUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }
}

void PlayerBase::CharacterDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void PlayerBase::CharacterAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void PlayerBase::CharacterUiDraw(void)
{
	// デバッグ用描画
	if (App::GetIns().IsDrawDebug()) {
		int number = 0;
		if (isOwnOperator) {
			number = 0;
		}
		else {
			for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
				if (Net::GetIns().GetSenderId() == (MSG_SENDER_ID)id) { continue; }
				number++;
				if (operatorSenderId == (MSG_SENDER_ID)id) { break; }
			}
		}

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 150; int xPos[4] = { 0, 400, 600, 900 };  const int FONT_SIZE = 20;

		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(xPos[number], yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		//debugDrwStr("プレイヤー～～～～～～");
		//debugDrwStr("座標" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));
		//debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		//debugDrwStr("スタミナ:" + std::to_string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).GetDashStamina()));
		//debugDrwStr("息切れ:" + std::string(dynamic_cast<PlayerMoveState&>(GetStateIns((int)STATE::MOVE)).IsTired() ? "true" : "false"));
		//debugDrwStr("HP：" + std::to_string(characterStats.hp));
		//debugDrwStr("～～～～～～('#；ω;`)");
	}
}

void PlayerBase::CharacterRelease(void)
{
	for (ActorBase*& c : subObjArray) {
		if (c) {
			c->Release();
			delete c;
			c = nullptr;
		}
	}
	subObjArray.clear();
}

void PlayerBase::ChangeState(int state)
{
	// 状態遷移
	CharacterBase::ChangeState(state);

	// 遷移するステート(状態)を送信
	if (isOwnOperator) { Net::GetIns().Send(MsgDataPlayerState(state)); }
}

void PlayerBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	// 判定そのものはホストがすべて請け負う
	if (!Net::GetIns().IsHost()) { return; }

	// 死亡状態の場合何もしない
	if (state == (int)STATE::DEATH) { return; }

	// 衝突物のコライダータグを取得
	COLLIDER_TAG otherColliderTag = other.GetTag();

#pragma region プラス効果の判定（無敵判定を無視して発動）

	switch (otherColliderTag) {
	case COLLIDER_TAG::PLAYER_HEAL: {	// 回復

		// 回復量を取得
		short heal = other.GetSkillStats().Power();

		// 回復をクライアントへ送信
		Net::GetIns().Send(MsgDataPlayerHeal(other.GetSkillStats().Power()), operatorSenderId);

		// 回復させる
		characterStats.HpHeal(heal);
		
		// エフェクト発生
		EffectManager::GetIns()->CreateEffect(EFFECT_NAME::HEAL, 0, &trans);
		return;
	}
	case COLLIDER_TAG::PLAYER_BUFF: {	// バフ

		// バフデータ構造体を取得
		const ModifierData& modifier = other.GetSkillStats().ModifierPower();

		// バフをクライアントへ送信
		Net::GetIns().Send(MsgDataPlayerModifier(modifier), operatorSenderId);
		
		// バフをかける
		characterStats.AddModifier(modifier);

		// エフェクト発生
		EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BUFF, 0, &trans);
		return;
	}
	}

#pragma endregion

	// 無敵時間の判定
	if (GetInviCounter() > 0) { return; }

#pragma region マイナス効果の判定

	switch (otherColliderTag) {
	case COLLIDER_TAG::BOSS_ATTACK: {	// 被ダメージ

		// ダメージ状態に遷移
		ChangeState((int)STATE::DAMAGE);

		// ボスの攻撃力とプレイヤーの防御力で、最終的なダメージ値を計算
		const short damage = CalculateDamage(other.GetSkillStats().Power(), characterStats.defensePower.Value());

		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), operatorSenderId);

		// ダメージ値分HPを減らす
		characterStats.hp -= damage;

		return;
	}
	}

#pragma endregion
}

void PlayerBase::AnimePlay(int type, bool loop)
{
	// 自身の操作者プレイヤーの更新により、呼び出された再生の場合、
	// 自身のPC以外のすべてに再生したことを送信する
	if (isOwnOperator) { Net::GetIns().Send(MsgDataPlayerAnimeType(type, loop)); }

	CharacterBase::AnimePlay(type, loop);
}
void PlayerBase::TargetPlayerNext(void)
{
	// ターゲット選択処理
	targetPlayerIndex++;

	// Indexがプレイヤー人数を超過した場合、0に戻す
	if (targetPlayerIndex >= Net::GetIns().GetConnectStatus().EntryCount()) {
		targetPlayerIndex = 0;
	}

	// ターゲットが自分なら、自分の座標を入れる。
	// 他プレイヤーがターゲットなら、そのプレイヤーの座標を代入
	if (targetPlayerIndex == 0) { targetPlayerPos = &trans.pos; }
	else {
		if (otherPlayerTrans.size() >= targetPlayerIndex) {
			targetPlayerPos = &otherPlayerTrans.at(targetPlayerIndex - 1)->pos;
		}
	}

	targetIconPos = HP_UI_POS[targetPlayerIndex] + ICON_LOCAL_POS;

	if (!Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataPlayerToPlayerTarget((unsigned char)targetPlayerIndex), operatorSenderId);
	}
}

void PlayerBase::ReceptionUpdate(void)
{
	// 座標・角度の同期
	while (MsgDataPlayerTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerTrans>(operatorSenderId)) {
		// 自分のキャラ（操作対象）の場合
		if (isOwnOperator) {
			// ホストから送られた座標と今の自分の座標の距離を測る
			float diff = (trans.pos - dataPtr->pos).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた座標に寄せる（補間）
				trans.pos = trans.pos * 0.9f + dataPtr->pos * 0.1f;
			}

			// ホストから送られた角度と今の自分の角度のずれを測る
			diff = (trans.angle - dataPtr->angle).Length();

			// 誤差が小さいなら無視
			if (diff > 0.5f) {
				// 誤差が大きい場合、少しずつホストから送られた角度を寄せる（補間）
				trans.angle = trans.angle * 0.9f + dataPtr->angle * 0.1f;
			}

		}
		// 他人のキャラなら、そのまま同期
		else { 
			trans.pos = dataPtr->pos; 
			trans.angle = dataPtr->angle;
		}


		delete dataPtr;
	}

	// アニメーション
	while (MsgDataPlayerAnimeType* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerAnimeType>(operatorSenderId, true)) {
		AnimePlay(dataPtr->animeType, dataPtr->loop);
		delete dataPtr;
	}

	// 攻撃を受けた時のダメージ
	while (MsgDataPlayerDamage* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerDamage>(operatorSenderId)) {
		characterStats.hp -= dataPtr->damage;
		ChangeState((int)STATE::DAMAGE);

		delete dataPtr;
	}

	// HP
	while (MsgDataPlayerHp* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerHp>(operatorSenderId)) {
		characterStats.hp = dataPtr->hp;
		delete dataPtr;
	}

	// 回復
	while (MsgDataPlayerHeal* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerHeal>(operatorSenderId)) {
		characterStats.HpHeal(dataPtr->heal);
		EffectManager::GetIns()->CreateEffect(EFFECT_NAME::HEAL, 0.0f, &trans);
		delete dataPtr;
	}

	// バフ
	while (MsgDataPlayerModifier* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerModifier>(operatorSenderId)) {
		characterStats.AddModifier(dataPtr->modifier);
		EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BUFF, 0.0f, &trans);
		delete dataPtr;
	}

	// 味方ターゲット変更
	while (MsgDataPlayerToPlayerTarget* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerToPlayerTarget>(operatorSenderId)) {

		targetPlayerIndex = dataPtr->targetIndex;

		// ターゲットが自分なら、自分の座標を入れる。
		// 他プレイヤーがターゲットなら、そのプレイヤーの座標を代入
		if (targetPlayerIndex == 0) { targetPlayerPos = &trans.pos; }
		else {
			if (otherPlayerTrans.size() >= targetPlayerIndex) {
				targetPlayerPos = &otherPlayerTrans.at(targetPlayerIndex - 1)->pos;
			}
		}

		delete dataPtr;
	}

	// 回避した時の「ミス！」表示
	while (MsgDataPlayerMissNotice* dataPtr = Net::GetIns().GetMsgData<MsgDataPlayerMissNotice>(operatorSenderId)) 
	{
		if (!Net::GetIns().IsHost()) {
			if (dataPtr->playerNo == Net::GetIns().GetSenderId()) {
				SubUiSerch<HitUI>()->MissSetting();
			}
		}
		
		delete dataPtr;
	}
}

void PlayerBase::SendUpdate(void)
{
	if (Net::GetIns().IsHost() || isOwnOperator) {
		// 自身の座標と角度を
		Net::GetIns().Send(MsgDataPlayerTrans(trans.pos, trans.angle), operatorSenderId);
	} 
}