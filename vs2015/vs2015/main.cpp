#include <stdio.h>
#include "TSParserLib.h"

int main(int argc, char **argv)
{
	CTransportStreamParser ts_parser(argv[1]);
	return 0;
}