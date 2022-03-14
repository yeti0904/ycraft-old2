#pragma once
#include "_components.hh"
#include "player.hh"
#include "level.hh"
#include "blockdef.hh"

class Game {
	public:
		// variables
		SDL_Window*     window;
		SDL_Renderer*   renderer;
		SDL_Event       event;
		SDL_Texture*    texturePack;

		Player          player;
		vector <Player> otherPlayers;
		Level           level;
		Blockdefs       blockdefs;
		Vec2            camera;

		string          gamePath;
		string          gameParentFolder;

		Vec2            mousePos;
		UVec2           selectedBlock;
		bool            mouseLeftPressed;
		bool            mouseRightPressed;

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