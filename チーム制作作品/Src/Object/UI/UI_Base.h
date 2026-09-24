#pragma once

#include "../../pch.h"

#include <map>
#include <string>

#include "UIDefine.h"

class UI_Base
{
public:

	UI_Base();
	virtual ~UI_Base() = default;

	virtual void Load(void) = 0;
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

protected:

	// UIの画像
	std::vector<int> uiImages;	

	/// <summary>
	/// 指定された画像を指定された配列の変数にロードする
	/// </summary>
	/// <param name="path">画像のパス(".png"は除く)</param>
	/// <param name="kinds">画像をロードする配列を指定</param>
	/// <param name="pathType">HP or スタミナ or スキル</param>
	void UILoadImage(const std::string& path, int kinds, FILE_PATH_TYPE pathType)
	{
		int pathIndex = (int)pathType;
		if (pathIndex < 0 || pathIndex >= (int)FILE_PATH_TYPE::MAX) { return; }

		// パス作成
		std::string fullPath = uiKindsPath[pathIndex] + path + ".png";

		// 必要サイズ確保
		if (kinds >= (int)uiImages.size()) {
			// 指定された画像をロード
			uiImages.resize(kinds + 1, LoadGraph(fullPath.c_str()));
		}
	}

	// 継承先で定義するクラスたち～～～
	virtual void SubInit(void) {}
	virtual void SubUpdate(void) {}
	virtual void SubDraw(void) {}
	virtual void SubRelease(void) {}
	// ～～～～～～～～～～～～～～～～
};
