#include "BossSelectSpotBase.h"

#include "../../../Scene/SceneManager/SceneManager.h"

#include "../../Common/Collider/XZCircleCollider.h"

BossSelectSpotBase::BossSelectSpotBase(
	BOSS_TYPE bossType,
	std::string MODEL_PATH,
	float defaultModelScale, float selectedModelScale,
	float DEFAULT_COLLIDER_RADIUS,
	Vector3 DEFAULT_SELECTED_IMAGE_OFFSET,
	const Vector3& pos, const Vector3& angle
) :
	ActorBase(),

	bossType(bossType),

	MODEL_PATH(MODEL_PATH),

	MODEL_SCALE{ { false, defaultModelScale }, { true, selectedModelScale } },

	COLLIDER_RADIUS(DEFAULT_COLLIDER_RADIUS * defaultModelScale),

	SELECTED_IMAGE_OFFSET(DEFAULT_SELECTED_IMAGE_OFFSET* defaultModelScale),

	selected(false),

	selectedEasingCounter(0.0f),
	selectedEasingRate(0.0f)
{
	trans.pos = pos;
	trans.angle = angle;
}

void BossSelectSpotBase::Load(void)
{
	// 静的オブジェクトに設定
	SetDynamicFlg(false);
	// オブジェクト設定の押し出しは無効
	SetPushFlg(false);

	// モデルを読み込みむ
	trans.Load(MODEL_PATH.c_str());

	// デフォルトスケール（非選択時）で初期化
	trans.scale = MODEL_SCALE.at(false);

	// 選択判定（飛行船が近くにいるか）を行うコライダー
	ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS, COLLIDER_RADIUS + COLLIDER_RADIUS_EXTRA, 10000.0f));

	// 押し出しを行うコライダー
	ColliderCreate(new XZCircleCollider(COLLIDER_TAG::BOSS, COLLIDER_RADIUS, 10000.0f));
	// このコライダーの押し出し設定だけ有効にする
	GetCollider().back()->SetPushFlg(true);
}

void BossSelectSpotBase::SubUpdate(void)
{
	// モデルの表示倍率が正しいか確認する
	if (trans.scale != MODEL_SCALE.at(selected)) {

		// 差分を計算
		Vector3 sub = trans.scale - MODEL_SCALE.at(selected);

		// まだ大きくズレている場合、差分を少しずつ消していく
		if (fabs(sub.MaxElementF()) > 0.00001f) { trans.scale -= sub * MODEL_SCALE_EASING_RATE; }
		// ズレが限りなく少ない場合、代入して完全に合わせる（安全処理）
		else { trans.scale = MODEL_SCALE.at(selected); }

		// 新しい表示倍率をモデルに適用
		trans.Attach();
	}


	// 選択中（近くに飛行船がいるかどうか）じゃなければ以降の処理なし
	if (!selected) { selectedEasingCounter = 0.0f; return; }

	// カウンター更新
	selectedEasingCounter += 0.1f;
	// イージングレート更新
	selectedEasingRate = sinf(selectedEasingCounter) * 20.0f;
	// カウンターリセット
	if (selectedEasingCounter > 100000.0f) { selectedEasingCounter = 0.0f; }

	// 選択中フラグリセット
	selected = false;
}

void BossSelectSpotBase::SelectedDraw(const int& selectedImage)
{
	if (!selected) { return; }

	DrawBillboard3D(
		(trans.pos + SELECTED_IMAGE_OFFSET + Vector3::Yonly(selectedEasingRate)).ToVECTOR(),
		0.5f, 0.5f,
		500.0f, 0.0f,
		selectedImage,
		true
	);
}

void BossSelectSpotBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)
{
	if (other.GetTag() == SKY_SHIP_COLL_TAG) { 
		selected = true;

		SceneManager::GetIns().SetSelectBossType(bossType);
	}
}