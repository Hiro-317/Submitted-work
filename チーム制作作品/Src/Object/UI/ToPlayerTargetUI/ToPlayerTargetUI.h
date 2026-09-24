#pragma once

#include "../UI_Base.h"
#include "../../../Common/Vector2.h"

class ToPlayerTargetUI : public UI_Base
{
public:
	ToPlayerTargetUI(const Vector2I& drawPos);
	~ToPlayerTargetUI()override = default;

	void Load(void) override;
private:
	void SubDraw(void) override;		// •`‰æ

	const Vector2I& drawPos;
};