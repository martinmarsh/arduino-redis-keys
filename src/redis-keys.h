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
// Comment out next line to only use byte, int and float types
// Saves about 2% of program space on UNO (about 760bytes)
// #define limited_key_types 1

#define max_receive_buffer 30      // Define max length of recieve line
#define max_key_definitions 20     // Define array size to hold key defs


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
        String name;
        char type;
        pval  pvar;
      };

    shared_data data_defs[max_key_definitions];
    String send;
    String ret = "/r/n";

    void convert(byte i, char* pReply);
    void to_string(byte i);
       
  public:
    char reply[max_receive_buffer];

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

    void set_callbacks(func print_func);
    void set_callbacks(func print_func,func1 call_func);

    byte data_name(String name);
    byte data(String name, float* var);
    byte data(String name, byte* var);
    byte data(String name, int* var);
#ifndef limited_key_types
    byte data(String name, unsigned int* var);
    byte data(String name, long* var);
    byte data(String name, unsigned long* var);
    byte data(String name, double* var);
    byte data(String name, char* var);
#endif


    void sync(char c);
    void set(byte ref);
    void get(byte ref);
};
      

#endif
