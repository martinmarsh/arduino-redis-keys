/*
  redis_shared_keys.h - Minimal Non blocking Library for
  sharing keys with a redis server.
  Created by Martin D Marsh, Marsh 07, 2020.
  Released into the public domain.
*/
#ifndef redis_keys
#define redis_keys
#include "Arduino.h"

class Redis{
  private:
    char *pReply = reply;
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

      shared_data data_defs[15];
       
  public:
    char reply[35];
    String send;
    void redis_callback(char* reply);
    byte index = 0;
    int var_follows = -1;

    typedef void func(String print);
    typedef void func1(char* reply);
    
    struct redis_callbacks
    { 
      void (*print)(String print);
      void (*println)(String println);
      bool enable; 
      void (*reply)(char* reply);
    };

    redis_callbacks call_back;

    void enable_reply_callback();
    void disable_reply_callback();

    void set_callbacks(func print_func, func println_func);
    void set_callbacks(func print_func, func println_func, func1 call_func);

    byte data_name(String name);
    byte data(String name, float* var);
    byte data(String name, byte* var);
    byte data(String name, int* var);
    byte data(String name, unsigned int* var);
    byte data(String name, long* var);
    byte data(String name, unsigned long* var);
    byte data(String name, double* var);
    byte data(String name, char* var);

    void convert(byte i, char* pReply);
    void to_string(byte i);
    void sync(char c);
    void set(byte ref);
    void get(byte ref);
};
      

#endif
