#pragma once

#include <list>

#include "EffectFactory.h"

#include "../../Object/Common/DataLoad/ParameterLoad.h"
#include "../Net/NetWorkManager.h"


class EffectManager {
public:

	// シングルトン（生成・取得・削除）
	static void CreateIns(void) { if (ins == nullptr) { ins = new EffectManager(); } };
	static EffectManager* GetIns(void) { return ins; };
	static void DeleteIns(void) { if (ins != nullptr) { delete ins; ins = nullptr; } }

	void UpdateEffect(void);
	void ReceptionUpdate(void);

	void DrawEffect(void);

	void CreateEffect(EFFECT_NAME name, const Vector3& local, const Transform* follow = nullptr,
		bool followRotX = true, bool followRotY = true, bool followRotZ = true);
	void StopEffect(EFFECT_NAME name);
	void StopEffectAll(void);

	// 一時停止
	void PauseEffect(void);

	// 再生(一時停止後)
	void PlayEffect(void);

private:

	EffectManager(void);
	~EffectManager();

	// コピー・ムーブ操作を禁止
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator=(EffectManager&&) = delete;

	static EffectManager* ins;

	// パラメーター外部ファイル管理クラス
	ParameterLoad* parameter;

	std::list<EffectBase*> effectInfo;
};