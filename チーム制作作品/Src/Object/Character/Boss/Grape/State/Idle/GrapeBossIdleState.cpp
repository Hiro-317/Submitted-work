#include "GrapeBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossIdleState::GrapeBossIdleState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, Vector3& angle,
	const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayIdleAnim,
	const std::function<void(void)> PlayWalkAnim,
	const std::function<void(void)> MoveChangeState,
	const std::function<void(void)> KickDownChangeState,
	const std::function<void(void)> StraightChangeState,
	const std::function<void(void)> StampChangeState,
	const std::function<void(void)> SingleChangeState,
	const std::function<void(void)> StalkerChangeState,
	const std::function<void(void)> RandomChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), angle(angle),
	playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	PlayIdleAnim(PlayIdleAnim),
	PlayWalkAnim(PlayWalkAnim),
	MoveChangeState(MoveChangeState),
	KickDownChangeState(KickDownChangeState),
	StraightChangeState(StraightChangeState),
	StampChangeState(StampChangeState),
	SingleChangeState(SingleChangeState),
	StalkerChangeState(StalkerChangeState),
	RandomChangeState(RandomChangeState),
	cnt(0), target(0), angleFlg(true)
{
}

void GrapeBossIdleState::Enter(void)
{
	cnt = CoolTime();
	target = GetTarget();
	PlayIdleAnim();
	angleFlg = true;
}

void GrapeBossIdleState::Update(void)
{
	// クールタイムの間
	if (cnt > 0) {
		cnt--;
		
		// プレイヤーとの角度を出す
		Vector3 tmp = *playerPos.at(target) - pos;
		float dir = atan2f(tmp.x, tmp.z);
		float norm = dir - angle.y;

		// 角度があってなかったら
		if (abs(norm) > NOT_ROT) {

			// フラグを立てる
			angleFlg = true;
		}

		// 角度合わせフラグが立ってるなら
		if (angleFlg) {
			// 歩きのアニメーションを流す
			PlayWalkAnim();

			// 自身との向きを-π～+πの範囲にする
			while (norm > DX_PI_F) {

				norm -= DX_TWO_PI_F;
			}
			while (norm < -DX_PI_F) {

				norm += DX_TWO_PI_F;
			}

			// 右なら足す
			if (norm > 0.0f) {
				if (norm > ROT_POW) {
					angle.y += ROT_POW;
				}
				else {
					// 角度があってるならおろす
					angle.y = dir;
					angleFlg = false;
				}
			}
			// 左なら引く
			else {
				if (norm < -ROT_POW) {
					angle.y -= ROT_POW;
				}
				else {
					// 角度があってるならおろす
					angle.y = dir;
					angleFlg = false;
				}
			}
		}
		else {

			// 角度合わせがないならアイドル
			PlayIdleAnim();
		}
		return;
	}
	// 距離と確率を出す
	float distance = (*playerPos.at(target) - pos).Length();
	int luck = GetRand(RANDOM);

	// 確率で変える
	if (luck <= KICKDOWN_LUCK) {
		// 距離が近いなら頭突き
		if (distance <= DISTANCE) {
			KickDownChangeState();
		}
		// 遠いなら移動
		else {
			MoveChangeState();
		}
	}
	else if (luck <= STRAIGHT_LUCK) {
		// 距離が遠いなら直線投擲
		if (distance >= DISTANCE) {
			StraightChangeState();
		}
		// 近いならかかと落としか追従
		else if (luck <= RELOOT_LUCK){
			KickDownChangeState();
		}
		else {
			StalkerChangeState();
		}
	}
	else if (luck <= STAMP_LUCK) {

		StampChangeState();
	}
	else if (luck <= SINGLE_LUCK) {

		SingleChangeState();
	}
	else if (luck <= STALKER_LUCK) {

		StalkerChangeState();
	}
	else {
		RandomChangeState();
	}
}

void GrapeBossIdleState::Exit(void)
{
}

void GrapeBossIdleState::AlwaysUpdate(void)
{
}
