/**
 * Copyright (c) 2020 Steven Hudson
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Steven Hudson shudson6
 * @version 2020.2
 **/
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
		virtual int getLength() const;
		/**
		 * Get a string representing the time of the object in hh:mm:ss format.
		 * For objects less than an hour, hh is left off so you get mm:ss.
		 */
		virtual const char* timeStr() const;
		/**
		 * Get a JSON object representing this MusicDescription.
		 * Should be overridden by extending classes; call this version to get started easily
		 */
		virtual Json::Value toJson() const;
		
		/**
		 * Generates a string in [hh:]mm:ss format for the length of time
		 * given by secs.
		 */
		static std::string TimeStr(int secs);

	private:
		std::string title;
		std::string artist;

	protected:
		// length in seconds
		int len;
		mutable std::string lenstr;
};

#endif // __INCLUDE_SHUDSON6_MUSICDESCRIPTION
