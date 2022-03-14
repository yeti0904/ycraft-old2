#pragma once
#include "_components.hh"
#include "vec2.hh"

namespace Util {
	string FormatString(const char* format, ...);
	string CurrentTime();
	void   Log(const char* format, ...);
	string GetExecutableLocation();
	bool   YesNoBox(string title, string message);
	bool   CheckFolders(string gamePath);
	void   Install(string gamePath);
	UVec2  GetTexturePosition(uint16_t id, SDL_Texture* texture);
}