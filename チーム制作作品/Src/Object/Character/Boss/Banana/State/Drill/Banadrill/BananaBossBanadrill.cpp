#include "BananaBossBanadrill.h"


BananaBossBanadrill::BananaBossBanadrill(
	const MSG_SENDER_ID operatorSenderId,
	const CharacterStats& stats
) :
	operatorSenderId(operatorSenderId),
	stats(stats)
{
	banadrill.Load("Character/Banana/Banadrill");
	banadrill.scale = SIZE;
}

void BananaBossBanadrill::Load(void)
{
	// プレイヤー当たり判定を生成する（XZコライダー）
	ColliderCreate(
		new XZCircleCollider(COLLIDER_TAG::BOSS_ATTACK, ATTACK_RADIUS, ATTACK_RADIUS)
	);
	// ステージ当たり判定を生成する（球体コライダー）
	ColliderCreate(
		new SphereCollider(COLLIDER_TAG::BOSS_ATTACK_AREA, RADIUS)
	);

	CreateAttackSkill(operatorSenderId, 85, &stats, COLLIDER_TAG::BOSS_ATTACK);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetPushFlg(false);
	SetJudge(false);

	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK).back()->SetTransformPtr(&banadrill);
	ColliderSerch(GetCollider(), COLLIDER_TAG::BOSS_ATTACK_AREA).back()->SetTransformPtr(&banadrill);

	collBack.Load("Range/CircleRangeBack");
	collFront.Load("Range/CircleRangeFront");

	collBack.scale = SCALE;
	collFront.scale = Vector3(0.0f);

	isDrawArea = false;
}

void BananaBossBanadrill::Update(void)
{
	// 起動
	if (isDrawArea) {
		// 空中なら加速し予測線の拡大
		if (!isGround) {
			collFront.scale = SCALE * ((ATTACK_HEIGHT - banadrill.pos.y) / (ATTACK_HEIGHT - HEIGHT));
			banadrill.pos.y -= 10.0f;
		}
		else {
			// 地面でぐるぐるする時間
			if (cnt == 0) {
				collFront.scale = SCALE;
			}
			cnt++;
			if (cnt > ATTACK_DURATION) {
				isDrawArea = false;
				SetJudge(false);
				cnt = 0;
			}
		}
	}
}

void BananaBossBanadrill::AlphaDraw(void)
{
	if (isDrawArea) {
		SetUseLighting(false);
		collBack.Draw();
		collFront.Draw();
		banadrill.Draw();
		SetUseLighting(true);
	}
}
