/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.
*/



/** @file
 *
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall hello.c `pkg-config fuse3 --cflags --libs` -o hello
 *
 * ## Source code ##
 * \include hello.c
 */


#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include "define.h"
#include "util.h"

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */

// struct Item* picture_list;
// int picture_num = 0;
struct Feature* head_feature = NULL;
const int FEATURE_NUM = 1;
const char * input_path = "/tmp/fs_input";
const char * output_path = "/tmp/fs_output";

static int afs_getattr(const char *path, struct stat *stbuf,
			 struct fuse_file_info *fi)
{
	memset(stbuf, 0, sizeof(struct stat));
	const char* curpath = path;
	if (strcmp(curpath, "/") == 0){
		stbuf -> st_mode = S_IFDIR | 0777;
		stbuf -> st_nlink = 2 + FEATURE_NUM;
		return 0;
	}

	curpath += 1;
	
	struct Feature* feature = head_feature;
	while(feature){
		
		if (strstr(curpath, feature ->name) == curpath){
			if(strlen(feature -> name) == strlen(curpath)){
				stbuf -> st_mode = S_IFDIR | 0777;
				stbuf -> st_nlink = 2 + feature ->tot;
				return 0;
			}
			else{
				struct Item* item = NULL;
				if ((item = ItemSearch(path, feature))){
					stbuf -> st_mode = S_IFREG | 0777;
					stbuf -> st_nlink = 1;
					stbuf -> st_size = item ->size;
					_append(item ->test_path, "stat\n");
					return 0;
				}
				else{
					return -ENOENT;
				}
			}	
		}
		feature = feature ->next;
	}
	return 0;
	// if (strstr(curpath, "Picture") == curpath){
	// 	stbuf -> st_mode = S_IFDIR | 0777;
	// 	stbuf -> st_nlink = 2 + picture_num;
	// 	return 0;
	// }
}

static int afs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi,
			 enum fuse_readdir_flags flags)
{
	const char* curpath = path;
// ls /
	if (strcmp(curpath,"/") == 0){
		filler(buf, ".", NULL, 0,0);
		filler(buf, "..", NULL, 0,0);
		struct Feature * feature = head_feature;
		while(feature){
			//if (strstr(curpath, feature -> name) == curpath){
			filler(buf, feature ->name, NULL, 0,0);
		//	}
			feature = feature -> next;
		}
		return 0;
	}

// ls /Poem
	curpath += 1; // pass /
	struct Feature * feature = head_feature;
	while(feature){
		if (strstr(curpath, feature ->name) == curpath){
			filler(buf, ".", NULL, 0,0);
			filler(buf, "..", NULL, 0,0);
			if (feature ->head != NULL){
				struct Item* item = feature -> head;
				while(item){
					filler(buf, item ->name, NULL,0, 0);
					item = item -> next;
				}
			}
		}
		feature = feature ->next;
	}

}

static int afs_open(const char *path, struct fuse_file_info *fi)
{
	const char * curpath = path + 1;
	struct Feature* feature = head_feature;
	while(feature){
		if (strstr(curpath, feature ->name) == curpath){
			struct Item * item = NULL;
			if ((item = ItemSearch(path, feature))){
				_append(item -> test_path, "open\n");

				return 0;
			}
		}
		feature = feature ->next;

	}
	return -ENOENT;
}	

static int afs_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi){

	const char* curpath = path;
	if (strcmp(curpath, "/") == 0) return 0;
	
	curpath = path + 1;
	struct Feature* feature = head_feature;
	while(feature){
		if (strstr(curpath, feature ->name) == curpath){
			struct Item* item = ItemSearch(path, feature);
			if (item == NULL){
				return  -errno;
			}
			int res = _read(item->t_path, buf, size, offset);
			_append(item -> test_path, "read\n");
//			char buf[50];
//			memset(buf, 0, sizeof(buf));
//			sprintf(buf, "read path is : %s and read return value: %d\n", item -> o_path, res);
			_append(item ->test_path, buf);
			return res;
		}
		feature = feature ->next;
	}
	return 0;	

}

static int afs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
	
	const char* curpath = path;
	if (strcmp(curpath, "/") == 0) return 0;
	curpath = path + 1;
	struct Feature* feature = head_feature;
	while(feature){
		if (strstr(curpath, feature ->name) == curpath){
			struct Item* item = insert_item(path, feature);
			_append(item -> test_path, "create\n");
			return 0;
		}
		feature = feature -> next;
	}
	return 0;
}

static int afs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	const char* curpath = path;
	if (strcmp(curpath, "/") == 0) return 0;
	
	curpath = path + 1;
	struct Feature* feature = head_feature;
	while(feature){
		if (strstr(curpath, feature ->name) == curpath){
			struct Item* item = ItemSearch(path, feature);
			if (item == NULL){
				return  -errno;
			}
			
			int res = _write(item->o_path, buf, size, offset);
			item -> size += res;

			_append(item -> test_path, "write\n");
//			_append(item -> test_path, item -> o_path);
//			char temp_buffer[50];
			//memset(temp_buffer, 0, sizeof(char)*50);
//			sprintf(temp_buffer, "write path is : %s \n write buffer is %s \n write size is %d \n write offset is %d\n write return value: %d\n", item -> o_path, buf, (int)size, (int)offset, res);
			_append(item ->test_path, buf);
		//Copy Input
			int copy_res = _cp(item ->o_path, input_path);
			if (copy_res != 0){
				_append(item -> test_path, "copy src failed\n");
				_append(item -> test_path, item -> o_path);
				_append(item -> test_path, input_path);
				return res;
			}
//			_append(item -> test_path, feature -> name);
		//API
/*
			FILE* fo = fopen(input_path,"r");
			if (!fo)
				_append(item -> test_path, "Open failed\n");
			else{
				char buff[50];
				fgets(buff,50,fo);
				_append(item -> test_path, buff);
				fclose(fo);
			}
*/

			int api_res = API(feature ->name);

		//Copy output
			if (api_res == 0){
				_append(item -> test_path, "api success\n");
				int copy_res = _cp(output_path, item -> t_path);
				if (copy_res != 0){
					_append(item -> test_path, "copy dst failed\n");
					return res;
				}
				item -> size = _size(item -> t_path);
			}else{
				_append(item -> test_path, "api failed\n");
				return res;
			}
			return res;
		}
		feature = feature ->next;
	}

	return 0;	
}

static int afs_fsync(const char *path, int type, struct fuse_file_info * fi){
	const char * curpath = path;
	if (strcmp(curpath, "/") == 0) return 0;
	curpath = path + 1;
	struct Feature * feature = head_feature;
	while(feature){
		if (strstr(curpath, feature -> name) == curpath){
			struct Item* item = ItemSearch(path, feature);
			if (item == NULL){
				return -errno;
			}
			_append(item -> test_path, "sync\n");
			return 0;
		}
	}
	return 0;
}

static struct fuse_operations afs_oper = {
	.getattr	= afs_getattr,
	.readdir	= afs_readdir,
	.open		= afs_open,
	.read		= afs_read,
	.create		= afs_create,
	.write		= afs_write,
//	.fsync		= afs_fsync,
};

int main(int argc, char *argv[])
{
	int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	/* Parse options */
//	if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
//		return 1;
	init("Trible", &head_feature);
	//printf("Insert: %s\n", head_feature->name);
	//insert_item("/Poem/temp.txt", head_feature);
	//printf("insert item name: %s\n", head_feature -> head -> name);
	//struct Item* item = ItemSearch("/Poem/temp.txt", head_feature);
	//printf("item name: %s\n",item -> name);
	//printf("item o_path: %s\n",item -> o_path);
//	printf("%d\n", API("Poem"));
//	printf("%d\n", _size("/ret/Poem@#@temp.ret"));
	init("Style", &head_feature);
	init("Double", &head_feature);
	init("Poem",&head_feature);
	init("Picture",&head_feature);
	init("Chinese",&head_feature);
	ret = fuse_main(args.argc, args.argv, &afs_oper, NULL);
	
	//Free Memory
/*
	struct Feature* feature = head_feature;
	while(feature){
		struct Feature* victim = feature;
		feature = feature -> next;
		printf("free: %s\n", feature -> name);
		free_feature(victim);
	}

	fuse_opt_free_args(&args);
*/	
	return ret;
}
