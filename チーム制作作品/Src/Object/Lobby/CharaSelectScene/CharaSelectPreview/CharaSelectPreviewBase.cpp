#include "CharaSelectPreviewBase.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../../Manager/Font/FontManager.h"

#include "../../../../Manager/Camera/DisplayAuto/DisplayAutoCamera.h"

#include "../../../Common/AnimationController/AnimationController.h""

CharaSelectPreviewBase::CharaSelectPreviewBase() :
	ActorBase(),

	anime(nullptr),
	playAnimeType(0),

	modelPreviewScreen(-1),

	camera(nullptr),

	CHARA_NAME(),

	HP(),
	ATTACK(),
	DEFENCE(),
	SPEED(),

	skill_1_image(-1),
	skill_2_image(-1),
	skill_3_image(-1)
{
}
CharaSelectPreviewBase::CharaSelectPreviewBase(const std::string& parameterPath) :
	ActorBase(parameterPath),

	anime(nullptr),
	playAnimeType(0),

	modelPreviewScreen(-1),

	camera(nullptr),

	CHARA_NAME(),

	HP(),
	ATTACK(),
	DEFENCE(),
	SPEED(),

	skill_1_image(-1),
	skill_2_image(-1),
	skill_3_image(-1)
{
}

void CharaSelectPreviewBase::Load(void)
{
	modelPreviewScreen = MakeScreen(440, 540, true);

	SubLoad();

	camera = new DisplayAutoCamera(trans.pos, Vector3(0, 100, -400), 0.0f);
}

void CharaSelectPreviewBase::AnimationControllerCreate(void)
{
	// ¶¬‚µ‚Ä‚¢‚È‚¯‚ê‚ÎƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ð¶¬
	if (anime == nullptr) { anime = new AnimationController(trans.model); }
}

void CharaSelectPreviewBase::SubInit(void)
{
#pragma region Šî’êƒNƒ‰ƒX‚É‚ ‚é‹@”\‚Ì‹““®Ý’è
	// “®“IƒIƒuƒWƒFƒNƒg‚Æ‚µ‚Ä‚Ì‹““®‚ð–³Œø‚É‚·‚é
	SetDynamicFlg(false);
	// Õ“ËŽž‚Ì‰Ÿ‚µo‚µ‚ð–³Œø‚É‚·‚é
	SetPushFlg(false);
#pragma endregion
	if (camera) { camera->Init(); }
	// ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	if (anime) { anime->Play(playAnimeType); }
}

void CharaSelectPreviewBase::Update(void)
{
	if (camera) { camera->Update(); }
	// ƒAƒjƒ[ƒVƒ‡ƒ“XV
	if (anime) { anime->Update(); }
}

void CharaSelectPreviewBase::Draw(void)
{
	// ‚à‚Æ‚Ì•`‰ææ‚ð•Û‘¶
	int oldDrawScreen = GetDrawScreen();

	// ƒ‚ƒfƒ‹•`‰æ‚ð1–‡ŠG‚É‚·‚é````````
	SetDrawScreen(modelPreviewScreen);
	ClearDrawScreen();
	camera->Apply();
	trans.Draw();
	// ````````ƒ‚ƒfƒ‹•`‰æ‚ð1–‡ŠG‚É‚·‚é

	// •`‰ææ‚ð–ß‚·
	SetDrawScreen(oldDrawScreen);

	// ƒLƒƒƒ‰‚Ì–¼‘O
	DrawStringToHandle(340, 120, CHARA_NAME.c_str(), 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32));

	// ŠeŽíƒpƒ‰ƒ[ƒ^[````````````````````````````````````````````````````````
	DrawFormatStringToHandle(850, 185, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", HP);
	DrawFormatStringToHandle(1160, 185, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", ATTACK);
	DrawFormatStringToHandle(850, 300, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", DEFENCE);
	DrawFormatStringToHandle(1160, 300, 0xffffff, Font::GetIns().GetFont(Font::FONT_KINDS_SIZE::GOKUSYOU_32), "%d", SPEED);
	// ````````````````````````````````````````````````````````ŠeŽíƒpƒ‰ƒ[ƒ^[

	// 1–‡ŠG‚É‚µ‚½ƒ‚ƒfƒ‹•`‰æ
	DrawRotaGraph(400, 455, 1, 0, modelPreviewScreen, true);

	// ŠeŽíƒXƒLƒ‹```````````````
	DrawGraph(660, 465, skill_1_image, true);
	DrawGraph(660, 545, skill_2_image, true);
	DrawGraph(660, 630, skill_3_image, true);
	// ```````````````ŠeŽíƒXƒLƒ‹
}

void CharaSelectPreviewBase::SubRelease(void)
{
	if (camera)	{
		camera->Release();
		delete camera;
		camera = nullptr;
	}

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚ðíœ
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}

	DeleteGraph(skill_1_image);
	DeleteGraph(skill_2_image);
	DeleteGraph(skill_3_image);
	DeleteGraph(modelPreviewScreen);
}

void CharaSelectPreviewBase::AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType)
{
	AnimationControllerCreate();
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
	this->playAnimeType = playAnimeType;
}

void CharaSelectPreviewBase::AddAnimation(const char* filePath, float speed, int index)
{
	AnimationControllerCreate();
	anime->Add(index, speed, filePath);
	playAnimeType = index;
}