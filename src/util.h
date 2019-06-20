#ifndef UTIL_H_
#define UTIL_H_
#include "define.h"
#include <stddef.h>
#include <unistd.h>
struct Item* ItemSearch(const char *path, struct Feature* feature);
void init(const char * _name, struct Feature** list);
struct Item* insert_item(const char * _path, struct Feature* feature);
int _write(const char * path, const char* buf, size_t size, off_t offset);
int _read(const char * path, char* buf, size_t size, off_t offset);
int _copy(const char * src, const char * dst);
int _cp(const char * src, const char * dst);
int _append(const char* path, const char* buf);
int _size(const char* path);
void free_feature(struct Feature * feature);
int API(const char* feature_name);
#endif
