#pragma once

#include"../../Object/Common/Transform.h"

#include <algorithm>

class CameraEventBase;

class CameraBase
{
public:

	// カメラのクリップ範囲
	static constexpr float VIEW_NEAR = 10.0f;
	static constexpr float VIEW_FAR = 30000.0f;

	// マウス感度
	static constexpr float MOUSE_SENSI = 35.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="angle">角度</param>
	/// <param name="fov">視野角</param>
	CameraBase(const Vector3& pos = Vector3(), const Vector3& angle = Vector3(), float fov = 60.0f * (DX_PI_F / 180.0f));
	// デストラクタ
	virtual ~CameraBase() = default;

#pragma region 主要関数

	// 初期化
	virtual void Init(void) {}

	// 更新
	void Update(void);

	// 適用
	void Apply(void);

	// デバッグ用描画
	virtual void DrawDebug(void) const;

	// 終了
	void Release(void);

#pragma endregion

#pragma region ゲット関数
	// 座標
	const Vector3& GetPos(void)const { return pos; }
	// 角度
	const Vector3& GetAngle(void)const { return angle; }
	// 視野角
	float GetFov(void)const { return fov; }
#pragma endregion

#pragma region セット関数
	// 座標
	void SetPos(const Vector3& pos) { this->pos = pos; }
	// 角度
	void SetAngle(const Vector3& angle) { this->angle = angle; }
	// 視野角
	void SetFov(float fov) { this->fov = fov; }
#pragma endregion

#pragma region イベント関数

	// イベント開始
	void StartEvent(CameraEventBase* event);

	// イベント終了（更新以外で呼び出された場合強制終了となる）
	void EndEvent(void);

	// イベント中か
	bool IsEvent(void) const { return cameraEvent != nullptr; }

#pragma endregion

protected:

	// 座標
	Vector3 pos;

	// 角度
	Vector3 angle;

	// 視野角
	float fov;

	// 通常の更新（派生先で再定義前提）
	virtual void NormalUpdate(void) {}

	// 追加の終了処理
	virtual void SubRelease(void) {}

	// 座標と注視点から角度を割り出す
	static Vector3 CalcCameraAngle(const Vector3& pos, const Vector3& lookAtPos) {

		// カメラから注視点へ向かうベクトル
		Vector3 vec = lookAtPos - pos;

		// 安全処理
		if (vec.LengthSq() <= 0.000001f) { return Vector3(); }

		// 正規化
		vec.Normalize();

		// 角度を返す
		return Vector3(-asinf(std::clamp(vec.y, -1.0f, 1.0f)), atan2f(vec.x, vec.z), 0.0f);
	}

private:

	// イベントカメラ
	CameraEventBase* cameraEvent;
};