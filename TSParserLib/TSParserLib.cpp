#include "TSParserLib.h"

using namespace std;

CTransportStreamParser::CTransportStreamParser(const char *file_name)
{
	if (file_name)
	{
		m_file_name = file_name;
	}

	m_file_size = 0;
}

CTransportStreamParser::~CTransportStreamParser()
{

}

int CTransportStreamParser::Parse()
{
	return -1;
}

int CTransportStreamParser::open_input_file()
{
	if (!m_file_name)
	{
		return -1;
	}

	m_inFileStream.open(m_file_name, ios_base::binary);
	if (!m_inFileStream.is_open())
	{
		return -1;
	}

}

