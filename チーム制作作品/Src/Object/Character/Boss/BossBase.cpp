#include "BossBase.h"

#include "../../../Manager/Font/FontManager.h"
#include "../../../Manager/Sound/SoundManager.h"
#include "../../../Scene/Game/GameScene.h"

#include "../../UI/HitUI/HitUI.h"

#include "../../Common/Collider/XZCircleCollider.h"
#include "../../Common/Collider/LineCollider.h"


BossBase::BossBase(

	const std::string& parameterPath,

	std::string modelPath,

	const std::vector<const Vector3*> playerPos,
	const std::vector<const bool*> playerDeath
) :
	CharacterBase(
		"Parameter",
		"HP_" + std::to_string(playerDeath.size()),
		"AttackPower",
		"DefensePower",
		"MoveSpeed",
		parameterPath),
	
	playerPos(playerPos),
	playerDeath(playerDeath)
{
	trans.Load(("Character/" + modelPath).c_str());

	this->operatorSenderId = Net::HOST_SENDER_ID;
	isOwnOperator = operatorSenderId == Net::GetIns().GetSenderId();
	collParam = new ParameterLoad("Data/Parameter/AttackRange/");

	// 攻撃対象探し用の総ダメ保存
	targetNum = 0;
	Net::GetIns().Send(MsgDataBossTarget((unsigned char)targetNum));

	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		damaged.emplace_back(0);
	}
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		nowDeath.emplace_back(false);
	}
	mostDamaged = 0;
}

void BossBase::CharacterLoad(void)
{
#pragma region モデル
	// モデルを読み込む
	PlayerLoad();

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

	// 当たり判定を生成する（XZ平面上円コライダー）
	if (IsParameterExist("Collider", "ToPlayerDistanceRadius") && IsParameterExist("Collider", "ToPlayerDistanceHeight")) {
		ColliderCreate(
			new XZCircleCollider(
				COLLIDER_TAG::BOSS_DISTANCE,
				GetParameter("Collider", "ToPlayerDistanceRadius"),
				GetParameter("Collider", "ToPlayerDistanceHeight")
			)
		);
	}

	// 当たり判定を生成する（線分コライダー）
	if (IsParameterExist("Collider", "LineColliderStartPos") && IsParameterExist("Collider", "LineColliderEndPos")) {
		ColliderCreate(
			new LineCollider(
				COLLIDER_TAG::BOSS,
				GetParameterToVector3("Collider", "LineColliderStartPos") * MODEL_SCALE,
				GetParameterToVector3("Collider", "LineColliderEndPos") * MODEL_SCALE
			)
		);
	}

#pragma endregion

	// まとめて読み込み処理
	for (ActorBase*& c : subObjArray) { c->Load(); }
}

void BossBase::CharacterInit(void)
{
	// 位置を初期位置にする
	trans.pos = INIT_POS;
	trans.angle = INIT_ROT;

	// 初期状態を移動状態にする
	ChangeState(static_cast<int>(STATE::IDLE));

	for (ActorBase*& c : subObjArray) { c->Init(); }
}

void BossBase::CharacterUpdate(void)
{
	for (ActorBase*& c : subObjArray) { c->Update(); }

	// プレイヤーの生存判定をする
	for (int i = 0; i < playerDeath.size(); i++) {
		// 今死んだ場合
		if (*playerDeath.at(i) && !nowDeath.at(i)) {
			// 死んでる判定にする
			nowDeath[i] = true;

			// 最大ダメージの再判定
			mostDamaged = -1;
			for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
				if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
				// センダーIDと死んだ奴が同じならスキップ
				if (!nowDeath.at(id)) {
					// 今の最大ダメージを超えたダメージ蓄積をしたらターゲットを変える
					if (mostDamaged < damaged.at(id)) {
						mostDamaged = damaged.at(id);
						targetNum = id;
						Net::GetIns().Send(MsgDataBossTarget((unsigned char)targetNum));
					}
				}
			}
		}
	}

#ifdef _DEBUG		// クールタイム用
	if (CheckHitKey(KEY_INPUT_0)) {
		short damage = 10;
		characterStats.hp -= damage;
		// プレイヤーが受けるダメージ値を、クライアント側に送信
		Net::GetIns().Send(MsgDataPlayerDamage(damage), operatorSenderId);
	}
#endif // _DEBUG
}

void BossBase::CharacterRemoteUpdate()
{
	for (ActorBase*& c : subObjArray) { c->Update(); }
}

void BossBase::CharacterDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->Draw(); }
}

void BossBase::CharacterAlphaDraw(void)
{
	for (ActorBase*& c : subObjArray) { c->AlphaDraw(); }
}

void BossBase::CharacterUiDraw(void)
{
	if (App::GetIns().IsDrawDebug()) {

		// 1行ずつ描画するためのラムダ式（デバッグ用）
		int yPos = 300; const int FONT_SIZE = 20;
		auto debugDrwStr = [&](std::string str)->void {
			DrawStringToHandle(0, yPos, str.c_str(), 0xffffff, Font::GetIns().GetFont(FontKinds::DEFAULT_20));
			yPos += FONT_SIZE;
			};

		// 加速度をデバッグ表示
		debugDrwStr("ボス～～～～～～～～～");
		debugDrwStr("座標" + std::to_string(trans.pos.x) + ", " + std::to_string(trans.pos.y) + ", " + std::to_string(trans.pos.z));
		debugDrwStr("加速度:" + std::to_string(accelSum.Length()));
		debugDrwStr("地面か:" + std::to_string(isGround));
		debugDrwStr("～～～～～～～(|3[___]");
	}
}

void BossBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (!Net::GetIns().IsHost() || state == (int)STATE::DEATH) return;

	// ボスとプレイヤーの攻撃との当たり判定
	if (ownTag == COLLIDER_TAG::BOSS_DISTANCE) {
		switch (other.GetTag()) {
		case COLLIDER_TAG::PLAYER_ATTACK: {
			bool isClitical = false;
			short damage = CalculateDamage(other.GetSkillStats().Power(&isClitical), characterStats.defensePower.Value());

			// ホストならそのままUI描画
			if (other.GetSkillStats().operatorSenderId == Net::GetIns().GetSenderId()) {
				SubUiSerch<HitUI>()->DamageSetting(collisionPoint, damage, isClitical);
			}

			// ダメージ減らす
			characterStats.hp -= damage;
			// アイドルだとアニメーションの再生
			if (state == (int)STATE::IDLE) {
				PlayDamage();
			}
			// ダメージとクリティカル情報を送信する
			Net::GetIns().Send(MsgDataBossHit(collisionPoint, damage, isClitical), other.GetSkillStats().operatorSenderId);

			// HPがゼロ以下になったら死亡状態に遷移
			if (characterStats.hp <= 0 && state != (int)STATE::DEATH) {
				ChangeState((int)STATE::DEATH);
				return;
			}

			// 与ダメを足す
			damaged[static_cast<int>(other.GetSkillStats().operatorSenderId)] += damage;

			// 与ダメの最大値を探す
			for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
				if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
				// 今の最大ダメージを超えたダメージ蓄積をしたらターゲットを変える
				if (mostDamaged < damaged.at(id)) {
					mostDamaged = damaged.at(id);
					targetNum = id;
					Net::GetIns().Send(MsgDataBossTarget((unsigned char)targetNum));
				}
			}

			// ホスト時の演出
			if (other.GetSkillStats().operatorSenderId == Net::GetIns().GetSenderId()) {
				// クリティカルなら揺らしクリティカル音を出す
				if (isClitical) {
					GameScene::Shake(ShakeKinds::HIG, ShakeSize::SMALL, 10);
					Snd::GetIns().Play("CriticalDamaged");
				}
				// 違うなら普通にダメージ音
				else {
					Snd::GetIns().Play("Damaged");
				}
			}
			GameScene::HitStop(4);
			SetInviCounter(150);

			break;
		}
		}
	}
}

void BossBase::CharacterRelease(void)
{
	for (ActorBase*& c : subObjArray) {
		if (c) {
			c->Release();
			delete c;
			c = nullptr;
		}
	}
	subObjArray.clear();
	collParam->Release();
	delete collParam;
	collParam = nullptr;
}

void BossBase::AnimePlay(int type, bool loop)
{
	// ホストだったら操作者PC以外に伝達する
	if (Net::GetIns().IsHost()) {

		// すべてのIDを精査する
		for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
			// そのIDが未参加だったらスキップ(それ以降もないため「break」)
			if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }

			// 情報を送る
			Net::GetIns().Send(MsgDataBossAnimeType(type, loop));
		}
	}

	CharacterBase::AnimePlay(type, loop);
}
void BossBase::ReceptionUpdate(void)
{
	// 座標/角度の同期
	while (MsgDataBossTrans* dataPtr = Net::GetIns().GetMsgData<MsgDataBossTrans>(operatorSenderId)) {

		// 座標/角度を同期
		trans.pos = dataPtr->pos;
		trans.angle = dataPtr->angle;
		trans.scale = dataPtr->scale;
		delete dataPtr;
	}

	// アニメーション
	while (MsgDataBossAnimeType* dataPtr = Net::GetIns().GetMsgData<MsgDataBossAnimeType>(operatorSenderId)) {
		AnimePlay(dataPtr->animeType, dataPtr->loop);
		delete dataPtr;
	}

	// 死亡判定
	while (MsgDataBossIsDeath* dataPtr = Net::GetIns().GetMsgData<MsgDataBossIsDeath>(operatorSenderId)) {
		SetIsDeath(dataPtr->flg);
		delete dataPtr;
	}

	// ダメージ受信
	while (MsgDataBossHit* dataPtr = Net::GetIns().GetMsgData<MsgDataBossHit>()) {

		characterStats.hp -= dataPtr->damage;

		if (dataPtr->header.senderId == Net::GetIns().GetSenderId()) {
			SubUiSerch<HitUI>()->DamageSetting(dataPtr->collisionPoint, dataPtr->damage, dataPtr->clitical);
			// クリティカルなら揺らしクリティカル音
			if (dataPtr->clitical) {
				GameScene::Shake(ShakeKinds::DIAG, ShakeSize::SMALL, 10);
				Snd::GetIns().Play("CriticalDamaged");
			}
			// 違うなら普通にダメージ音
			else {
				Snd::GetIns().Play("Damaged");
			}
			GameScene::HitStop(4);
			SetInviCounter(150);
		}

		delete dataPtr;
	}
	// 標的の受信
	while (MsgDataBossTarget* dataPtr = Net::GetIns().GetMsgData<MsgDataBossTarget>()) {

		targetNum = (int)dataPtr->targetNum;

		delete dataPtr;
	}
}

void BossBase::SendUpdate(void)
{
	if (Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataBossTrans(trans.pos, trans.angle, trans.scale));
	}
}