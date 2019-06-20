
input_path = "/tmp/fs_input"
output_path = "/tmp/fs_output"
fr = open(input_path,'r')
#fw = open(output_path,'w')
s = fr.read()
if len(s) == 0:
	exit(-1)
fr.close()
fw = open(output_path,'w')
fw.write(s)
fw.write(s)
fw.write(s)
#fr.close()
fw.close()
exit(0)

