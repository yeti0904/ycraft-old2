#pragma once
#include "_components.hh"

namespace FS {
	namespace Directory {
		bool Exists(string dname);
		void Create(string dname);
	}
	namespace File {
		bool Exists(string fname);
		void Create(string fname);
		void Write(string fname, string data);
		string Read(string fname);
	}
}