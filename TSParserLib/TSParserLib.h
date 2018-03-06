
#ifdef _MSC_VER
#ifdef TSPARSERLIB_EXPORTS
#define TSPARSERLIB_API __declspec(dllexport)
#else
#define TSPARSERLIB_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define TSPARSERLIB_API  
#endif

#include "include.h"

class TSPARSERLIB_API CTransportStreamParser
{
public:
	CTransportStreamParser(const char *file_name);
	~CTransportStreamParser();
	
	// Parse FLV file...
	int Parse();
private:
	const char *m_file_name;
	UINT64		m_file_size;

	std::ifstream m_inFileStream;	// input file stream...
	int		open_input_file();		// open input file by file name...
};