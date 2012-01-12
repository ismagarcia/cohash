#include <vector>
#include <string>

#include <img_tga.h>

struct ConfigParams
{
  std::vector<std::string> sparams;
  std::string image_name;
  unsigned int num_keys;
  unsigned int num_extra;
  float dens;
  float rate_non_valid_keys;
  unsigned int seed;
  bool sorted_access;
  bool rand_num_mode;
  bool image_mode;
  bool access_null_keys;
  bool is_set;

  bool rh_rand_hash_state;
  bool rh_coh_hash_state;
  bool coh_hash;
  bool rand_hash;
  //--------------------------------------------
  float rh_coh_hash_build_time;
  float rh_coh_hash_access_time;
  float rh_rand_hash_build_time;
  float rh_rand_hash_access_time;
  //--------------------------------------------
  float rh_coh_hash_build_keys_per_sec;
  float rh_coh_hash_access_keys_per_sec;
  float rh_rand_hash_build_keys_per_sec;
  float rh_rand_hash_access_keys_per_sec;
  //--------------------------------------------
  unsigned int u3D_w;
  unsigned int u3D_h;
  unsigned int u3D_d;
  unsigned int u2D_w;
  unsigned int u2D_h;
  Texture *tex;
};