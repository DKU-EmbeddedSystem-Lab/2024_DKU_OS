#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

#ifndef SCHD_H
#define SCHD_H

struct Job{
    int name = 0; // 작업 이름
    int arrival_time = 0; // 작업 도착 시간
    int service_time = 0; // 작업 소요(= burst) 시간
    int remain_time = 0; // 남은 작업 시간 (load_workload에서 service_time과 동일하게 초기화)
    double first_run_time = 0.0; // 작업 첫 실행 시간
    double completion_time = 0.0; // 작업 완료 시간 
};

// Scheduler 클래스는 모든 스케줄러의 부모 클래스임
class Scheduler {
    protected:
        // 스케줄러 이름
        std::string name;
        // workload 작업들이 이름 순으로 정렬된 큐
        std::queue<Job> job_queue_; 
        // 작업이 종료된 job을 저장하는 vector
        std::vector<Job> end_jobs_; 
        // context_switch 시간 (= 기존 작업 저장 + 새로운 작업 불러오는 시간)
        // switch time은 스케줄링 순서에도 영향을 미치니, 주의해야 함
        double switch_time_; 
        // 현재 시간 = 기존 총 작업 실행 시간 + 기존 총 문맥 교환 시간
        // arrival_time, waiting_time 또한 이를 기준으로 함. 
        double current_time_ = 0;
        // 현재 작업 (처음에는 존재하지 않는 job(name=0)으로 초기화되어 있음)
        Job current_job_;

    public:
        Scheduler() = default;
        /*
        (1) 생성자 함수
        - 스케줄링 전, 초기화 및 전처리를 담당하는 함수
        - "부모" 생성자는 수정할 수 없음
        - "자식" 생성자는 부모 클래스의 생성자 함수를 "반드시 호출" 해야함
        - "자식" 생성자의 부모 생성자 함수 호출 및 인자는 수정 불가
        - 그 외에는 자유롭게 "오버라이딩"하여 작성 가능
        */
        Scheduler(std::queue<Job> jobs, double switch_overhead) 
            : job_queue_(jobs), switch_time_(switch_overhead) {
                name = "Default";
        }
        /*
        (2) 스케줄링 함수
        - run() 함수는 다음 1초 동안 실행할 작업의 이름을 반환함
        - run() 함수는 모든 작업이 완료된 경우, "-1"을 반환함
        - "부모" 클래스의 run() 함수는 무조건 "-1"을 반환함
        - "자식" 클래스의 run() 함수는 각 각 스케줄러의 정책에 맞게 재작성 (오버라이딩)
        - 각 job의 구조체 멤버 변수는 모두 정확하게 기록 되어야 함
        - 각 job을 완료한 뒤, 완료한 순서대로 "end_jobs_"에 저장(push back)해야 함
        */
        virtual int run() {
            return -1;
        }
        
        // (3) 완료된 작업 정보를 반환하는 함수
        virtual std::vector<Job> get_jobs_end () final{
            return end_jobs_;
        }

        // (4) 스케줄러 이름 반환
        virtual std::string get_name () final{
            return name;
        }
};

class FCFS : public Scheduler{
    public:
        // 자식 클래스 생성자 
        FCFS(std::queue<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead) {
            name = "FCFS";
        }

        // 스케줄링 함수 
        int run() override {
            // 할당된 작업이 없고, job_queue가 비어있지 않으면 작업 할당
            if (current_job_.name == 0 && !job_queue_.empty()){
                current_job_ = job_queue_.front();
                job_queue_.pop();
            }

            // 현재 작업이 모두 완료되면
            if(current_job_.remain_time == 0){
                // 작업 완료 시간 기록
                current_job_.completion_time = current_time_;
                // 작업 완료 벡터에 저장
                end_jobs_.push_back(current_job_);

                // 남은 작업이 없으면 종료
                if (job_queue_.empty()) return -1;
                
                // 새로운 작업 할당
                current_job_ = job_queue_.front();
                job_queue_.pop();
                // context switch 타임 추가
                current_time_ += switch_time_;
            }
            
            // 현재 작업이 처음 스케줄링 되는 것이라면
            if (current_job_.service_time == current_job_.remain_time){
                // 첫 실행 시간 기록
                current_job_.first_run_time = current_time_;
            }

            // 현재 시간 ++
            current_time_++;
            // 작업의 남은 시간 --
            current_job_.remain_time--;

            // 스케줄링할 작업명 반환
            return current_job_.name;
        }
};
#endif // SCHD_H
