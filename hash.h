#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>
#include <openssl/evp.h>
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
	unsigned char buff[4096];
	size_t bytes;
	union hash hash;
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	unsigned int hash_len;

	if(!ctx) {
		printf("EVP_MD_CTX_new() returned NULL, returning\n");
		return (union hash){};
	}
	if(EVP_DigestInit_ex(ctx, EVP_sha512(), NULL) != 1) {
		printf("EVP_DigestInit_ex failed, returning\n");
		return (union hash){};
	}

	while((bytes = fread(buff, 1, sizeof(buff), stdin)) > 0) {
		if(EVP_DigestUpdate(ctx, buff, bytes) != 1) {
			printf("EVP_DigestUpdate failed, returning\n");
			return (union hash){};
		}
	}

	if(EVP_DigestFinal_ex(ctx, hash.bytes, &hash_len) != 1) {
		printf("EVP_DigestFinal_ex failed, returning\n");
		return (union hash){};
	}

	EVP_MD_CTX_free(ctx);
	return hash;
}

#endif

