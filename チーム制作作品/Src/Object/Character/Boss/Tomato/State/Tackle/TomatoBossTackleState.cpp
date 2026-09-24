#include "TomatoBossTackleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../Manager/Effect/EffectManager.h"

TomatoBossTackleState::TomatoBossTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle, const Transform* trans,
	const std::vector<const Vector3*> playerPos,
	TomatoTackleCollOperator* collOperator,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> ResetAngle,
	const std::function<void(void)> DeleteColl,
	const std::function<void(void)> ReviveColl,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) 
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle), trans(trans),
	playerPos(playerPos),
	collOperator(collOperator),
	GetTarget(GetTarget),
	ResetAngle(ResetAngle),
	DeleteColl(DeleteColl),
	ReviveColl(ReviveColl),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
	rotPow = ROTATION_POW;
}

void TomatoBossTackleState::Enter(void)
{
	time = 0;
	rotPow = ROTATION_POW;
	DeleteColl();
	target = GetTarget();
	collOperator->ResetStageHit();
	SetCoolTime();
	
	SoundManager::GetIns().Play("TackleCharge");

	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
}

void TomatoBossTackleState::Update(void)
{
	angle.x += rotPow;

	// 一秒の溜め
	if (time < CHARGE_POW) {
		
		time++;

		// 回転の更新
		moveDir = (*playerPos.at(target) - pos).Normalized();
		angle.y = atan2f(moveDir.x, moveDir.z);
		rotPow += ROTATION_POW;

		// 予測線の更新
		auto p = Vector3::XZonly(pos.x, pos.z);
		auto a = angle.y;
		auto s = ((float)time - 180.0f) / 180.0f + 1.0f;

		collOperator->SetViewPos(p);
		collOperator->SetAngle(a);
		collOperator->SetScale(s);

		// 送信
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, p, s, a));
	}
	else {
		// エフェクトの再生
		if (time == CHARGE_POW) {
			EffectManager::GetIns()->CreateEffect(EFFECT_NAME::TACKLE_MOVE, Vector3(), trans, false);
		}
		// 位置の更新
		pos += moveDir * MOVE_SPEED;

		collOperator->CollSet(true);
		
		if (time < FORCE_MOVE_TIME) {
			time++;
			SoundManager::GetIns().Stop("TackleCharge");
			SoundManager::GetIns().Play("TackleMove");
			collOperator->ResetStageHit();
		}
		else if (collOperator->GetStageHit()) {
			SoundManager::GetIns().Play("TackleRock");
			DefaultChangeState();
			return;
		}
	}
	angle.x += rotPow;
	collOperator->SetPos(Vector3::XZonly(trans->pos.x, trans->pos.z));
}

void TomatoBossTackleState::Exit(void)
{
	ResetAngle();
	ReviveColl();
	collOperator->SetDrawArea(false);
	collOperator->CollSet(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
	EffectManager::GetIns()->StopEffect(EFFECT_NAME::TACKLE_MOVE);
}

void TomatoBossTackleState::AlwaysUpdate(void)
{
}
