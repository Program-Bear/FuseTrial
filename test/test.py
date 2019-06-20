from subprocess import call
print("Test temp -> tmp/Trible")
call("cp temp ./tmp/Trible", shell=True)

print("Test temp -> tmp/Double")
call("cp temp ./tmp/Double", shell=True)

print("Test temp1 -> tmp/Trible")
call("cp temp1 ./tmp/Trible", shell=True)

print("Test temp2 -> tmp/Double")
call("cp temp2 ./tmp/Double", shell=True)

print("Test temp2 -> tmp/Trible")
call("cp temp2 ./tmp/Trible", shell=True)

print("Test temp1 -> tmp/Double")
call("cp temp1 ./tmp/Double", shell=True)
