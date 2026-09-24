#include"CollisionManager.h"

#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>

#include "../../Application/Application.h"

#include"../../Object/Common/Collider/LineCollider.h"
#include"../../Object/Common/Collider/SphereCollider.h"
#include"../../Object/Common/Collider/CapsuleCollider.h"
#include"../../Object/Common/Collider/BoxCollider.h"
#include"../../Object/Common/Collider/ModelCollider.h"
#include"../../Object/Common/Collider/XZCircleCollider.h"

#pragma region 初期化系

// コライダーを追加（単体）
void CollisionManager::Add(ColliderBase* collider)
{
	// 安全処理
	if (!collider) { return; }

	// グループを一時変数として定義（「その他」で初期化）
	COLLIDER_GROUP group = COLLIDER_GROUP::Other;

	// グループリストに記述が存在すれば、取得
	if (TAG_TO_GROUP_LIST.contains(collider->GetTag())) {
		group = TAG_TO_GROUP_LIST.at(collider->GetTag());
	}

	// 最終的なグループで格納
	groupColliders[(int)group].colliders.emplace_back(collider);
}

// コライダーを追加（複数）
void CollisionManager::Add(std::vector<ColliderBase*> collider)
{
	// 指定されたコライダーを順番に単体追加関数を使用して追加していく
	for (ColliderBase*& c : collider) { Add(c); }
}

// 現状抱えているコライダーすべてのチャンク分けを実行
void CollisionManager::InitBuildChunks(void)
{
	// すべてのグループを範囲for文で参照していく
	for (ColliderGroupData& group : groupColliders) {

		// チャンク分け情報を完全リセット～～～
		group.staticChunks.clear();
		group.dynamicChunks.clear();
		// ～～～チャンク分け情報を完全リセット

		// 抱えるコライダーを1つずつ範囲for文で参照していく
		for (ColliderBase* collider : group.colliders) {
			// 安全処理
			if (!collider) { continue; }

			// 動的コライダーの場合
			if (collider->GetDynamicFlg()) { RegisterToChunks(group.dynamicChunks, collider); }
			// 静的コライダーの場合
			else { RegisterToChunks(group.staticChunks, collider); }
		}
	}
}

#pragma endregion

#pragma region 判定実行

void CollisionManager::Check(void)
{
	// チャンク分け
	BuildChunks();

	// 二重判定防止の判定記録配列をリセット
	checkedPairs.clear();

	// ①プレイヤー系 × ステージ系
	Matching(COLLIDER_GROUP::Player, COLLIDER_GROUP::Stage);

	// ②エネミー系 × ステージ系
	Matching(COLLIDER_GROUP::Enemy, COLLIDER_GROUP::Stage);


	// ③プレイヤー系 × エネミー系
	Matching(COLLIDER_GROUP::Player, COLLIDER_GROUP::Enemy);


	// ④プレイヤー系 × プレイヤー系にだけ当たるコライダー
	Matching(COLLIDER_GROUP::Player, COLLIDER_GROUP::PlayerOnly);

	// ⑤エネミー系 × エネミー系にだけ当たるコライダー
	Matching(COLLIDER_GROUP::Enemy, COLLIDER_GROUP::EnemyOnly);

	// ⑥ステージ系 × ステージ系にだけ当たるコライダー
	Matching(COLLIDER_GROUP::Stage, COLLIDER_GROUP::StageOnly);


	// ⑦その他 × ステージ系
	Matching(COLLIDER_GROUP::Other, COLLIDER_GROUP::Stage);

	// ⑧その他 × プレイヤー系
	Matching(COLLIDER_GROUP::Other, COLLIDER_GROUP::Player);

	// ⑨その他 × エネミー系
	Matching(COLLIDER_GROUP::Other, COLLIDER_GROUP::Enemy);

	// ⑩その他 × その他
	Matching(COLLIDER_GROUP::Other);
}

#pragma endregion

#pragma region 判定振り分け

void CollisionManager::BuildChunks(void)
{
	for (ColliderGroupData& group : groupColliders) {

		// 動的コライダーチャンク分け配列をリセット
		group.dynamicChunks.clear();

		for (ColliderBase* collider : group.colliders) {
			// 安全処理
			if (!collider) { continue; }

			// 静的コライダーは変更なし
			if (!collider->GetDynamicFlg()) { continue; }

			// 動的コライダーチャンク分け配列に割り当てなおす
			RegisterToChunks(group.dynamicChunks, collider);
		}
	}
}

// 指定のグループ同士を判定(2グループ指定)
void CollisionManager::Matching(COLLIDER_GROUP groupA, COLLIDER_GROUP groupB)
{
	ColliderGroupData& a = groupColliders[(int)groupA];
	ColliderGroupData& b = groupColliders[(int)groupB];

	// 動的A × 動的B
	MatchingChunks(a.dynamicChunks, b.dynamicChunks);

	// 動的A × 静的B（3Dチャンク）
	MatchingChunks(a.dynamicChunks, b.staticChunks);

	// 静的A × 動的B（3Dチャンク）
	MatchingChunks(a.staticChunks, b.dynamicChunks);

	// 静的A × 静的Bは基本不要
}

// 指定のグループを総当たりで判定(1グループ指定)
void CollisionManager::Matching(COLLIDER_GROUP group)
{
	// グループを取得
	ColliderGroupData& g = groupColliders[(int)group];

	// 動的同士
	MatchingChunks(g.dynamicChunks);

	// 動的 × 静的
	MatchingChunks(g.dynamicChunks, g.staticChunks);

	// 静的同士は基本不要
}

// チャンクを振り分けて判定実行
void CollisionManager::MatchingChunks(ChunkMap& aChunks, ChunkMap& bChunks)
{
	// 1つ目のチャンクマップ(aChunks)を基準とし、ループして全参照
	for (auto& aChunk : aChunks) {

		// そのチャンク座標(aChunk.first) が 2つ目のチャンクマップ(bChunks)に存在するかをチェック
		auto bChunk = bChunks.find(aChunk.first);
		// なければこのループは終了
		if (bChunk == bChunks.end()) { continue; }

		// あれば互いにそのチャンクに含まれるコライダー同士、総当たりで判定実行
		for (ColliderBase* a : aChunk.second.colliders) {
			for (ColliderBase* b : bChunk->second.colliders) {
				CheckPairOnce(a, b);
			}
		}
	}
}

// チャンクを振り分けて判定実行
void CollisionManager::MatchingChunks(ChunkMap& chunks)
{
	// 全ループ
	for (auto& chunk : chunks) {

		// そのチャンクに含まれるコライダーを取得
		auto& colliders = chunk.second.colliders;

		// 順番に先頭から総当たりで判定実行
		for (size_t i = 0; i < colliders.size(); i++) {
			for (size_t j = i + 1; j < colliders.size(); j++) {
				CheckPairOnce(colliders[i], colliders[j]);
			}
		}
	}
}

// 重複判定チェック
void CollisionManager::CheckPairOnce(ColliderBase* a, ColliderBase* b)
{
	// 安全処理
	if (!a || !b) { return; }

	// 同じもの同士は判定しない
	if (a == b) { return; }

	// 2つセットの構造体を生成
	ColliderPairKey key(a, b);

	// このセットが重複判定チェック配列に存在する(このループ中既にこの組み合わせでの判定を行った)かどうかをチェックする
	if (checkedPairs.find(key) != checkedPairs.end()) { return; }

	// このセットがまだこのループ中判定を行っていなかった場合、重複判定チェック配列に登録する
	checkedPairs.insert(key);

	// 衝突点の座標を格納する変数
	Vector3 collisionPoint = Vector3();

	// 判定を実行する
	if (IsHit(a, b, collisionPoint)) {
		// 当たっていればお互いのコールバック関数を呼ぶ
		a->CallOnCollision(a->GetTag(), *b, collisionPoint);
		b->CallOnCollision(b->GetTag(), *a, collisionPoint);
	}
}

// 形状の振り分け
bool CollisionManager::IsHit(ColliderBase* a, ColliderBase* b, Vector3& collisionPoint)
{
	// 当たり判定フラグを確認
	if (!a->GetJudge() || !b->GetJudge()) { return false; }

	// ローカル変数で各形状を保持（ゲット関数の呼び出しを1回で済ませるため）
	const SHAPE aShape = a->GetShape(), bShape = b->GetShape();

	// どちらか、あるいは両方の形状が未設定だったら判定なし（falseで返却）
	if (aShape == SHAPE::NON || bShape == SHAPE::NON) { return false; }

	// そもそも、同じタグ同士は当たり判定しない
	if (a->GetTag() == b->GetTag()) {
		if (TAG_TO_GROUP_LIST.contains(a->GetTag())) { return false; }
	}

	// 雑な判定（軽量化目的）
	ColliderBase::AABB aAABB = a->GetAABB(), bAABB = b->GetAABB();
	if (aAABB.max.x < bAABB.min.x || aAABB.min.x > bAABB.max.x ||
		aAABB.max.y < bAABB.min.y || aAABB.min.y > bAABB.max.y ||
		aAABB.max.z < bAABB.min.z || aAABB.min.z > bAABB.max.z)
	{
		return false;
	}

#pragma region 形状を判別して適切な関数にて判定を行う

	// 同形状同士～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～
	
	// 線分同士
	if (aShape == SHAPE::LINE && bShape == SHAPE::LINE) { return LineToLine(dynamic_cast<LineCollider*>(a), dynamic_cast<LineCollider*>(b), collisionPoint); }

	// 球体同士
	if (aShape == SHAPE::SPHERE && bShape == SHAPE::SPHERE) { return SphereToSphere(dynamic_cast<SphereCollider*>(a), dynamic_cast<SphereCollider*>(b), collisionPoint); }

	// カプセル同士
	if (aShape == SHAPE::CAPSULE && bShape == SHAPE::CAPSULE) { return CapsuleToCapsule(dynamic_cast<CapsuleCollider*>(a), dynamic_cast<CapsuleCollider*>(b), collisionPoint); }

	// ボックス同士
	if (aShape == SHAPE::BOX && bShape == SHAPE::BOX) { return BoxToBox(dynamic_cast<BoxCollider*>(a), dynamic_cast<BoxCollider*>(b), collisionPoint); }

	// モデルポリゴン同士
	if (aShape == SHAPE::MODEL && bShape == SHAPE::MODEL) { return ModelToModel(dynamic_cast<ModelCollider*>(a), dynamic_cast<ModelCollider*>(b), collisionPoint); }

	// XZ平面上の円同士
	if (aShape == SHAPE::XZ_CIRCLE && bShape == SHAPE::XZ_CIRCLE) { return XZCircleToXZCircle(dynamic_cast<XZCircleCollider*>(a), dynamic_cast<XZCircleCollider*>(b), collisionPoint); }

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～同形状同士
	
	// 別形状同士～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// 線分×球体
	if (aShape == SHAPE::LINE && bShape == SHAPE::SPHERE) { return LineToSphere(dynamic_cast<LineCollider*>(a), dynamic_cast<SphereCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::SPHERE && bShape == SHAPE::LINE) { return LineToSphere(dynamic_cast<LineCollider*>(b), dynamic_cast<SphereCollider*>(a), collisionPoint); }

	// 線分×カプセル
	if (aShape == SHAPE::LINE && bShape == SHAPE::CAPSULE) { return LineToCapsule(dynamic_cast<LineCollider*>(a), dynamic_cast<CapsuleCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::CAPSULE && bShape == SHAPE::LINE) { return LineToCapsule(dynamic_cast<LineCollider*>(b), dynamic_cast<CapsuleCollider*>(a), collisionPoint); }

	// 線分×ボックス
	if (aShape == SHAPE::LINE && bShape == SHAPE::BOX) { return LineToBox(dynamic_cast<LineCollider*>(a), dynamic_cast<BoxCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::BOX && bShape == SHAPE::LINE) { return LineToBox(dynamic_cast<LineCollider*>(b), dynamic_cast<BoxCollider*>(a), collisionPoint); }

	// 線分×モデルポリゴン
	if (aShape == SHAPE::LINE && bShape == SHAPE::MODEL) { return LineToModel(dynamic_cast<LineCollider*>(a), dynamic_cast<ModelCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::MODEL && bShape == SHAPE::LINE) { return LineToModel(dynamic_cast<LineCollider*>(b), dynamic_cast<ModelCollider*>(a), collisionPoint); }

	// 球体×カプセル
	if (aShape == SHAPE::SPHERE && bShape == SHAPE::CAPSULE) { return SphereToCapsule(dynamic_cast<SphereCollider*>(a), dynamic_cast<CapsuleCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::CAPSULE && bShape == SHAPE::SPHERE) { return SphereToCapsule(dynamic_cast<SphereCollider*>(b), dynamic_cast<CapsuleCollider*>(a), collisionPoint); }

	// 球体×ボックス
	if (aShape == SHAPE::SPHERE && bShape == SHAPE::BOX) { return SphereToBox(dynamic_cast<SphereCollider*>(a), dynamic_cast<BoxCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::BOX && bShape == SHAPE::SPHERE) { return SphereToBox(dynamic_cast<SphereCollider*>(b), dynamic_cast<BoxCollider*>(a), collisionPoint); }

	// 球体×モデルポリゴン
	if (aShape == SHAPE::SPHERE && bShape == SHAPE::MODEL) { return SphereToModel(dynamic_cast<SphereCollider*>(a), dynamic_cast<ModelCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::MODEL && bShape == SHAPE::SPHERE) { return SphereToModel(dynamic_cast<SphereCollider*>(b), dynamic_cast<ModelCollider*>(a), collisionPoint); }

	// 球体×XZ平面上の円
	if (aShape == SHAPE::SPHERE && bShape == SHAPE::XZ_CIRCLE) { return SphereToXZCircle(dynamic_cast<SphereCollider*>(a), dynamic_cast<XZCircleCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::XZ_CIRCLE && bShape == SHAPE::SPHERE) { return SphereToXZCircle(dynamic_cast<SphereCollider*>(b), dynamic_cast<XZCircleCollider*>(a), collisionPoint); }

	// カプセル×ボックス
	if (aShape == SHAPE::CAPSULE && bShape == SHAPE::BOX) { return CapsuleToBox(dynamic_cast<CapsuleCollider*>(a), dynamic_cast<BoxCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::BOX && bShape == SHAPE::CAPSULE) { return CapsuleToBox(dynamic_cast<CapsuleCollider*>(b), dynamic_cast<BoxCollider*>(a), collisionPoint); }

	// カプセル×モデルポリゴン
	if (aShape == SHAPE::CAPSULE && bShape == SHAPE::MODEL) { return SphereToModel(dynamic_cast<SphereCollider*>(a), dynamic_cast<ModelCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::MODEL && bShape == SHAPE::CAPSULE) { return SphereToModel(dynamic_cast<SphereCollider*>(b), dynamic_cast<ModelCollider*>(a), collisionPoint); }

	// カプセル×XZ平面上の円
	if (aShape == SHAPE::CAPSULE && bShape == SHAPE::XZ_CIRCLE) { return CapsuleToXZCircle(dynamic_cast<CapsuleCollider*>(a), dynamic_cast<XZCircleCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::XZ_CIRCLE && bShape == SHAPE::CAPSULE) { return CapsuleToXZCircle(dynamic_cast<CapsuleCollider*>(b), dynamic_cast<XZCircleCollider*>(a), collisionPoint); }

	// ボックス×モデルポリゴン
	if (aShape == SHAPE::BOX && bShape == SHAPE::MODEL) { return BoxToModel(dynamic_cast<BoxCollider*>(a), dynamic_cast<ModelCollider*>(b), collisionPoint); }
	if (aShape == SHAPE::MODEL && bShape == SHAPE::BOX) { return BoxToModel(dynamic_cast<BoxCollider*>(b), dynamic_cast<ModelCollider*>(a), collisionPoint); }

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～別形状同士

#pragma endregion
	
	// どの組み合わせにも属さなかった場合判定なし（falseで返却）
	return false;
}

#pragma endregion

#pragma region 各形状の実判定

// 線分×線分
bool CollisionManager::LineToLine(LineCollider* a, LineCollider* b, Vector3& collisionPoint)
{
	return false;
}

// 球体×球体
bool CollisionManager::SphereToSphere(SphereCollider* a, SphereCollider* b, Vector3& collisionPoint)
{
#pragma region 必要情報を求める

	// 押し出し前の中心座標を保存
	const Vector3 aPos = a->GetPos();
	const Vector3 bPos = b->GetPos();

	// bからaへ向かうベクトル
	Vector3 normal = aPos - bPos;

	// 中心間距離の2乗
	const float distanceSq = normal.LengthSq();

	// 半径
	const float aRadius = a->GetRadius();
	const float bRadius = b->GetRadius();

	// 半径の合計
	const float radiusSum = aRadius + bRadius;

#pragma endregion

#pragma region 衝突判定

	// 中心間距離が半径の合計より大きければ衝突していない
	if (distanceSq > radiusSum * radiusSum) { return false; }

#pragma endregion

#pragma region 衝突方向を求める

	// 球の中心が完全に一致している場合は、
	// 中心座標から衝突方向を求められないため代替方向を使用する
	if (distanceSq <= 0.000001f) {

		// 第一候補：Aの移動方向
		const Vector3 velocity = a->GetTransform().Velocity();

		// 第一候補がゼロベクトルなら、第二候補：Y軸方向を使用する
		normal = (velocity.LengthSq() > 0.000001f) ? velocity.Normalized() : Vector3::Yonly(1.0f);
	}
	else { normal.Normalize(); }

#pragma endregion

#pragma region 衝突点を求める

	// Aの表面上で、Bに最も近い点
	const Vector3 pointOnA = aPos - normal * aRadius;

	// Bの表面上で、Aに最も近い点
	const Vector3 pointOnB = bPos + normal * bRadius;

	// 2つの表面点の中間を衝突点とする
	collisionPoint = (pointOnA + pointOnB) * 0.5f;

#pragma endregion

#pragma region 押し出しが必要か->必要なら押し出し

	if (NeedPush(a, b)) {
		// 中心間距離
		const float distance = std::sqrt(distanceSq);

		// めり込んだ量
		const float overlap = radiusSum - distance;

		ApplyPush(a, b, normal, overlap);
	}

#pragma endregion

	return true;
}

// カプセル×カプセル
bool CollisionManager::CapsuleToCapsule(CapsuleCollider* a, CapsuleCollider* b, Vector3& collisionPoint)
{
#pragma region 必要情報を取得
	// 線分の 始点/終点 
	const Vector3 aStartPos = a->GetStartPos(), aEndPos = a->GetEndPos();
	const Vector3 bStartPos = b->GetStartPos(), bEndPos = b->GetEndPos();

	// 半径
	const float   aRadius = a->GetRadius(), bRadius = b->GetRadius();
#pragma endregion

#pragma region 衝突判定（お互いの線分上における最近点を求めてその２点間の距離をはかって 未衝突なら終了）
	// 線分同士の最近接点を求める ～～～～～～～～～～～

	// ここに最近接点が入る
	Vector3 pa = {}, pb = {};

	// Aの方向ベクトル
	Vector3 u = aEndPos - aStartPos;
	// Bの方向ベクトル
	Vector3 v = bEndPos - bStartPos;

	// Bの始点からAの始点までのベクトル
	Vector3 w = aStartPos - bStartPos;

	float aLen = u.LengthSq();
	float bLen = v.LengthSq();
	float ab = u.Dot(v);
	float aw = u.Dot(w);
	float bw = v.Dot(w);

	float denom = aLen * bLen - ab * ab;
	float s, t;

	if (denom < 1e-6f) {
		// 線分がほぼ平行 → 片方に合わせて計算
		s = 0.0f;
		t = bw / bLen;
	}
	else {
		s = (ab * bw - bLen * aw) / denom;
		t = (aLen * bw - ab * aw) / denom;
	}

	// 線分内に clamp
	s = std::clamp(s, 0.0f, 1.0f);
	t = std::clamp(t, 0.0f, 1.0f);

	pa = aStartPos + u * s;  // A線分上の最近点
	pb = bStartPos + v * t;  // B線分上の最近点

	// 距離計算
	Vector3 normal = pa - pb;
	float distSq = normal.LengthSq();
	float radSum = aRadius + bRadius;

	// 
	if (distSq >= radSum * radSum) { return false; }

#pragma endregion

#pragma region 衝突方向を求める

	// 距離
	float dist = std::sqrt(distSq);

	// 衝突方向
	Vector3 collisionNormal;

	// B側からA側へ向かう方向
	if (dist > 1e-6f) { collisionNormal = normal / dist; }
	// 最近点が完全に同じ位置だった場合
	else {
		// 第一候補：Aの移動方向
		Vector3 velocity = -a->GetTransform().Velocity();

		// 第一候補がゼロベクトルなら、第二候補：Y軸方向を使用する
		collisionNormal = (velocity.LengthSq() > 1e-6f) ? velocity.Normalized() : Vector3::Yonly(1.0f);
	}

#pragma endregion

#pragma region 衝突点を求める

	// Aカプセル表面の、B側にある点
	const Vector3 pointOnA = pa - collisionNormal * aRadius;

	// Bカプセル表面の、A側にある点
	const Vector3 pointOnB = pb + collisionNormal * bRadius;

	// 両方の表面点の中間を衝突座標とする
	collisionPoint = (pointOnA + pointOnB) * 0.5f;

#pragma endregion

#pragma region 押し出しが必要か->必要なら押し出し
	// 押し出しが必要かどうか
	if (NeedPush(a, b)) {

		// めり込み量
		float overlap = radSum - dist;

		// 押し出し処理
		ApplyPush(a, b, normal.Normalized(), overlap);
	}
#pragma endregion

	// 当たった
	return true;
}

// ボックス×ボックス
bool CollisionManager::BoxToBox(BoxCollider* a, BoxCollider* b, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	// Bの中心からAの中心へ向かうベクトル
	const Vector3 centerDifference = a->GetPos() - b->GetPos();

	// 各ボックスの回転後の3軸
	const std::array<Vector3, 3> aAxes = a->GetAxes();

	const std::array<Vector3, 3> bAxes = b->GetAxes();

	// 最小めり込み量
	float minimumOverlap = FLT_MAX;

	// 最小めり込み軸
	Vector3 minimumAxis(0.0f, 1.0f, 0.0f);

#pragma endregion

#pragma region 衝突判定

	// 1軸分のSAT判定をするラムダ関数
	auto CheckAxis = [&](const Vector3& testAxis) -> bool {
		const float axisLengthSq = testAxis.LengthSq();

		// 外積がほぼゼロの場合、分離軸として使えないので無視
		if (axisLengthSq < 1e-8f) { return true; }

		// SAT判定用に正規化
		const Vector3 axis = testAxis / std::sqrt(axisLengthSq);

		// 各ボックスをこの軸に投影した半径
		const float aProjectionRadius = a->GetProjectionRadius(axis);

		const float bProjectionRadius = b->GetProjectionRadius(axis);

		// 軸上での中心間距離
		const float centerDistance = std::abs(centerDifference.Dot(axis));

		// めり込み量
		const float overlap = aProjectionRadius + bProjectionRadius - centerDistance;

		// この軸で離れているなら衝突していない
		if (overlap < 0.0f) { return false; }

		// 最もめり込みが浅い軸を保存
		if (overlap < minimumOverlap) {
			minimumOverlap = overlap;
			minimumAxis = axis;

			// BからAへ向く方向に統一
			if (centerDifference.Dot(minimumAxis) < 0.0f) { minimumAxis = -minimumAxis; }
		}

		return true;
		};


	// A側の3軸を判定
	for (const Vector3& axis : aAxes) {
		if (!CheckAxis(axis)) { return false; }
	}

	// B側の3軸を判定
	for (const Vector3& axis : bAxes) {
		if (!CheckAxis(axis)) { return false; }
	}

	// 各軸の外積9本を判定
	for (const Vector3& aAxis : aAxes) {
		for (const Vector3& bAxis : bAxes) {
			if (!CheckAxis(aAxis.Cross(bAxis))) { return false; }
		}
	}
#pragma endregion

#pragma region 衝突点を計算

	// Aの表面上でB側にある点
	const Vector3 pointOnA = a->GetSupportPoint(-minimumAxis);

	// Bの表面上でA側にある点
	const Vector3 pointOnB = b->GetSupportPoint(minimumAxis);

	// 両方の接触候補点の中間を衝突点とする
	collisionPoint = (pointOnA + pointOnB) * 0.5f;

#pragma endregion

	// 必要なら押し出し
	if (NeedPush(a, b)) {
		ApplyPush(a, b, minimumAxis, minimumOverlap);
	}

	return true;
}

// モデル×モデル
bool CollisionManager::ModelToModel(ModelCollider* a, ModelCollider* b, Vector3& collisionPoint)
{
	return false;
}

// XZ平面上の円形×XZ平面上の円形
bool CollisionManager::XZCircleToXZCircle(XZCircleCollider* a, XZCircleCollider* b, Vector3& collisionPoint)
{
#pragma region 必要情報を取得
	// XZ平面上のベクトルを取得
	Vector2 vec = a->GetPos().ToVector2XZ() - b->GetPos().ToVector2XZ();

	// 半径の合計
	float radius = a->GetRadius() + b->GetRadius();

	// Y方向の長さを取得
	float aYLength = a->GetYLength(), bYLength = b->GetYLength();
#pragma endregion

#pragma region 衝突判定

	// Y方向の判定
	if (abs(a->GetPos().y - b->GetPos().y) > (aYLength + bYLength) * 0.5f) { return false; }

	// XZ平面上の判定
	if (vec.LengthSq() > radius * radius) { return false; }

#pragma endregion

#pragma region 衝突確定：必要なら押し出し
	if (NeedPush(a, b)) {
		// めり込み量
		float overlap = radius - vec.Length();

		// ベクトルを正規化
		vec.Normalize();
		
		// 押し出し
		ApplyPush(a, b, Vector3::XZonly(vec.x, vec.y), overlap);
	}
#pragma endregion

	return true;
}

// 線分×球体
bool CollisionManager::LineToSphere(LineCollider* line, SphereCollider* sphere, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	// sphere（球体）
	const Vector3 spherePos = sphere->GetPos();
	const float radius = sphere->GetRadius();

#pragma endregion

#pragma region 衝突判定（Sphere中心から線分最近点までの距離）

	// 球体の中心に最も近い線分上の点
	const Vector3 closestPoint = line->ClosestPoint(spherePos);

	// 最近点から球体中心までのベクトル
	const Vector3 difference = spherePos - closestPoint;

	const float distanceSq = difference.LengthSq();

	if (distanceSq > radius * radius) { return false; }

#pragma endregion

#pragma region 衝突点を設定

	// 線分上にある、球体中心との最近点
	collisionPoint = closestPoint;

#pragma endregion

#pragma region 衝突確定：必要に応じて押し出し

	if (NeedPush(line, sphere))
	{
		// 現在も衝突しているか調べる
		auto IsColliding = [&]() -> bool {
			const Vector3 currentSpherePos = sphere->GetPos();

			const Vector3 currentClosestPoint = line->ClosestPoint(currentSpherePos);

			const float currentDistanceSq = (currentSpherePos - currentClosestPoint).LengthSq();

			return currentDistanceSq <= radius * radius;
			};

		// 1回の押し出し量
		const float onePush = 5.0f;

		// 線分の進行方向に押し出す
		const Vector3 pushVector = line->GetDirection().Normalized() * onePush;

		// 当たらなくなるまで少しずつ押し出す
		// 無限ループ防止のため最大50回
		for (unsigned char i = 0; i < 50; ++i) {
			line->SetTransformPosAdd(pushVector);

			if (!IsColliding()) { break; }
		}
	}

#pragma endregion

	return true;
}

// 線分×カプセル
bool CollisionManager::LineToCapsule(LineCollider* line, CapsuleCollider* capsule, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	// 判定対象の線分
	const Vector3 lineStart = line->GetStartPos();
	const Vector3 lineEnd = line->GetEndPos();

	// カプセルの中心軸
	const Vector3 capsuleStart = capsule->GetStartPos();
	const Vector3 capsuleEnd = capsule->GetEndPos();

	const float radius = capsule->GetRadius();

#pragma endregion

#pragma region 2本の線分間の最近点を取得

	// 線分1と線分2の最近点ペアを取得する
	auto ClosestPointsBetweenSegments = [](const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2, Vector3& closest1, Vector3& closest2) {
		constexpr float epsilon = 1e-8f;

		const Vector3 d1 = q1 - p1;
		const Vector3 d2 = q2 - p2;
		const Vector3 r = p1 - p2;

		const float a = d1.Dot(d1);
		const float e = d2.Dot(d2);
		const float f = d2.Dot(r);

		float s = 0.0f;
		float t = 0.0f;

		// 両方とも長さがほぼ0
		if (a <= epsilon && e <= epsilon) { closest1 = p1; closest2 = p2; return; }

		// line側だけ長さがほぼ0
		if (a <= epsilon) { s = 0.0f; t = std::clamp(f / e, 0.0f, 1.0f); }
		else {
			const float c = d1.Dot(r);

			// capsule中心軸側だけ長さがほぼ0
			if (e <= epsilon) {
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else {
				const float b = d1.Dot(d2);
				const float denominator = a * e - b * b;

				// 平行でなければline側の割合を計算
				if (std::abs(denominator) > epsilon) { s = std::clamp((b * f - c * e) / denominator, 0.0f, 1.0f); }
				// ほぼ平行
				else { s = 0.0f; }

				// capsule中心軸側の割合
				t = (b * s + f) / e;

				// capsule側の範囲から外れた場合、
				// 端点に固定してline側を再計算
				if (t < 0.0f) {
					t = 0.0f;
					s = std::clamp(-c / a, 0.0f, 1.0f);
				}
				else if (t > 1.0f) {
					t = 1.0f;
					s = std::clamp((b - c) / a, 0.0f, 1.0f);
				}
			}
		}

		closest1 = p1 + d1 * s;
		closest2 = p2 + d2 * t;
		};

	Vector3 lineClosest;
	Vector3 capsuleAxisClosest;

	ClosestPointsBetweenSegments(lineStart, lineEnd, capsuleStart, capsuleEnd, lineClosest, capsuleAxisClosest);

#pragma endregion

#pragma region 衝突判定

	const Vector3 difference = lineClosest - capsuleAxisClosest;

	const float distanceSq = difference.LengthSq();

	if (distanceSq > radius * radius) { return false; }

#pragma endregion

#pragma region 衝突位置を設定

	// 線分上にある、カプセル中心軸との最近点
	collisionPoint = lineClosest;

#pragma endregion

#pragma region 衝突確定：必要に応じて押し出し

	if (NeedPush(line, capsule))	{

		// 現在も衝突しているか判定
		auto IsColliding = [&]() -> bool {
			Vector3 currentLineClosest;
			Vector3 currentCapsuleClosest;

			ClosestPointsBetweenSegments(line->GetStartPos(), line->GetEndPos(), capsule->GetStartPos(), capsule->GetEndPos(), currentLineClosest, currentCapsuleClosest);

			return (currentLineClosest - currentCapsuleClosest).LengthSq() <= radius * radius;
			};

		// 1回で押し出す量
		const float step = 5.0f;

		// 線分の進行方向へ押し出す
		const Vector3 pushVector = line->GetDirection().Normalized() * step;

		// 当たらなくなるまで細かく押し出す
		for (int i = 0; i < 50; ++i) {
			line->SetTransformPosAdd(pushVector);

			if (!IsColliding()) { break; }
		}
	}

#pragma endregion

	return true;
}

// 線分×ボックス
bool CollisionManager::LineToBox(LineCollider* line, BoxCollider* box, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	const Vector3 lineStart = line->GetStartPos();
	const Vector3 lineEnd = line->GetEndPos();

	// 線分ベクトル
	const Vector3 lineVector = lineEnd - lineStart;

	const Vector3 pushDir = line->GetDirection().Normalized();

	const Vector3 boxPos = box->GetPos();

	const Vector3 half = box->GetHalfSize();

	// 回転後のBoxの3軸
	const std::array<Vector3, 3> boxAxes = box->GetAxes();

#pragma endregion

#pragma region 線分をBoxのローカル座標へ変換

	// Box中心から見た線分開始・終了位置
	const Vector3 startDifference = lineStart - boxPos;

	const Vector3 endDifference = lineEnd - boxPos;

	// Boxの各軸との内積を取ることで、
	// 回転していないBoxの座標系へ変換する
	const Vector3 localStart(
		startDifference.Dot(boxAxes[0]),
		startDifference.Dot(boxAxes[1]),
		startDifference.Dot(boxAxes[2])
	);

	const Vector3 localEnd(
		endDifference.Dot(boxAxes[0]),
		endDifference.Dot(boxAxes[1]),
		endDifference.Dot(boxAxes[2])
	);

	const Vector3 localDirection = localEnd - localStart;

#pragma endregion

#pragma region 線分とローカルAABBの交差判定

	// 線分上の位置
	// 0.0f = 開始点
	// 1.0f = 終了点
	float enterTime = 0.0f;
	float exitTime = 1.0f;

	// 衝突したBox面の法線
	Vector3 localHitNormal;

	auto CheckAxis =
		[&](float start, float direction, float minValue, float maxValue, const Vector3& negativeNormal, const Vector3& positiveNormal)		-> bool
		{
			constexpr float epsilon = 1e-8f;

			// 線分がこの軸方向へほぼ移動していない
			if (std::abs(direction) < epsilon) {
				// この軸のBox範囲外にいるなら交差しない
				if (start < minValue || start > maxValue) { return false; }

				return true;
			}

			float time1 = (minValue - start) / direction;

			float time2 = (maxValue - start) / direction;

			Vector3 normal1 = negativeNormal;
			Vector3 normal2 = positiveNormal;

			// 入る時刻と出る時刻を正しい順番にする
			if (time1 > time2) { std::swap(time1, time2); std::swap(normal1, normal2); }

			// より遅い侵入時刻を採用
			if (time1 > enterTime) { enterTime = time1; localHitNormal = normal1; }

			// より早い退出時刻を採用
			exitTime = (std::min)(exitTime, time2);

			// 入る時刻が出る時刻を越えたら交差しない
			if (enterTime > exitTime) { return false; }

			return true;
		};

	// X軸
	if (!CheckAxis(localStart.x, localDirection.x, -half.x, half.x, Vector3::Xonly(-1.0f), Vector3::Xonly(1.0f))) { return false; }

	// Y軸
	if (!CheckAxis(localStart.y, localDirection.y, -half.y, half.y, Vector3::Yonly(-1.0f), Vector3::Yonly(1.0f))) { return false; }

	// Z軸
	if (!CheckAxis(localStart.z, localDirection.z, -half.z, half.z, Vector3::Zonly(-1.0f), Vector3::Zonly(1.0f))) { return false; }

	// 線分の範囲外で交差している
	if (exitTime < 0.0f || enterTime > 1.0f) { return false; }

#pragma endregion

#pragma region 衝突点を計算

	// 線分がBoxへ入った位置
	const float hitTime = (std::max)(enterTime, 0.0f);

	collisionPoint = lineStart + lineVector * hitTime;

#pragma endregion

#pragma region 衝突確定：必要に応じて押し出し

	if (NeedPush(line, box)) {
		// 衝突面のローカル法線をワールド空間へ変換
		Vector3 worldHitNormal = boxAxes[0] * localHitNormal.x + boxAxes[1] * localHitNormal.y + boxAxes[2] * localHitNormal.z;

		// ワールド空間の法線を正規化
		worldHitNormal.Normalize();

		// 線分の始点がBox内部にある場合、終点よりも深く入り込んでいる可能性があるので、
		const float startDepth = -(lineStart - collisionPoint).Dot(worldHitNormal);

		// 線分の終点がBox内部にある場合、始点よりも深く入り込んでいる可能性があるので、
		const float endDepth = -(lineEnd - collisionPoint).Dot(worldHitNormal);

		// 始点・終点のうち、より深く入り込んでいる方を使う
		float overlap = (std::max)(startDepth, endDepth);

		// 浮動小数点誤差で負の値になる場合があるので、0未満なら0にする
		if (overlap < 0.0f) { overlap = 0.0f; }

		// 浮動小数点誤差による再衝突を防ぐ小さな余白
		overlap += 0.001f;

		// Boxの接触面法線方向へ押し出す
		line->SetTransformPosAdd(worldHitNormal * overlap);

		// 上向きの面に接触
		if (worldHitNormal.y > 0.5f) { line->CallOnGrounded(); }
	}

#pragma endregion

	return true;
}

// 線分×モデル
bool CollisionManager::LineToModel(LineCollider* line, ModelCollider* model, Vector3& collisionPoint)
{
	return false;
}

// 球体×カプセル
bool CollisionManager::SphereToCapsule(SphereCollider* sphere, CapsuleCollider* capsule, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	// sphere（球体）
	const Vector3 C = sphere->GetPos();
	const float rS = sphere->GetRadius();

	// capsule（カプセル）
	const Vector3 A = capsule->GetStartPos();
	const Vector3 B = capsule->GetEndPos();
	const float rC = capsule->GetRadius();

#pragma endregion

#pragma region カプセル中心軸上の最近点を取得

	const Vector3 AB = B - A;
	const Vector3 AC = C - A;

	const float abLenSq = AB.LengthSq();

	float t = 0.0f;

	if (abLenSq > 1e-6f) {
		t = AC.Dot(AB) / abLenSq;
		t = std::clamp(t, 0.0f, 1.0f);
	}

	// 球中心から最も近いカプセル中心軸上の点
	const Vector3 Q = A + AB * t;

#pragma endregion

#pragma region 衝突判定

	// カプセル中心軸から球中心へ向かうベクトル
	Vector3 normal = C - Q;

	const float distSq = normal.LengthSq();

	const float radiusSum = rS + rC;

	if (distSq >= radiusSum * radiusSum) { return false; }

#pragma endregion

#pragma region 衝突点を計算

	constexpr float epsilon = 1e-6f;

	if (distSq > epsilon * epsilon) {
		// カプセル中心軸から球体側へ向かう単位ベクトル
		const Vector3 normalN = normal / std::sqrt(distSq);

		// カプセル表面上の接触候補点
		const Vector3 pointOnCapsule = Q + normalN * rC;

		// 球体表面上の接触候補点
		const Vector3 pointOnSphere = C - normalN * rS;

		// めり込み中でも偏りが出にくいよう、
		// 両表面点の中間を代表衝突点にする
		collisionPoint = (pointOnCapsule + pointOnSphere) * 0.5f;
	}
	else {
		// 球中心がカプセル中心軸と完全に一致している場合、
		// 接触方向を一意に決められない
		Vector3 fallbackNormal = -sphere->GetTransform().Velocity();

		if (fallbackNormal.LengthSq() <= epsilon * epsilon) { fallbackNormal = Vector3::Yonly(1.0f); }

		fallbackNormal.Normalize();

		collisionPoint = Q + fallbackNormal * rC;

		normal = fallbackNormal;
	}

#pragma endregion

#pragma region 衝突確定：必要なら押し出し

	if (NeedPush(sphere, capsule)) {
		float dist = std::sqrt(distSq);

		if (dist < epsilon) {
			// 完全一致時は先ほど決めた予備方向を使用
			if (normal.LengthSq() <= epsilon * epsilon) {
				normal = -sphere->GetTransform().Velocity();

				if (normal.LengthSq() <= epsilon * epsilon) { normal = Vector3::Yonly(1.0f); }
			}

			dist = 0.0f;
		}

		const float overlap = radiusSum - dist;

		ApplyPush(sphere, capsule, normal.Normalized(), overlap);
	}

#pragma endregion

	return true;
}

// 球体×ボックス
bool CollisionManager::SphereToBox(SphereCollider* sphere, BoxCollider* box, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	const Vector3 spherePos = sphere->GetPos();

	const float radius = sphere->GetRadius();

	const Vector3 boxPos = box->GetPos();

	const Vector3 half = box->GetHalfSize();

	// 回転後のBoxのローカル3軸
	const std::array<Vector3, 3> boxAxes = box->GetAxes();

#pragma endregion

#pragma region 球中心をBoxローカル座標へ変換

	const Vector3 centerDiff = spherePos - boxPos;

	const Vector3 localSpherePos(
		centerDiff.Dot(boxAxes[0]),
		centerDiff.Dot(boxAxes[1]),
		centerDiff.Dot(boxAxes[2])
	);

#pragma endregion

#pragma region Box上の最近点を取得

	// Boxローカル空間での最近点
	Vector3 localNearest;

	localNearest.x = std::clamp(localSpherePos.x, -half.x, half.x);

	localNearest.y = std::clamp(localSpherePos.y, -half.y, half.y);

	localNearest.z = std::clamp(localSpherePos.z, -half.z, half.z);

	// 最近点をワールド座標へ戻す
	Vector3 nearest = boxPos + boxAxes[0] * localNearest.x + boxAxes[1] * localNearest.y + boxAxes[2] * localNearest.z;

#pragma endregion

#pragma region 衝突判定

	// Box表面の最近点から球中心へ向かうベクトル
	Vector3 normal = spherePos - nearest;

	const float distSq = normal.LengthSq();

	if (distSq > radius * radius) { return false; }

#pragma endregion

#pragma region 押し出し方向・めり込み量を取得

	constexpr float epsilon = 1e-6f;

	Vector3 pushNormal;
	float overlap = 0.0f;

	if (distSq > epsilon * epsilon) {
		// 球中心がBoxの外側にある通常ケース
		const float dist = std::sqrt(distSq);

		pushNormal = normal / dist;

		overlap = radius - dist;

		// Box表面上の最近点を衝突点にする
		collisionPoint = nearest;
	}
	else
	{
		// 球中心がBox内部、またはBox表面と完全一致しているケース

		// 各面までの距離
		const float distanceToXFace = half.x - std::abs(localSpherePos.x);

		const float distanceToYFace = half.y - std::abs(localSpherePos.y);

		const float distanceToZFace = half.z - std::abs(localSpherePos.z);

		Vector3 localPushNormal;
		Vector3 localSurfacePoint = localSpherePos;

		float distanceToSurface = 0.0f;

		// 最も近い面を選ぶ
		if (distanceToXFace <= distanceToYFace && distanceToXFace <= distanceToZFace) {

			const float sign = localSpherePos.x >= 0.0f ? 1.0f : -1.0f;

			localPushNormal = Vector3(sign, 0.0f, 0.0f);

			localSurfacePoint.x = sign * half.x;

			distanceToSurface = distanceToXFace;
		}
		else if (distanceToYFace <= distanceToXFace && distanceToYFace <= distanceToZFace) {

			const float sign = localSpherePos.y >= 0.0f ? 1.0f : -1.0f;

			localPushNormal = Vector3(0.0f, sign, 0.0f);

			localSurfacePoint.y = sign * half.y;

			distanceToSurface = distanceToYFace;
		}
		else
		{
			const float sign = localSpherePos.z >= 0.0f ? 1.0f : -1.0f;

			localPushNormal = Vector3(0.0f, 0.0f, sign);

			localSurfacePoint.z = sign * half.z;

			distanceToSurface = distanceToZFace;
		}

		// ローカル法線をワールド法線へ変換
		pushNormal = boxAxes[0] * localPushNormal.x + boxAxes[1] * localPushNormal.y + boxAxes[2] * localPushNormal.z;

		pushNormal.Normalize();

		// Box内部から外へ出す距離＋球の半径
		overlap = distanceToSurface + radius;

		// 選択した最寄り面上の点をワールド座標へ変換
		collisionPoint = boxPos + boxAxes[0] * localSurfacePoint.x + boxAxes[1] * localSurfacePoint.y + boxAxes[2] * localSurfacePoint.z;
	}

#pragma endregion

#pragma region 衝突確定：必要に応じて押し出し

	if (NeedPush(sphere, box)) {
		if (overlap < 0.0f) { overlap = 0.0f; }

		ApplyPush(sphere, box, pushNormal, overlap);
	}

#pragma endregion

	return true;
}

// 球体×モデル
bool CollisionManager::SphereToModel(SphereCollider* sphere, ModelCollider* model, Vector3& collisionPoint)
{
	//// まず球のAABBを作る
	//ColliderBase::AABB sphereAABB = sphere->GetAABB();

	//// モデル内部チャンクから近い三角形だけ取得
	//const auto& triangles = model->GetNearTriangles(sphereAABB);

	//for (const auto* tri : triangles) {
	//	// 三角形AABBと球AABBが離れてたらスキップ
	//	if (!AABBToAABB(sphereAABB, tri->aabb)) { continue; }

	//	// 球中心から三角形への最近点を求める
	//	Vector3 closest = ClosestPointOnTriangle(
	//		sphere->GetPos(),
	//		tri->p0,
	//		tri->p1,
	//		tri->p2
	//	);

	//	Vector3 diff = sphere->GetPos() - closest;

	//	// 当たった
	//	if (diff.LengthSq() <= sphere->GetRadius() * sphere->GetRadius()) { return true; }
	//}

	return false;
}

// 球体×XZ平面上の円形
bool CollisionManager::SphereToXZCircle(SphereCollider* sphere, XZCircleCollider* xzcircle, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	// 球体の中心座標
	const Vector3 spherePos = sphere->GetPos();
	// 球体の半径
	const float sphereRadius = sphere->GetRadius();

	// XZCircleColliderの中心座標
	const Vector3 circlePos = xzcircle->GetPos();
	// XZCircleColliderの半径
	const float circleRadius = xzcircle->GetRadius();
	// XZCircleColliderのY方向の長さ
	const float yLength = xzcircle->GetYLength();


	// XZ平面上で、円の中心から球の中心へ向かうベクトル
	Vector2 vec = spherePos.ToVector2XZ() - circlePos.ToVector2XZ();

	// 半径の合計
	const float radiusSum = sphereRadius + circleRadius;

#pragma endregion

#pragma region Y方向の早期リターン

	// XZCircleColliderのY方向範囲
	const float circleMinY = circlePos.y - yLength;

	const float circleMaxY = circlePos.y + yLength;

	// SphereのY方向範囲
	const float sphereMinY = spherePos.y - sphereRadius;

	const float sphereMaxY = spherePos.y + sphereRadius;

	// Y方向で完全に離れている
	if (sphereMaxY < circleMinY || sphereMinY > circleMaxY) { return false; }

#pragma endregion

#pragma region XZ平面上で衝突判定

	const float distanceSq = vec.LengthSq();

	if (distanceSq > radiusSum * radiusSum) { return false; }

#pragma endregion

#pragma region XZ方向の法線を取得

	constexpr float epsilon = 1e-6f;

	Vector3 pushNormal;

	float distance = 0.0f;

	if (distanceSq > epsilon * epsilon) {
		distance = std::sqrt(distanceSq);

		pushNormal = Vector3::XZonly(vec.x / distance, vec.y / distance);
	}
	else {
		/*
			XZ座標が完全一致している場合は
			押し出す方向を計算できない。

			そのため、Sphereの移動方向の逆方向を使う。
		*/

		Vector3 fallbackNormal = -sphere->GetTransform().Velocity();

		// Y方向は使用しない
		fallbackNormal.y = 0.0f;

		if (fallbackNormal.LengthSq() > epsilon * epsilon) { pushNormal = fallbackNormal.Normalized(); }
		// 移動していない場合は固定方向
		else { pushNormal = Vector3::Xonly(1.0f); }

		distance = 0.0f;
	}

#pragma endregion

#pragma region 衝突点を計算

	/*
		衝突点のY座標。

		Sphereの中心Yを、XZCircleColliderの
		Y範囲内に収めた位置にする。
	*/
	const float collisionY = std::clamp(spherePos.y, circleMinY, circleMaxY);

	// XZCircleCollider表面上の点
	const Vector3 pointOnCircle(circlePos.x + pushNormal.x * circleRadius, collisionY, circlePos.z + pushNormal.z * circleRadius);

	// Sphere表面上の点
	const Vector3 pointOnSphere(spherePos.x - pushNormal.x * sphereRadius, collisionY, spherePos.z - pushNormal.z * sphereRadius);

	// 両方の表面点の中間を代表衝突点にする
	collisionPoint = (pointOnCircle + pointOnSphere) * 0.5f;

#pragma endregion

#pragma region 衝突確定：必要ならXZ方向へ押し出し

	if (NeedPush(sphere, xzcircle)) {
		// XZ平面上のめり込み量
		const float overlap = radiusSum - distance;

		ApplyPush(sphere, xzcircle, pushNormal, overlap);
	}

#pragma endregion

	return true;
}

// カプセル×ボックス
bool CollisionManager::CapsuleToBox(CapsuleCollider* capsule, BoxCollider* box, Vector3& collisionPoint)
{
#pragma region 必要情報の取得

	// カプセル中心軸
	const Vector3 capsuleStart = capsule->GetStartPos();
	const Vector3 capsuleEnd = capsule->GetEndPos();
	const float capsuleRadius = capsule->GetRadius();

	// Box
	const Vector3 boxPos = box->GetPos();
	const Vector3 half = box->GetHalfSize();

	// 回転後のBoxの3軸
	const std::array<Vector3, 3> boxAxes = box->GetAxes();

#pragma endregion

#pragma region カプセル中心軸をBoxローカル座標へ変換

	const Vector3 startDifference = capsuleStart - boxPos;

	const Vector3 endDifference = capsuleEnd - boxPos;

	const Vector3 localStart(startDifference.Dot(boxAxes[0]), startDifference.Dot(boxAxes[1]), startDifference.Dot(boxAxes[2]));

	const Vector3 localEnd(endDifference.Dot(boxAxes[0]), endDifference.Dot(boxAxes[1]), endDifference.Dot(boxAxes[2]));

#pragma endregion

#pragma region 線分とAABBの最近点ペアを求める

	// Boxローカル空間でのカプセル中心軸線分
	const Vector3 localSegment = localEnd - localStart;

	// 線分上の割合
	float t = 0.5f;

	// 
	Vector3 localCapsuleClosest;
	Vector3 localBoxClosest;

	constexpr int iterationCount = 8;

	for (int i = 0; i < iterationCount; ++i) {
		// 現在の線分上の点
		localCapsuleClosest = localStart + localSegment * t;

		// その点からBoxへの最近点
		localBoxClosest.x = std::clamp(localCapsuleClosest.x, -half.x, half.x);

		localBoxClosest.y = std::clamp(localCapsuleClosest.y, -half.y, half.y);

		localBoxClosest.z = std::clamp(localCapsuleClosest.z, -half.z, half.z);

		// Box最近点を線分へ再投影
		const float segmentLengthSq = localSegment.LengthSq();

		if (segmentLengthSq <= 1e-8f) { t = 0.0f;			break; }

		t = (localBoxClosest - localStart).Dot(localSegment) / segmentLengthSq;
		t = std::clamp(t, 0.0f, 1.0f);
	}

	// 最終結果を再計算
	localCapsuleClosest = localStart + localSegment * t;

	localBoxClosest.x = std::clamp(localCapsuleClosest.x, -half.x, half.x);
	localBoxClosest.y = std::clamp(localCapsuleClosest.y, -half.y, half.y);
	localBoxClosest.z = std::clamp(localCapsuleClosest.z, -half.z, half.z);

#pragma endregion

#pragma region 最近点をワールド座標へ変換

	const Vector3 capsuleClosest = boxPos + boxAxes[0] * localCapsuleClosest.x + boxAxes[1] * localCapsuleClosest.y + boxAxes[2] * localCapsuleClosest.z;
	const Vector3 boxClosest = boxPos + boxAxes[0] * localBoxClosest.x + boxAxes[1] * localBoxClosest.y + boxAxes[2] * localBoxClosest.z;

#pragma endregion

#pragma region 衝突判定

	Vector3 normal = capsuleClosest - boxClosest;

	const float distSq = normal.LengthSq();

	if (distSq > capsuleRadius * capsuleRadius) { return false; }

#pragma endregion

#pragma region 衝突方向・めり込み量・衝突点を計算

	constexpr float epsilon = 1e-6f;

	Vector3 pushNormal;
	float overlap = 0.0f;

	if (distSq > epsilon * epsilon) {
		// カプセル中心軸がBox外側にある通常ケース
		const float distance = std::sqrt(distSq);

		pushNormal = normal / distance;

		overlap = capsuleRadius - distance;

		// カプセル表面上の点
		const Vector3 pointOnCapsule = capsuleClosest - pushNormal * capsuleRadius;

		// Box表面上の最近点との中間
		collisionPoint = (pointOnCapsule + boxClosest) * 0.5f;
	}
	else {

		const float distanceToXFace = half.x - std::abs(localCapsuleClosest.x);
		const float distanceToYFace = half.y - std::abs(localCapsuleClosest.y);
		const float distanceToZFace = half.z - std::abs(localCapsuleClosest.z);

		Vector3 localPushNormal;
		Vector3 localSurfacePoint = localCapsuleClosest;

		float distanceToSurface = 0.0f;

		if (distanceToXFace <= distanceToYFace && distanceToXFace <= distanceToZFace) {
			float sign = 1.0f;

			if (localCapsuleClosest.x < 0.0f) { sign = -1.0f; }

			localPushNormal = Vector3::Xonly(sign);

			localSurfacePoint.x = half.x * sign;

			distanceToSurface = distanceToXFace;
		}
		else if (distanceToYFace <= distanceToXFace && distanceToYFace <= distanceToZFace) {
			float sign = 1.0f;

			if (localCapsuleClosest.y < 0.0f) { sign = -1.0f; }

			localPushNormal = Vector3::Yonly(sign);

			localSurfacePoint.y = half.y * sign;

			distanceToSurface = distanceToYFace;
		}
		else {
			float sign = 1.0f;

			if (localCapsuleClosest.z < 0.0f) { sign = -1.0f; }

			localPushNormal = Vector3::Zonly(sign);

			localSurfacePoint.z = half.z * sign;

			distanceToSurface = distanceToZFace;
		}

		// ローカル法線をワールド空間へ変換
		pushNormal = boxAxes[0] * localPushNormal.x + boxAxes[1] * localPushNormal.y + boxAxes[2] * localPushNormal.z;
		pushNormal.Normalize();

		// Box内部から表面までの距離＋カプセル半径
		overlap = distanceToSurface + capsuleRadius;

		// 最寄り面上の点をワールド座標へ変換
		collisionPoint = boxPos + boxAxes[0] * localSurfacePoint.x + boxAxes[1] * localSurfacePoint.y + boxAxes[2] * localSurfacePoint.z;
	}

#pragma endregion

#pragma region 衝突確定：必要なら押し出し

	if (NeedPush(capsule, box)) {
		if (overlap < 0.0f) { overlap = 0.0f; }

		ApplyPush(capsule, box, pushNormal, overlap);
	}

#pragma endregion

	return true;
}

// カプセル×モデル
bool CollisionManager::CapsuleToModel(CapsuleCollider* capsule, ModelCollider* model, Vector3& collisionPoint)
{
	return false;
}

// カプセル×XZ平面上の円形
bool CollisionManager::CapsuleToXZCircle(CapsuleCollider* capsule, XZCircleCollider* xzcircle, Vector3& collisionPoint)
{
#pragma region 必要情報を取得

	const Vector3 capsuleStart = capsule->GetStartPos();

	const Vector3 capsuleEnd = capsule->GetEndPos();

	const float capsuleRadius = capsule->GetRadius();

	const Vector3 circlePos = xzcircle->GetPos();

	const float circleRadius = xzcircle->GetRadius();

	const float circleYLength = xzcircle->GetYLength();

	const float radiusSum = capsuleRadius + circleRadius;

#pragma endregion

#pragma region Y方向の早期リターン

	// カプセル中心軸のY方向範囲
	const float capsuleAxisMinY = (std::min)(capsuleStart.y, capsuleEnd.y);

	const float capsuleAxisMaxY = (std::max)(capsuleStart.y, capsuleEnd.y);

	// カプセル全体のY方向範囲
	const float capsuleMinY = capsuleAxisMinY - capsuleRadius;

	const float capsuleMaxY = capsuleAxisMaxY + capsuleRadius;

	// XZCircleColliderのY方向範囲
	const float circleMinY = circlePos.y - circleYLength;

	const float circleMaxY = circlePos.y + circleYLength;

	// Y方向で完全に離れている
	if (capsuleMaxY < circleMinY || capsuleMinY > circleMaxY) { return false; }

#pragma endregion

#pragma region XZ平面へ投影

	Vector3 startXZ = capsuleStart;

	Vector3 endXZ = capsuleEnd;

	Vector3 circleXZ = circlePos;

	startXZ.y = 0.0f;
	endXZ.y = 0.0f;
	circleXZ.y = 0.0f;

#pragma endregion

#pragma region カプセル中心軸上の最近点を取得

	// XZ平面上のカプセル中心軸
	const Vector3 segmentXZ = endXZ - startXZ;

	const Vector3 startToCircle = circleXZ - startXZ;

	const float segmentLengthSq = segmentXZ.LengthSq();

	float t = 0.0f;

	if (segmentLengthSq > 1e-8f) {
		t = segmentXZ.Dot(startToCircle) / segmentLengthSq;

		t = std::clamp(t, 0.0f, 1.0f);
	}

	// XZ平面上の最近点
	const Vector3 closestXZ = startXZ + segmentXZ * t;

	// XZ平面上で求めたtを使って、実際の3Dカプセル中心軸上の点も求める
	const Vector3 capsuleAxis = capsuleEnd - capsuleStart;
	const Vector3 closestOnCapsuleAxis = capsuleStart + capsuleAxis * t;

#pragma endregion

#pragma region XZ方向の衝突判定

	const Vector3 differenceXZ = closestXZ - circleXZ;

	const float distanceSq = differenceXZ.LengthSq();

	if (distanceSq > radiusSum * radiusSum) { return false; }

#pragma endregion

#pragma region XZ方向の法線を取得

	constexpr float epsilon = 1e-6f;

	Vector3 pushNormal;

	float distance = 0.0f;

	if (distanceSq > epsilon * epsilon) {
		distance = std::sqrt(distanceSq);

		pushNormal = Vector3::XZonly(differenceXZ.x / distance, differenceXZ.z / distance);
	}
	// XZ座標が完全に一致している場合
	else {

		// 移動方向の逆をXZ平面へ投影して使用する。
		Vector3 fallbackNormal = -capsule->GetTransform().Velocity();
		fallbackNormal.y = 0.0f;

		if (fallbackNormal.LengthSq() > epsilon * epsilon) { pushNormal = fallbackNormal.Normalized(); }
		// 移動していなければ固定方向
		else { pushNormal = Vector3::Xonly(1.0f); }

		distance = 0.0f;
	}

#pragma endregion

#pragma region 衝突点を計算

	/*
		衝突点のY座標は、
		カプセル中心軸上の最近点Yを
		XZCircleColliderの高さ範囲に収める。
	*/
	const float collisionY = std::clamp(closestOnCapsuleAxis.y, circleMinY, circleMaxY);

	// XZCircleCollider表面上の点
	const Vector3 pointOnCircle(circlePos.x + pushNormal.x * circleRadius, collisionY, circlePos.z + pushNormal.z * circleRadius);

	// カプセル表面上の点
	const Vector3 pointOnCapsule(closestOnCapsuleAxis.x - pushNormal.x * capsuleRadius, collisionY, closestOnCapsuleAxis.z - pushNormal.z * capsuleRadius);

	// 両形状の表面点の中間
	collisionPoint = (pointOnCircle + pointOnCapsule) * 0.5f;

#pragma endregion

#pragma region 衝突確定：必要ならXZ方向へ押し出し

	if (NeedPush(capsule, xzcircle)) {
		const float overlap = radiusSum - distance;

		ApplyPush(capsule, xzcircle, pushNormal, overlap);
	}

#pragma endregion

	return true;
}

// ボックス×モデル
bool CollisionManager::BoxToModel(BoxCollider* box, ModelCollider* model, Vector3& collisionPoint)
{
	return false;
}

#pragma endregion

#pragma region デバッグ描画

// チャンクのグリッド線描画
void CollisionManager::DrawChunkGrid(const Vector3& cameraPos) const
{
	if (!App::GetIns().IsDrawDebug()) { return; }

	const unsigned int color = GetColor(255, 255, 255);

	// カメラ周辺どこまで描くか
	const float drawRange = 3000.0f;

	int minX = ToChunkIndex(cameraPos.x - drawRange, CHUNK_SIZE);
	int maxX = ToChunkIndex(cameraPos.x + drawRange, CHUNK_SIZE);

	int minY = ToChunkIndex(cameraPos.y - drawRange, CHUNK_SIZE);
	int maxY = ToChunkIndex(cameraPos.y + drawRange, CHUNK_SIZE);

	int minZ = ToChunkIndex(cameraPos.z - drawRange, CHUNK_SIZE);
	int maxZ = ToChunkIndex(cameraPos.z + drawRange, CHUNK_SIZE);

	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			DrawLine3D(
				VGet(x * CHUNK_SIZE, y * CHUNK_SIZE, minZ * CHUNK_SIZE),
				VGet(x * CHUNK_SIZE, y * CHUNK_SIZE, (maxZ + 1) * CHUNK_SIZE),
				color
			);
		}
	}

	for (int z = minZ; z <= maxZ; z++) {
		for (int y = minY; y <= maxY; y++) {
			DrawLine3D(
				VGet(minX * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE),
				VGet((maxX + 1) * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE),
				color
			);
		}
	}

	for (int x = minX; x <= maxX; x++) {
		for (int z = minZ; z <= maxZ; z++) {
			DrawLine3D(
				VGet(x * CHUNK_SIZE, minY * CHUNK_SIZE, z * CHUNK_SIZE),
				VGet(x * CHUNK_SIZE, (maxY + 1) * CHUNK_SIZE, z * CHUNK_SIZE),
				color
			);
		}
	}
}


#pragma endregion
