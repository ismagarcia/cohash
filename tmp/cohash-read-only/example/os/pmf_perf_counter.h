#ifndef _PMF_PERF_COUNTER_H
#define _PMF_PERF_COUNTER_H

/* We must never includes any system includes in pmf headers. 
 * So, we need this forward declaration.
 */
struct perf_event_attr;
class pmf_perf_counter
{
public:
	enum types
	{
		COUNTER_NONE,
		CLOCK_CYCLES,
		INSTRUCTION_COUNTER,
		L1_MISSES,
		DATA_WAIT,
	};
	pmf_perf_counter(types init_type);
	pmf_perf_counter(int init_cpu, int init_pid, types init_type);
	~pmf_perf_counter();
	bool start();
	bool stop();
	unsigned long long value();
private:
	bool reset();
	void populate_event();
	bool open_fd();
	int cpu;
        int pid;
        int group_fd;
	types type;
        int file;
        unsigned long id;
        bool running;
	unsigned long long saved_result;
        struct perf_event_attr * event;
};
#endif
