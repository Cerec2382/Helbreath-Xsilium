
#include <windows.h>
#include <string>
#include <Tlhelp32.h>
#include <fstream>
#include <iostream>
#include <iosfwd>
#include <vector>
#include <sstream>

enum Side
{
	OBJNEUTRAL,
	OBJARESDEN,
	OBJELVINE
};


#define _ms					_s / 1000
#define _s					* CLOCKS_PER_SEC
#define _m					_s * 60
#define _h					_m * 60
#define _d					_h * 24

#define MAXRANKS 31 //21
#define C800x600 1
#define C640x480 2
#define C1024x768 3

using namespace std;
typedef unsigned long long u64;
typedef signed long long i64;
typedef unsigned long u32;
typedef signed long i32;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned char u8;
typedef signed char i8;

template <typename T, class = typename enable_if<!is_pointer<T>::value>::type >
static void Push(char * &cp, T value) {
	auto p = (T *)cp;
	*p = (T)value;
	cp += sizeof(T);
}

template <typename T, class = typename enable_if<!is_pointer<T>::value>::type >
static void Pop(char * &cp, T &v) {
	T * p = (T *)cp;
	v = *p;
	cp += sizeof(T);
}

static void Push(char * &dest, const char * src, u32 len) {
	memcpy(dest, src, len);
	dest += len;
}

static void Push(char * &dest, const char * src) {

	strcpy(dest, src);
	dest += strlen(src) + 1;
}

static void Push(char * &dest, const string & str) {
	strcpy(dest, str.c_str());
	dest += str.length() + 1;
}

static void Pop(char * &src, char * dest, u32 len) {
	memcpy(dest, src, len);
	src += len;
}
static void Pop(char * &src, char * dest) {

	u32 len = strlen(src) + 1;
	memcpy(dest, src, len);
	src += len;
}

static void Pop(char * &src, string & str) {
	str = src;
	src += str.length() + 1;
}

//HeatoN events
enum class EventMsg : u16
{
	Status,
	Enter,
};

enum class EventID : u8
{
	Deathmatch,
	GunGameV2,
	Max
};

static char * _eventName[(int)EventID::Max] =
{
	"Deathmatch",
	"GunGame"	
};

static char * _eventNameShort[(int)EventID::Max] =
{
	"dm",
	"gungame"
};

static char * getEventName(EventID id)
{
	return _eventName[(int)id];
}

static char * getEventNameShort(EventID id)
{
	return _eventNameShort[(int)id];
}

static EventID _tpEvents[] =
{
	EventID::Deathmatch,
	EventID::GunGameV2,
};

enum ObjectAction
{
	ACTION_OBJECTSTOP,
	ACTION_OBJECTMOVE,
	ACTION_OBJECTRUN,
	ACTION_OBJECTATTACK,
	ACTION_OBJECTMAGIC,
	ACTION_OBJECTGETITEM,
	ACTION_OBJECTDAMAGE,
	ACTION_OBJECTDAMAGEMOVE,
	ACTION_OBJECTATTACKMOVE,
	ACTION_OBJECTDYING,
	ACTION_OBJECTNULLACTION,
	ACTION_OBJECTDEAD
};