#include "Arduino.h"
#include "redis-keys.h"

void Redis::set_callbacks(func print){
    call_back.print = print;
    call_back.enable = false;
}

void Redis::set_callbacks(func print, func1 call_func){
    call_back.print = print;
    call_back.reply = call_func;
    call_back.enable = true;
}

void Redis::disable_reply_callback(){
    call_back.enable = false;
}

void Redis::enable_reply_callback(){
    call_back.enable = true;
}

byte Redis::data_name(const char* name){
    data_defs[index].name = name;
    return index++;
}

byte Redis::data(const char* name, float* var){
    data_defs[index].type = is_float;
    data_defs[index].pvar.pfloat = var;
    return data_name(name); 
}

byte Redis::data(const char* name, byte* var){
    data_defs[index].type = is_byte;
    data_defs[index].pvar.pbyte = var;
    return data_name(name); 
}

byte Redis::data(const char* name, int* var){
    data_defs[index].type = is_int;
    data_defs[index].pvar.pint = var;
    return data_name(name); ;
    
}

#ifndef limited_key_types
byte Redis::data(const char* name, unsigned int* var){
    data_defs[index].type = is_uint;
    data_defs[index].pvar.puint = var;
    return data_name(name); ;
    
}

byte Redis::data(const char* name, long* var){
    data_defs[index].type = is_long;
    data_defs[index].pvar.plong = var;
    return data_name(name);
    
}

byte Redis::data(const char* name, unsigned long* var){
    data_defs[index].type = is_ulong;
    data_defs[index].pvar.pulong = var;
    return data_name(name);
    
}

byte Redis::data(const char* name, double* var){
    data_defs[index].type = is_double;
    data_defs[index].pvar.pdouble = var;
    return data_name(name);
    
}

byte Redis::data(const char* name, char* var){
    data_defs[index].type = is_char;
    data_defs[index].pvar.pchar = var;
    return data_name(name);
    
}
#endif

void Redis::convert(byte i, char* pReply){
  switch(data_defs[i].type){
    case is_byte:
      *data_defs[i].pvar.pbyte = atoi(pReply);
      break;
    case is_int:
      *data_defs[i].pvar.pint = atoi(pReply);
      break;
    case is_float:                           
      *data_defs[i].pvar.pfloat = atof(pReply);
      break;

#ifndef limited_key_types
    case is_uint:
      *data_defs[i].pvar.puint = atoi(pReply);
      break;
    case is_double:
      *data_defs[i].pvar.pdouble = atof(pReply);
      break;
    case is_long:
      *data_defs[i].pvar.plong = atol(pReply);
      break;
    case is_ulong:
      *data_defs[i].pvar.pulong = atol(pReply);
      break;
    case is_char:
       strncpy(data_defs[i].pvar.pchar, pReply, 10);
#endif
  };              
}

void Redis::to_string(byte i){
  switch(data_defs[i].type){
    case is_byte:
      snprintf(send, max_send_buffer, "%u", *data_defs[i].pvar.pbyte);
      break;
    case is_int:
      snprintf(send, max_send_buffer, "%i", *data_defs[i].pvar.pint);
      break;
    case is_float:
      dtostrf(*data_defs[i].pvar.pfloat, 16, 6, send);
      break;

#ifndef limited_key_types
    case is_uint:
      snprintf(send, max_send_buffer, "%u",*data_defs[i].pvar.puint);
      break;
    case is_double:
      dtostrf(*data_defs[i].pvar.pdouble, 22, 6, send);
      break;
    case is_long:
      snprintf(send, max_send_buffer, "%li", *data_defs[i].pvar.plong);
      break;
    case is_ulong:
      snprintf(send, max_send_buffer, "%lu", *data_defs[i].pvar.pulong);
      break;
    case is_char:
      snprintf(send, max_send_buffer, "%s", data_defs[i].pvar.pchar);
      break;
#endif      
  };              
}

void Redis::sync(char c){
  byte i;

  if (c == 0x0D){
      *pReply = '\0'; 
      if (reply[0] == '@' && reply[1] == '@' ){
        pReply = reply + 2;
        var_follows = atoi(pReply);
        pReply = reply; 
      }else{
        pReply = reply; 
        if(var_follows < 255){
          if(reply[0] != '$'){
            convert(var_follows, pReply);
            var_follows = 255;
            if(call_back.enable == true){
                call_back.reply(pReply);
            }   
          }else if(reply[1] == '-'){
             // value not set in redis ie reply = -1
             var_follows = 255;
          }
        }   
      }
    
    }else if(c != 0x0A && pReply < pmax_reply){
    *pReply++ = c;
  } 

}


void Redis::set(byte ref){       
  call_back.print("set ");
  call_back.print(data_defs[ref].name);
  call_back.print(" ");
  to_string(ref);
  call_back.print(send);
  call_back.print(ret);
}

void Redis::get(byte ref){ 
    call_back.print("Echo @@");
    snprintf(send, max_send_buffer, "%u", ref);
    call_back.print(send); 
    call_back.print(ret);      
    call_back.print("get ");
    call_back.print(data_defs[ref].name);
    call_back.print(ret);
}


