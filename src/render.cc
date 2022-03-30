#include "game.hh"
#include "util.hh"
#include "constants.hh"

void Game::RenderText(string text, size_t x, size_t y, size_t scale) {
	if (font == NULL) {
		printf("Error: font is NULL\n(called from Game::RenderText)\n");
		exit(1);
	}
	/*if (text == "") {
		char* s = "lol";
		*s = 'a';
	}*/
	SDL_Colour colour = {0, 0, 0, 100};
	SDL_Rect   renderAt;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);
	if (textSurface == NULL) {
		printf("SDL2_ttf Error: %s (\"%s\", %d, %d, %d)\n", TTF_GetError(), text.c_str(), (int)x, (int)y, (int)scale);
		exit(1);
	}
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_QueryTexture(textTexture, NULL, NULL, &renderAt.w, &renderAt.h);
	renderAt.x  = x;
	renderAt.y  = y;
	renderAt.w *= scale;
	renderAt.h *= scale;

	SDL_RenderCopy(renderer, textTexture, NULL, &renderAt);
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);

	colour = {255, 255, 255, 255};
	textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, textTexture, NULL, &renderAt);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}

SDL_Rect Game::GetTextSize(string text, size_t scale) {
	SDL_Colour   colour;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect     ret;

	colour  = {255, 255, 255, 255};

	if (font == NULL) {
		printf("Error: game font is NULL (Game::GetTextSize)\n");
		exit(1);
	}

	surface = TTF_RenderText_Solid(font, text.c_str(), colour);
	// valgrind is giving me so many errors and its all coming from the above line
	if (surface == NULL) {
		printf("SDL2_ttf Error: %s (%s %d)\n", TTF_GetError(), text.c_str(), (int)scale);
		exit(1);
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_QueryTexture(texture, NULL, NULL, &ret.w, &ret.h);
	ret.w *= scale;
	ret.h *= scale;

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return ret;
}

void Game::RenderBackground() {
	UVec2 dirtPos = Util::GetTexturePosition(blockdefs.Get(2).textureID, texturePack);
	SDL_Rect src;
	src.x = dirtPos.x;
	src.y = dirtPos.y;
	src.w = BLOCK_SIZE;
	src.h = BLOCK_SIZE;
	SDL_Rect block;
	block.w = BLOCK_SIZE;
	block.h = BLOCK_SIZE;
	for (size_t i = 0; i < (APP_WINDOW_H / BLOCK_SIZE) + 2; ++i) {
		for (size_t j = 0; j < (APP_WINDOW_W / BLOCK_SIZE) + 2; ++j) {
			block.x = j * BLOCK_SIZE;
			block.y = (i * BLOCK_SIZE) - loadingScreenScroll;
			SDL_RenderCopy(renderer, texturePack, &src, &block);
		}
	}
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (gameState.titleScreen) {
		RenderBackground();
		switch (gameState.titleScreenState) {
			case TitleScreenState::MenuScreen: {
				// render main stuff
				string logoText = "ycraft";
				SDL_Rect position = GetTextSize(logoText.c_str(), 4);
				position.x = (APP_WINDOW_W / 2) - (position.w / 2);
				position.y = 10;
				RenderText(logoText, position.x, position.y, 2);

				// render ui
				mainMenu.playButton.Render(*this);
				break;
			}
			case TitleScreenState::NewWorldScreen: {
				break;
			}
		}
	}

	else if (!level.isGenerated) {
		// render background
		++ loadingScreenScroll;
		if (loadingScreenScroll >= 16) {
			loadingScreenScroll = 0;
		}
		RenderBackground();

		// render text
		string loadingText = "Generating level";
		SDL_Rect position = GetTextSize(loadingText.c_str(), 2);
		position.x = (APP_WINDOW_W / 2) - (position.w / 2);
		position.y = (APP_WINDOW_H / 2) - (position.h / 2);
		RenderText(loadingText, position.x, position.y, 2);

		// render progress bar
		SDL_Rect bar;
		bar.x = (APP_WINDOW_W / 2) - 50;
		bar.y = position.y + position.h + 10;
		bar.w = 100;
		bar.h = 10;
		SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
		SDL_RenderFillRect(renderer, &bar);
		SDL_SetRenderDrawColor(renderer,78, 207, 71, 255);
		bar.w = level.percentGenerated;
		SDL_RenderFillRect(renderer, &bar);
	}

	else if (level.isGenerated) {
		// render level
		SDL_Rect block;
		SDL_Rect block_src;
		block.w = BLOCK_SIZE;
		block.h = BLOCK_SIZE;
		block_src.w = BLOCK_SIZE;
		block_src.h = BLOCK_SIZE;

		UVec2 max;
		max.x = (camera.x / BLOCK_SIZE) + (APP_WINDOW_W / BLOCK_SIZE) + 1;
		max.y = (camera.y / BLOCK_SIZE) + (APP_WINDOW_H / BLOCK_SIZE) + 1;

		for (blockCoordinate y = 0; (y<max.y) && (y<level.h); ++y) {
			for (blockCoordinate x = 0; (x<=max.x) && (x<level.w); ++x) {
				block.x = (x * BLOCK_SIZE) - camera.x;
				block.y = (y * BLOCK_SIZE) - camera.y;
				if ((blockdefs.Get(level.backLayer[x][y]).type != BlockType::Gas) && 
					(
						(blockdefs.Get(level.frontLayer[x][y]).type == BlockType::Gas) ||
						(blockdefs.Get(level.frontLayer[x][y]).type == BlockType::SemiSolid)
					)
				) {
					UVec2 src = Util::GetTexturePosition(blockdefs.Get(level.backLayer[x][y]).textureID, texturePack);
					block_src.x = src.x;
					block_src.y = src.y;
					SDL_RenderCopy(renderer, texturePack, &block_src, &block);
				}
				if (blockdefs.Get(level.frontLayer[x][y]).type != BlockType::Gas) {
					UVec2 src = Util::GetTexturePosition(blockdefs.Get(level.frontLayer[x][y]).textureID, texturePack);
					block_src.x = src.x;
					block_src.y = src.y;
					SDL_RenderCopy(renderer, texturePack, &block_src, &block);
				}
			}
		}

		// loop through front layer to do outlines
		for (blockCoordinate x = 0; (x<max.x) && (x<level.w); ++x) {
			for (blockCoordinate y = 0; (y<max.y) && (y<level.h); ++y) {
				block.x = (x * BLOCK_SIZE) - camera.x;
				block.y = (y * BLOCK_SIZE) - camera.y;
				if (blockdefs.Get(level.frontLayer[x][y]).type == BlockType::Solid) {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					if (level.frontLayer[x - 1][y] == 0) {
						SDL_RenderDrawLine(renderer, block.x, block.y, block.x, block.y + BLOCK_SIZE - 1);
					}
					if (level.frontLayer[x + 1][y] == 0) {
						SDL_RenderDrawLine(renderer, block.x + BLOCK_SIZE - 1, block.y, block.x + BLOCK_SIZE - 1, block.y + BLOCK_SIZE - 1);
					}
					if (level.frontLayer[x][y - 1] == 0) {
						SDL_RenderDrawLine(renderer, block.x, block.y, block.x + BLOCK_SIZE - 1, block.y);
					}
					if (level.frontLayer[x][y + 1] == 0) {
						SDL_RenderDrawLine(renderer, block.x, block.y + BLOCK_SIZE - 1, block.x + BLOCK_SIZE - 1, block.y + BLOCK_SIZE - 1);
					}
				}
				if ((mousePos.x >= block.x) && (mousePos.x < block.x + BLOCK_SIZE) && (mousePos.y >= block.y) && (mousePos.y < block.y + BLOCK_SIZE)) {
					selectedBlock.x = x;
					selectedBlock.y = y;
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(renderer, &block);
				}
			}
		}

		// render player
		block.x = player.position.x - camera.x;
		block.y = player.position.y - camera.y;
		//printf("%d %d (%d - %d) (%d - %d)\n", block.x, block.y, player.position.x, camera.x, player.position.y, camera.y);
		SDL_RenderCopy(renderer, player.skin, NULL, &block);

		// render inventory
		{
			size_t startX = (APP_WINDOW_W / 2) - ((BLOCK_SIZE * player.inventory.size()) / 2);
			size_t startY = APP_WINDOW_H - (BLOCK_SIZE + 6);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Rect background;
			background.x = startX;
			background.y = startY;
			background.w = BLOCK_SIZE * player.inventory.size();
			background.h = BLOCK_SIZE;
			SDL_RenderFillRect(renderer, &background);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			for (size_t i = 0; i<player.inventory.size(); ++i) {
				block.x = startX + (i * BLOCK_SIZE) ;
				block.y = startY;
				block_src.x = Util::GetTexturePosition(blockdefs.Get(player.inventory[i]).textureID, texturePack).x;
				block_src.y = Util::GetTexturePosition(blockdefs.Get(player.inventory[i]).textureID, texturePack).y;
				SDL_RenderCopy(renderer, texturePack, &block_src, &block);
				if ((int)i == player.heldBlock) {
					SDL_RenderDrawRect(renderer, &block);
				}
			}

			SDL_Rect inventoryOutline;
			inventoryOutline.x = startX - 1;
			inventoryOutline.y = startY - 1;
			inventoryOutline.w = BLOCK_SIZE * player.inventory.size() + 2;
			inventoryOutline.h = BLOCK_SIZE + 2;
			SDL_SetRenderDrawColor(renderer, 222, 222, 222, 255);
			SDL_RenderDrawRect(renderer, &inventoryOutline);
			-- inventoryOutline.x;
			-- inventoryOutline.y;
			inventoryOutline.w += 2;
			inventoryOutline.h += 2;
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_RenderDrawRect(renderer, &inventoryOutline);
		}

		// render cursor
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, mousePos.x, mousePos.y - 1, mousePos.x, mousePos.y - 3);
		SDL_RenderDrawLine(renderer, mousePos.x - 1, mousePos.y, mousePos.x - 3, mousePos.y);
		SDL_RenderDrawLine(renderer, mousePos.x + 1, mousePos.y, mousePos.x + 3, mousePos.y);
		SDL_RenderDrawLine(renderer, mousePos.x, mousePos.y + 1, mousePos.x, mousePos.y + 3);
	}

	//RenderText("ycraft beta", 10, 10, 1);

	SDL_RenderPresent(renderer);
}