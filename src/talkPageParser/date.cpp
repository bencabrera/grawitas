#include "date.h"

namespace Grawitas {
	Date make_date(int sec, int min, int hour, int day, int month, int year)
	{
		Date t;

		set<SECOND>(t, sec);
		set<MINUTE>(t, min);
		set<HOUR>(t, hour);
		set<DAY>(t, day);
		set<MONTH>(t, month);
		set<YEAR>(t, year);

		return t;
	}

}

bool operator==(const Grawitas::Date& d1, const Grawitas::Date& d2) {
	using namespace Grawitas;

	return (
		get<SECOND>(d1) == get<SECOND>(d2)
		&& get<MINUTE>(d1) == get<MINUTE>(d2)
		&& get<HOUR>(d1) == get<HOUR>(d2)
		&& get<DAY>(d1) == get<DAY>(d2)
		&& get<MONTH>(d1) == get<MONTH>(d2)
		&& get<YEAR>(d1) == get<YEAR>(d2)
	);
}

bool operator>=(const Grawitas::Date& d1, const Grawitas::Date& d2) {
    using namespace Grawitas;
    if(get<YEAR>(d1) >= get<YEAR>(d2)) {
        return true;
    } else if(get<YEAR>(d1) == get<YEAR>(d2)) {
        if(get<MONTH>(d1) >= get<MONTH>(d2)) {
            return true;
        } else if(get<MONTH>(d1) == get<MONTH>(d2)) {
            if(get<DAY>(d1) >= get<DAY>(d2)) {
                return true; 
            } else if (get<DAY>(d1) == get<DAY>(d2)) {
                return (get<HOUR>(d1) >= get<HOUR>(d2));
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}


