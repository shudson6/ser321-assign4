#ifndef __INCLUDE_SHUDSON6_TRACK
#define __INCLUDE_SHUDSON6_TRACK

#include "MusicDescription.h"

#define UNKNOWN_ALBUM "[unknown album]"

class Track : public MusicDescription {
	public:
		static Track fromJson(Json::Value json);

		Track(const std::string& _title, const std::string& _artist, const std::string& _album, int _rank, int _len);
		Track(const char* _title, const char* _artist, const char* _album, int _rank, int _len);

		const char* getAlbum() const;
		int getRank() const;
		Json::Value toJson() const;

		bool operator<(Track&) const;

	private:
		std::string album;
		int rank;
};

#endif // __INCLUDE_SHUDSON6_TRACK
