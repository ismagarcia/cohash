#include "pmf_perf_counter.h"
#ifdef _WIN32
pmf_perf_counter::pmf_perf_counter(pmf_perf_counter::types init_type) {}
pmf_perf_counter::pmf_perf_counter(int init_cpu, int init_pid, pmf_perf_counter::types init_type) {}
pmf_perf_counter::~pmf_perf_counter() {}
bool pmf_perf_counter::start() {return false;}
bool pmf_perf_counter::stop() {return false;}
unsigned long long pmf_perf_counter::value() { return 0; }
bool pmf_perf_counter::reset() { return true; }
void pmf_perf_counter::populate_event() {}
bool pmf_perf_counter::open_fd() { return true; }
#else
#include <sys/ioctl.h>
//#include <sys/unistd.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

extern "C"
{
#include "os/perf_event.h" /* should be read in kernel sources */
};

class pmf_perf_attr_map
{
public:
	struct attr_map_t
	{
		pmf_perf_counter::types type;
		unsigned long perf_type;
		unsigned long long perf_config;
	};
	static const attr_map_t attr_map[4];
	static const char * get_processor_name();
	static unsigned long get_perf_events_type_and_config(unsigned long long* config, pmf_perf_counter::types type);	
};


void u64_to_longlong(unsigned long long* result, __u64* value)
{
	*result = *((unsigned long long *)value);
}

struct perf_event_read_data_value_struct
{
	__u64 value;
	__u64 id;
};

struct perf_event_read_data_nogroup
{
	__u64 value;
	__u64 time_enabled;
	__u64 time_running;
};

#ifndef __NR_perf_event_open
/* not defined in bionic */
#define __NR_perf_event_open 364
extern "C" int syscall(int number, ...);
#endif

pmf_perf_counter::~pmf_perf_counter()
{
	if (event != NULL)
		free(event);
	if(file > 0)
		close(file);
}

pmf_perf_counter::pmf_perf_counter(pmf_perf_counter::types init_type)
	: cpu(-1), pid(0), group_fd(-1), type(init_type), file(0), running(false), event(NULL)
{
	populate_event();
}

pmf_perf_counter::pmf_perf_counter(int init_cpu, int init_pid, pmf_perf_counter::types init_type)
	: cpu(init_cpu), pid(init_pid), group_fd(-1), type(init_type), file(0), running(false), event(NULL)
{
	populate_event();
}

bool pmf_perf_counter::start()
{
	reset();
	if(running)
		return true;

	if(ioctl(file, PERF_EVENT_IOC_ENABLE,0) < 0)
		return false;

	running = true;
	return true;
}

bool pmf_perf_counter::stop()
{
	if(!running)
		return false;

	if(ioctl(file, PERF_EVENT_IOC_DISABLE, 0) < 0)
		return false;
	pmf_perf_counter::value();
	running = false;
	return true;
}

bool pmf_perf_counter::reset()
{
	return ioctl(file, PERF_EVENT_IOC_RESET, 0) >= 0;
}

unsigned long long pmf_perf_counter::value()
{
	if (!running)
		return saved_result;
	__u64 data[256]; // allows 252 max simultaneous counters in a group
	int status = read(file, data, sizeof(data));
	if(status <= 0)
	{
		saved_result = 0;
		return 0;
	}
	struct perf_event_read_data_nogroup* data_struct = (struct perf_event_read_data_nogroup*)(data);
	u64_to_longlong(&saved_result, &data_struct->value);
	/* BEGIN Workaround for a invalid signed extension on Pandaboard kernel */
	if (saved_result > 0x8000000000000000ULL)
	{
		printf("\n%llx -> %llx\n", saved_result, saved_result & 0xffffffff);
		saved_result &= 0xffffffff;
	}
	/* END Workaround for a invalid signed extension on Pandaboard kernel */	
	return saved_result;
}

void pmf_perf_counter::populate_event()
{
	event = (struct perf_event_attr * )malloc(sizeof(struct perf_event_attr));
	if (event == NULL)
		return;
	memset(event, 0, sizeof(struct perf_event_attr));
	event->type = pmf_perf_attr_map::get_perf_events_type_and_config(&event->config, type);
	event->size = sizeof(struct perf_event_attr);
	event->disabled = 1;
	event->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED |
						 PERF_FORMAT_TOTAL_TIME_RUNNING;
	open_fd();
}

bool pmf_perf_counter::open_fd()
{
	file = syscall(__NR_perf_event_open, event, pid, cpu, group_fd, 0);
	id = 0;
	return (file > 0);
}

#define ARMV7_PERFCTR_DCACHE_DEP_STALL_CYCLES 0x61,
#define ARMV7_PERFCTR_DCACHE_REFILL 0x03, /* L1 */

const pmf_perf_attr_map::attr_map_t pmf_perf_attr_map::attr_map[4] =
{
	{pmf_perf_counter::CLOCK_CYCLES, PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES },
	{pmf_perf_counter::INSTRUCTION_COUNTER, PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS },
//	{pmf_perf_counter::L1_MISSES, PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16) },
  {pmf_perf_counter::L1_MISSES, PERF_TYPE_RAW, ARMV7_PERFCTR_DCACHE_REFILL },
	{pmf_perf_counter::DATA_WAIT, PERF_TYPE_RAW, ARMV7_PERFCTR_DCACHE_DEP_STALL_CYCLES },
};

const char * pmf_perf_attr_map::get_processor_name()
{
	FILE * fd = fopen("/proc/cpuinfo","r");
	if (fd == NULL)
		return "";
	char temp[256];
	while (!feof(fd))
	{
		if (fscanf(fd, "%[^\n]", temp) <  1)
		{
			fscanf(fd, "%*[\n]");
			continue;
		}
		if (strstr(temp, "CPU part") != NULL)
			if (strstr(temp, "0xc09") != NULL)
			{
				fclose(fd);
				return "A9";
			}
		if (strstr(temp, "CPU part") != NULL)
			if (strstr(temp, "0xc08") != NULL)
			{
				fclose(fd);
				return "A8";
			}
	}
	fclose(fd);
	return "";
}

unsigned long pmf_perf_attr_map::get_perf_events_type_and_config(unsigned long long* config, pmf_perf_counter::types type)
{
	*config = 0;
	unsigned int count;
	for(count=0; count < sizeof(attr_map); count++)
	{
		if(attr_map[count].type == type)
		{
			*config = attr_map[count].perf_config;
			return attr_map[count].perf_type;
		}
	}
	return PERF_TYPE_MAX;
}
#endif
