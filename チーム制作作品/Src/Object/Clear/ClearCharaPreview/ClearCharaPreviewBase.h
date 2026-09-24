#pragma once

#include "../../ActorBase.h"

class AnimationController;

class ClearCharaPreviewBase : public ActorBase
{
public:
	ClearCharaPreviewBase(const Vector3& pos, unsigned char operatorNumber);
	virtual ~ClearCharaPreviewBase()override = default;

	// “Ç‚İ‚İˆ—Ä’è‹`
	void Load(void)override;
	// XVˆ—Ä’è‹`
	void Update(void)override;

private:

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[
	AnimationController* anime;
	void AnimationControllerCreate(void);

	// å—vŠÖ”Ä’è‹``````````
	void SubInit(void)override;
	void SubRelease(void)override;
	// ````````````````

	// Ä¶‚·‚éƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv
	int playAnimeType;

protected:
	virtual void CharacterLoad(void) = 0;
	virtual void CharacterInit(void) {}

	// ƒ‚ƒfƒ‹‚É‚­‚Á‚Â‚¢‚Ä‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	void AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType);
	// •Êƒtƒ@ƒCƒ‹‚É•Û‘¶‚³‚ê‚Ä‚¢‚éFBXƒAƒjƒ[ƒVƒ‡ƒ“‚ğ“o˜^‚·‚é
	void AddAnimation(const char* filePath, float speed, int index = 0);
};