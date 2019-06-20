#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "define.h"
#include "util.h"
struct Item* ItemSearch(const char *path, struct Feature* feature){
    const char* filename = path + 1 + strlen(feature ->name) + 1;
    struct Item* item = feature ->head;
    while(item){
        if (strcmp(item->name, filename) == 0)
            return item;
        item = item -> next;
    }
    return NULL;
}   

void init(const char* _name, struct Feature** list){
    struct Feature* target = (struct Feature*)malloc(sizeof(struct Feature));
    strcpy(target ->name, _name);
    target -> tot = 0;
    target -> head = NULL;
    target ->next = *(list);
    *(list) = target;
}

void get_path(char* target,const char* prefix, const char* featurename, const char* filename, const char* spliter, const char* suffix){
    strcpy(target, prefix);
    strcat(target, featurename);
    strcat(target, spliter);
    strcat(target, filename);
    strcat(target, suffix);
}

struct Item* insert_item(const char * _path, struct Feature* feature){
    struct Item* item = (struct Item*)malloc(sizeof(struct Item));
	memset(item, 0, sizeof(struct Item));
    const char* filename = _path + 1 + strlen(feature ->name) + 1;
	strcpy(item->name , filename);
    get_path(item ->o_path, "/tmp/", feature->name, filename, "@#@", ".org");
    get_path(item ->t_path, "/home/bear/ret/", feature->name,filename, "@#@", ".ret");
    get_path(item -> test_path, "/tmp/", feature->name, filename, "@#@", ".test");
	item -> next = feature ->head;
    item -> size = _size(item -> t_path);
	feature -> head = item;
	feature -> tot ++;
    return item;
}

int _cp(const char* src, const char* dst){
	char command[100];
	strcpy(command, "sudo cp ");
	strcat(command, src);
	strcat(command, " ");
	strcat(command, dst);
	return system(command);
}

int _copy(const char* src, const char* dst){
    FILE* source, *target;
    source = fopen(src, "rb");
    target = fopen(dst, "wb");
    if (!source)
        return -1;
    if (!target)
	return -1; 
    char ch;
    int index = 0;
    fseek(source,0,SEEK_END);
    int length = ftell(source);
    if (length == 0)
        return -1;
    while(!fseek(source, index, SEEK_SET)){
        fread(&ch, 1, 1, source);
        fwrite(&ch, 1, 1, target);
        if (index == length -1)
            break;
        index++;
    }
    fclose(source);
    fclose(target);
    return 0;
}

int _size(const char * path){
	struct stat statbuf;
	int res = stat(path, &statbuf);
	if (res != 0)
		return 0;
	return statbuf.st_size;	
}

int _write(const char* path, const char* buf, size_t size, off_t offset){
	int fd;
	int res;

	fd = open(path, O_WRONLY|O_CREAT,S_IRWXU);
	
	if (fd == -1)
		return -1111;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

int _read(const char* path, char* buf, size_t size, off_t offset){
	int fd;
	int res;

	fd = open(path, O_RDONLY);
	
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}
int _append(const char* path, const char* buffer){
    FILE* fp = fopen(path, "a");
    if (fp == NULL){
        return -errno;
    }
    int res = fprintf(fp, "%s", buffer);
    if (res == -1)
        return -errno;
    return fclose(fp);
}

void free_feature(struct Feature* vic){
    struct Item* item = vic -> head;
    while(item){
        struct Item* victim = item;
        item = item -> next;
        free(victim);
    }
    free(vic);
}

int API(const char* feature_name){
    if (strcmp("Poem", feature_name) == 0)
	return system("cd /aifs_libs/tensorflow_poems && sudo python3 compose_poem.py");
    if (strcmp("Double", feature_name) == 0)
	return system("cd /APITest && sudo python3 double.py");
    if (strcmp("Trible",feature_name) == 0)
	return system("cd /APITest && sudo python3 trible.py");
    if (strcmp("Picture",feature_name) == 0)
    	return system("cd /aifs_libs/PhotographicImageSynthesis && python2 demo_512p.py");
    if (strcmp("Style",feature_name) == 0)
	return system("cd /aifs_libs/fast-neural-style-tensorflow && sh run.sh");
    if (strcmp("Chinese",feature_name) == 0)
	return system("cd /aifs_libs/fast-neural-style-tensorflow && sh run2.sh");
}
