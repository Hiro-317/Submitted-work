#pragma once

#include <vector>

#include "../Object/ActorBase.h"

// 配列の中から特定のオブジェクトを探す（単体）
template<typename T = ActorBase>
T* ObjSerch(std::vector<ActorBase*>& objects) {
	for (auto* obj : objects) {
		if (T* cast = dynamic_cast<T*>(obj)) { return cast; }
	}
	return nullptr;
}

// 配列の中から特定のオブジェクトを探す（複数）
template<typename T = ActorBase>
std::vector<T*> ObjArraySerch(std::vector<ActorBase*>& objects) {
	std::vector<T*> objArray;
	for (auto* obj : objects) {
		if (T* cast = dynamic_cast<T*>(obj)) { objArray.emplace_back(cast); }
	}
	return objArray;
}