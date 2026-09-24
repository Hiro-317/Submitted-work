#include "BananaBossBoomerangState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../Manager/Effect/EffectManager.h"

BananaBossBoomerangState::BananaBossBoomerangState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	BananaBossBanamerang* collOperator,
	const std::function<Vector3(void)> HandPos,
	const std::function<void(void)> PlayStartAnim,
	const std::function<void(void)> PlayLoopAnim,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<void(void)> PlayIdleAnim,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	HandPos(HandPos),
	PlayStartAnim(PlayStartAnim),
	PlayLoopAnim(PlayLoopAnim),
	PlayAttackAnim(PlayAttackAnim),
	PlayIdleAnim(PlayIdleAnim),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossBoomerangState::Enter(void)
{
	first = true;
	startAnim = true;
	cnt = 0.0f;
	SetCoolTime();
	PlayStartAnim();
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
}

void BananaBossBoomerangState::Update(void)
{
	// 共通のカウント
	cnt++;
	// 溜めなら
	if (first) {
		// 溜め開始アニメーションが終わっていたらループに変える
		if (startAnim && IsAnimeEnd()) {
			PlayLoopAnim();
			startAnim = false;
		}
		// 攻撃描画を更新
		collOperator->SetScale(cnt / START_TIME);
		// バナメランを追従
		collOperator->SetBanameranPos(HandPos());
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, HandPos(), cnt / START_TIME));
		// ため時間を超えてたら
		if (cnt >= START_TIME) {
			// 攻撃の発生とカウントの初期化
			PlayAttackAnim();
			collOperator->CollSet(true);
			first = false;
			cnt = 0;
		}
	}
	// 攻撃中なら
	else {
		// 回転させる
		collOperator->SetBanameranRot(Vector3::Yonly(Deg2Rad(cnt * 20.0f)));
		collOperator->SetBanameranPos(Vector3(POS.x * cosf(Deg2Rad(cnt)), POS.y, POS.z + DIFF * -sinf(Deg2Rad(cnt))));
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, Vector3(POS.x * cosf(Deg2Rad(cnt)), POS.y, POS.z + DIFF * -sinf(Deg2Rad(cnt))), 1.0f, Deg2Rad(cnt * 10.0f)));
		// 時間が越えてたら終わる
		if (cnt >= END_TIME) {
			DefaultChangeState();
		}
		// 攻撃のアニメーションが終了してたらアイドルへ
		if (IsAnimeEnd()) {
			PlayIdleAnim();
		}
	}
}

void BananaBossBoomerangState::Exit(void)
{
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
	collOperator->SetScale(0.0f);
	Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, Vector3()));
}

void BananaBossBoomerangState::AlwaysUpdate(void)
{
}
