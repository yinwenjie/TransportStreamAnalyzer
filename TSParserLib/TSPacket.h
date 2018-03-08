#ifndef _TS_PACKET_H_
#define _TS_PACKET_H_

class CTransportStreamPacket
{
public:
	CTransportStreamPacket();
	~CTransportStreamPacket();

	int Parse_one_ts_packet(BYTE *buffer);
	void Dump_packet_info(int pkt_idx);

	UINT8	sync_byte;
	bool	transport_error_indicator;
	bool	payload_unit_start_indicator;
	bool	transport_priority;
	UINT16	PID;
	UINT8	trasport_scrambling_control;
	UINT8	adaption_field_control;
	UINT8	continuity_counter;

private:
	BYTE*	payload_start;
	UINT8	payload_length;
};

#endif
