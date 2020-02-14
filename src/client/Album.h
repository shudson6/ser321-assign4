#ifndef __INCLUDE_SHUDSON6_ALBUM
#define __INCLUDE_SHUDSON6_ALBUM

#include "MusicDescription.h"
#include "Track.h"
#include <string>
#include <vector>
#include <json/json.h>

#define NO_IMAGE ""
#define NO_SUMMARY "[no summary available]"

class Album : public MusicDescription {
	public:
		static Album fromJson(const Json::Value& json);
		
		Album(const std::string& _title, const std::string& _artist, const std::string& _imgUrl, const std::string& _summary);
		Album(const char* _title, const char* _artist, const char* _imgUrl, const char* _summary);

		const char* getImgUrl() const;
		const char* getSummary() const;
		/**
		 * Get the Track on this Album with given rank.
		 * Returns NULL if there is no such Track.
		 */
		const Track* getTrack(int) const;
		const std::vector<std::string>& getGenres() const;
		const std::vector<Track>& getTracks() const;
		/**
		 * Get the number of tracks on this Album.
		 */
		int size() const;

		bool addGenre(const char*);
		bool removeGenre(const char*);

		/**
		 * Add a track to this Album.
		 * If this Album has no Track with the rank of the input, the input Track is copied into this Album and a
		 * pointer to the copy is returned. If a track with that rank already existed, it is not replaced and a
		 * pointer to the input is returned. Thus, one can check the success of this function by looking at the
		 * returned pointer: if it is valid and does not point to the original Track, then it was added. On error,
		 * NULL may be returned.
		 */
		const Track* addTrack(const Track&);
		/**
		 * Remove a Track from this Album.
		 * The required parameter is the rank of the Track to be removed. Since the uniqueness of Tracks on an Album
		 * is enforced based on rank only, this is the only removal method that can guarantee the intended target
		 * is removed.
		 * Return value indicates whether the track was removed. Return value of false most likely indicates no
		 * such Track existed, though it may also be that the input value was invalid (zero or negative).
		 * Note that since Album instances keep internal copies of their Tracks, any references to the removed
		 * Track will be rendered invalid if this function succeeds.
		 */
		bool removeTrack(int);
		
		Json::Value toJson() const;

	private:
		std::string imgUrl;
		std::string summary;
		std::vector<std::string> genres;
		std::vector<Track> tracks;
};

#endif // __INCLUDE_SHUDSON6_ALBUM
