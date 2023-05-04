#include <unistd.h>

#include <climits>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <vector>
using namespace std;

bool vflag = false;
bool qflag = false;
bool fflag = false;
bool sflag = false;

char sched_type = 'N';
int total_time = 0;  // long long int
int tot_movement = 0;
int tot_request = 0;
int max_waittime = -1;

class IO_request {
   public:
    IO_request(int _iop, int _arr_time, int _track)
        : iop(_iop), arr_time(_arr_time), track(_track) {}
    int iop;
    int arr_time;
    int track;
    int start_time;
    int end_time;
};

vector<IO_request*> request_order;

void init_io_request(string& ifile_name) {
    ifstream ifile;
    ifile.open(ifile_name);
    if (!ifile.is_open()) {
        printf("Unable to open input_file\n");
        exit(1);
    }
    int req_num = 0;
    int arr_time = 0;
    int track = 0;
    string line;
    while (getline(ifile, line)) {
        if (line[0] == '#') {
            continue;
        } else {
            sscanf(line.c_str(), "%d %d", &arr_time, &track);
            IO_request* req = new IO_request(req_num, arr_time, track);
            request_order.emplace_back(req);
            req_num++;
        }
    }
    tot_request = request_order.size();
    ifile.close();
}

class IOSched {
   public:
    virtual void add_request(IO_request* req) = 0;
    virtual IO_request* get_next_request() = 0;
    virtual bool is_pending() = 0;

    void seek(int track) {
        if (track > hand) {
            hand++;
        } else if (track < hand) {
            hand--;
        }
    }

    bool is_complete(int track) { return track == hand; }

   protected:
    int hand = 0;
};

class FCFS : public IOSched {
   public:
    void add_request(IO_request* req) { io_queue.emplace(req); }

    IO_request* get_next_request() {
        IO_request* req = io_queue.front();
        io_queue.pop();
        return req;
    }

    bool is_pending() { return !io_queue.empty(); }

   private:
    queue<IO_request*> io_queue;
};

class SSTF : public IOSched {
   public:
    void add_request(IO_request* req) { io_queue.emplace_back(req); }

    IO_request* get_next_request() {
        int min_dis = INT_MAX;
        auto req_ptr = io_queue.end();
        for (auto it = io_queue.begin(); it != io_queue.end(); it++) {
            int dis = abs(hand - (*it)->track);
            if (dis < min_dis) {
                min_dis = dis;
                req_ptr = it;
            }
        }
        IO_request* req = *req_ptr;
        io_queue.erase(req_ptr);
        return req;
    }

    bool is_pending() { return !io_queue.empty(); }

   private:
    list<IO_request*> io_queue;
};

class LOOK : public IOSched {
   public:
    void add_request(IO_request* req) { io_queue.emplace_back(req); }

    IO_request* get_next_request() {
        int min_dis = INT_MAX;
        auto req_ptr = io_queue.end();
        while (true) {
            for (auto it = io_queue.begin(); it != io_queue.end(); it++) {
                int dis = ((*it)->track - hand) * dir;
                if (dis >= 0 && dis < min_dis) {
                    min_dis = dis;
                    req_ptr = it;
                }
            }
            if (req_ptr == io_queue.end()) {
                dir *= -1;
            } else {
                break;
            }
        }
        IO_request* req = *req_ptr;
        io_queue.erase(req_ptr);
        return req;
    }

    bool is_pending() { return !io_queue.empty(); }

   private:
    int dir = 1;
    list<IO_request*> io_queue;
};

class CLOOK : public IOSched {
   public:
    void add_request(IO_request* req) { io_queue.emplace_back(req); }

    IO_request* get_next_request() {
        int min_dis = INT_MAX;
        auto req_ptr = io_queue.end();
        for (auto it = io_queue.begin(); it != io_queue.end(); it++) {
            int dis = (*it)->track - hand;
            if (dis >= 0 && dis < min_dis) {
                min_dis = dis;
                req_ptr = it;
            }
        }
        if (req_ptr == io_queue.end()) {
            for (auto it = io_queue.begin(); it != io_queue.end(); it++) {
                int dis = (*it)->track - hand;
                if (dis < min_dis) {
                    min_dis = dis;
                    req_ptr = it;
                }
            }
        }
        IO_request* req = *req_ptr;
        io_queue.erase(req_ptr);
        return req;
    }

    bool is_pending() { return !io_queue.empty(); }

   private:
    list<IO_request*> io_queue;
};

class FLOOK : public IOSched {
   public:
    void add_request(IO_request* req) { add_queue.emplace_back(req); }

    IO_request* get_next_request() {
        if (active_queue.empty()) {
            swap(active_queue, add_queue);
        }
        int min_dis = INT_MAX;
        auto req_ptr = active_queue.end();
        while (true) {
            for (auto it = active_queue.begin(); it != active_queue.end();
                 it++) {
                int dis = ((*it)->track - hand) * dir;
                if (dis >= 0 && dis < min_dis) {
                    min_dis = dis;
                    req_ptr = it;
                }
            }
            if (req_ptr == active_queue.end()) {
                dir *= -1;
            } else {
                break;
            }
        }
        IO_request* req = *req_ptr;
        active_queue.erase(req_ptr);
        return req;
    }

    bool is_pending() { return !(active_queue.empty() && add_queue.empty()); }

   private:
    list<IO_request*> active_queue;
    list<IO_request*> add_queue;
    int dir = 1;
};

IOSched* THE_SCHEDULER = nullptr;

void init_io_sched() {
    switch (sched_type) {
        case 'N':
            THE_SCHEDULER = new FCFS();
            break;
        case 'S':
            THE_SCHEDULER = new SSTF();
            break;
        case 'L':
            THE_SCHEDULER = new LOOK();
            break;
        case 'C':
            THE_SCHEDULER = new CLOOK();
            break;
        case 'F':
            THE_SCHEDULER = new FLOOK();
            break;
        default:
            printf("Unknown Scheduler spec : -v { NSLCF }\n");
            exit(1);
    }
}

void simulation() {
    IO_request* curr_request = nullptr;
    IO_request* next_request = request_order[0];
    int curr_time = 0;
    while (true) {
        if (next_request != nullptr && curr_time == next_request->arr_time) {
            THE_SCHEDULER->add_request(next_request);
            if (next_request->iop + 1 < tot_request) {
                next_request = request_order[next_request->iop + 1];
            } else {
                next_request = nullptr;
            }
        }
        if (curr_request != nullptr &&
            THE_SCHEDULER->is_complete(curr_request->track)) {
        req_end:
            curr_request->end_time = curr_time;
            tot_movement += curr_time - curr_request->start_time;
            curr_request = nullptr;
        }
        if (curr_request == nullptr) {
            if (THE_SCHEDULER->is_pending()) {
                curr_request = THE_SCHEDULER->get_next_request();
                curr_request->start_time = curr_time;
                max_waittime = max(max_waittime, curr_request->start_time -
                                                     curr_request->arr_time);
                if (THE_SCHEDULER->is_complete(curr_request->track)) {
                    goto req_end;
                }
            } else if (next_request == nullptr) {
                total_time = curr_time;
                break;
            }
        }
        if (curr_request != nullptr) {
            THE_SCHEDULER->seek(curr_request->track);
        }
        curr_time++;
    }
}

void summary() {
    double time_io_was_busy = 0;
    double tot_turnaround = 0;
    double tot_waittime = 0;
    for (IO_request* r : request_order) {
        time_io_was_busy += r->end_time - r->start_time;
        tot_turnaround += r->end_time - r->arr_time;
        tot_waittime += r->start_time - r->arr_time;
        printf("%5d: %5d %5d %5d\n", r->iop, r->arr_time, r->start_time,
               r->end_time);
    }
    double io_utilization = time_io_was_busy / total_time;
    double avg_turnaround = tot_turnaround / tot_request;
    double avg_waittime = tot_waittime / tot_request;
    printf("SUM: %d %d %.4lf %.2lf %.2lf %d\n", total_time, tot_movement,
           io_utilization, avg_turnaround, avg_waittime, max_waittime);
}

int main(int argc, char** argv) {
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "vqfs:")) != -1) {
        switch (c) {
            case 'v':
                vflag = true;
                break;
            case 'q':
                qflag = true;
                break;
            case 'f':
                fflag = true;
                break;
            case 's':
                sched_type = optarg[0];
                break;
            case '?':
                if (optopt == 's') {
                    fprintf(stderr,
                            "./mmu: option requires an argument -- "
                            "'%c'\nillegal option\n",
                            optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr,
                            "./mmu: invalid option -- '%c'\nillegal option\n",
                            optopt);
                } else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n",
                            optopt);
                }
                return 1;
            default:
                abort();
        }
    }

    if (argc - optind == 0) {
        printf("inputfile name not supplied\n");
        return 1;
    }

    string ifile_name = argv[optind];

    init_io_sched();
    init_io_request(ifile_name);

    simulation();

    summary();
}