#include "GrapeBossSingleState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossSingleState::GrapeBossSingleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::vector<const Vector3*> playerPos,
	BombType* bombType,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	playerPos(playerPos),
	bombType(bombType),
	GetTarget(GetTarget),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void GrapeBossSingleState::Enter(void)
{
	PlayAttackAnim();
	target = GetTarget();
	SetCoolTime();
}

void GrapeBossSingleState::Update(void)
{
	// アニメーションが終わっていたら
	if (IsAnimeEnd()) {
		// 生成数まで回す
		for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::SingleBomb]; i++) {

			// スタート位置の設定
			Vector3 tmpPos = Vector3(playerPos.at(target)->x, HEIGHT, playerPos.at(target)->z);
			bombType[i].weaponIns->SetStartPos(tmpPos);
			bombType[i].weaponIns->SetViewPosCircle();

			// 起動を通知
			Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::SingleBomb, i, tmpPos));

			// 起動
			bombType[i].live = true;

			// ステートを抜ける
			DefaultChangeState();
		}
	}
}

void GrapeBossSingleState::Exit(void)
{
}

void GrapeBossSingleState::AlwaysUpdate(void)
{
}
