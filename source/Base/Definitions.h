#pragma once

#include <QObject>
#include <QtGlobal>
#include <QColor>
#include <QVector2D>
#include <QVector2D>
#include <QSize>
#include <QMap>
#include <QSet>
#include <QList>
#include <QDataStream>
#include <qmath.h>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <string>
#include <set>
#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "DllExport.h"
#include "Tracker.h"

class NumberGenerator;
class TagGenerator;

using std::uint64_t;
using std::uint32_t;
using std::uint32_t;
using std::int64_t;
using std::int32_t;
using std::int32_t;
using std::vector;
using std::map;
using std::set;
using std::list;
using std::unordered_set;
using std::unordered_map;
using std::pair;
using std::string;
using boost::optional;
using boost::shared_ptr;

class _Worker;
using Worker = boost::shared_ptr<_Worker>;

class Job;

const double FLOATINGPOINT_HIGH_PRECISION = 1.0e-7;
const double FLOATINGPOINT_MEDIUM_PRECISION = 1.0e-4;
const double FLOATINGPOINT_LOW_PRECISION = 1.0e-1;

inline float toFloat(int value)
{
    return static_cast<float>(value);
}

inline QColor toQColor(unsigned int const& rgba)
{
    return QColor((rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff, rgba >> 24);
}

struct IntRect;
struct BASE_EXPORT IntVector2D {
	int x = 0;
	int y = 0;

	IntVector2D() = default;
	IntVector2D(std::initializer_list<int> l);
	IntVector2D(QVector2D const& vec);
	QVector2D toQVector2D();
	IntVector2D& restrictToRect(IntRect const& rect);
	bool operator==(IntVector2D const& vec);
    void operator-=(IntVector2D const& vec);

};

struct RealRect;
struct BASE_EXPORT RealVector2D
{
    float x = 0;
    float y = 0;

    RealVector2D() = default;
    RealVector2D(std::initializer_list<float> l);
    RealVector2D(QVector2D const& vec);
    QVector2D toQVector2D();
    RealVector2D& restrictToRect(RealRect const& rect);
    bool operator==(RealVector2D const& vec);
    void operator-=(RealVector2D const& vec);
};

BASE_EXPORT std::ostream& operator << (std::ostream& os, const IntVector2D& vec);

struct BASE_EXPORT IntRect {
	IntVector2D p1;
	IntVector2D p2;

	IntRect() = default;
	IntRect(std::initializer_list<IntVector2D> l);
	IntRect(QRectF const& rect);
	bool isContained(IntVector2D const& p) const
    {
        return p1.x <= p.x && p1.y <= p.y && p.x <= p2.x && p.y <= p2.y;
    }

	IntVector2D center() const { return IntVector2D({(p1.x + p2.x) / 2, (p1.y + p2.y) / 2}); }
};

struct BASE_EXPORT RealRect
{
    RealVector2D p1;
    RealVector2D p2;

    RealRect() = default;
    RealRect(std::initializer_list<RealVector2D> l);
    RealRect(QRectF const& rect);
    bool isContained(RealVector2D const& p) const { return p1.x <= p.x && p1.y <= p.y && p.x <= p2.x && p.y <= p2.y; }

    RealVector2D center() const { return RealVector2D({(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f}); }
};


#define SET_CHILD(previousChild, newChild)\
	if (previousChild != newChild) { \
		if(previousChild) { \
			previousChild->deleteLater(); \
		} \
		previousChild = newChild; \
		previousChild->setParent(this); \
	}

#define THROW_NOT_IMPLEMENTED() throw std::exception("not implemented")

#define CHECK(expression)\
	if(!(expression)) {\
		throw std::exception("check failed");\
	}

#define MEMBER_DECLARATION(className, type, name, initialValue)\
    type _ ## name = initialValue; \
    className& name(type const& name)\
    { \
        _ ## name = name; \
        return *this; \
    }
