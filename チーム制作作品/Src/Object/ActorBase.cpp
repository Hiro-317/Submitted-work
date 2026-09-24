#include "ActorBase.h"

#include "../Application/Application.h"

#include "Common/Collider/ColliderBase.h"

ActorBase::ActorBase() :
	trans(),
	collider(),

	dynamicFlg(true),
	isGravity(false),

	pushFlg(true),
	pushWeight(0),

	accelSum(0.0f),

	isGroundMaster(false),

	isDraw(true),
	isAlphaDraw(false),

	parameter(nullptr),

	skillStats()
{
}

ActorBase::ActorBase(const std::string& parameterPath) :
	trans(),
	collider(),

	dynamicFlg(true),
	isGravity(false),

	pushFlg(true),
	pushWeight(0),

	accelSum(0.0f, 0.0f, 0.0f),

	isGroundMaster(false),

	isDraw(true),
	isAlphaDraw(false),

	parameter(new ParameterLoad(parameterPath)),

	skillStats()
{
}

void ActorBase::Init(void)
{
	SubInit();

	trans.Attach();
	
	// パラメータの解放
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}
}

void ActorBase::Update(void)
{
	// 動的オブジェクトは１フレーム前の座標を保持
	if (dynamicFlg) { trans.prevPos = trans.pos; }

	// 派生先追加更新
	SubUpdate();

	// 重力処理
	if (dynamicFlg && isGravity) { Gravity(); }

	// 加速度更新
	if (dynamicFlg) { AccelUpdate(); }

	// 接地判定のリセット
	if (dynamicFlg) { isGroundMaster = false; }
}

void ActorBase::Draw(void)
{
	// 派生先追加描画
	SubDraw();

	// 描画判定
	if (!isDraw) { return; }

	// モデルの描画
	if (!isAlphaDraw) { trans.Draw(); }
}

void ActorBase::AlphaDraw(void)
{
	// 派生先追加アルファ描画
	SubAlphaDraw();

	// 描画判定
	if (!isDraw) { return; }

	// モデルの描画（アルファ描画）
	if (isAlphaDraw) { trans.Draw(); }

	// 当たり判定のデバッグ描画
	if (App::GetIns().IsDrawDebug()) {
		for (ColliderBase*& c : collider) { if (c->GetJudge()) c->DrawDebug(); }
	}	
}

void ActorBase::Release(void)
{
	// 派生先追加解放
	SubRelease();

	// パラメータの解放
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}

	// スキル情報の解放
	for (SkillStats*& s : skillStats) {
		if (!s) { continue; }
		delete s;
		s = nullptr;
	}
	skillStats.clear();

	// 当たり判定情報を解放
	for (ColliderBase*& c : collider) {
		if (!c) { continue; }
		delete c;
		c = nullptr;
	}
	collider.clear();

	// モデル制御情報の解放
	trans.Release();
}

bool ActorBase::GetJudgeFlg(void)
{
	for (ColliderBase*& c : collider) {
		if (!c) { continue; }
		if (c->GetJudge()) { return true; }
	}
	return false;
}

void ActorBase::AccelUpdate(void)
{
	// 横軸(横軸は減衰もする)～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// 横軸のみの加速度の長さを求める
	float widthAccelLen = sqrt(accelSum.x * accelSum.x + accelSum.z * accelSum.z);

	// 横軸の加速度がある場合、減衰処理を行う
	if (widthAccelLen > 0.0f) {

		// 横軸の加速度が減衰の半分以下の場合は、横軸の加速度を0にする
		if (widthAccelLen <= ATTENUATION * 0.5f) { accelSum.x = accelSum.z = 0.0f; }

		float scale = (widthAccelLen - ATTENUATION) / widthAccelLen;
		if (widthAccelLen > ACCEL_MAX) { scale = ACCEL_MAX / widthAccelLen; }

		accelSum.x *= scale;
		accelSum.z *= scale;
	}

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～


	// 加速度を座標に反映
	if (accelSum != 0.0f) { trans.pos += accelSum; }
}

void ActorBase::Gravity(void)
{
	accelSum.y += GRAVITY;
	if (accelSum.y < GRAVITY_MAX) { accelSum.y = GRAVITY_MAX; }
}

void ActorBase::ColliderCreate(ColliderBase* newClass)
{
	collider.emplace_back(newClass);
	collider.back()->SetTransformPtr(&trans);
	collider.back()->SetDynamicFlg((dynamicFlg) ? true : false);
	collider.back()->SetPushFlg(pushFlg);
	collider.back()->SetPushWeight(pushWeight);
	collider.back()->SetOnCollisionFunc([this](COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint) { this->OnCollision(ownTag, other, collisionPoint); });
	collider.back()->SetOnGroundedFunc([this](void) { this->OnGrounded(); });
	ColliderToSetSkill();
}

void ActorBase::SetDynamicFlg(bool flg)
{
	dynamicFlg = flg;
	trans.dynamicFlg = flg;
	for (ColliderBase*& collider : collider) {
		collider->SetDynamicFlg(flg);
	}
}

void ActorBase::SetPushFlg(bool flg)
{
	pushFlg = flg;
	for (ColliderBase*& coll : collider) { coll->SetPushFlg(flg); }
}

void ActorBase::SetPushWeight(unsigned char weight)
{
	pushWeight = weight;
	for (ColliderBase*& coll : collider) { coll->SetPushWeight(weight); }
}
#pragma region パラメーター外部ファイル管理に関する関数

bool ActorBase::IsParameterExist(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->IsParameterExist(fileName, parameterName);
}

float ActorBase::GetParameter(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameter(fileName, parameterName, index);
}

const std::vector<float>& ActorBase::GetParameterArray(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterArray(fileName, parameterName);
}

int ActorBase::GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterToInt(fileName, parameterName, index);
}

Vector3 ActorBase::GetParameterToVector3(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterToVector3(fileName, parameterName);
}

#pragma endregion


#pragma region スキル設定

void ActorBase::ColliderToSetSkill(void)
{
	for (const SkillStats* skill : skillStats) {
		for (ColliderBase* coll : collider) {
			if (skill->COLL_TAG == coll->GetTag() || skill->COLL_TAG == COLLIDER_TAG::NON) { coll->SetSkillStats(skill); }
		}
	}
}

void ActorBase::CreateAttackSkill(MSG_SENDER_ID operatorSenderId, short SKILL_POWER, const CharacterStats* characterStats, COLLIDER_TAG tag)
{
	skillStats.emplace_back(new SkillStats(operatorSenderId, SKILL_POWER, characterStats, tag));
	ColliderToSetSkill();
}

void ActorBase::CreateHealSkill(MSG_SENDER_ID operatorSenderId, short SKILL_POWER, COLLIDER_TAG tag)
{
	skillStats.emplace_back(new SkillStats(operatorSenderId, SKILL_POWER, nullptr, tag));
	ColliderToSetSkill();
}

void ActorBase::CreateModifierSkill(MSG_SENDER_ID operatorSenderId, ModifierType modifierType, short SKILL_POWER, short SKILL_TIME, COLLIDER_TAG tag)
{
	skillStats.emplace_back(new SkillStats(operatorSenderId, modifierType, SKILL_POWER, SKILL_TIME, tag));
	ColliderToSetSkill();
}

#pragma endregion

void ActorBase::SetJudge(bool flg)
{
	for (ColliderBase*& c : collider) {
		if (!c) { continue; }
		c->SetJudgeFlg(flg);
	}
}