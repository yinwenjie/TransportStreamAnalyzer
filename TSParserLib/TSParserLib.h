
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

// TS parsing error codes...
const int kTSParserError_NoError = 0;
const int kTSParserError_NullFileName = -1;
const int kTSParserError_OpenInputFileFailed = -2;
const int kTSParserError_ReadInputFileFailed = -3;
const int kTSParserError_PacketNotStartWith47 = -4;
const int kTSParserError_PESNotStartWith001 = -5;

class TSPARSERLIB_API CTransportStreamParser
{
public:
	CTransportStreamParser(const char *file_name);
	~CTransportStreamParser();
	
	// Parse TS file...
	int Parse();
	void Dump_ts_info();
private:
	const char*	m_file_name;
	UINT64		m_file_size;
	BYTE*		m_file_buf;
	UINT32		m_packet_count;

	std::ifstream m_in_file_stream;	// input file stream...
	int		open_input_file();		// open input file by file name...
	int     get_input_file_data();	// read input file data to buffer...
	void	close_input_file();		// close input file...
};