#ifndef TYPES_H
#define TYPES_H

//---------------------------------------------------------------------------------------------------------------------
// This class represents a single point in 2D space
//---------------------------------------------------------------------------------------------------------------------
class Point
{
public:
    long x, y;

    // construction
    Point(void) { x = y = 0; }
    Point(const long newX, const long newY) { x = newX; y = newY; }
    Point(const Point& newPoint) { x = newPoint.x; y = newPoint.y; }
    Point(const Point* pNewPoint) { x = pNewPoint->x; y = pNewPoint->y; }
//    Point(const POINT& newPoint) { x = newPoint.x; y = newPoint.y; }

    // assignment
    Point& operator=(const Point& newPoint) { x = newPoint.x; y = newPoint.y; return (*this); }
    Point& operator=(const Point* pNewPoint) { x = pNewPoint->x; y = pNewPoint->y; return (*this); }

    // addition/subtraction
    Point& operator+=(const Point& newPoint) { x += newPoint.x; y += newPoint.y; return (*this); }
    Point& operator-=(const Point& newPoint) { x -= newPoint.x; y -= newPoint.y; return (*this); }
    Point& operator+=(const Point* pNewPoint) { x += pNewPoint->x; y += pNewPoint->y; return (*this); }
    Point& operator-=(const Point* pNewPoint) { x -= pNewPoint->x; y -= pNewPoint->y; return (*this); }
    Point operator+(const Point& other) { Point temp(this); temp += other; return temp; }
    Point operator-(const Point& other) { Point temp(this); temp -= other; return temp; }

    // comparison
    bool operator==(const Point& other) const { return ((x == other.x) && (y == other.y)); }
    bool operator!=(const Point& other) const { return (!((x == other.x) && (y == other.y))); }

    // accessors (needed for Lua)
    long GetX() const { return x; }
    long GetY() const { return y; }
    void SetX(const long newX) { x = newX; }
    void SetY(const long newY) { y = newY; }
    void Set(const long newX, const long newY) { x = newX; y = newY; }

    // somewhat hacky vector emulation (maybe I should just write my own vector class)
    float Length() const { return sqrt((float)(x*x+y*y)); }
};

inline Point operator-(const Point& left, const Point& right) { Point temp(left); temp -= right; return temp; }


//---------------------------------------------------------------------------------------------------------------------
// This class represents a rectangle
//---------------------------------------------------------------------------------------------------------------------
class Rect
{
public:
    long top, left, right, bottom;

    enum RectCorner { INVALID_CORNER = 0, TOPLEFT_CORNER, TOPRIGHT_CORNER, BOTTOMLEFT_CORNER, BOTTOMRIGHT_CORNER };

    //construction
    Rect(void) { left = top = right = bottom = 0; }
    Rect(long newLeft, long newTop, long newRight, long newBottom) { Set(newLeft,newTop,newRight,newBottom); }
    Rect(const Rect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }
    Rect(Rect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; }
    Rect(const Point& topLeft, const Point& bottomRight) { top = topLeft.y; left = topLeft.x; right = bottomRight.x; bottom = bottomRight.y; }
//    Rect(const RECT& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }

    //assignment
    Rect& operator=(const Rect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; return (*this); }
    Rect& operator=(const Rect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; return (*this); }

    // addition/subtraction
    Rect& operator+=(const Rect& newRect) { left += newRect.left; top += newRect.top; right += newRect.right; bottom += newRect.bottom; return (*this); }
    Rect& operator-=(const Rect& newRect) { left -= newRect.left; top -= newRect.top; right -= newRect.right; bottom -= newRect.bottom; return (*this); }
    Rect& operator+=(const Rect* pNewRect) { left += pNewRect->left; top += pNewRect->top; right += pNewRect->right; bottom += pNewRect->bottom; return (*this); }
    Rect& operator-=(const Rect* pNewRect) { left -= pNewRect->left; top -= pNewRect->top; right -= pNewRect->right; bottom -= pNewRect->bottom; return (*this); }
    Rect operator+(Rect& other) { Rect temp(this); temp += other; return temp; }
    Rect operator-(Rect& other) { Rect temp(this); temp -= other; return temp; }

    // move a rectangle by an amount defined by a Point, keeping the rectanle's size constant
    Rect& operator+=(const Point& delta) { left += delta.x; top += delta.y; right += delta.x; bottom += delta.y; return (*this); }
    Rect& operator-=(const Point& delta) { left -= delta.x; top -= delta.y; right -= delta.x; bottom -= delta.y; return (*this); }
    Rect& operator+=(const Point* pDelta) { left += pDelta->x; top += pDelta->y; right += pDelta->x; bottom += pDelta->y; return (*this); }
    Rect& operator-=(const Point* pDelta) { left -= pDelta->x; top -= pDelta->y; right -= pDelta->x; bottom -= pDelta->y; return (*this); }
    Rect operator+(Point& delta) { Rect temp(this); temp += delta; return temp; }
    Rect operator-(Point& delta) { Rect temp(this); temp -= delta; return temp; }

    //comparison
    bool operator==(const Rect& other) const { return ((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom)); }
    bool operator!=(const Rect& other) const { return (!((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom))); }

    // collision
    bool Collide(const Rect& other) const { if (other.bottom < top || other.right < left || other.left > right || other.top > bottom) {return false;} else {return true;} }
    bool Collide(const Point& p) const { if (p.x > left && p.x < right && p.y < bottom && p.y > top) {return true;} else {return false;} }
    bool IsWithin(const Rect& other) const { return ( (left >= other.left && top >= other.top && right <= other.right && bottom <= other.bottom) || (other.left >= left && other.top >= top && other.right <= right && other.bottom <= bottom) ); }
    bool IsWithin(const Point& other) const { return (other.x >= left && other.x <= right && other.y >= top && other.y <= bottom); }
    bool IsValid(void) const { return (left <= right && top <= bottom); }
    bool IsNull(void) const { return (left == 0 && right == 0 && top == 0 && bottom == 0); }

    // convenience functions
    void ShiftX(int dx) { left += dx; right += dx; }
    void ShiftY(int dy) { top += dy; bottom += dy; }
    void SetX(int x) { int dx = x - left; ShiftX(dx); }
    void SetY(int y) { int dy = y - top; ShiftY(dy); }

    // accessors for Lua
    long GetTop(void) const { return top; }
    long GetLeft(void) const { return left; }
    long GetRight(void) const { return right; }
    long GetBottom(void) const { return bottom; }
    Point GetCenter(void) const
    {
        if (IsValid())
            return (Point(left + ((right - left) / 2), top + ((bottom - top) / 2)));
//        GCC_ERROR("Attempting to get the center of an invalid Rect");
        return Point();
    }
    Point TopLeft(void) const { return Point(left, top); }
    Point TopRight(void) const { return Point(right, top); }
    Point BottomLeft(void) const { return Point(left, bottom); }
    Point BottomRight(void) const { return Point(right, bottom); }
    long GetWidth(void) const { return right - left; }
    long GetHeight(void) const { return bottom - top; }
    void Set(long newLeft, long newTop, long newRight, long newBottom) { left = newLeft; top = newTop; right = newRight; bottom = newBottom; }
    void MoveDelta(long dx, long dy) { left += dx; top += dy; right += dx; bottom += dy; }
    void MoveDelta(const Point deltaPoint) { MoveDelta(deltaPoint.x, deltaPoint.y); }
    void MoveTo(long x, long y)
    {
        long width = right - left;
        long height = bottom - top;
        left = x;
        right = left + width;
        top = y;
        bottom = top + height;
    }
    void MoveTo(const Point& point) { MoveTo(point.x, point.y); }
};

#endif // TYPES_H
