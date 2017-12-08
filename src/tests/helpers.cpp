#include "helpers.h"
#include "../output/dateOutput.h"

bool operator==(const std::tm t1, std::tm t2)
{
	return (t1.tm_mday == t2.tm_mday && t1.tm_min == t2.tm_min && t1.tm_mon == t2.tm_mon && t1.tm_year == t2.tm_year && t1.tm_hour == t2.tm_hour && t1.tm_sec == t2.tm_sec);
}

std::ostream& operator<<(std::ostream& ostr, const Grawitas::Comment& c)
{
	ostr << "User: " << c.User << std::endl;
	ostr << "Date: " << c.Date << std::endl;
	ostr << "Text: " << std::endl << c.Text << std::endl;

	return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const std::tm& d)
{
	ostr << (d.tm_year+1900) << "-" << (d.tm_mon+1) << "-" << d.tm_mday;
	ostr << " ";
	ostr << d.tm_hour << ":" << d.tm_min << ":" << d.tm_sec;

	return ostr;
}
