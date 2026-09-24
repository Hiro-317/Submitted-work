#include "GrapeBossStraightState.h"

#include "../../Weapon/GrapeBossWeaponManager.h"
#include "../../Weapon/Straight/GrapeBossStraight.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

GrapeBossStraightState::GrapeBossStraightState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	BombType* bombType,
	const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayAttackAnim,
	const std::function<float(void)> GetAnimPlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	bombType(bombType),
	playerPos(playerPos),
	GetTarget(GetTarget),
	PlayAttackAnim(PlayAttackAnim),
	GetAnimPlayRatio(GetAnimPlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void GrapeBossStraightState::Enter(void)
{
	target = GetTarget();
	PlayAttackAnim();
	first = true;
	SetCoolTime();
}

void GrapeBossStraightState::Update(void)
{
	// アニメーションの再生割合で生成
	if (first) {
		if (GetAnimPlayRatio() >= RATIO) {
			// 正面から30度ずらした一発目
			float ang = angle.y - Deg2Rad(30.0f);

			// 生成数まで回す
			for (int i = 0; i < WeponDuplicateNum[(int)WeaponType::Straight]; i++) {

				// キャストさせる
				auto ins = dynamic_cast<GrapeBossStraight*>(bombType[i].weaponIns);

				// スタート位置の設定
				Vector3 tmpPos = Vector3(pos.x, HEIGHT, pos.z);
				ins->SetStartPos(tmpPos);
				// 進む向きを設定（ここのためにキャスト）
				Vector3 tmpAng = Vector3(sinf(ang), 0.0f, cosf(ang));
				ins->SetStartDir(tmpAng);

				// 起動を通知
				Net::GetIns().Send(MsgDataBossBombInform(MsgDataBossBombInform::INFORM_TYPE::Straight, i, tmpPos, tmpAng));

				// 起動
				bombType[i].live = true;
				ins->SetColliderFlg(true);

				//次の周のため角度をずらす
				ang += Deg2Rad(30.0f);
			}
			//音を出す
			Snd::GetIns().Play("Throw");
			// 二度目は通さないようにフラグを折る
			first = false;
		}
	}

	// アニメーションが終わっていたらステートを抜ける
	if (IsAnimeEnd()) {

		DefaultChangeState();
	}
}

void GrapeBossStraightState::Exit(void)
{
}

void GrapeBossStraightState::AlwaysUpdate(void)
{
}
