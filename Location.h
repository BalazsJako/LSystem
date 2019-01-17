#pragma once

#include <string>

#include "Debug.h"

namespace LSystem
{
	class Location
	{
	public:
		Location();
		Location(const char *aFileName, int aFirstLine, int aFirstColumn, int aLastLine, int aLastColumn);
		bool operator == (const Location &o) const;
		bool operator != (const Location &o) const;

		inline const std::string& GetFileName() const { return mFileName; }
		inline int GetFirstLine() const { return mFirstLine; }
		inline int GetFirstColumn() const { return mFirstColumn; }
		inline int GetLastLine() const { return mLastLine; }
		inline int GetLastColumn() const { return mLastColumn; }

		std::string ToString() const;
		std::string ToShortString() const;

		Location operator +(const Location& rhs) const;
		Location& operator += (const Location& rhs);

	private:
		std::string	mFileName;
		int			mFirstLine;
		int			mFirstColumn;
		int			mLastLine;
		int			mLastColumn;
	};
}
