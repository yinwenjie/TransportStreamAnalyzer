
#ifdef _MSC_VER
#ifdef FLVPARSERLIB_EXPORTS
#define TSPARSERLIB_API __declspec(dllexport)
#else
#define TSPARSERLIB_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define TSPARSERLIB_API  
#endif

class TSPARSERLIB_API CTransportStreamParser
{
public:
	CTransportStreamParser(const char *fileName);
	~CTransportStreamParser();
};