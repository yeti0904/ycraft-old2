#include "_components.hh"
#include "game.hh"
#include "constants.hh"
#include "util.hh"

int main(void) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}
	
	Game game;
	bool finishedGen = false;

	Util::Log("Found game at %s", game.gamePath.c_str());
	Util::Log("Game at directory \"%s\"", game.gameParentFolder.c_str());

	if (!Util::CheckFolders(game.gameParentFolder)) {
		if (Util::YesNoBox("ycraft", "Do you want to install/update ycraft at " + game.gameParentFolder + "?")) {
			Util::Install(game.gameParentFolder);
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ycraft", "ycraft has been installed", NULL);
		}
		else {
			Util::Log("Installation prompt denied, Exit");
			exit(0);
		}
	}

	game.Init(APP_NAME);
	Util::Log("Initialized %s", APP_NAME);

	game.LoadTexturePack("default");
	game.LoadDefaultBlockdefs();
	game.LoadPlayerSkin("default");

	game.level.Generate(24, 24);
	Util::Log("Started level gen (width: %d, height: %d)", game.level.w, game.level.h);

	while (true) {
		if (!finishedGen) {
			if (game.level.isGenerated) {
				finishedGen = true;
				Util::Log("Finished level gen");
			}
		}
		game.Update();
		game.Render();
	}
}