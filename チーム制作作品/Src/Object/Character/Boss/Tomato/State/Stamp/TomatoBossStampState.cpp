#include "TomatoBossStampState.h"

#include "../../../../../ActorBase.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../Manager/Effect/EffectManager.h"


TomatoBossStampState::TomatoBossStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoStampCollOperator* collOperator,
	Vector3& pos, Vector3& angle, const bool& isGround,
	const std::vector<const Vector3*>playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> offCollider,
	const std::function<void(void)> onCollider,
	const std::function<void(void)> SetCoolTime
) :CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator), 
	pos(pos), angle(angle), isGround(isGround),
	playerPos(playerPos),
	GetTarget(GetTarget),
	DefaultChangeState(DefaultChangeState),
	offCollider(offCollider),
	onCollider(onCollider),
	SetCoolTime(SetCoolTime)
{
	prevPos = pos.y;
	nowAttackTime = 0;
	attackCnt = 0;
	isAttack = true;
}

void TomatoBossStampState::Enter(void)
{
	target = GetTarget();
	collOperator->SetPos(*playerPos.at(target)); 
	attackPos = Vector3();
	attackDistRate = Vector3();
	collOperator->DrawAreaOn();
	nowAttackTime = 0;
	prevPos = pos.y - 0.5f;
	attackCnt = 0;
	isAttack = true;
	SetCoolTime();
	Vector3 tmp = *playerPos.at(target) - pos;
	angle.y = atan2f(tmp.x, tmp.z);

	SoundManager::GetIns().Play("StampJump");
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB));
}

void TomatoBossStampState::Update(void)
{
	// 地面にいなく初めなら
	if (!isGround || isAttack) {
		// 初めのフラグを落とす
		if (isAttack) {
			isAttack = false;
		}
		// 重力に負けるまで上げ
		// ボス自体のコライダを消す
		if (prevPos < pos.y) {
			prevPos = pos.y;
			pos.y += JUMP_POW;
			offCollider();
		}
		// 重力に任せる
		// ボス自体のコライダを復活させる
		else {
			prevPos = pos.y;
			onCollider();
		}
	}
	// 地面についた
	else {
		// 初めなら攻撃を出し音を出す
		if (attackCnt == 0) {
			SoundManager::GetIns().Play("StampLand");
			collOperator->CollOn();
			EffectManager::GetIns()->CreateEffect(EFFECT_NAME::STAMP_LAND, collOperator->GetAttackPos());
		}
		// 持続時間過ぎたらステートを変える
		if (attackCnt > ATTACK_DURATION) {
			DefaultChangeState();
		}
		// 時間を経たせる
		attackCnt++;
	}

	// 移動に掛ける時間まで
	if (nowAttackTime < TIME_RATE) {

		// 押し出されるため毎回とる
		attackPos = collOperator->GetAttackPos();
		// 現在の自身と攻撃地点の距離から進む距離を出す
		attackDistRate = Vector3::XZonly(attackPos.x - pos.x, attackPos.z - pos.z) / (TIME_RATE - (float)nowAttackTime);

		// 現在の時間を足す
		nowAttackTime++;
		// 位置を足す
		pos += attackDistRate;

		// 予測線の更新
		float scale = 1.0f - (TIME_RATE - (float)nowAttackTime) / TIME_RATE;
		collOperator->SetScale(scale);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB, attackPos, scale));
	}
}

void TomatoBossStampState::Exit(void)
{
	collOperator->Off();
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB, false));
}

void TomatoBossStampState::AlwaysUpdate(void)
{
}
