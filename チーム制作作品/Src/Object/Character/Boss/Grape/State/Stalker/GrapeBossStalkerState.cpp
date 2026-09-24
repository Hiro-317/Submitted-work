#include "GrapeBossStalkerState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossStalkerState::GrapeBossStalkerState(
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

void GrapeBossStalkerState::Enter(void)
{
	PlayAttackAnim();
	end = false;
	target = GetTarget();
	SetCoolTime();
	num = 0;
}

void GrapeBossStalkerState::Update(void)
{
	// アニメーションが終わっていたらフラグを立てる
	if (IsAnimeEnd()) {

		PlayAttackAnim();
		end = true;
	}

	// フラグを立ってるなら
	if (end) {
		// 間隔をとる
		if (disp >= DISP_TIME) {
			disp = 0;
			// 生成数まで回す
			if (num < WeponDuplicateNum[(int)WeaponType::StalkerBomb]) {

				// スタート位置の設定
				Vector3 tmpPos = Vector3(playerPos.at(target)->x, HEIGHT, playerPos.at(target)->z);
				bombType[num].weaponIns->SetStartPos(tmpPos);
				bombType[num].weaponIns->SetViewPosCircle();

				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::StalkerBomb, num, tmpPos));

				// 起動
				bombType[num].live = true;

				num++;
			}
			// 全生成したらデフォルトに戻る
			else {
				DefaultChangeState();
			}
		}
		else {
			disp++;
		}
	}
}

void GrapeBossStalkerState::Exit(void)
{
}

void GrapeBossStalkerState::AlwaysUpdate(void)
{
}
