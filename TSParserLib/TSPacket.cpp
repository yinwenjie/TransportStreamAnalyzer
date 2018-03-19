#include "include.h"
#include "TSParserLib.h"
#include "TSPacket.h"

using namespace std;

CTransportStreamPacket::CTransportStreamPacket()
{
}

CTransportStreamPacket::~CTransportStreamPacket()
{
}

int CTransportStreamPacket::Parse_one_ts_packet()
{
	sync_byte = read_byte();
	if (sync_byte != 0x47)
	{
		return kTSParserError_PacketNotStartWith47;
	}

	UINT16 temp16 = read_duo_byte();// buffer[1] + buffer[2] << 8;
	transport_error_indicator = (temp16 >> 15) & 1;
	payload_unit_start_indicator = (temp16 >> 14) & 1;
	transport_priority = (temp16 >> 13) & 1;
	PID = temp16 & 0x1FFF;

	BYTE temp8 = read_byte();
	trasport_scrambling_control = (temp8 >> 6) & 3;
	adaption_field_control = (temp8 >> 4) & 3;
	continuity_counter = temp8 & 15;

	if (adaption_field_control == 2 || adaption_field_control == 3)
	{
		parse_adaptation_field();
	}

	if (adaption_field_control == 1)
	{
		if (PID == 0)
		{
			parse_prog_associ_table();
		}

		if (pmt_id == PID)
		{
			parse_prog_map_table();
		}
	}

	return kTSParserError_NoError;
}


void CTransportStreamPacket::Dump_packet_info(int pkt_idx)
{
#if DUMP_PACKET_INFO_ENABLED_CONSOLE
	cout << "-----------------------------------" << endl;
	cout << "Packet index: " << to_string(pkt_idx) << endl;
	cout << "sync_byte = " << to_string(sync_byte) << endl;
	cout << "transport_error_indicator = " << to_string(transport_error_indicator) << endl;
	cout << "payload_unit_start_indicator = " << to_string(payload_unit_start_indicator) << endl;
	cout << "transport_priority = " << to_string(transport_priority) << endl;
	cout << "PID = " << to_string(PID) << endl;
	cout << "trasport_scrambling_control = " << to_string(trasport_scrambling_control) << endl;
	cout << "adaption_field_control = " << to_string(adaption_field_control) << endl;
	cout << "continuity_counter = " << to_string(continuity_counter) << endl;
#endif

#if DUMP_TS_INFO_ENABLED_LOG
	g_logoutFile << "-----------------------------------" << endl;
	g_logoutFile << "Packet index: " << to_string(pkt_idx) << endl;
	g_logoutFile << "sync_byte = " << to_string(sync_byte) << endl;
	g_logoutFile << "transport_error_indicator = " << to_string(transport_error_indicator) << endl;
	g_logoutFile << "payload_unit_start_indicator = " << to_string(payload_unit_start_indicator) << endl;
	g_logoutFile << "transport_priority = " << to_string(transport_priority) << endl;
	g_logoutFile << "PID = " << to_string(PID) << endl;
	g_logoutFile << "trasport_scrambling_control = " << to_string(trasport_scrambling_control) << endl;
	g_logoutFile << "adaption_field_control = " << to_string(adaption_field_control) << endl;
	g_logoutFile << "continuity_counter = " << to_string(continuity_counter) << endl;
	if (adaption_field_control == 2 || adaption_field_control == 3)
	{
		dump_adaptation_field();
	}
	if (adaption_field_control == 1)
	{
		if (PID == 0)
		{
			dump_prog_associ_table();
		}

		if (pmt_id == PID)
		{
			dump_prog_map_table();
		}
	}
#endif
}

int CTransportStreamPacket::parse_adaptation_field()
{
	adaption_field.adaptation_field_length = read_byte();
	if (adaption_field.adaptation_field_length > 0)
	{
		BYTE temp8 = read_byte();
		adaption_field.discontinuity_indicator = temp8 & (1 << 0);
		adaption_field.random_access_indicator = temp8 & (1 << 1);
		adaption_field.elementary_stream_priority_indicator = temp8 & (1 << 2);
		adaption_field.PCR_flag = temp8 & (1 << 3);
		adaption_field.OPCR_flag = temp8 & (1 << 4);
		adaption_field.splicing_point_flag = temp8 & (1 << 5);
		adaption_field.transport_private_data_flag = temp8 & (1 << 6);
		adaption_field.adaptation_field_extension_flag = temp8 & (1 << 7);
	}

	return kTSParserError_NoError;
}

void CTransportStreamPacket::dump_adaptation_field()
{
	g_logoutFile << "adaptation_field_length: " << to_string(adaption_field.adaptation_field_length) << endl;
	if (adaption_field.adaptation_field_length > 0)
	{
		g_logoutFile << "discontinuity_indicator: " << to_string(adaption_field.discontinuity_indicator) << endl;
		g_logoutFile << "random_access_indicator: " << to_string(adaption_field.random_access_indicator) << endl;
		g_logoutFile << "elementary_stream_priority_indicator: " << to_string(adaption_field.elementary_stream_priority_indicator) << endl;
		g_logoutFile << "PCR_flag: " << to_string(adaption_field.PCR_flag) << endl;
		g_logoutFile << "OPCR_flag: " << to_string(adaption_field.OPCR_flag) << endl;
		g_logoutFile << "splicing_point_flag: " << to_string(adaption_field.splicing_point_flag) << endl;
		g_logoutFile << "transport_private_data_flag: " << to_string(adaption_field.transport_private_data_flag) << endl;
		g_logoutFile << "adaptation_field_extension_flag: " << to_string(adaption_field.adaptation_field_extension_flag) << endl;
	}
}

int CTransportStreamPacket::parse_prog_associ_table()
{
	BYTE temp8 = 0;
	UINT16 temp16 = 0;
	if (payload_unit_start_indicator)
	{
		read_byte();//pointer_field;
	}
	pat.table_id = read_byte();
	temp16 = read_duo_byte();
	pat.section_syntax_indicator = (temp16 >> 15) & 0x01;
	pat.section_length = temp16 & 0x0FFF;
	pat.transport_stream_id = read_duo_byte();

	temp8 = read_byte();
	pat.version_number = (temp8 >> 1) & 0x1F;
	pat.current_next_indicator = temp8 & 0x01;
	pat.section_number = read_byte();
	pat.last_section_number = read_byte();

	int N = (pat.section_length - 9) / 4;
	pat.program_number = (UINT16*)malloc(N * sizeof(UINT16));
	pat.PID_type = (bool*)malloc(N * sizeof(bool));
	pat.PID_array = (UINT16*)malloc(N * sizeof(UINT16));

	for (int idx = 0; idx < N; idx++)
	{
		pat.program_number[idx] = read_duo_byte();
		if (pat.program_number[idx] == 0)
		{
			pat.PID_type[idx] = 0;
		} 
		else
		{
			pat.PID_type[idx] = 1;
		}
		pat.PID_array[idx] = read_duo_byte() & 0x1FFF;
		if (pat.PID_type[idx] == 1)
		{
			pmt_id = pat.PID_array[idx];
		}
	}

	return kTSParserError_NoError;
}

void CTransportStreamPacket::dump_prog_associ_table()
{
	g_logoutFile << "PAT:table_id = " << to_string(pat.table_id) << endl;
	g_logoutFile << "PAT:section_syntax_indicator = " << to_string(pat.section_syntax_indicator) << endl;
	g_logoutFile << "PAT:section_length = " << to_string(pat.section_length) << endl;
	g_logoutFile << "PAT:transport_stream_id = " << to_string(pat.transport_stream_id) << endl;
	g_logoutFile << "PAT:version_number = " << to_string(pat.version_number) << endl;
	g_logoutFile << "PAT:current_next_indicator = " << to_string(pat.current_next_indicator) << endl;
	g_logoutFile << "PAT:section_number = " << to_string(pat.section_number) << endl;
	g_logoutFile << "PAT:last_section_number = " << to_string(pat.last_section_number) << endl;
	int N = (pat.section_length - 9) / 4;
	for (int idx = 0; idx < N; idx++)
	{
		g_logoutFile << "PAT:program_number = " << to_string(pat.program_number[idx]) << endl;
		g_logoutFile << (pat.PID_type[idx] ? "PAT:program_map_PID = " : "PAT:network_PID = ") << to_string(pat.PID_array[idx]) << endl;
	}
}

int CTransportStreamPacket::parse_prog_map_table()
{
	BYTE temp8 = 0;
	UINT16 temp16 = 0;
	if (payload_unit_start_indicator)
	{
		read_byte();//pointer_field;
	}

	pmt.table_id = read_byte();
	temp16 = read_duo_byte();
	pmt.section_syntax_indicator = (temp16 >> 15) & 0x0001;
	pmt.section_length = temp16 & 0x0FFF;

	pmt.program_number = read_duo_byte();
	temp8 = read_byte();
	pmt.version_number = (temp8 >> 1) & 0x1F;
	pmt.current_next_indicator = temp8 & 0x01;

	pmt.section_number = read_byte();
	pmt.last_section_number = read_byte();
	temp16 = read_duo_byte();
	pmt.PCR_PID = temp16 & 0x1FFF;
	temp16 = read_duo_byte();
	pmt.program_info_length = temp16 & 0x0FFF;

	skip_bytes(pmt.program_info_length);
	int len_rem = pmt.section_length - 13 - pmt.program_info_length, idx = 0;
	while (len_rem > 0)
	{
		pmt.stream_type[idx] = read_byte();
		pmt.elementary_PID[idx] = read_duo_byte() & 0x1FFF;
		pmt.ES_info_length[idx] = read_duo_byte() & 0x0FFF;
		len_rem -= (pmt.ES_info_length[idx] + 5);
		idx++;
	}
	pmt.ES_Count = idx;


	return kTSParserError_NoError;
}

void CTransportStreamPacket::dump_prog_map_table()
{
	g_logoutFile << "PMT:table_id = " << to_string(pmt.table_id) << endl;
	g_logoutFile << "PMT:section_syntax_indicator = " << to_string(pmt.section_syntax_indicator) << endl;
	g_logoutFile << "PMT:section_length = " << to_string(pmt.section_length) << endl;
	g_logoutFile << "PMT:program_number = " << to_string(pmt.program_number) << endl;
	g_logoutFile << "PMT:version_number = " << to_string(pmt.version_number) << endl;
	g_logoutFile << "PMT:current_next_indicator = " << to_string(pmt.current_next_indicator) << endl;
	g_logoutFile << "PMT:section_number = " << to_string(pmt.section_number) << endl;
	g_logoutFile << "PMT:last_section_number = " << to_string(pmt.last_section_number) << endl;
	g_logoutFile << "PMT:PCR_PID = " << to_string(pmt.PCR_PID) << endl;
	g_logoutFile << "PMT:program_info_length = " << to_string(pmt.program_info_length) << endl;

	for (int idx = 0; idx < pmt.ES_Count; idx++)
	{
		g_logoutFile << to_string(idx) << "-PMT:stream_type = " << to_string(pmt.stream_type[idx]) << endl;
		g_logoutFile << to_string(idx) << "-PMT:elementary_PID = " << to_string(pmt.elementary_PID[idx]) << endl;
		g_logoutFile << to_string(idx) << "-PMT:ES_info_length = " << to_string(pmt.ES_info_length[idx]) << endl;
	}
}
