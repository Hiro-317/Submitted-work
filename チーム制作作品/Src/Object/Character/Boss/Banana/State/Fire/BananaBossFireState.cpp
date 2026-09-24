#include "BananaBossFireState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../Manager/Effect/EffectManager.h"

BananaBossFireState::BananaBossFireState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	BananaFireCollOperator* collOperator,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void BananaBossFireState::Enter(void)
{
	first = true;
	effect = false;
	SetCoolTime();
	PlayAttackAnim();
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB));
}

void BananaBossFireState::Update(void)
{
	// アニメーションのある再生割合以上で
	if (GetAnimPlayRatio() >= EFFECT_RATE && !effect) {
		// エフェクト生成
		effect = true;
		EffectManager::GetIns()->CreateEffect(EFFECT_NAME::FIRE, EFFECT_POS);
	}
	if (GetAnimPlayRatio() >= ATTACK_RATE) {
		// アニメーションの再生割合で生成
		if (first) {
			// フレーム位置に攻撃と音を発生
			collOperator->CollSet(true);
			//Snd::GetIns().Play("Scratch");
		}
		// 二度目は通さないようにフラグを折る
		first = false;
		// アニメーションが終わったら終わり
		if (IsAnimeEnd()) {

			DefaultChangeState();
		}
	}
	else {
		// 攻撃描画を更新
		collOperator->SetScale(GetAnimPlayRatio() / ATTACK_RATE);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB, Vector3(), GetAnimPlayRatio() / ATTACK_RATE));
	}
}

void BananaBossFireState::Exit(void)
{
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB, false));
}

void BananaBossFireState::AlwaysUpdate(void)
{
}
