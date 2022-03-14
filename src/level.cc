#include "level.hh"

void generateLevel(size_t w, size_t h, bool& isGenerated
, unordered_map <blockCoordinate, unordered_map <blockCoordinate, uint16_t>>& backLayer,
unordered_map <blockCoordinate, unordered_map <blockCoordinate, uint16_t>>& frontLayer
) {
	for (blockCoordinate y = 0; y<h; ++y) {
		for (blockCoordinate x = 0; x<w; ++x) {
			backLayer[x][y] = 1;
			frontLayer[x][y] = 0;
		}
	}
	isGenerated = true;
}

void Level::Generate(size_t aw, size_t ah) {
	isGenerated = false;
	thread t(generateLevel, aw, ah, std::ref(isGenerated), std::ref(backLayer), std::ref(frontLayer));
	t.detach();
	w = aw;
	h = ah;
}
