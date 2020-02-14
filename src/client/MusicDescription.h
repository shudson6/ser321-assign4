#ifndef __INCLUDE_SHUDSON6_MUSICDESCRIPTION
#define __INCLUDE_SHUDSON6_MUSICDESCRIPTION

#include <string>
#include <json/json.h>

#define NO_TITLE "[no title]"
#define UNKNOWN_ARTIST "[unknown artist]"

class MusicDescription {
	public:
		MusicDescription(const std::string& _title, const std::string& _artist, int _len);
		MusicDescription(const char* _title, const char* _artist, int _len);

		const char* getTitle() const;
		const char* getArtist() const;
		/**
		 * Length in seconds.
		 */
		int getLength() const;
		/**
		 * Get a string representing the time of the object in hh:mm:ss format.
		 * For objects less than an hour, hh is left off so you get mm:ss.
		 */
		const char* timeStr() const;
		/**
		 * Get a JSON object representing this MusicDescription
		 */
		Json::Value toJson() const;
		
		/**
		 * Generates a string in [hh:]mm:ss format for the length of time
		 * given by secs.
		 */
		static std::string TimeStr(int secs);

	private:
		std::string title;
		std::string artist;
		// length in seconds
		int len;
		std::string lenstr;
};

#endif // __INCLUDE_SHUDSON6_MUSICDESCRIPTION
