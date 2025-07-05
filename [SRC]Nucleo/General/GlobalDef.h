#pragma warning( disable : 4996)
/***********************************************/

#define DEF_DEBUG

/***********************************************/

//Sockets
#define WM_USER_TIMERSIGNAL			WM_USER + 50
#define WM_USER_ACCEPT				WM_USER + 100 //Port 2828 messages
#define WM_ONMAPSERVERSOCKETEVENT	WM_USER + 200
#define WM_ONCLIENTSOCKETEVENT		WM_ONMAPSERVERSOCKETEVENT + DEF_MAXCLIENTS + 1

#define DEF_USE_ENCRYPTION			0

#define DEF_UPPERVERSION			111111111
#define DEF_LOWERVERSION			333333333
#define DEF_ARGENTVERSION			666666666

/* LOCAL DEFINITIONS */
#define DEF_MSGACCOUNTSET_NULL		0
#define DEF_MSGACCOUNTSET_INIT		1
#define DEF_MSGACCOUNTSET_WAIT		2
#define DEF_MSGACCOUNTSET_PLAYING	3

/* GENERALS */
#define DEF_MSGTYPE_CONFIRM								0x0F14
#define DEF_MSGTYPE_REJECT								0x0F15
