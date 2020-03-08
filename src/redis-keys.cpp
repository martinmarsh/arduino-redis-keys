#include "Arduino.h"
#include "redis-keys.h"

void Redis::set_callbacks(func print, func println){
    call_back.print = print;
    call_back.println = println;
    call_back.enable = false;
}

void Redis::set_callbacks(func print, func println, func1 call_func){
    call_back.print = print;
    call_back.println = println;
    call_back.reply = call_func;
    call_back.enable = true;
}

void Redis::disable_reply_callback(){
    call_back.enable = false;
}

void Redis::enable_reply_callback(){
    call_back.enable = true;
}

byte Redis::data_name(String name){
    data_defs[index].name = name;
    return index++;
}

byte Redis::data(String name, float* var){
    data_defs[index].type = is_float;
    data_defs[index].pvar.pfloat = var;
    return data_name(name); 
}

byte Redis::data(String name, byte* var){
    data_defs[index].type = is_byte;
    data_defs[index].pvar.pbyte = var;
    return data_name(name); 
}

byte Redis::data(String name, int* var){
    data_defs[index].type = is_int;
    data_defs[index].pvar.pint = var;
    return data_name(name); ;
    
}

byte Redis::data(String name, unsigned int* var){
    data_defs[index].type = is_uint;
    data_defs[index].pvar.puint = var;
    return data_name(name); ;
    
}

byte Redis::data(String name, long* var){
    data_defs[index].type = is_long;
    data_defs[index].pvar.plong = var;
    return data_name(name);
    
}

byte Redis::data(String name, unsigned long* var){
    data_defs[index].type = is_ulong;
    data_defs[index].pvar.pulong = var;
    return data_name(name);
    
}

byte Redis::data(String name, double* var){
    data_defs[index].type = is_double;
    data_defs[index].pvar.pdouble = var;
    return data_name(name);
    
}

byte Redis::data(String name, char* var){
    data_defs[index].type = is_char;
    data_defs[index].pvar.pchar = var;
    return data_name(name);
    
}


void Redis::convert(byte i, char* pReply){
  switch(data_defs[i].type){
    case is_byte:
      *data_defs[i].pvar.pbyte = atoi(pReply);
      break;
    case is_int:
      *data_defs[i].pvar.pint = atoi(pReply);
      break;
    case is_uint:
      *data_defs[i].pvar.puint = atoi(pReply);
      break;
    case is_float:                           
      *data_defs[i].pvar.pfloat = atof(pReply);
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
      *data_defs[i].pvar.pchar = *pReply;
  };              
}

void Redis::to_string(byte i){
  switch(data_defs[i].type){
    case is_byte:
      send = String(*data_defs[i].pvar.pbyte);
      break;
    case is_int:
      send = String(*data_defs[i].pvar.pint);
      break;
    case is_uint:
      send = String(*data_defs[i].pvar.puint);
      break;
    case is_float:
      send = String(*data_defs[i].pvar.pfloat); 
      break;
    case is_double:
      send = String(*data_defs[i].pvar.pdouble);
      break;
    case is_long:
      send = String(*data_defs[i].pvar.plong);
      break;
    case is_ulong:
      send = String(*data_defs[i].pvar.pulong);
      break;
    case is_char:
      send = String(*data_defs[i].pvar.pchar);
      break;
  };              
}

void Redis::sync(char c){
  byte i;

  if (c == 0x0D){
      if ((reply[0] == '$' || reply[0] == '+') && reply[1] == '@' ){
        var_follows = reply[2];
        convert(i, pReply+3); 
        pReply = reply; 
      }else{
        *pReply = '\0'; 
          pReply = reply; 
        if(var_follows >= 0){
          convert(var_follows, pReply);
        }
        if(call_back.enable == true){
            call_back.reply(pReply);
        }      
        var_follows = -1;
      }
    
    }else if(c != 0x0A){
    *pReply++ = c;
  }   

}


void Redis::set(byte ref){       
    call_back.print("set ");
    call_back.print(data_defs[ref].name);
    call_back.print(" ");
    to_string(ref);
    call_back.println(send);
}

void Redis::get(byte ref){ 
    call_back.print("Echo @");
    send = String(ref);
    call_back.println(send);       
    call_back.print("get ");
    call_back.print(data_defs[ref].name);
}


