#pragma once
#include "_components.hh"

enum class BlockType {
	Gas,
	Solid,
	Liquid,
	SemiSolid
};

struct Blockdef {
	string    name;
	uint16_t  textureID;
	BlockType type;
	uint16_t  hardness;
};

class Blockdefs {
	public:
		// variables
		unordered_map <uint16_t, Blockdef> blockdefs;

		// functions
		Blockdefs() {};
		void      Add(uint16_t id, Blockdef def);
		void      AddNewBlockdef(uint16_t id, string name, uint16_t textureID, BlockType type);
		void      Delete(uint16_t id);
		void      DeleteAll();
		Blockdef& Get(uint16_t id);
};