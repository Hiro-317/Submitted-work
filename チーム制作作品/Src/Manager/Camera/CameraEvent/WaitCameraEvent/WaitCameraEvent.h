#pragma once

#include "../CameraEventBase.h"

#include <algorithm>

class WaitCameraEvent : public CameraEventBase
{
public:
	WaitCameraEvent(int frame):
		CameraEventBase(),

		currentFrame(0),
		maxFrame(std::max(frame, 1))
	{
	}

	~WaitCameraEvent()override = default;

	void Update(CameraBase& camera) override { ++currentFrame; }

    bool IsEnd() const override { return currentFrame >= maxFrame; }

private:

	const int maxFrame;
	int currentFrame;
};