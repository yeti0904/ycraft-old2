#include "blockdef.hh"
#include "util.hh"

void Blockdefs::Add(uint16_t id, Blockdef def) {
	blockdefs[id] = def;
	Util::Log("Added new blockdef: %s (%d)", def.name.c_str(), id);
}

void Blockdefs::AddNewBlockdef(uint16_t id, string name, uint16_t textureID, BlockType type) {
	Blockdef def;
	def.name      = name;
	def.textureID = textureID;
	def.type      = type;
	Add(id, def);
}

void Blockdefs::Delete(uint16_t id) {
	blockdefs.erase(id);
}

void Blockdefs::DeleteAll() {
	blockdefs.clear();
}

Blockdef& Blockdefs::Get(uint16_t id) {
	return blockdefs[id];
}