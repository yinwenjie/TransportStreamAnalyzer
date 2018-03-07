#include <stdio.h>
#include "TSParserLib.h"

int main(int argc, char **argv)
{
	CTransportStreamParser ts_parser(argv[1]);

	ts_parser.Parse();

	return 0;
}