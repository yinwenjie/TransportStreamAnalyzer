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

typedef struct Program_Association_Section
{
	UINT8	table_id;
	bool	section_syntax_indicator;
	UINT16	section_length;
	UINT16	transport_stream_id;
	UINT8	version_number;
	bool	current_next_indicator;
	UINT8	section_number;
	UINT8	last_section_number;
	UINT16	*program_number;
	bool	*PID_type;
	UINT16	*PID_array;
} Program_Association_Section;

class CTransportStreamPacket
{
public:
	CTransportStreamPacket();
	~CTransportStreamPacket();

	int Parse_one_ts_packet();
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
	Program_Association_Section pat;

private:
	BYTE*	payload_start;
	UINT8	payload_length;

	int parse_adaptation_field();
	void dump_adaptation_field();

	int parse_prog_associ_table();
	void dump_prog_associ_table();
};

#endif
