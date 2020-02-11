#ifndef __INCLUDE_SHUDSON6_MUSICDESCRIPTION
#define __INCLUDE_SHUDSON6_MUSICDESCRIPTION

#include <string>
using std::string;

class MusicDescription {
	public:
		MusicDescription(const string& _title, const string& _artist, int _len);

		string getTitle() const;
		string getArtist() const;
		int getLength() const;
		string getLengthStr() const;

	private:
		string title;
		string artist;
		int len;
		string lenstr;

		void makeLenStr();
};

#endif // __INCLUDE_SHUDSON6_MUSICDESCRIPTION
