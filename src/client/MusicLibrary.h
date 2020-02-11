#ifndef __INCLUDE_SHUDSON6_MUSICLIBRARY
#define __INCLUDE_SHUDSON6_MUSICLIBRARY

#include "Album.h"
#include "Track.h"
#include <string>
#include <map>
#include <json/json.h>

class MusicLibrary {
	public:
		MusicLibrary();
		bool addAlbum(Album& toAdd);
		const Album* getAlbum(string name) const;
		string* getAlbumNames() const;
		bool removeAlbum(Album& toRm);
		bool removeAlbum(string name);
		int size() const;
		Json::Value toJSON();
		bool save(string&);
		void load(string&);

	private:
		std::map<string, Album> library;

		string loadFrom(string&);
		Json::Value jsonLibrary(string);
		std::map<string, Album> parseJSON(Json::Value);
};

#endif // __INCLUDE_SHUDSON6_MUSICLIBRARY
