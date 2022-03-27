#pragma once
#include "_components.hh"
#include "constants.hh"
#include "vec2.hh"

enum class Biome {
	Ocean,
	Plains,
	Desert
};

class Level {
	public:
		uint32_t w, h;
		bool   isGenerated;

		unordered_map <blockCoordinate, unordered_map <blockCoordinate, uint16_t>> frontLayer;
		unordered_map <blockCoordinate, unordered_map <blockCoordinate, uint16_t>> backLayer;

		Level() {
			isGenerated = false;
		};
		void Generate(size_t aw, size_t ah);
};