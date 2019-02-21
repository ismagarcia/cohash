#include <stdio.h>
#include <time.h>
#include <sqlite3.h>
#include <statsdb.h>

#include <math.h>
#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

int statsdb_init(struct statsdb *stats_db)
{  
    int retval;
    time_t t = time(0);
    struct tm *lt;
    lt = localtime(&t);
    //char date[8];
    //sprintf(date, "%02d%02d%04d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    //sprintf(stats_db->query,"fir_float_%s.sqlite3",date);
	sprintf(stats_db->query,"fir_float.sqlite3");
    retval = sqlite3_open(stats_db->query,&stats_db->handle);
    if(retval)
    {
        printf("Database connection failed\n");
        return -1;
    }
    sprintf(stats_db->query, "CREATE TABLE IF NOT EXISTS fir_float_perf_bench (fir_float_mode TEXT, in_width INTEGER, in_height INTEGER, num_runs INTEGER, avg_time_ns FLOAT, min_time_ns FLOAT, max_time_ns FLOAT, std_time_ns FLOAT, perf_rate FLOAT)");
    retval = sqlite3_exec(stats_db->handle, stats_db->query, 0, 0, 0);

    return retval;
}

int statsdb_log(struct statsdb *stats_db, const char *fir_float_mode, int in_width, int in_height, int num_runs, float avg_time_ns, float min_time_ns, float max_time_ns, float std_time_ns, float perf_rate)
{
    int retval;
    sprintf(stats_db->query, "INSERT INTO fir_float_perf_bench VALUES('%s', %d, %d, %d, %f, %f, %f, %f, %f)", fir_float_mode, in_width, in_height, num_runs, avg_time_ns, min_time_ns, max_time_ns, std_time_ns, perf_rate);
    retval = sqlite3_exec(stats_db->handle, stats_db->query, 0, 0, 0);
    return retval;
}

int statsdb_close(struct statsdb *stats_db)
{
    int retval;
    retval = sqlite3_close(stats_db->handle);
    return retval;
}

int statvar_init(struct statvar *stat_var, const char *name)  
{ 
  stat_var->min    = (double)0;
  stat_var->max    = (double)0;
  stat_var->avg    = (double)0;
  stat_var->avgSq  = (double)0;
  stat_var->num    = 0;
  stat_var->name   = name;
  return 0;
}

int statvar_reset(struct statvar *stat_var)
{
  stat_var->min    = (double)0;
  stat_var->max    = (double)0;
  stat_var->avg    = (double)0;
  stat_var->avgSq  = (double)0;
  stat_var->num    = 0;
	return 0;
}

int statvar_print(struct statvar *stat_var)
{
  if (stat_var->num > 0) 
	{
    char log[1024];
    sprintf(log, "[%s] min = %f, max = %f, avg = %f, std = %f, num = %d\n",
      stat_var->name,(float)statvar_min(stat_var),(float)statvar_max(stat_var),(float)statvar_avg(stat_var),(float)statvar_std(stat_var),statvar_num(stat_var));
    printf("%s", log);
  }
	return 0;
}

unsigned int statvar_num(struct statvar *stat_var) { return (stat_var->num); }
double statvar_min(struct statvar *stat_var)       { return (stat_var->min); }
double statvar_max(struct statvar *stat_var)       { return (stat_var->max); }
double statvar_avg(struct statvar *stat_var)       { if (stat_var->num == 0) { return (double)0; } else { return (stat_var->avg / ((double)stat_var->num)); } }
double statvar_std(struct statvar *stat_var)       { if (stat_var->num == 0 || stat_var->num == 1) { return (double)0; } else { double a = statvar_avg(stat_var); return sqrt( stat_var->avgSq / ((double)stat_var->num) - a*a ); } }

int statvar_set(struct statvar *stat_var, const double t)
{
  if (stat_var->num == 0) 
	{
    stat_var->min = t;
    stat_var->max = t;
  } 
	else 
	{
    stat_var->min = min(t,stat_var->min);
    stat_var->max = max(t,stat_var->max);
  }
  stat_var->avg   = stat_var->avg + t;
  stat_var->avgSq = stat_var->avgSq + (t*t);
  stat_var->num++;
	return 0;
}
