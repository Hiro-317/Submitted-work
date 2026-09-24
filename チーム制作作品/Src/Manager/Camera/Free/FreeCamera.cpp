#include "FreeCamera.h"

#include "../../../Application/Application.h"

#include "../../Input/KeyManager.h"

FreeCamera::FreeCamera(float MOVE_POWER, float ROT_POWER, AngleTag, const Vector3& pos, const Vector3& angle, float fov) :
	CameraBase(pos, angle, fov),

	MOVE_POWER(MOVE_POWER),
	ROT_POWER(ROT_POWER)
{
	// カメラ回転が有効だったらマウスを固定しておく
	if (ROT_POWER != 0.0f) { Key::GetIns().SetMouseFixed(true); }
}

FreeCamera::FreeCamera(float MOVE_POWER, float ROT_POWER, LookAtTag, const Vector3& pos, const Vector3& lookPos, float fov) :
	CameraBase(pos, CalcCameraAngle(pos, lookPos), fov),

	MOVE_POWER(MOVE_POWER),
	ROT_POWER(ROT_POWER)
{
	// カメラ回転が有効だったらマウスを固定しておく
	if (ROT_POWER != 0.0f) { Key::GetIns().SetMouseFixed(true); }
}

void FreeCamera::NormalUpdate(void)
{
#pragma region 角度 (コントローラースティック -> マウス -> ボタン の順に確認して入力があったもので回転させる)

	// コントローラーの右スティックベクトルを代入
	Vector3 rot = Key::GetIns().GetRightStickVec(false).ToVector3YX();

	// コントローラーの右スティックが入力なしならマウスの移動ベクトルを代入
	if (rot == 0.0f) { rot = Key::GetIns().GetMouseMoveSize().ToVector2().ToVector3YX() / MOUSE_SENSI; }

	// マウスが動いてなかったらボタンでの入力を検出してボタンごとに回転方向を 加算/減算 していく
	if (rot == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_LEFT).now) { rot.y--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_RIGHT).now) { rot.y++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_FRONT).now) { rot.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_BACK).now) { rot.x++; }
		rot.Normalize();
	}

	// 最終的に入力が１つでもあれば回転させる
	if (rot != 0.0f) {

		// 回転させる
		angle += rot * ROT_POWER;

		// 回転の数値制御
		if (angle.y <= Deg2Rad(0.0f)) { angle.y += Deg2Rad(360.0f); }
		if (angle.y >= Deg2Rad(360.0f)) { angle.y -= Deg2Rad(360.0f); }
		if (angle.x <= Deg2Rad(0.0f)) { angle.x += Deg2Rad(360.0f); }
		if (angle.x >= Deg2Rad(360.0f)) { angle.x -= Deg2Rad(360.0f); }
	}
#pragma endregion

#pragma region 移動 (コントローラースティック -> ボタン の順に確認して入力があったもので移動させる)
	// コントローラーの左スティックベクトルを代入
	Vector3 dir = Key::GetIns().GetLeftStickVec(false).ToVector3XZ();

	// コントローラーの左スティックの入力が検出されなかった場合、ボタンでの入力を検出してボタンごとに移動方向を 加算/減算 していく
	if (dir == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_MOVE_FRONT).now) { dir.z++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_MOVE_BACK).now) { dir.z--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_MOVE_RIGHT).now) { dir.x++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_MOVE_LEFT).now) { dir.x--; }
		dir.Normalize();
	}

	// Y軸はボタンのため共通して検出する
	if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_MOVE_UP).now) { dir.y++; }
	if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_MOVE_DOWN).now) { dir.y--; }

	// 入力があれば、方向×スピードで移動量を作って、座標に足して移動
	if (dir != 0.0f) { pos += dir.TransMat(MGetRotY(angle.y)) * MOVE_POWER; }
#pragma endregion
}

void FreeCamera::SubRelease(void)
{
	// 生成時固定したマウスを戻す
	Key::GetIns().SetMouseFixed(false);
}