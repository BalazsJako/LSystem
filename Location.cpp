
#include <sstream>
#include <algorithm>
#include <ASSERT.h>

#include "Location.h"

using namespace std;

namespace LSystem
{
	Location::Location()
		: mFileName("")
		, mFirstLine(1)
		, mFirstColumn(1)
		, mLastLine(1)
		, mLastColumn(1)
	{ }

	Location::Location(const char *aFileName, int aFirstLine, int aFirstColumn, int aLastLine, int aLastColumn)
		: mFileName(aFileName)
		, mFirstLine(aFirstLine)
		, mFirstColumn(aFirstColumn)
		, mLastLine(aLastLine)
		, mLastColumn(aLastColumn)
	{
		ASSERT(mFirstLine <= mLastLine);
		ASSERT(mFirstColumn <= mLastColumn);
	}


	bool Location::operator == (const Location &o) const
	{
		return mFirstColumn == o.mFirstColumn
			&& mFirstLine == o.mFirstLine
			&& mLastColumn == o.mLastColumn
			&& mLastLine == o.mLastLine
			&& mFileName == o.mFileName;
	}

	bool Location::operator != (const Location &o) const
	{
		return mFirstColumn != o.mFirstColumn
			|| mFirstLine != o.mFirstLine
			|| mLastColumn != o.mLastColumn
			|| mLastLine != o.mLastLine
			|| mFileName != o.mFileName;
	}

	std::string Location::ToString() const
	{
		std::ostringstream s;
		s << GetFileName() << ":" << GetFirstLine() << "," << GetFirstColumn() << "-" << GetLastLine() << "," << GetLastColumn();
		return s.str();
	}

	std::string Location::ToShortString() const
	{
		std::ostringstream s;
		s << GetFileName() << ":" << GetFirstLine() << "," << GetFirstColumn();
		return s.str();
	}

	Location Location::operator +(const Location& rhs) const
	{
		Location r(*this);		
		r.mFirstLine = min(mFirstLine, rhs.mFirstLine);
		r.mFirstColumn = min(mFirstColumn, rhs.mFirstColumn);
		r.mLastLine = max(mLastLine, rhs.mLastLine);
		r.mLastColumn = max(mLastColumn, rhs.mLastColumn);

		ASSERT(mFirstLine <= mLastLine);
		ASSERT(mFirstColumn <= mLastColumn);

		return r;
	}

	Location& Location::operator += (const Location& rhs)
	{
		mFirstLine = min(mFirstLine, rhs.mFirstLine);
		mFirstColumn = min(mFirstColumn, rhs.mFirstColumn);
		mLastLine = max(mLastLine, rhs.mLastLine);
		mLastColumn = max(mLastColumn, rhs.mLastColumn);

		ASSERT(mFirstLine <= mLastLine);
		ASSERT(mFirstColumn <= mLastColumn);

		return *this;
	}
}