#include "fs.hh"

bool FS::Directory::Exists(string dname) {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		DWORD ftyp = GetFileAttributesA(dname.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;
		return false;
	#else
		DIR* dir = opendir(dname.c_str());
		if (dir) {
			closedir(dir);
			return true;
		}
		return false;
	#endif
}

void FS::Directory::Create(string dname) {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		CreateDirectoryA(dname.c_str(), NULL);
	#else
		mkdir(dname.c_str(), 0777);
	#endif
}

bool FS::File::Exists(string fname) {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		DWORD ftyp = GetFileAttributesA(fname.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return false;
		return true;
	#else
		ifstream ifile(fname.c_str());
		if (ifile) {
			ifile.close();
			return true;
		}
		return false;
	#endif
}

void FS::File::Create(string fname) {
	ofstream fhnd(fname);
	fhnd.close();
}

void FS::File::Write(string fname, string data) {
	ofstream fhnd(fname);
	fhnd << data;
	fhnd.close();
}

string FS::File::Read(string fname) {
	ifstream fhnd(fname);
	string data;
	string line;
	while (getline(fhnd, line)) {
		data += line + "\n";
	}
	fhnd.close();
	return data;
}