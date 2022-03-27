#include "game.hh"
#include "constants.hh"
#include "util.hh"

Game::Game() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		exit(1);
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("IMG_Init Error: %s\n", IMG_GetError());
		exit(1);
	}
	gamePath = Util::GetExecutableLocation();
	if (gamePath == "") {
		Util::Log("Error getting executable location");
		exit(1);
	}
	#if defined(__linux__) || defined(__APPLE__)
		gameParentFolder = gamePath.substr(0, gamePath.rfind('/'));
	#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		gameParentFolder = gamePath.substr(0, gamePath.rfind('\\'));
	#endif

	texturePack = NULL;
	window      = NULL;
	renderer    = NULL;
	player.skin = NULL;

	player.position.x = 32;
	player.position.y = 32;
	camera.x          = 0;
	camera.y          = 0;
	player.velocity   = 0;

	mouseLeftPressed  = false;
	mouseRightPressed = false;
	mousePos.x        = 0;
	mousePos.y        = 0;
}

Game::~Game() {
	Util::Log("Exit (from destructor)");

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != NULL) {
		SDL_DestroyWindow(window);
	}
	if (texturePack != NULL) {
		SDL_DestroyTexture(texturePack);
	}
	if (player.skin != NULL) {
		SDL_DestroyTexture(player.skin);
	}
	
	IMG_Quit();
	SDL_Quit();
}

void Game::Init(string title) {
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_RenderSetLogicalSize(renderer, APP_WINDOW_W, APP_WINDOW_H);

	SDL_ShowCursor(SDL_DISABLE);

	for (size_t i = 0; i<player.inventory.size(); ++i) {
		player.inventory[i] = i + 1;
	}
	player.heldBlock    = 0;
	player.position.x   = APP_WINDOW_W / 2;
	player.position.y   = APP_WINDOW_H / 2;
	camera.x            = 0;
	camera.y            = 0;
}

void Game::Update() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				exit(0);
			}
			case SDL_MOUSEMOTION: {
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				if (event.button.button == SDL_BUTTON_LEFT) {
					mouseLeftPressed = true;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					mouseRightPressed = true;
				}
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				if (event.button.button == SDL_BUTTON_LEFT) {
					mouseLeftPressed = false;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					mouseRightPressed = false;
				}
				break;
			}
			case SDL_MOUSEWHEEL: {
				if (event.wheel.y > 0) { // scroll up
					-- player.heldBlock;
					if (player.heldBlock < 0) {
						player.heldBlock = player.inventory.size() - 1;
					}
				}
				else if (event.wheel.y < 0) { // scroll down
					++ player.heldBlock;
					if (player.heldBlock >= (int)player.inventory.size()) {
						player.heldBlock = 0;
					}
				}
				break;
			}
		}
	};

	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_W]) {
		if (player.position.y > 0) {
			-- player.position.y;
			-- camera.y;
		}
		else {
			player.position.y = 0;
		}
	}
	if (keystate[SDL_SCANCODE_S]) {
		if (player.position.y < (level.h * BLOCK_SIZE) - 1 - BLOCK_SIZE) {
			++ player.position.y;
			++ camera.y;
		}
	}
	if (keystate[SDL_SCANCODE_A]) {
		if (player.position.x > 0) {
			-- player.position.x;
			-- camera.x;
		}
		else {
			player.position.x = 0;
		}
	}
	if (keystate[SDL_SCANCODE_D]) {
		if (player.position.x < (level.w * BLOCK_SIZE) - 1 - BLOCK_SIZE) {
			++ player.position.x;
			++ camera.x;
		}
	}
	if (keystate[SDL_SCANCODE_1]) {
		player.heldBlock = 0;
	}
	if (keystate[SDL_SCANCODE_2]) {
		player.heldBlock = 1;
	}
	if (keystate[SDL_SCANCODE_3]) {
		player.heldBlock = 2;
	}
	if (keystate[SDL_SCANCODE_4]) {
		player.heldBlock = 3;
	}
	if (keystate[SDL_SCANCODE_5]) {
		player.heldBlock = 4;
	}
	if (keystate[SDL_SCANCODE_6]) {
		player.heldBlock = 5;
	}
	if (keystate[SDL_SCANCODE_7]) {
		player.heldBlock = 6;
	}
	if (keystate[SDL_SCANCODE_8]) {
		player.heldBlock = 7;
	}
	if (keystate[SDL_SCANCODE_9]) {
		player.heldBlock = 8;
	}

	if (mouseLeftPressed) {
		if (blockdefs.Get(level.frontLayer[selectedBlock.x][selectedBlock.y]).type == BlockType::Gas) {
			level.backLayer[selectedBlock.x][selectedBlock.y] = 0;
		}
		else {
			level.frontLayer[selectedBlock.x][selectedBlock.y] = 0;
		}
		mouseLeftPressed = false;
	}
	if (mouseRightPressed) {
		if (blockdefs.Get(level.backLayer[selectedBlock.x][selectedBlock.y]).type == BlockType::Gas) {
			level.backLayer[selectedBlock.x][selectedBlock.y] = player.inventory[player.heldBlock];
		}
		else if (blockdefs.Get(level.frontLayer[selectedBlock.x][selectedBlock.y]).type == BlockType::Gas) {
			level.frontLayer[selectedBlock.x][selectedBlock.y] = player.inventory[player.heldBlock];
		}
		mouseRightPressed = false;
	}
}

void Game::LoadTexturePack(string name) {
	string texturePackPath = gameParentFolder + "/texpacks/" + name + ".png";
	texturePack = IMG_LoadTexture(renderer, texturePackPath.c_str());
	if (texturePack == NULL) {
		Util::Log("IMG_LoadTexture Error: %s\n", IMG_GetError());
		exit(1);
	}
}

void Game::LoadDefaultBlockdefs() {
	blockdefs.AddNewBlockdef(0,  "air", 0, BlockType::Gas);
	blockdefs.AddNewBlockdef(1,  "grass", 0, BlockType::Solid);
	blockdefs.AddNewBlockdef(2,  "dirt", 1, BlockType::Solid);
	blockdefs.AddNewBlockdef(3,  "sand", 2, BlockType::Solid);
	blockdefs.AddNewBlockdef(4,  "gravel", 3, BlockType::Solid);
	blockdefs.AddNewBlockdef(5,  "stone", 4, BlockType::Solid);
	blockdefs.AddNewBlockdef(6,  "cobblestone", 5, BlockType::Solid);
	blockdefs.AddNewBlockdef(7,  "gold ore", 6, BlockType::Solid);
	blockdefs.AddNewBlockdef(8,  "iron ore", 7, BlockType::Solid);
	blockdefs.AddNewBlockdef(9,  "coal ore", 8, BlockType::Solid);
	blockdefs.AddNewBlockdef(10, "stone bricks", 9, BlockType::Solid);
	blockdefs.AddNewBlockdef(11, "water", 10, BlockType::Liquid);
	blockdefs.AddNewBlockdef(12, "lava", 11, BlockType::Liquid);
	blockdefs.AddNewBlockdef(13, "bricks", 12, BlockType::Solid);
	blockdefs.AddNewBlockdef(14, "rock", 13, BlockType::SemiSolid);
}

void Game::LoadPlayerSkin(string name) {
	if (player.skin != NULL) {
		SDL_DestroyTexture(player.skin);
	}
	string playerSkinPath = gameParentFolder + "/skins/" + name + ".png";
	player.skin = IMG_LoadTexture(renderer, playerSkinPath.c_str());
	if (player.skin == NULL) {
		Util::Log("IMG_LoadTexture Error: %s\n", IMG_GetError());
		exit(1);
	}
	SDL_Point skinsize;
	SDL_QueryTexture(player.skin, NULL, NULL, &skinsize.x, &skinsize.y);
	if (skinsize.x != skinsize.y) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Note", "Player skin may not appear correctly. It is recommended to use a square skin.", NULL);
	}
}