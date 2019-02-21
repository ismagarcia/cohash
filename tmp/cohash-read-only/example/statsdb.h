#include <stdio.h>
#include <time.h>
#include <sqlite3.h>

struct statsdb
{ 
    char query[1024];
    sqlite3_stmt *stmt;
    sqlite3 *handle;
};

int statsdb_init(struct statsdb *stats_db);
int statsdb_log(struct statsdb *stats_db, const char *fir_float_mode, int in_width, int in_height, int num_runs, float avg_time_ns, float min_time_ns, float max_time_ns, float std_time_ns, float perf_rate);
int statsdb_close(struct statsdb *stats_db);

struct statvar
{
  double              min;
  double              max;
  double              avg;
  double              avgSq;
  unsigned int        num;
  const char         *name;
};

int statvar_init(struct statvar *stat_var, const char *name);
int statvar_reset(struct statvar *stat_var);
int statvar_print(struct statvar *stat_var);
int statvar_set(struct statvar *stat_var, const double t);

unsigned int statvar_num(struct statvar *stat_var);
double statvar_min(struct statvar *stat_var);
double statvar_max(struct statvar *stat_var);
double statvar_avg(struct statvar *stat_var);
double statvar_std(struct statvar *stat_var);


