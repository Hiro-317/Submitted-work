#include "GrapeBossRandomState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossRandomState::GrapeBossRandomState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const Vector3& pos, BombType* bombType,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), bombType(bombType),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void GrapeBossRandomState::Enter(void)
{
	PlayAttackAnim();
	SetCoolTime();
}

void GrapeBossRandomState::Update(void)
{
	// アニメーションが終わっていたらフラグを立てる
	if (IsAnimeEnd()) {
		// 生成数まで回す
		for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::RandomBomb]; i++) {

			// 前後左右の制定
			float FB = (float)GetRand(RADIUS);
			float LR = (float)GetRand(RADIUS);

			// ランダム位置の生成
			float posX = 0.0f;
			float posZ = 0.0f;

			// 半分の確率で前・後ろ
			if (FB > RADIUS / 2) {
				posX = -(float)GetRand(RADIUS) - DIST;
			}
			else {
				posX = (float)GetRand(RADIUS) + DIST;
			}

			// 半分の確率で左・右
			if (LR > RADIUS / 2) {
				posZ = -(float)GetRand(RADIUS) - DIST;
			}
			else {
				posZ = (float)GetRand(RADIUS) + DIST;
			}

			// スタート位置の設定
			Vector3 tmpPos = Vector3(pos.x + posX, HEIGHT, pos.z + posZ);
			bombType[i].weaponIns->SetStartPos(tmpPos);
			bombType[i].weaponIns->SetViewPosCircle();

			// 起動を通知
			Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::RandomBomb, i, tmpPos));

			// 起動
			bombType[i].live = true;
		}
		// ステートを変える
		DefaultChangeState();
	}
}

void GrapeBossRandomState::Exit(void)
{
}

void GrapeBossRandomState::AlwaysUpdate(void)
{
}
