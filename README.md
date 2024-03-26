# 2024_DKU_OS


This is a place for studying Operaing System in Dankook University.
- Professor : Jongmoo Choi
- Assistant : Minguk Choi (Email : mgchoi@dankook.ac.kr)
- 2024 DKU Operating System Course Information [(Link)](http://embedded.dankook.ac.kr/~choijm/course/course.html#OS)


## Download Environment
Virtual Machine Platform: [VirtualBox 6.1.32](https://www.virtualbox.org/wiki/Downloads)
- This environment is based on VirtualBox 6.1.32.
- If image execution is not possible, we recommend reinstalling [VirtualBox 6.0](https://www.virtualbox.org/wiki/Download_Old_Builds_6_0).

Windows Subsystem for Linux Install Guide [(WSL)](https://docs.microsoft.com/ko-KR/windows/wsl/install-win10#step-4---download-the-linux-kernel-update-package)
Operating System: Ubuntu 20.04

## Lab0
Lab0 contains information about installing a virtual machine and Ubuntu. Set the environment according to the documentation. The documentation for lab0 is at the link below. The OS image to use for preferences is also on the link below. 
- [Lab0 Document](https://drive.google.com/file/d/1pBeBZlKkimcdILv7AwA26p1IwYQKCPUZ/view?usp=sharing)
- [DKU OS Image](https://drive.google.com/file/d/19qontuOYiDqqQVj312CjC2TqFWc5XDPS/view?usp=sharing) (Root password : 1234)


## Lab1
If you want to proceed to Lab1, go to command below :
```
sudo apt install openssl
sudo apt-get install cmake
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib

cd ~ 
git clone https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS
cd 2024_DKU_OS/lab1
make
./test
```

Document for conducting the assignment can be found at following link: [Lab1 Document](./[DKU_OS_LAB1]CPU_Scheduler_Simulator.pdf)

## Lab2
Will be updated soon
## Lab3
Will be updated soon
