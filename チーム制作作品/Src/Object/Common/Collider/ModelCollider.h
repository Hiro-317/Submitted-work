#pragma once

#include"ColliderBase.h"

#include "../../../Manager/Collision/ChunkStDefine.h"

class ModelCollider : public ColliderBase
{
private:

	// ポリゴン構造体
	struct Triangle {

		// 頂点座標
		Vector3 p0, p1, p2;

		// 法線ベクトル
		Vector3 normal;

		// ポリゴン 最小/最大 座標
		AABB aabb;

		Triangle(
			const Vector3& p0, const Vector3& p1, const Vector3& p2,
			const Vector3& normal,
			AABB aabb
		) :
			p0(p0), p1(p1), p2(p2),
			normal(normal),
			aabb(aabb)
		{
		}

		Triangle(void) :
			p0(), p1(), p2(),
			normal(),
			aabb()
		{
		}
	};

public:
	ModelCollider(COLLIDER_TAG type, const Vector3& pos = Vector3(), const Vector3& angle = Vector3()) :
		ColliderBase(type, pos, angle)
	{
		SetShape(SHAPE::MODEL);
	}
	~ModelCollider()override {}

	// 自分が占有している範囲(チャンク分けに使用)
	AABB GetAABB(void) const override{
		
	}

	void DrawDebug(unsigned int color = 0xffffff)override {
	}

	const std::vector<Triangle*>& GetNearTriangles(const AABB& aabb) {

	}

private:

	std::vector<Triangle> triangles;

	// メッシュ用チャンク
	std::unordered_map<ChunkIndex, std::vector<int>, ChunkIndexHash> meshChunks;

	AABB modelAABB;

};