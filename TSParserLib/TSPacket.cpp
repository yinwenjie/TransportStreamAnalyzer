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

int CTransportStreamPacket::Parse_one_ts_packet(BYTE *buffer)
{
	sync_byte = buffer[0];
	if (sync_byte != 0x47)
	{
		return kTSParserError_PacketNotStartWith47;
	}

	UINT16 temp = buffer[1] + buffer[2] << 8;
	transport_error_indicator = (temp >> 15) & 1;
	payload_unit_start_indicator = (temp >> 14) & 1;
	transport_priority = (temp >> 13) & 1;
	PID = temp & 0x1FFF;

	trasport_scrambling_control = (buffer[4] >> 6) & 3;
	adaption_field_control = (buffer[4] >> 4) & 3;
	continuity_counter = buffer[4] & 15;

	if (adaption_field_control == 2 || adaption_field_control == 3)
	{
		parse_adaptation_field(buffer + 5);
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
#endif
}

int CTransportStreamPacket::parse_adaptation_field(BYTE *buffer)
{
	adaption_field.adaptation_field_length = buffer[0];
	if (adaption_field.adaptation_field_length > 0)
	{
		adaption_field.discontinuity_indicator = buffer[1] & (1 << 0);
		adaption_field.random_access_indicator = buffer[1] & (1 << 1);
		adaption_field.elementary_stream_priority_indicator = buffer[1] & (1 << 2);
		adaption_field.PCR_flag = buffer[1] & (1 << 3);
		adaption_field.OPCR_flag = buffer[1] & (1 << 4);
		adaption_field.splicing_point_flag = buffer[1] & (1 << 5);
		adaption_field.transport_private_data_flag = buffer[1] & (1 << 6);
		adaption_field.adaptation_field_extension_flag = buffer[1] & (1 << 7);
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
