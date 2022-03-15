#pragma once
#include "_components.hh"
#include "vec2.hh"

struct Player {
	string              name;
	uint8_t             health;
	UVec2               position;
	array <uint16_t, 9> inventory;
	int8_t              heldBlock;
	SDL_Texture*        skin;
	uint8_t             velocity;
};