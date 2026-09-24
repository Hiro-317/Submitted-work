#pragma once

#include "../../../pch.h"

#include <algorithm>
#include <functional>
#include <vector>

#include "../Transform.h"

#include "ColliderTagDefine.h"

#include "../../Character/CharacterStatsDefine.h"

class ColliderBase
{
public:

	// 形状列挙型定義
	enum class SHAPE
	{
		NON = -1,
		LINE,
		SPHERE,
		CAPSULE,
		BOX,
		MODEL,
		XZ_CIRCLE,
	};

	// 最小/最大 座標 構造体
	struct AABB { Vector3 min, max; };

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">当たり判定タイプ</param>
	/// <param name="pos">相対座標（引数省略で{0.0f,0.0f,0.0f}）</param>
	ColliderBase(COLLIDER_TAG type, const Vector3& pos = Vector3(), const Vector3& angle = Vector3()) :
		trans(nullptr),
		pos(pos),
		angle(angle),
		judgeFlg(true),
		dynamicFlg(true),
		pushFlg(true),
		pushWeight(0),
		tag(type),
		shape(SHAPE::NON),
		OnCollision(nullptr),
		skillStats(nullptr)
	{
	}
	virtual ~ColliderBase() = default;

	// デバッグ描画
	virtual void DrawDebug(unsigned int color = 0xffffff) = 0;

#pragma region 初期設定
	// モデル制御情報セット
	void SetTransformPtr(Transform* ptr) { trans = ptr; }

	// 当たり判定通知用関数セット
	void SetOnCollisionFunc(std::function<void(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)> OnCollisionFunc) { OnCollision = std::move(OnCollisionFunc); }

	// 接地判定通知用関数セット
	void SetOnGroundedFunc(std::function<void(void)>OnGroundedFunc) { OnGrounded = std::move(OnGroundedFunc); }

	// スキル情報セット
	void SetSkillStats(const SkillStats* ptr) { skillStats = ptr; }
#pragma endregion

#pragma region 各ゲット関数
	// コライダー座標（モデル制御情報の座標がそのままコライダーの座標とは限らない為、計算済みの座標を取得する関数を用意）
	Vector3 GetPos(void)const { return (trans->pos + trans->VTrans(pos)); }

	// 1フレーム前のコライダー座標（モデル制御情報の座標がそのままコライダーの座標とは限らない為、計算済みの座標を取得する関数を用意）
	Vector3 GetPrevPos(void)const { return (trans->prevPos + trans->VTrans(pos)); }

	// コライダー角度（モデル制御情報の角度がそのままコライダーの角度とは限らない為、計算済みの角度を取得する関数を用意）
	Vector3 GetAngle(void)const { return (trans->angle + angle); }

	MATRIX GetAngleMat(void)const { return MatrixAllMultXZY({ trans->angle,angle }); }

	Vector3 VTrans(const Vector3& v)const { return (v != 0.0f) ? Vector3(VTransform(v.ToVECTOR(), GetAngleMat())) : Vector3(); }

	// モデル制御情報を直接取得
	const Transform& GetTransform(void)const { return *trans; }

	// 動的オブジェクトか否か（true = 動的、false = 静的）
	bool GetDynamicFlg(void)const { return dynamicFlg; }

	// 当たり判定フラグ（true = 「判定する」、false = 「判定しない」）
	bool GetJudge(void)const { return judgeFlg; }

	// 押し出しのフラグ
	bool GetPushFlg(void)const { return pushFlg; }

	// 押し出しを行う際の重さ（0 ～ 100）
	unsigned char GetPushWeight(void)const { return pushWeight; }

	// 当たり判定のタイプ
	COLLIDER_TAG GetTag(void)const { return tag; }

	// 当たり判定の形状
	SHAPE GetShape(void)const { return shape; }

	// 判定通知の呼び出し
	void CallOnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint) { OnCollision(ownTag,other,collisionPoint); }

	// 接地判定の呼び出し
	void CallOnGrounded(void) { OnGrounded(); }

	// 自分が占有している範囲(チャンク分けに使用)
	virtual AABB GetAABB(void) const = 0;

	// スキル情報参照
	const SkillStats& GetSkillStats(void)const { return *skillStats; }
#pragma endregion

#pragma region 各セット関数
	// モデル制御情報の座標情報を書き換える
	void SetTransformPos(const Vector3& pos) { trans->pos = pos; }
	void SetTransformPosAdd(const Vector3& vec) { trans->pos += vec; }

	// 動的オブジェクトか否かを設定する（true = 動的、false = 静的）
	void SetDynamicFlg(bool flg) { dynamicFlg = flg; }

	// 当たり判定フラグセット（true = 「判定する」、false = 「判定しない」）
	void SetJudgeFlg(bool flg) { judgeFlg = flg; }

	// 押し出しを行うかどうかのフラグを設定（true = 「押し出す」、false = 「押し出さない」）
	void SetPushFlg(bool flg) { pushFlg = flg; }

	// 押し出しを行う際の重さ（0 ～ 100 で設定）
	void SetPushWeight(unsigned char weight) { pushWeight = weight; }
#pragma endregion

private:
	// モデル制御情報をポインタで受け取って保持
	Transform* trans;
	
	// モデル制御情報の座標からの相対座標
	Vector3 pos;

	// モデル制御情報の角度からの相対角度
	Vector3 angle;

	// 動的オブジェクトか否か（true = 動的、false = 静的）
	bool dynamicFlg;

	// 当たり判定フラグ（true = 「判定する」、false = 「判定しない」）
	bool judgeFlg;

	// 判定をする場合、押し出しを行うかどうかのフラグ（true = 「押し出す」、false = 「押し出さず通り抜ける」）
	bool pushFlg;

	// 押し出しを行う際の重さ（0 ～ 100 で設定）
	unsigned char pushWeight;
	

	// 当たり判定タイプ（何と当たったかを見分ける用）
	COLLIDER_TAG tag;

	// 当たり判定形状
	SHAPE shape;
	
	// 当たったときに呼び出す関数をポインタで受け取って保持
	std::function<void(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)>OnCollision;

	// 接地したときに呼び出す関数をポインタで受け取って保持
	std::function<void(void)>OnGrounded;

	// 必要に応じてスキル情報をポインタで受け取って保持
	const SkillStats* skillStats;

protected:
	void SetShape(SHAPE s) { shape = s; }
};

using SHAPE = ColliderBase::SHAPE;

/// <summary>
/// 特定のコライダーを探す
/// </summary>
/// <typeparam name="T">探したいクラス</typeparam>
/// <param name="tag">タグ種類</param>
/// <returns></returns>
template<typename T = ColliderBase>
std::vector<T*> ColliderSerch(std::vector<ColliderBase*> collider, COLLIDER_TAG tag = COLLIDER_TAG::NON) {
	std::vector<T*> out;
	out.reserve(collider.size());

	for (auto c : collider) {
		if (!c) continue;
		if (auto* ptr = dynamic_cast<T*>(c)) {
			if (c->GetTag() == tag || tag == COLLIDER_TAG::NON) { out.push_back(ptr); }
		}
	}
	return out;
}