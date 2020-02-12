#ifndef __INCLUDE_SHUDSON6_ALBUMFINDER
#define __INCLUDE_SHUDSON6_ALBUMFINDER

#include "Album.h"
#include "Track.h"
#include <vector>
#include <json/json.h>
#include <stdexcept>

class AlbumFinder {
	public:
		AlbumFinder(const char* _lastfmkey);
		AlbumFinder(string _lastfmKey);
		Album query(string& title, string& artist);

	private:
		string lastfmKey;

		string buildUrlStr(string& title, string& artist);
		string executeQuery(string& url);
		Album parse(string& jsonStr);
		Album parseAlbum(Json::Value);
		std::vector<Track> parseTracks(Json::Value, string);
		Track parseTrack(Json::Value, string);
		std::vector<string> parseGenres(Json::Value);
		string parseImg(Json::Value);

		static string& trim(string&);
		static string& wsToPlus(string&);
		static bool isWhiteSpace(char&);

		static const char* const URLFMT;
};

class AlbumFinderException : public std::logic_error {
	public:
		AlbumFinderException(const char* what) : logic_error(what) {}
};

#endif // __INCLUDE_SHUDSON6_ALBUMFINDER
