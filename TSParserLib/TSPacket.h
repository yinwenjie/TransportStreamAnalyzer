#ifndef _TS_PACKET_H_
#define _TS_PACKET_H_

typedef struct Adaptation_field
{
	UINT8 adaptation_field_length;
	bool discontinuity_indicator;
	bool random_access_indicator;
	bool elementary_stream_priority_indicator;
	bool PCR_flag;
	bool OPCR_flag;
	bool splicing_point_flag;
	bool transport_private_data_flag;
	bool adaptation_field_extension_flag;
} Adaptation_field;

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

	Adaptation_field adaption_field;

private:
	BYTE*	payload_start;
	UINT8	payload_length;

	int parse_adaptation_field(BYTE *buffer);
	void dump_adaptation_field();
};

#endif
