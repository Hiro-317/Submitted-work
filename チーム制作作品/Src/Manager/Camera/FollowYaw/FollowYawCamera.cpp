#include "FollowYawCamera.h"

#include "../../../Application/Application.h"

#include "../../Input/KeyManager.h"

FollowYawCamera::FollowYawCamera(
	const Vector3* targetPos,
	const Vector3& cameraOffset,
	const Vector3& lookAtOffset,
	float ROT_POWER,
	const Vector3& angle,
	float fov
) :
	CameraBase(Vector3(), Vector3(), fov),

	targetPos(targetPos),

	cameraOffset(cameraOffset),

	lookAtOffset(lookAtOffset),
	lookAtPos(),

	ROT_POWER(ROT_POWER),

	controlAngle(angle)
{
	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	pos = *targetPos + cameraOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x,controlAngle.y) }));
	lookAtPos = *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x, controlAngle.y) }));

	// 角度
	this->angle = CalcCameraAngle(pos, lookAtPos);

	// カメラ回転が有効だったらマウスを固定しておく
	if (ROT_POWER != 0.0f) { Key::GetIns().SetMouseFixed(true); }
}

void FollowYawCamera::NormalUpdate(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (targetPos == nullptr) { return; }

	// 回転処理(コントローラー -> マウス-> ボタン の順に入力を確認していく)

	// コントローラーの右スティックベクトルを代入
	Vector3 rot = Vector3::Yonly(Key::GetIns().GetRightStickVec(false).ToVector3YX().y);

	// コントローラーの右スティックが入力なしならマウスの移動ベクトルを代入
	if (rot == 0.0f) { rot = Vector3::Yonly((float)Key::GetIns().GetMouseMoveSize().x / MOUSE_SENSI); }

	// マウスが動いてなかったらボタンでの入力を検出してボタンごとに回転方向を 加算/減算 していく
	if (rot == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_RIGHT).now) { rot.y++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::CAMERA_ROT_LEFT).now) { rot.y--; }
		rot.Normalize();
	}

	// 最終的に入力が1つでもあれば回転させる
	if (rot != 0.0f) {
		controlAngle += rot * ROT_POWER;

		// 回転の数値制御
		if (controlAngle.y <= Deg2Rad(0.0f)) { controlAngle.y += Deg2Rad(360.0f); }
		if (controlAngle.y >= Deg2Rad(360.0f)) { controlAngle.y -= Deg2Rad(360.0f); }
	}

	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	pos = *targetPos + cameraOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x,controlAngle.y) }));
	lookAtPos = *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x, controlAngle.y) }));

	// 角度
	angle = CalcCameraAngle(pos, lookAtPos);
}

void FollowYawCamera::SubRelease(void)
{
	// 生成時固定したマウスを戻す
	Key::GetIns().SetMouseFixed(false);
}