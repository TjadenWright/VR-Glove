//Contains the definitions that need to be evaluated before the main config file (lucidgloves-firmware.ino).
//These shouldn't need to be changed.

#include <mutex>
#include <condition_variable>
#include <queue>

int lockTime = 0;
int lockLoops = 1;
int lockTimeTotal = 0;
int lockTimeLast = 0;

class ordered_lock {
    std::queue<std::condition_variable *> cvar;
    std::mutex                            cvar_lock;
    bool                                  locked;
public:
    ordered_lock() : locked(false) {};
    void lock() {
        lockTime = micros() - lockTimeLast;
        lockTimeLast = micros();
        lockTimeTotal += lockTime;
        lockLoops++;
        std::unique_lock<std::mutex> acquire(cvar_lock);
        if (locked) {
            std::condition_variable signal;
            cvar.emplace(&signal);
            signal.wait(acquire);
        } else {
            locked = true;
        }
    }
    void unlock() {
        std::unique_lock<std::mutex> acquire(cvar_lock);
        if (cvar.empty()) {
            locked = false;
        } else {
            cvar.front()->notify_one();
            cvar.pop();
        }
    }
};

//Comm
#define COMM_SERIAL 0   
#define COMM_BTSERIAL 1 
#define COMM_BLESERIAL 2 
#define COMM_UWBSERIAL 3

//Encode
#define ENCODE_LEGACY 0
#define ENCODE_ALPHA  1

//Multiplexer
#define MUX(p) (p + 100)
#define UNMUX(p) (p % 100)
#define ISMUX(p) (p >= 100)

//finger mixing
#define MIXING_NONE 0
#define MIXING_SINCOS 2

//intermediate filtering
#define INTERFILTER_NONE 0
#define INTERFILTER_LIMITS 1
#define INTERFILTER_ALL 2
