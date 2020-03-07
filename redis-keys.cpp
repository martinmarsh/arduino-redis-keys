#ifndef redis_keys

#include "Arduino.h"
#include "redis-keys.h"

#endif

   
void Redis::set_call_back(func call_func ){
    call_back.fn = call_func;
    call_back.enable = true;
  
}

void Redis::disable_call_back(){
    call_back.enable = false;
}

byte Redis::data_name(String name){
    sh[index].name = name;
    return index++;
}

byte Redis::data(String name, float* var){
    sh[index].type = is_float;
    sh[index].pvar.pfloat = var;
    return data_name(name); 
}

byte Redis::data(String name, byte* var){
    sh[index].type = is_byte;
    sh[index].pvar.pbyte = var;
    return data_name(name); 
}

byte Redis::data(String name, int* var){
    sh[index].type = is_int;
    sh[index].pvar.pint = var;
    return data_name(name); ;
    
}

byte Redis::data(String name, unsigned int* var){
    sh[index].type = is_uint;
    sh[index].pvar.puint = var;
    return data_name(name); ;
    
}

byte Redis::data(String name, long* var){
    sh[index].type = is_long;
    sh[index].pvar.plong = var;
    return data_name(name);
    
}

byte Redis::data(String name, unsigned long* var){
    sh[index].type = is_ulong;
    sh[index].pvar.pulong = var;
    return data_name(name);
    
}

byte Redis::data(String name, double* var){
    sh[index].type = is_double;
    sh[index].pvar.pdouble = var;
    return data_name(name);
    
}

byte Redis::data(String name, char* var){
    sh[index].type = is_char;
    sh[index].pvar.pchar = var;
    return data_name(name);
    
}


void Redis::connect(byte *ip){
  Serial.println("Redis connect");
    if (client.connect(ip, 6379)) {
        has_connected = 1;
          Serial.println("conneted");
    } else {
        has_connected = 0;
        Serial.println("not conneted");
    }
  
}


void Redis::convert(byte i, char* pReply){
  switch(sh[i].type){
    case is_byte:
      *sh[i].pvar.pbyte = atoi(pReply);
      break;
    case is_int:
      *sh[i].pvar.pint = atoi(pReply);
      break;
    case is_uint:
      *sh[i].pvar.puint = atoi(pReply);
      break;
    case is_float:                           
      *sh[i].pvar.pfloat = atof(pReply);
      break;
    case is_double:
      *sh[i].pvar.pdouble = atof(pReply);
      break;
    case is_long:
      *sh[i].pvar.plong = atol(pReply);
      break;
    case is_ulong:
      *sh[i].pvar.pulong = atol(pReply);
      break;
    case is_char:
      *sh[i].pvar.pulong = *pReply;
  };              
  
}

void Redis::sync(char c){
  byte i;

  if (c == 0x0D){
      rec_time = micros();
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
            call_back.fn(pReply);
        }      
        var_follows = -1;
      }
    
    }else if(c != 0x0A){
    *pReply++ = c;
  }   

}


void Redis::set(byte ref){        
    client.print("set");
    client.print(" ");
    client.print(sh[ref].name);
    client.print(" ");
    client.println(*sh[ref].pvar.pfloat);
}

  void Redis::get(byte ref){ 
    client.print("Echo @");
    client.println(ref);       
    client.print("get");
    client.print(" ");
    client.println(sh[ref].name);
}


