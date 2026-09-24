#include "../../pch.h"

#include "EffectManager.h"


EffectManager*  EffectManager::ins = nullptr;

EffectManager::EffectManager(void)
{
	parameter = new ParameterLoad("Data/Parameter/Effect/");
}

EffectManager::~EffectManager()
{
	StopEffectAll();
	parameter->Release();
	delete parameter;
}

void EffectManager::UpdateEffect(void) {

	for (auto info = effectInfo.begin(); info != effectInfo.end();) {
		(*info)->UpdateEffect();
		if ((*info)->IsEnd()) { (*info)->Release(); delete (*info); info = effectInfo.erase(info); }
		else { info++; }
	}
}

void EffectManager::ReceptionUpdate(void) {

	while (MsgDataEffectCreate* dataPtr = Net::GetIns().GetMsgData<MsgDataEffectCreate>(MSG_SENDER_ID::None, true)) {
		if (dataPtr->flg) {
			effectInfo.emplace_back(EffectFactory::CreateEffect(*parameter, (EFFECT_NAME)dataPtr->name, dataPtr->local, nullptr, false, false, false));
		}
		else {
			if (dataPtr->name == -1) {
				for (auto effect : effectInfo) {
					effect->StopEffect();
					delete effect;
					effect = nullptr;
				}
				effectInfo.clear();
			}
			else {
				for (auto info = effectInfo.begin(); info != effectInfo.end();) {
					if ((*info)->GetName() == (EFFECT_NAME)dataPtr->name) { (*info)->StopEffect(); delete (*info); (*info) = nullptr; info = effectInfo.erase(info); }
					else { info++; }
				}
			}
		}
		delete dataPtr;
	}

	while (MsgDataEffectFollow* dataPtr = Net::GetIns().GetMsgData<MsgDataEffectFollow>(MSG_SENDER_ID::None, true)) {

		for (auto info : effectInfo) {
			if (info->GetName() == (EFFECT_NAME)dataPtr->name) { info->SetFollowReception(dataPtr->pos, dataPtr->angle); }
		}
		delete dataPtr;
	}
}

void EffectManager::DrawEffect(void)
{
	for (auto& effect : effectInfo) {
		effect->DrawEffect();
	}
}

void EffectManager::PauseEffect(void)
{
	for (auto& effect : effectInfo) {
		effect->PauseEffect();
	}
}

void EffectManager::PlayEffect(void)
{
	for (auto& effect : effectInfo) {
		effect->PlayEffect();
	}
}

void EffectManager::CreateEffect(EFFECT_NAME name, const Vector3& local, const Transform* follow,
	bool followRotX, bool followRotY, bool followRotZ) {

	effectInfo.emplace_back(EffectFactory::CreateEffect(*parameter, name, local, follow, followRotX, followRotY, followRotZ));
	Net::GetIns().Send(MsgDataEffectCreate(true, (int)name, local));
}

void EffectManager::StopEffect(EFFECT_NAME name) {

	for (auto info = effectInfo.begin(); info != effectInfo.end();) {
		if ((*info)->GetName() == name) { (*info)->StopEffect(); delete (*info); (*info) = nullptr; info = effectInfo.erase(info); }
		else { info++; }
	}
	Net::GetIns().Send(MsgDataEffectCreate(false, (int)name));
}

void EffectManager::StopEffectAll(void)
{
	for (auto& effect : effectInfo) {
		effect->StopEffect();
		delete effect;
		effect = nullptr;
	}
	effectInfo.clear();
	Net::GetIns().Send(MsgDataEffectCreate(false));
}
