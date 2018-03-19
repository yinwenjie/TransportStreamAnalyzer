#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "DataTypes.h"

typedef struct File_context
{
	BYTE *file_buf;
	int	byte_position;
} File_context;

extern File_context f_ctx;

void reset_ctx(BYTE *buf);
void skip_bytes(int len);
BYTE read_byte();
UINT16 read_duo_byte();

#endif
