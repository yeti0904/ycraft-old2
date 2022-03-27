#include "game.hh"
#include "util.hh"
#include "constants.hh"

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (level.isGenerated) {
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


	SDL_RenderPresent(renderer);
}