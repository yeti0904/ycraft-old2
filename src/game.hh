#pragma once
#include "_components.hh"
#include "player.hh"
#include "level.hh"
#include "blockdef.hh"
#include "ui.hh"

enum class TitleScreenState {
	MenuScreen = 0,
	NewWorldScreen
};

struct GameState {
	bool             inventoryOpen;
	bool             playerCanMove;
	bool             titleScreen;
	TitleScreenState titleScreenState;
};

struct MainMenu {
	UI::Button playButton;
	UI::Button exitButton;
};

class Game {
	public:
		// variables
		SDL_Window*     window;
		SDL_Renderer*   renderer;
		SDL_Event       event;
		SDL_Texture*    texturePack;
		TTF_Font*       font;

		Player          player;
		vector <Player> otherPlayers;
		Level           level;
		Blockdefs       blockdefs;
		Vec2            camera;
		GameState       gameState;
		uint8_t         loadingScreenScroll;

		string          gamePath;
		string          gameParentFolder;

		Vec2            mousePos;
		UVec2           selectedBlock;
		bool            mouseLeftPressed;
		bool            mouseRightPressed;

		// menus
		MainMenu mainMenu;

		// utils
		void     RenderText(string text, size_t x, size_t y, size_t scale);
		SDL_Rect GetTextSize(string text, size_t scale);
		void     RenderBackground();

		// functions
		Game();
		~Game();
		void Init(string title);
		void Update();
		void Render();
		void LoadTexturePack(string name);
		void LoadDefaultBlockdefs();
		void LoadPlayerSkin(string name);
};