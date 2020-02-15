#ifndef __INCLUDE_SHUDSON6_ALBUMFINDER
#define __INCLUDE_SHUDSON6_ALBUMFINDER

#include "Album.h"
#include "Track.h"
#include <vector>
#include <json/json.h>
#include <stdexcept>

class AlbumFinder {
	public:
		AlbumFinder(const char* const _lastfmkey);
		// search lastfm for an album of name by artist; on success, result is store in the referenced album
		bool query(const char* const name, const char* const artist, Album& album) const;

	private:
		std::string lastfmkey;

		const char* const buildUrl(const char* const name, const char* const artist) const;
		void errorMessage(const char* const, const std::exception&) const;
		std::string executeQuery(const char* const) const;
		Json::Value jsonObj(std::string&) const;
		void parseGenres(Json::Value&, Album&) const;
		std::string parseImg(Json::Value&) const;
		void parseJson(Json::Value&, Album&) const;
		Track parseTrack(Json::Value&, const char* const) const;
		void parseTracks(Json::Value&, Album&) const;

		static const char* const URLFMT;
};

class AlbumFinderException : public std::logic_error {
	public:
		AlbumFinderException(const char* what) : logic_error(what) {}
};

#endif // __INCLUDE_SHUDSON6_ALBUMFINDER
