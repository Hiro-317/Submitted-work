#pragma once

#include "../../../../Manager/Net/NetWorkDefine.h"

#include "../../../ActorBase.h"

class AnimationController;
class CameraBase;

class CharaSelectPreviewBase : public ActorBase
{
public:
	CharaSelectPreviewBase();
	CharaSelectPreviewBase(const std::string& parameterPath);
	virtual ~CharaSelectPreviewBase()override = default;

	// å—vŠÖ”Ä’è‹``````````
	void Load(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	// ````````````````

private:

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[
	AnimationController* anime;
	void AnimationControllerCreate(void);

	// å—vŠÖ”Ä’è‹``````````
	virtual void SubLoad(void) = 0;
	void SubInit(void)override;
	void SubRelease(void)override;
	// ````````````````

	// Ä¶‚·‚éƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv
	int playAnimeType;

	int modelPreviewScreen;

	CameraBase* camera;

protected:

	std::string CHARA_NAME;
	// ƒpƒ‰ƒ[ƒ^[
	short HP;
	short ATTACK;
	short DEFENCE;
	short SPEED;

	int skill_1_image;
	int skill_2_image;
	int skill_3_image;

	// ƒ‚ƒfƒ‹‚É‚­‚Á‚Â‚¢‚Ä‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	void AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType);
	// •Êƒtƒ@ƒCƒ‹‚É•Û‘¶‚³‚ê‚Ä‚¢‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	void AddAnimation(const char* filePath, float speed, int index = 0);
};