#include "util.hh"
#include "fs.hh"
#include "constants.hh"

string Util::FormatString(const char* format, ...) {
	va_list args;
	va_start(args, format);
	
	char* ret;
	vasprintf(&ret, format, args);
	
	va_end(args);
	
	return string(ret);
}

string Util::CurrentTime() {
	time_t rawtime;
	struct tm* timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	char buffer[80];
	strftime(buffer, 80, "%H:%M:%S", timeinfo);
	
	return string(buffer);
}

void Util::Log(const char* format, ...) {
	va_list args;
	va_start(args, format);
	
	char* ret;
	vasprintf(&ret, format, args);
	
	va_end(args);
	
	printf("[%s] %s\n", CurrentTime().c_str(), ret);
	free(ret);
}

string Util::GetExecutableLocation() {
	char buffer[1024];
	#ifdef __linux__
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer));
		if (len != -1) {
			buffer[len] = '\0';
			return string(buffer);
		}
	#elif defined(__APPLE__)
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0) {
			return string(buffer);
		}
	#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		GetModuleFileName(NULL, buffer, sizeof(buffer));
		return string(buffer);
	#endif
	
	return "";
}

bool Util::YesNoBox(string title, string message) {
	const SDL_MessageBoxButtonData buttons[] = {
		{0, 0, "Yes"},
		{0, 1, "No"}
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		title.c_str(),
		message.c_str(),
		SDL_arraysize(buttons),
		buttons,
		NULL
	};
	int clickedButton;
	if (SDL_ShowMessageBox(&messageboxdata, &clickedButton) < 0) {
		Util::Log("Error showing message box");
		exit(1);
	}
	if (clickedButton == -1) {
		return false;
	}
	else return buttons[clickedButton].buttonid == 0;
}

bool Util::CheckFolders(string gamePath) {
	return (
		FS::Directory::Exists(gamePath + "/texpacks") &&
		FS::Directory::Exists(gamePath + "/fonts") &&
		FS::Directory::Exists(gamePath + "/maps") &&
		FS::Directory::Exists(gamePath + "/skins") &&
		FS::File::Exists(gamePath + "/options.properties")
	);
}

void Util::Install(string gamePath) {
	FS::Directory::Create(gamePath + "/texpacks");
	FS::Directory::Create(gamePath + "/fonts");
	FS::Directory::Create(gamePath + "/maps");
	FS::Directory::Create(gamePath + "/skins");
	FS::File::Create(gamePath + "/options.properties");
}

UVec2 Util::GetTexturePosition(uint16_t id, SDL_Texture* texture) {
	SDL_Point texturesize;
	SDL_QueryTexture(texture, NULL, NULL, &texturesize.x, &texturesize.y);

	return {
		((unsigned int)id % texturesize.y) * BLOCK_SIZE,
		((unsigned int)id / texturesize.y) * BLOCK_SIZE
	};
}