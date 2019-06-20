编译FUSE部分：
1. make 可以编译链接src/ArtisfFS.c中的ArtistFS.c以及util.c，它能够生成一个ArtistFS的可执行文件
2. 执行 ./ArtistFS ./mount 可以将这个FUSE文件系统挂载到 ./mount 上
3. 在 ./mount 下可以看到我们的各类功能，我们的主要功能 
3. 可以通过 cp test/temp ./mount/Double 中进行简单的测试，可以看到在 cat test/temp 时看到的结果在 cat ./mount/Double/temp 时出现了加倍的现象，可见通过了测试。

AIFS_libs部分：
1. 请将aifs_libs文件夹拷贝到根目录下
2. 将需要的图片，诗歌关键字拷贝到./mount下面的对应文件夹中，实现相应的功能，其中：
./mount/Poem 下对应诗歌生成
./mount/Picture 下对应简笔画的生成
./mount/Style 下对应梵高风格画生成
./mount/Chinese 下对应清明上河图风格画生成
