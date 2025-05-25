#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>
#include <string.h>

#include "brush.h"

union hash {
	unsigned char bytes[64];
	brush_command cmds[64 / sizeof(brush_command)];
};

union hash make_hash(char *str);
union hash make_hash_stdin();

union hash make_hash(char *str) {
	union hash hash;

	SHA512((const unsigned char *)str, strlen(str), hash.bytes);
	return hash;
}

union hash make_hash_stdin() {
	SHA512_CTX ctx;
	unsigned char buff[4096];
	size_t bytes;
	union hash hash;

	SHA512_Init(&ctx);
	
	while((bytes = fread(buff, 1, sizeof(buff), stdin)) > 0) {
		SHA512_Update(&ctx, buff, bytes);
	}

	SHA512_Final(hash.bytes, &ctx);
	return hash;
}

#endif

