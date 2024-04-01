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

### Fix Log
수정사항에 대한 설명은 [git issue](https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS/issues/2)를 참고하시길 바랍니다.

- [901265a](https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS/commit/901265af11ff4ba8341a8ed6639b32d86931b04b) (24-03-28) : 통계 결과 계산 수정 (average response time)
- [be0ff83](https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS/commit/be0ff833658c50aa39081c7953f8e00be8c9a49f) (24-04-01) : 통계 결과 채점 수정 (scheduling stat)
- [2fae0b3](https://github.com/DKU-EmbeddedSystem-Lab/2024_DKU_OS/commit/2fae0b3ec37bee4e1b95924538a3ae3c2f6baf2e) (24-04-01) : 결과 출력 형식 수정

이미 `git clone`한 경우, 수정사항을 반영하기 위해 아래와 같이 `git pull`하여 과제를 수행하시길 바랍니다.
```
cd 2024_DKU_OS
git pull origin main 
```

## Lab2
Will be updated soon
## Lab3
Will be updated soon
