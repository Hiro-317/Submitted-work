#pragma once

#include "../../pch.h"

#include "../../Utility/Utility.h"

#include "../Net/NetWorkManager.h"
#include "../../Object/Common/Transform.h"
#include "../../Object/Common/DataLoad/ParameterLoad.h"


// エフェクトの種類
enum class EFFECT_NAME {

	NON = -1,

	TACKLE_MOVE,
	STAMP_LAND,
	HEAL,
	BUFF,
	TACKLE_CHARGE,
	STAMP_SHOCK_WAVE,
	BOMB_SMALL,
	BOMB_BIG,
	FIRE,

	MAX
};

// エフェクト再生に必要な情報構造体
struct EFFECT_INFO
{
	EFFECT_NAME name = EFFECT_NAME::NON;
	Transform trans = Transform();
	const Transform* follow = nullptr;
	bool flg = true;
	bool followRotX;
	bool followRotY;
	bool followRotZ;
};

class EffectBase {
public:

	EffectBase(const ParameterLoad& parameter) : parameter(parameter){}

	virtual ~EffectBase() = default;

	// 更新
	virtual void UpdateEffect(void)
	{
		// 描画開始
		if (playHandle == -1) {
			playHandle = PlayEffekseer3DEffect(info.trans.model);
			// 追従しないなら
			if (info.follow == nullptr) {
				// 初期値を代入
				SetPosPlayingEffekseer3DEffect(playHandle, info.trans.pos.x, info.trans.pos.y, info.trans.pos.z);
				SetRotationPlayingEffekseer3DEffect(playHandle, info.trans.angle.x, info.trans.angle.y, info.trans.angle.z);
			}
			// サイズの更新
			SetScalePlayingEffekseer3DEffect(playHandle, info.trans.scale.x, info.trans.scale.y, info.trans.scale.z);
		}
		// 描画が終わっているなら消す
		if (IsEffekseer3DEffectPlaying(playHandle) == -1) {
			playHandle = -1;
			end = true;
			return;
		}
		// 追従するなら
		if (info.follow != nullptr) {
			// 座標の更新
			Vector3 pos = info.follow->pos + info.trans.pos.TransMat(MatrixAllMultZXY({ info.follow->angle }));
			SetPosPlayingEffekseer3DEffect(playHandle, pos.x, pos.y, pos.z);

			// 角度の更新
			if (info.followRotX || info.followRotY || info.followRotZ) {

				// 角度も追従するなら更新
				Vector3 angle = info.follow->angle + info.trans.angle;
				// 追従しない分もあるので保留
				Vector3 temp = info.trans.angle;

				// 追従するものだけ書き換え
				if (info.followRotX) {
					temp.x = angle.x;
				}
				if (info.followRotY) {
					temp.y = angle.y;
				}
				if (info.followRotZ) {
					temp.z = angle.z;
				}
				// 更新
				SetRotationPlayingEffekseer3DEffect(playHandle, temp.x, temp.y, temp.z);
				Net::GetIns().Send(MsgDataEffectFollow((int)info.name, pos, temp));
			}
			else {
				// 角度の追従しないなら初期値を入れる
				SetRotationPlayingEffekseer3DEffect(playHandle, info.trans.angle.x, info.trans.angle.y, info.trans.angle.z);
				Net::GetIns().Send(MsgDataEffectFollow((int)info.name, pos, info.trans.angle));
			}
		}
	}

	virtual void DrawEffect(void) {
		if (info.flg) {
			DrawEffekseer3D_Draw(playHandle);
		}
	}

	virtual void Release(void) 
	{
		// 消す
		DeleteEffekseerEffect(info.trans.model);
	}

	virtual void StopEffect(void)
	{
		// エフェクトを止め消す
		StopEffekseer3DEffect(playHandle);
		playHandle = -1;
		Release();
	}

	void SetFollowReception(Vector3 pos, Vector3 angle) {

		SetPosPlayingEffekseer3DEffect(playHandle, pos.x, pos.y, pos.z);
		SetRotationPlayingEffekseer3DEffect(playHandle, angle.x, angle.y, angle.z);
	}

	// 一時停止
	void PauseEffect(void) {
		info.flg = false;
	}
	// 再生(一時停止後)
	void PlayEffect(void) {
		info.flg = true;
	}

	bool IsEnd(void) { return end; }

	EFFECT_NAME GetName(void) { return info.name; }

protected:

#pragma region パラメーター外部ファイル管理に関する関数
	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけを取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	float GetParameter(const std::string& fileName, const std::string& parameterName, int index = 0)const {
		return parameter.GetParameter(fileName, parameterName, index);
	}

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけをint型にキャストして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	int GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index = 0)const {
		return parameter.GetParameterToInt(fileName, parameterName, index);
	}

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターをVector3構造体にして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	Vector3 GetParameterToVector3(const std::string& fileName, const std::string& parameterName) {
		return parameter.GetParameterToVector3(fileName, parameterName);
	}

	EFFECT_INFO info;

	int playHandle = -1;

	bool end = false;

private:

	const ParameterLoad& parameter;
};