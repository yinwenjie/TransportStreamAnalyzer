#include "include.h"
#include "TSParserLib.h"
#include "TSPacket.h"

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
	transport_priority = buffer[4] & 3;

	return kTSParserError_NoError;
}
