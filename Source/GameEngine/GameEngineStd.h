


//C Runtime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>


#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

#if 0
class GCC_noncopyable
{
private:
    GCC_noncopyable(const GCC_noncopyable& x);
    GCC_noncopyable& operator =(const GCC_noncopyable& x);
public:
    GCC_noncopyable(){}
};
#endif

// Game Code Complete - Chapter 12, page 446-447
#if defined(_DEBUG)
#	define GCC_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define GCC_NEW new
#endif


typedef unsigned long long  DWORDLONG;
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;

#include <QMessageBox>



// GameCode #includes
//#include "../Debugging/logger.h"  // this should be the first of the gcc includes since it defines GCC_ASSERT()
//#include "../Utilities/types.h"
//#include "../Utilities/templates.h"
//#include "../Graphics3D/geometry.h"

typedef QVector4D Color;

extern Color g_White;
extern Color g_Black;
extern Color g_Cyan;
extern Color g_Red;
extern Color g_Green;
extern Color g_Blue;
extern Color g_Yellow;
extern Color g_Gray40;
extern Color g_Gray25;
extern Color g_Gray65;
extern Color g_Transparent;

//extern Vec3 g_Up;
//extern Vec3 g_Right;
//extern Vec3 g_Forward;

//extern Vec4 g_Up4;
//extern Vec4 g_Right4;
//extern Vec4 g_Forward4;

#include "interfaces.h"


// Useful #defines

extern const float fOPAQUE;
extern const int iOPAQUE;
extern const float fTRANSPARENT;
extern const int iTRANSPARENT;

extern const int MEGABYTE;
extern const float SIXTY_HERTZ;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

#if !defined(SAFE_DELETE)
    #define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
    #define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL;
#endif

#if !defined(SAFE_RELEASE)
    #define SAFE_RELEASE(x) if(x) x->release(); x=NULL;
#endif

#ifdef UNICODE
    #define _tcssprintf wsprintf
    #define tcsplitpath _wsplitpath
#else
    #define _tcssprintf sprintf
    #define tcsplitpath _splitpath
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

//#include "../GameEngineMain/gameengineapp.h"

//extern int GameEngine(int argc, char *argv[]);
