/*
  redis_shared_keys.h - Minimal Non blocking Library for
  sharing keys with a redis server.
  Created by Martin D Marsh, Marsh 07, 2020.
  Released into the public domain.
*/
#ifndef redis_keys
#define redis_keys
#include "Arduino.h"

/*****************************************/
/** Options to reduce resources used *****/
/*****************************************/
// Comment in next line to only use byte, int and float types
// Saves about 2% of program space on UNO (about 760bytes)
// #define limited_key_types 1

#define max_receive_buffer 40      // Define max length of recieve line
#define max_send_buffer 70  // Define max length of send line includes key name and sting
#define max_key_definitions 30     // Define array size to hold key defs
#define max_var_size 30            // Define max length of a variable data as string

// Use defines rather than enum for types to save space

#define is_byte  0
#define is_int   1
#define is_float 3
#define is_uint 4
#define is_double 5
#define is_long 6
#define is_ulong 7
#define is_char 8

class Redis{
  private:
    char *pReply = reply;
    union pval {
          byte* pbyte;
          int* pint;
          float* pfloat;
#ifndef limited_key_types
          unsigned int* puint;
          double* pdouble;
          long* plong;
          unsigned long* pulong;
          char* pchar;
#endif
      } pvals;

 
    struct shared_data
      { 
        const char* name;
        byte type;
        pval  pvar;
        byte param;
      };

    shared_data data_defs[max_key_definitions];
    char send[max_send_buffer];
    char var[max_var_size];
    byte var_len;
    byte key_len;

    void convert(byte i, char* pReply);
    void to_string(byte i);
       
  public:
    char reply[max_receive_buffer];
    char *pmax_reply = reply + max_receive_buffer -2;
    void redis_callback(char* reply);
    byte index = 0;
    byte var_follows = 255;
    typedef void func(const char* print);
    typedef void func1(char* reply);
    
    struct redis_callbacks
    { 
      void (*print)(const char* print);
      bool enable; 
      void (*reply)(char* reply);
    };

    redis_callbacks call_back;

    void enable_reply_callback();
    void disable_reply_callback();

    void set_callbacks(func print_func);
    void set_callbacks(func print_func,func1 call_func);

    byte data_name(const char* name);
    byte data(const char* name, float* var, byte precision);
    byte data(const char* name, byte* var);
    byte data(const char* name, int* var);
#ifndef limited_key_types
    byte data(const char* name, unsigned int* var);
    byte data(const char* name, long* var);
    byte data(const char* name, unsigned long* var);
    byte data(const char* name, double* var, byte precision);
    byte data(const char* name, char* var, byte length);
#endif


    void sync(char c);
    void set(byte ref);
    void get(byte ref);
};
      

#endif
