#include "GrapeBossKickBomb.h"

#include "../../../../../Common/Collider/SphereCollider.h"
#include "../../../../../Common/Collider/XZCircleCollider.h"

#include "../../../../../../Manager/Effect/EffectManager.h"


GrapeBossKickBomb::GrapeBossKickBomb(int model, int front, int back)
	: GrapeBossWeaponBase(model, front, back)
{
	// 当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(
			COLLIDER_TAG::BOSS_ATTACK,
			ATTACK_RANGE,
			1000.0f
		)
	);

	// 当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(
			COLLIDER_TAG::BOSS_ATTACK_AREA,
			MODEL_RADIUS * ATTACK_SIZE
		)
	);

	trans.scale = Vector3(ATTACK_SIZE);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetJudgeFlg(true);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetPushFlg(false);
	SetGravityFlg(true);
	count = 0;
}

void GrapeBossKickBomb::Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats)
{
	CreateAttackSkill(operatorSenderId, 75, &stats);

	collBack.scale = Vector3::XZonly(ATTACK_RANGE / MODEL_RADIUS, ATTACK_RANGE / MODEL_RADIUS);
	collFront.scale = Vector3(0.0f);
}

void GrapeBossKickBomb::SubUpdate(void)
{
	// 地面についてないなら加速して落ちる
	if (!isGround) {

		trans.pos.y -= 8.0f;
	}
	// ついてるなら爆発カウントを進める
	else {
		// 前フレームの判定が残ってるので
		// それ防止に
		// 開始されたフレームの検知
		if (!now) {
			// 今開始されたらフラグを立てる
			now = true;
			// ほんとはついてないので落下
			trans.pos.y -= 8.0f;
			return;
		}
		count++;
		// カウントに応じて状態を変える
		if (BOMBER_COUNT >= count) {

			SetViewScaleCircle((ATTACK_RANGE / MODEL_RADIUS) * (count / BOMBER_COUNT));
		}
		// 攻撃判定オン
		else if ((BOMBER_COUNT + ATTACK_DURATION) >= count) {
			ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(true);
		}
		//攻撃判定オフ、終わり
		else {
			ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetJudgeFlg(false);
			end = true;
			now = false;
			count = 0;
			SetViewScaleCircle(0.0f);
			// ホストのみエフェクト再生
			if (Net::GetIns().IsHost()) {
				EffectManager::GetIns()->CreateEffect(EFFECT_NAME::BOMB_SMALL, trans.pos);
			}
		}
	}
}
