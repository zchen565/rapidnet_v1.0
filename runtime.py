import sys
import subprocess

subprocess.call("touch runtime.txt",shell=True)
subprocess.call("truncate -s 0 runtime.txt", shell=True)

for i in range(10):
    check_in = "./waf --run hyper-text-class-query-test"
    pipe = subprocess.Popen(check_in,stdout=subprocess.PIPE, shell=True)
    text = pipe.communicate()[0]
    myout = text.decode()
    words = myout.split()
    time = words[-2]
    subprocess.call("echo %s>>runtime.txt" %time, shell=True)
