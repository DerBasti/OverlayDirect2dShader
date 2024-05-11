#ifndef __REGISTER_SHADER__
#define __REGISTER_SHADER__

#pragma once

#include <vector>
#include "CustomLeagueEffect.h"
#include <functional>

class RegisterShaderFactory {
private:
	RegisterShaderFactory() {}
public:
	static std::vector<std::pair<GUID, std::function<HRESULT(ID2D1Factory1*)>>> GetShadersForRegistration();
};

__REGISTER_SHADER__

#endif //__REGISTER_SHADER__