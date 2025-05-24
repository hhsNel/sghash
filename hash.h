#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>
#include <string.h>

#include "brush.h"

union hash {
	unsigned char bytes[64];
	struct brush_command cmds[64 / sizeof(struct brush_command)];
};

union hash make_hash(char *str);

union hash make_hash(char *str) {
	union hash hash;

	SHA512((const unsigned char *)str, strlen(str), hash.bytes);
	return hash;
}

#endif

