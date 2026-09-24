
#include"CharacterBase.h"
#include "../UI/UI_Base.h"

#include "../../Manager/Net/NetWorkManager.h"

CharacterBase::CharacterBase() :
	ActorBase(),

	characterStats(0, 0, 0, 0),

	state(-1),
	stateMap(),

	DEFAULT_COLOR(),

	anime(nullptr),

	inviCounter(0),
	isInviEffect(false),

	operatorSenderId(MSG_SENDER_ID::None),
	isOwnOperator(false),

	isDeath(false)
{
}

CharacterBase::CharacterBase(
	short HP_MAX,
	short ATTACK_POWER,
	short DEFENSE_POWER,
	short SPEED_POWER
) :
	characterStats(HP_MAX, ATTACK_POWER, DEFENSE_POWER, SPEED_POWER),

	ActorBase(),

	state(0),
	stateMap(),

	DEFAULT_COLOR(),

	anime(nullptr),

	inviCounter(0),
	isInviEffect(false),

	operatorSenderId(MSG_SENDER_ID::None),
	isOwnOperator(false),

	isDeath(false)
{
}

CharacterBase::CharacterBase(const std::string& parameterPath):
	characterStats(0,0,0,0),

	ActorBase(parameterPath),

	state(0),
	stateMap(),

	DEFAULT_COLOR(),

	anime(nullptr),

	inviCounter(0),
	isInviEffect(false),

	operatorSenderId(MSG_SENDER_ID::None),
	isOwnOperator(false),

	isDeath(false)
{
}

CharacterBase::CharacterBase(
	const std::string fileName,
	const std::string hpParameterID,
	const std::string attackPowerParameterID,
	const std::string defensePowerParameterID,
	const std::string moveSpeedParameterID,

	const std::string& parameterPath
) :
	ActorBase(parameterPath),

	state(0),
	stateMap(),

	DEFAULT_COLOR(),

	anime(nullptr),

	inviCounter(0),
	isInviEffect(false),

	characterStats(
		GetParameterToInt((fileName).c_str(), (hpParameterID).c_str()),
		GetParameterToInt((fileName).c_str(), (attackPowerParameterID).c_str()),
		GetParameterToInt((fileName).c_str(), (defensePowerParameterID).c_str()),
		GetParameterToInt((fileName).c_str(), (moveSpeedParameterID).c_str())
		),

	operatorSenderId(MSG_SENDER_ID::None),
	isOwnOperator(false),

	isDeath(false)
{
}

void CharacterBase::Load(void)
{
	CharacterLoad();

	for (UI_Base*& ui : ui_ArrayIns) { ui->Load(); }
}

void CharacterBase::SubInit(void)
{
	// モデルのカラーの初期化
 	SetInviEffectFlg();

	// キャラクター固有の初期化
	CharacterInit();

	for (UI_Base*& ui : ui_ArrayIns) { ui->Init(); }
}

void CharacterBase::SubUpdate(void)
{
	// 無敵カウンターの更新
	Invi();

	if (isOwnOperator) {

		// キャラクター固有の更新
		CharacterUpdate();

		// ステートの更新
		if (stateMap.size() > 0) {
			stateMap.at(state)->OtherStateConditionsUpdate();
			stateMap.at(state)->Update();
			for (std::pair<const int, CharacterStateBase*>& s : stateMap) { s.second->AlwaysUpdate(); }
		}
	}
	else {
		CharacterRemoteUpdate();
	}

	for (UI_Base*& ui : ui_ArrayIns) { ui->Update(); }

	characterStats.ModifinerTimeUpdate();

	// アニメーション更新
	if (anime) { anime->Update(); }
}

void CharacterBase::SubDraw(void)
{
	// キャラクター固有の描画
	CharacterDraw();
}

void CharacterBase::SubAlphaDraw(void)
{
	// キャラクター固有の描画
	CharacterAlphaDraw();
}

void CharacterBase::UiDraw(void)
{
	CharacterUiDraw();

	for (UI_Base*& ui : ui_ArrayIns) { ui->Draw(); }
}

void CharacterBase::SubRelease(void)
{
	// キャラクター固有の解放
	CharacterRelease();

	// ステート管理用マップの解放
	for (auto& s : stateMap) {
		if (s.second) { delete s.second; }
	}
	stateMap.clear();

	// デフォルトカラー情報の解放
	if (!DEFAULT_COLOR.empty()) {
		DEFAULT_COLOR.clear();
	}

	// アニメーションコントローラーの解放（使われていたら）
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}

	// UIの解放
	for (UI_Base*& ui : ui_ArrayIns) {
		if (ui) {
			ui->Release();
			delete ui;
			ui = nullptr;
		}
	}
	ui_ArrayIns.clear();
}

void CharacterBase::SetInviCounter(unsigned char counter)
{
	inviCounter = (counter < 0) ? 0 : (counter > 255) ? 255 : counter;

	// 0 以下の代入であればついでに（バグ防止のために）
	if (counter <= 0) {
		for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
			MV1SetMaterialEmiColor(trans.model, i, DEFAULT_COLOR[i]);
		}
	}
}

void CharacterBase::Invi(void)
{
	if (inviCounter > 0) { inviCounter--; }
	else { inviCounter = 0; }

	// ダメージ演出
	if (!isInviEffect) { return; }

	if (inviCounter > 1) {
		if (inviCounter / 10 % 2 == 0) {
			for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
				MV1SetMaterialEmiColor(trans.model, i, DEFAULT_COLOR[i]);
			}
		}
		else {
			for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
				COLOR_F emi = DEFAULT_COLOR[i];
				emi.r = (std::min)(DEFAULT_COLOR[i].r + 0.6f, 1.0f);
				MV1SetMaterialEmiColor(trans.model, i, emi);
			}
		}

	}
	else if (inviCounter == 1) {
		for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
			MV1SetMaterialEmiColor(trans.model, i, DEFAULT_COLOR[i]);
		}
	}
}

void CharacterBase::AddInFbxAnimation(int inFbxMaxIndex, float speed)
{
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
}

void CharacterBase::AddInFbxAnimation(int inFbxMaxIndex, const float* speed)
{
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed[index], index);
	}
}

void CharacterBase::AddAnimation(int index, float speed, const char* filePath)
{
	anime->Add(index, speed, filePath);
}

void CharacterBase::AnimePlay(int type, bool loop)
{
	anime->Play(type, loop);
}