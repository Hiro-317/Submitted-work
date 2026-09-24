#pragma once

#include "../../../../../../ActorBase.h"
#include "BananaBossBanadrill.h"

#include "../../../../../../Common/Collider/XZCircleCollider.h"


class BananaBossBanadrillManager : public ActorBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stats">ステータス</param>
	BananaBossBanadrillManager(
		const MSG_SENDER_ID operatorSenderId,
		const CharacterStats& stats
	);
	~BananaBossBanadrillManager()override = default;

	void Load(void)override;

	BananaBossBanadrill* GetBanadrill(int num) { return banadrill[num]; }

private:

	// バナドリルの数
	static const int NUM = 3;

#pragma endregion

	void SubUpdate(void) override;
	void SubAlphaDraw(void) override;

	BananaBossBanadrill* banadrill[NUM];

};