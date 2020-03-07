/*
  redis_shared_keys.h - Minimal Non blocking Library for
  sharing keys with a redis server.
  Created by Martin D Marsh, Marsh 07, 2020.
  Released into the public domain.
*/
#ifndef redis_keys

#include "Arduino.h"


union pval {
          byte* pbyte;
          int* pint;
          unsigned int* puint;
          float* pfloat;
          double* pdouble;
          long* plong;
          unsigned long* pulong;
          char* pchar;  
      } pvals;

enum val_type { is_byte=0, is_int, is_uint, is_float, is_double, is_long, is_ulong, is_char } types;

struct shared_data
    { 
      String name;
      val_type type;
      pval  pvar;
    };


shared_data  sh[15];

class Redis{
    private:
       char *pReply = reply;
       
    public:
    char reply[40];
    byte has_connected;
    int reply_count;
    int reply_status;
    unsigned long rec_time = 0;
    void redis_callback(char* reply);
    byte index = 0;
    int var_follows = -1;

    typedef void func(char* reply);
    
    struct redis_callbacks
    {
      bool enable; 
      void (*fn)(char* reply);
      
    };

    redis_callbacks call_back = {false};
};   



#endif