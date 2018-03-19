#include "Memory.h"

File_context f_ctx;

void reset_ctx(BYTE *buf)
{
	f_ctx.file_buf = buf;
	f_ctx.byte_position = 0;
}

void skip_bytes(int len)
{
	f_ctx.byte_position += len;
}

BYTE read_byte()
{
	return f_ctx.file_buf[f_ctx.byte_position++];
}

UINT16 read_duo_byte()
{
	UINT16 val = 0;
	val = (read_byte() << 8) + read_byte();
	return val;
}
