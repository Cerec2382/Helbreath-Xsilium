//HELBREATH ARGENTINA LALO RAMOS Cliente 13
//#define DEF_TESTOFICIAL
//#define DEF_LOCAL
#define DEF_OFICIAL

#define DEF_SECURITY_LOGIN		834759873
#define DEF_ANTI_HACKERS		872346823
#define DEF_USING_LAUNCHER      0 // 1 -> ON // 0 -> OFF
#ifdef DEF_OFICIAL
#define DEF_SERVER_IP	 		"192.3.146.151" //"135.148.120.176" //"192.168.0.121"
#define DEF_SERVER_PORT			9735
#define NAME_WORLDNAME1			"SEB"
#define MSG_WORLDNAME1			"Helbreath Xsilium"
#define MSG_HOMEPAGE			"https://github.com/Cerec2382/Xsilium/"
#define MSG_FORUMPAGE			"https://github.com/Cerec2382/Xsilium/"
#define MSG_CREATEACC			"https://github.com/Cerec2382/Xsilium/"
#define MSG_CHANGEPASS			"https://github.com/Cerec2382/Xsilium/"

#define DEF_UPPERVERSION			111111111
#define DEF_LOWERVERSION			333333333
#define DEF_ARGENTVERSION			666666666

#define DEF_STATS_LIMIT         200
#define DEF_UPPERVERSION_FANTASMA    14
#define DEF_LOWERVERSION_FANTASMA	 1
#define DEF_LOWERVERSION_FANTASMA2	 2
#define DEF_SMALLBAR			0 // 0 Barra grande - 1 Barra chica
//#define DEF_USE_DNS
#define DEF_SERVER_DNS 
//#define DEF_USE_LOGIN			// Login.cfg option
//#define  _DEBUGS 
#endif
//#define DEF_USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAXDIALOG				1
//#define DEF_SELECTSERVER		// Show/Hides server selection screen. (Activar para Proxys)

// #define DEF_COMMA_GOLD		// Allows to show comma nbe as original HB (ie: 1,200,000)
// #define DEF_4XXP_PER_LVL		// limits xp niv 140, then 4 x XP per lvl (affects XP shown as % on status)
            		
//#define DEF_MIXEDTORNER// listo le cambie eso por hacksDEF_WINDOWED_MODE		// Shows HB in a windowed mode (for debug purpose only, sprite will bug....) 
#define DEF_MSGTYPE_CONFIRM								0x0F14
#define DEF_MSGTYPE_REJECT								0x0F15

/*** Special conditions   **/
#ifdef _DEBUGS
	#define DEF_MIXEDTORNER //DEF_WINDOWED_MODE es este c
#endif
