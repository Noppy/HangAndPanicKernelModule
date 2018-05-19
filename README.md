# Hang and Panic kernel module for Test
hang_and_panic module causes a hang condition or a panic condition.  
This module is assumed to be used for TESTING purposes.
## Installation
### Required packages for build
For Amazon Linux 2, need the following packages for build.
- git
- gcc
- kernel-devel
### build
Install required packages.
```
$ sudo yum -y install git gcc kernel-devel
```
Clone Git repository
```
$ git clone https://github.com/Noppy/HangAndPanicKernelModule.git
$ cd HangAndPanicKernelModule/
```
Build the kernel module.  
Then a module with the file name is  hang_panic.ko will be built.
```
$ make
$ ls *.ko
hang_panic.ko
```
## Usage
### (1)Install the module
```
$ sudo insmod hang_panic.ko
$ lsmod |grep hang_panic
```
### (2)Read Help
```
$ cat /proc/hang_panic 
<<Hang&Panic module>>
'echo c > /proc/hang_panic' >>> panic
'echo h > /proc/hang_panic' >>> hang(disable local irq and preempt)
'echo H > /proc/hang_panic' >>> hang(disable only local irq)
```
### (3)Cause Hang condition
(a)Lock LocalIRQ and preemption(Can not ping, probably).
```
echo h > /proc/hang_panic
```
(b)Lock only LocalIRQ(Can ping)
```
echo h > /proc/hang_panic
```
### (4)Cause Panic condition
This behavior is similar to `echo c > /proc/sysrq-trigger`.
```
echo c > /proc/hang_panic
```
### (5)Remove the module
```
$ sudo rmmod hang_panic
```
