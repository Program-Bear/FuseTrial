from subprocess import call
print("Test temp2.txt -> tmp/Picture")
call("cp temp2.txt ./tmp/Picture", shell=True)

print("Test temp2.txt -> tmp/Poem")
call("cp temp2.txt ./tmp/Poem", shell=True)

print("Test temp1.txt -> tmp/Picture")
call("cp temp1.txt ./tmp/Picture", shell=True)

print("Test temp -> tmp/Poem")
call("cp temp ./tmp/Poem", shell=True)

print("Test temp -> tmp/Picture")
call("cp temp ./tmp/Picture", shell=True)

print("Test temp -> tmp/Poem")
call("cp temp ./tmp/Poem", shell=True)

print("Test temp -> tmp/Picture")
call("cp temp ./tmp/Poem", shell=True)

print("Test temp -> tmp/Poem")
call("cp temp ./tmp/Poem", shell=True)

print("Test temp1.txt -> tmp/Poem")
call("cp temp1.txt ./tmp/Poem", shell=True)

