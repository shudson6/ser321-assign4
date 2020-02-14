#ifndef __INCLUDE_SHUDSON6_ALBUM
#define __INCLUDE_SHUDSON6_ALBUM

#include "MusicDescription.h"
#include "Track.h"
#include <vector>
#include <json/json.h>

class Album : public MusicDescription {
	public:
		Album(const string& _title, const string& _artist, std::vector<Track>& _tracks, 
				std::vector<string>& _genres, const string& _imgURL, const string& _summary);
		Album(const string& _title, const string& _artist, const string& _imgURL, const string& _summary);

		static Album fromJSON(Json::Value json);

		std::vector<Track> getTracks() const;
		const Track* getTrack(string name) const;
		std::vector<string> getGenres() const;
		string getImgURL() const;
		string getSummary() const;
		int size() const;
		Json::Value toJSON() const;

	private:
		std::vector<Track> tracks;
		std::vector<string> genres;
		string imgURL;
		string summary;

		Json::Value trackListJSON() const;
		Json::Value genresJSON() const;

		static std::vector<Track> parseTracks(Json::Value);
		static std::vector<string> parseGenres(Json::Value);
		static int calcLen(std::vector<Track>& tracks);
};

#endif // __INCLUDE_SHUDSON6_ALBUM
