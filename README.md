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
## Lab3
Will be updated soon
