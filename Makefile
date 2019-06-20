#though:
#	gcc test_0/*.c -g -D_FILE_OFFSET_BITS=64 `pkg-config fuse3 --cflags --libs` -o ThoughFS
all:
	gcc src/ArtistFS.c src/util.c -g -D_FILE_OFFSET_BITS=64 `pkg-config fuse3 --cflags --libs` -o ArtistFS

clean:
	sudo rm -rf /tmp/*.test
	sudo rm -rf /home/bear/ret/*
	rm -f ArtistFS
	sudo umount ./tmp
