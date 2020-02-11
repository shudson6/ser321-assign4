#ifndef __INCLUDE_SHUDSON6_TRACK
#define __INCLUDE_SHUDSON6_TRACK

#include "MusicDescription.h"
#include <json/json.h>

class Track : public MusicDescription {
	public:
		Track(const string& _title, const string& _artist, const string& _album, int _rank, int _len);

		static Track fromJSON(Json::Value json);

		string getAlbum() const;
		int getRank() const;
		bool operator<(Track& that) const;
		Json::Value toJSON() const;

	private:
		string album;
		int rank;
};

#endif // __INCLUDE_SHUDSON6_TRACK
