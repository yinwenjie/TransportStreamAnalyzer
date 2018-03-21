#ifndef _TS_PACKET_H_
#define _TS_PACKET_H_

#define MAX_ES_INFO_IN_PMT	64

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

typedef struct Program_Map_Section
{
	UINT8	table_id;
	bool	section_syntax_indicator;
	UINT16	section_length;
	UINT16	program_number;
	UINT8	version_number;
	bool	current_next_indicator;
	UINT8	section_number;
	UINT8	last_section_number;
	UINT16	PCR_PID;
	UINT16	program_info_length;

	int		ES_Count;
	UINT8	stream_type[MAX_ES_INFO_IN_PMT];
	UINT16	elementary_PID[MAX_ES_INFO_IN_PMT];
	UINT16	ES_info_length[MAX_ES_INFO_IN_PMT];
};

enum PES_Stream_Type
{
	PES_Unknown = 0,
	PES_Video = 1,
	PES_Audio = 2,
};

typedef struct PES_Packet
{
	UINT8	stream_id;
	UINT16	PES_packet_length;
	UINT8	stream_type;
	UINT8	PES_scrambling_control;
	bool	PES_priority;
	bool	data_alignment_indicator;
	bool	copyright;
	bool	original_or_copy;
	UINT8	PTS_DTS_flags;
	bool	ESCR_flag;
	bool	ES_rate_flag;
	bool	DSM_trick_mode_flag;
	bool	additional_copy_info_flag;
	bool	PES_CRC_flag;
	bool	PES_extension_flag;
	UINT8	PES_header_data_length;
	UINT8	PTS_arr[5];
	UINT64	PTS;
	UINT8	DTS_arr[5];
	UINT64	DTS;
} PES_Packet;

class CTransportStreamPacket
{
public:
	CTransportStreamPacket();
	~CTransportStreamPacket();

	int		Parse_one_ts_packet();
	void	Dump_packet_info(int pkt_idx);

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
	Program_Map_Section pmt;
	UINT16	pmt_id;

	PES_Packet	pes;

private:
	BYTE*	payload_start;
	UINT8	payload_length;

	int		parse_adaptation_field();
	void	dump_adaptation_field();

	int		parse_prog_associ_table();
	void	dump_prog_associ_table();

	int		parse_prog_map_table();
	void	dump_prog_map_table();

	bool	is_PES_packet();
	int		parse_PES_packet();
	void	dump_PES_packet();
};

#endif
