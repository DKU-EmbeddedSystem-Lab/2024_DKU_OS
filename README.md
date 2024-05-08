# 2024_DKU_OS


This is a place for studying Operaing System in Dankook University.
- Professor : Jongmoo Choi
- Assistant : Minguk Choi (Email : mgchoi@dankook.ac.kr)
- 2024 DKU Operating System Course Information [(Link)](http://embedded.dankook.ac.kr/~choijm/course/course.html#OS)


## Download Environment
Virtual Machine Platform: [VirtualBox 7.0.14](https://www.virtualbox.org/wiki/Downloads)
- This environment is based on VirtualBox 7.0.14.

Windows Subsystem for Linux Install Guide [(WSL)](https://docs.microsoft.com/ko-KR/windows/wsl/install-win10#step-4---download-the-linux-kernel-update-package)

Operating System: Ubuntu 22.04.4 LTS

## Lab0
Lab0 contains information about installing a virtual machine and Ubuntu. Set the environment according to the documentation. The documentation for lab0 is at the link below. The OS image to use for preferences is also on the link below. 
- [Lab0 Document](./[DKU_OS_LAB0]%20Linux%20Image%20Manual.pdf)
- [DKU OS Image](https://drive.google.com/file/d/1YHhEv67pzsZ3nJT6q4Fb9FXHWTRmTeex/view?usp=sharing) (Root password : 1234)


## Lab1
If you want to proceed to Lab1, go to command below :
```
sudo apt install openssl
sudo apt-get install libgtest-dev

git clone https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS.git
cd 2024_DKU_OS/lab1
make
./test
```

Document for conducting the assignment can be found at following link: [Lab1 Document](./[DKU_OS_LAB1]%20CPU_Scheduler_Simulator.pdf)


## Lab2
```
sudo apt-get install libgtest-dev
git clone https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS.git
cd 2024_DKU_OS/lab2
```
(1) Queue
```
cd queue
make
./test
```
(2) Binary Search Tree
```
cd bst
make
./test
```

Document for conducting the assignment can be found at following link: [Lab2 Document](./[DKU_OS_LAB2]%20Concurrent_Data_Structure.pdf)

**Fine-grained Queue/BST 채점기준 추가**
- Multi-threading 환경에서, Fine-grained Queue/BST가 Coarse-grained Queue/BST보다 수행시간이 더 긴 경우 50% 감점
- 단, ENQ_THEN_DEQ Workload는 Fine/Coarse-grained Queue의 수행시간이 비슷한 경우도 인정

## Lab3
### You must use VirtualBox(Ubuntu Linux Env) with "new Ubuntu" image!

New Ubuntu Image Upload : [Lab3 Image Link](https://drive.google.com/file/d/1lfWipurgLTlyQxotV7OJdEYPEowwhp9E/view?usp=sharing)

Password : 1234

VirtualBox image is upload at above.

If you want for proceed to Lab3, go to command below :
```
cd lab3
make
insmod ramdisk.ko
mkfs.ext2 /dev/ramdisk
mkdir mnt
mount /dev/ramdisk ./mnt
./create.sh
./apd mnt/A/BC 13 A/BC-13        //A, B, C is your last 3 digit of you student ID : 32XXXABC
./apd mnt/A/CB 13 A/CB-13        //A, B, C is your last 3 digit of you student ID : 32XXXABC
```
Then you can proceed Lab3

If you want to proceed to Lab3-Bounus, go to command below:

```
umount /dev/ramdisk
rmmod ramdisk
insmod ramdisk.ko
cd os_ext2        //Change the source code
make
insmod os_ext2.ko
mkfs.ext2 /dev/ramdisk
mount -t os_ext2 /dev/ramdisk ../mnt
dmesg | grep [YOUR_NAME]
```

See the document for details.

[Lab3 Document Link](./[DKU_OS_LAB3]%20File%20System.pdf)