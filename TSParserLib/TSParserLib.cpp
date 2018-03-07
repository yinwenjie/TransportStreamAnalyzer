#include "TSParserLib.h"
#include "TSPacket.h"

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

//*************** Public API ***************
int CTransportStreamParser::Parse()
{
	int err = open_input_file();
	if (err < 0)
	{
		return err;
	}

	err = get_input_file_data();
	if (err < 0)
	{
		return err;
	}

	for (int idx = 0; idx < m_packet_count; idx++)
	{
		BYTE *pkt_buf = m_file_buf + 188 * idx;
		CTransportStreamPacket pkt;
		pkt.Parse_one_ts_packet(pkt_buf);
	}

	return kTSParserError_NoError;
}

//*************** Private Methods ***************
int CTransportStreamParser::open_input_file()
{
	if (!m_file_name)
	{
		return kTSParserError_NullFileName;
	}

	m_in_file_stream.open(m_file_name, ios_base::binary);
	if (!m_in_file_stream.is_open())
	{
		return kTSParserError_OpenInputFileFailed;
	}

	return kTSParserError_NoError;
}

int CTransportStreamParser::get_input_file_data()
{
	m_in_file_stream.seekg(0, ios_base::end);
	m_file_size = m_in_file_stream.tellg();
	m_in_file_stream.seekg(0, ios_base::beg);
	m_packet_count = m_file_size / 188;

	m_file_buf = new BYTE[m_file_size];
	m_in_file_stream.read(reinterpret_cast<char *>(m_file_buf), m_file_size);
	if ((m_in_file_stream.rdstate() & ifstream::failbit) || (m_in_file_stream.rdstate() & ifstream::badbit))
	{
		close_input_file();
		return kTSParserError_ReadInputFileFailed;
	}

	return kTSParserError_NoError;
}

void CTransportStreamParser::close_input_file()
{
	if (m_in_file_stream.is_open())
	{
		m_in_file_stream.close();
	}
}

