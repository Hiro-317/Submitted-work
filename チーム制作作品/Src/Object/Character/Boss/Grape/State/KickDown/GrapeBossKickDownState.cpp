#include "GrapeBossKickDownState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

GrapeBossKickDownState::GrapeBossKickDownState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& pos, Vector3& angle, const Vector3 LOCAL_ROT,
	BombType* bombType,
	const Vector3& MODEL_SCALE,
	const std::vector<const Vector3*> playerPos,
	GrapeKickDownCollOperator* collOperator,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle), LOCAL_ROT(LOCAL_ROT),
	bombType(bombType),
	MODEL_SCALE(MODEL_SCALE),
	playerPos(playerPos),
	collOperator(collOperator),
	GetTarget(GetTarget),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime),
	cnt(0), first(false), target(0)
{
}

void GrapeBossKickDownState::Enter(void)
{
	PlayAttackAnim();
	first = true;
	cnt = 0;
	SetCoolTime();
	target = GetTarget();
	angle.y = atan2f(playerPos.at(target)->x - pos.x, playerPos.at(target)->z - pos.z);
	attackPos = Vector3::XZonly(pos.x, pos.z) + (FOOT_VIEW_POS * MODEL_SCALE).TransMat(MatrixAllMultZXY({ LOCAL_ROT, angle }));
	collOperator->SetPos(attackPos);
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA));
	Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA, attackPos, 0.0f));
}

void GrapeBossKickDownState::Update(void)
{
	// アニメーションが終わっていたらステートを抜ける
	if (IsAnimeEnd()) {
		// アニメーションの再生割合で生成
		if (first) {
			// 生成数まで回す
			for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::KickBomb]; i++) {

				// 前後左右の制定
				float FB = (float)GetRand(RADIUS);
				float LR = (float)GetRand(RADIUS);

				// ランダム位置の生成
				float posX = 0.0f;
				float posZ = 0.0f;

				// 半分の確率で前・後ろ
				if (FB > RADIUS / 2) {
					posX = -(float)GetRand(RADIUS) - (float)RADIUS;
				}
				else {
					posX = (float)GetRand(RADIUS) + (float)RADIUS;
				}

				// 半分の確率で左・右
				if (LR > RADIUS / 2) {
					posZ = -(float)GetRand(RADIUS) - (float)RADIUS;
				}
				else {
					posZ = (float)GetRand(RADIUS) + (float)RADIUS;
				}

				// スタート位置の設定
				Vector3 tmpPos = Vector3(pos.x + posX, HEIGHT, pos.z + posZ);
				bombType[i].weaponIns->SetStartPos(tmpPos);
				bombType[i].weaponIns->SetViewPosCircle();
				
				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::KickBomb, i, tmpPos));

				// 起動
				bombType[i].live = true;
			}
			// フレーム位置に攻撃と音を発生
			collOperator->CollSet(true);
			Snd::GetIns().Play("Kick");
		}
		// 二度目は通さないようにフラグを折る
		first = false;
		// 攻撃持続時間の加算
		cnt++;

		// 攻撃持続時間の判定
		if (cnt > ATTACK_DURATION) {

			DefaultChangeState();
		}
	}
	else {
		// 攻撃描画を更新
		collOperator->SetScale(GetAnimPlayRatio());
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA, attackPos, GetAnimPlayRatio()));
	}
}

void GrapeBossKickDownState::Exit(void)
{
	// フレーム位置に攻撃を消去
	collOperator->CollSet(false);
	collOperator->SetDrawArea(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA, false));
}

void GrapeBossKickDownState::AlwaysUpdate(void)
{
}
