#pragma once

#include <zlib.h>

class Compressor{
public:
	static int compress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed);
};

