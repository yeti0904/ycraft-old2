#pragma once

// C standard libraries
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <dirent.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <windows.h>
#elif __linux__ || __APPLE__ || __unix__
	#include <unistd.h>
	#include <sys/stat.h>
	#if defined(__APPLE__)
		#include <mach-o/dyld.h>
	#endif
#endif

// C++ standard libraries
#include <array>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <unordered_map>
using std::array;
using std::string;
using std::vector;
using std::thread;
using std::ifstream;
using std::ofstream;
using std::to_string;
using std::unordered_map;

// C libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>