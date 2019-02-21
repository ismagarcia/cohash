/*
 *  (C) copyright  2011, Ismael Garcia, (U.Girona/ViRVIG, Spain & INRIA/ALICE, France)
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef EXAMPLE_01_CU_
#define EXAMPLE_01_CU_

#include <iostream>
#include <iomanip>
#include <cuda.h>
#include <cuda_runtime.h>

#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <config_params.h>

#include <libh/hash.h>
#include <libhu/hash_utils.h>
#include <img_tga.inl>

#include <key_value_rand_hash.h>
#include <key_value_coh_hash.h>
#include <key_rand_hash.h>
#include <key_coh_hash.h>

#include <mt19937ar.h>

#if WIN32
#include <Windows.h>

struct timespec {
time_t tv_sec; /* Seconds since 00:00:00 GMT, */
               /* 1 January 1970 */
long tv_nsec; /* Additional nanoseconds since */
              /* tv_sec */
} timespec_t; 

LARGE_INTEGER
getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;
 
    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}
 
int
clock_gettime(int X, struct timespec *tv)
{
    LARGE_INTEGER           t;
    FILETIME            f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;
 
    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }
 
    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_nsec = (t.QuadPart % 1000000) * 1000;
    return (0);
}

#endif

//------------------------------------------------------------------------

#include <test_cu_robin_hood_hash.cu>

//------------------------------------------------------------------------

void runRobinHoodTest(ConfigParams& cfg) 
{
  testRobinHoodHash(cfg);
}

//------------------------------------------------------------------------

int runHashTest(int argc, char** argv, ConfigParams& cfg)
{
  if (argc < 6)
  {
    std::cerr << "* Num_threads                  -- " << OMP_CUSTOM_NUM_THREADS << std::endl;
    std::cerr << "* Random numbers hashing:"                                                                    << std::endl;
    std::cerr << "phash.exe [num_keys] [access_rate_non_valid_keys] [density] [seed] [access_mode]"             << std::endl;
    std::cerr << "  [num_keys]                   -- Integer number of keys"                                     << std::endl;
    std::cerr << "  [access_rate_non_valid_keys] -- Integer percentage 0-100"                                   << std::endl;
    std::cerr << "                                  (e.g. 20, means a total query of 20% non-valid"             << std::endl;
    std::cerr << "                                  accessed keys + [num_keys] of 80% valid keys)"              << std::endl;
    std::cerr << "  [density]                    -- Integer percentage 0-100"                                   << std::endl;
    std::cerr << "  [seed]                       -- Integer seed"                                               << std::endl;
    std::cerr << "  [access_mode]                -- 1 (sorted) / 0 (random shuffle)"                            << std::endl;
    std::cerr << std::endl;
    std::cerr << "(e.g. 'example_01.exe 32000000 100 80 61332125 1 -coh_hash')"                                 << std::endl;
    std::cerr << std::endl;
    std::cerr << "* Image data hashing:"                                                                        << std::endl;
    std::cerr << "phash.exe [image_file] [access_null_keys] [density] [seed] [access_mode]"                     << std::endl;
    std::cerr << "  [image_file]                 -- image data"                                                 << std::endl;
    std::cerr << "                                  (Use uncompressed tga images with RGBA"                     << std::endl;
    std::cerr << "                                   channels, value '#00000000' identify"                      << std::endl;
    std::cerr << "                                   non-valid key-data pixel entries)"                         << std::endl;
    std::cerr << "  [access_null_keys]           -- 1 access valid & non-valid keys /"                          << std::endl;
    std::cerr << "                                  0 access only valid keys"                                   << std::endl;
    std::cerr << "  [density]                    -- Integer percentage 0-100"                                   << std::endl;
    std::cerr << "  [seed]                       -- Integer seed"                                               << std::endl;
    std::cerr << "  [access_mode]                -- 1 (sorted) / 0 (random shuffle)"                            << std::endl;
    std::cerr << std::endl;
    std::cerr << "(e.g. 'example_01.exe flower_1024.tga 1 80 77016577 1 -coh_hash')"                            << std::endl;
    std::cerr << std::endl;
    
    return 0;
  }

  std::vector<std::string> sparams(argv, argv+argc);
  
  size_t found;
  found=sparams[1].find(".tga");
  if (found!=std::string::npos)
  {
    std::cerr << "image_data mode enabled" << std::endl;

    cfg.rand_num_mode     = false;
    cfg.image_mode        = true;
    cfg.image_name        = sparams[1];
    cfg.access_null_keys  = bool(atoi(sparams[2].c_str()));
    cfg.dens              = libhu::F32(atoi(sparams[3].c_str())) / 100.0f;
    cfg.seed              = atoi(sparams[4].c_str());
    cfg.sorted_access     = atoi(sparams[5].c_str());

    cfg.tex = loadTGA((char*)cfg.image_name.c_str());

    saveTGA(cfg.tex, "image_to_hash.tga");

    libhu::U32 tnnz = 0;
    for (libhu::U32 i = 0; i < cfg.tex->w * cfg.tex->h; i++)
    {
      libhu::U32 *imgPtr = (libhu::U32*)cfg.tex->data;
      if (imgPtr[i] != 0)
      {
        tnnz++;
      }
    }
    cfg.num_keys = tnnz;
    cfg.num_extra = cfg.tex->w * cfg.tex->h;
    if (cfg.access_null_keys)
    {
      cfg.rate_non_valid_keys = (float)(cfg.num_extra - cfg.num_keys) / (float)cfg.num_extra;
    }
    else
    {
      cfg.rate_non_valid_keys = 0.0;
      cfg.num_extra      = cfg.num_keys;
    }

  }
  else
  {
    cfg.rand_num_mode       = true;
    cfg.image_mode          = false;
    cfg.num_keys            = atoi(sparams[1].c_str());
    cfg.rate_non_valid_keys = (atoi(sparams[2].c_str()) / 100.0f);
    cfg.num_extra           = (cfg.num_keys / (1.0 - (atoi(sparams[2].c_str()) / 100.0f)));
    cfg.dens                = libhu::F32(atoi(sparams[3].c_str())) / 100.0f;
    cfg.seed                = atoi(sparams[4].c_str());
    cfg.sorted_access       = atoi(sparams[5].c_str());
  }
  
  if (argc > 6)
  {
    cfg.coh_hash          = (sparams[6] == "-coh_hash") ? 1 : 0;
    cfg.rand_hash         = (sparams[6] == "-rand_hash") ? 1 : 0;
  }
  else
  {
    cfg.coh_hash          = 1;
    cfg.rand_hash         = 1;
  }

  cfg.is_set = false;

  // Default 2D universe size
  cfg.u2D_w = 16384;
  cfg.u2D_h = 16384;

  // Default 3D universe size
  cfg.u3D_w = 512;
  cfg.u3D_h = 512;
  cfg.u3D_d = 512;

  runRobinHoodTest(cfg);

  libhu::F32 NUM_1M_KEYS = 1000000;
  libhu::F32 build_keys  = cfg.num_keys / NUM_1M_KEYS;
  libhu::F32 access_keys = ((cfg.rate_non_valid_keys == 0) ? cfg.num_keys : cfg.num_extra) / NUM_1M_KEYS;

  if (cfg.coh_hash)
  {
    std::cerr << "rh_coh_hash                      : " << cfg.rh_coh_hash_state << std::endl;
    std::cerr << "build rh_coh_hash                : " << std::setiosflags(std::ios::fixed) << std::setprecision(4) << cfg.rh_coh_hash_build_keys_per_sec << " Mkeys/sec" << std::endl;
    std::cerr << "access rh_coh_hash               : " << std::setiosflags(std::ios::fixed) << std::setprecision(4) << cfg.rh_coh_hash_access_keys_per_sec << " Mkeys/sec" << std::endl;
    std::cerr << std::endl;
    std::cerr << "-------------------------------------------" << std::endl;
  }
  else if (cfg.rand_hash)
  {
    std::cerr << "rh_rand_hash                     : " << cfg.rh_rand_hash_state << std::endl;
    std::cerr << "build rh_rand_hash               : " << std::setiosflags(std::ios::fixed) << std::setprecision(4) << cfg.rh_rand_hash_build_keys_per_sec << " Mkeys/sec" << std::endl;
    std::cerr << "access rh_rand_hash              : " << std::setiosflags(std::ios::fixed) << std::setprecision(4) << cfg.rh_rand_hash_access_keys_per_sec << " Mkeys/sec" << std::endl;
    std::cerr << std::endl;
    std::cerr << "-------------------------------------------" << std::endl;
  }

  if (cfg.image_mode)
  {
    delete cfg.tex;
  }

}

//------------------------------------------------------------------------

int main(int argc,char **argv)
{

  ConfigParams cfg;
  runHashTest(argc, argv, cfg);
  
  return 0;

}

//------------------------------------------------------------------------

#endif
