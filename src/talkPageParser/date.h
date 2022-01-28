#pragma once

#include <ctime>

namespace Grawitas {
	using Date = std::tm;
	enum DateField {SECOND, MINUTE, HOUR, DAY, MONTH, YEAR};
	
	Date make_date(int sec, int min, int hour, int day, int month, int year);

	template<DateField field>
	inline int get(const Date& d) 
	{
		if(field == SECOND)
			return d.tm_sec;
		if(field == MINUTE)
			return d.tm_min;
		if(field == HOUR)
			return d.tm_hour;
		if(field == DAY)
			return d.tm_mday;
		if(field == MONTH)
			return d.tm_mon+1;
		if(field == YEAR)
			return d.tm_year+1;
	}

	template<DateField field>
	inline void set(Date& d, int val) 
	{
		if(field == SECOND)
			d.tm_sec = val;
		if(field == MINUTE)
			d.tm_min = val;
		if(field == HOUR)
			d.tm_hour = val;
		if(field == DAY)
			d.tm_mday = val;
		if(field == MONTH)
			d.tm_mon = val-1;
		if(field == YEAR)
			d.tm_year = val-1900;
	}

}

bool operator==(const Grawitas::Date& d1, const Grawitas::Date& d2);
bool operator>=(const Grawitas::Date& d1, const Grawitas::Date& d2);
