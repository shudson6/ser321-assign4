#ifndef __INCLUDE_SHUDSON6_MUSICLIBRARY
#define __INCLUDE_SHUDSON6_MUSICLIBRARY

#include "Album.h"
#include "MusicDescription.h"
#include "Track.h"
#include <map>
#include <string>
#include <json/json.h>

class MusicLibrary : public MusicDescription {
	public:
		MusicLibrary();
		bool addAlbum(const Album&);
		bool addTrack(const Track&, const char* const);
		bool removeAlbum(const char* name);
		void load(const char* filename);
		void parseJson(Json::Value&);
		const char* timeStr();

		const Album* getAlbum(const char* name) const;
		const std::string* getAlbumNames() const;
		int size() const;
		int getLength() const;
		Json::Value toJson() const;
		bool save(const char* filename) const;

	private:
		std::map<std::string, Album> albums;
};

#endif // __INCLUDE_SHUDSON6_MUSICLIBRARY
