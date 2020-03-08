#include <Ethernet.h>
#include <redis-keys.h> 

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 1);
IPAddress myDns(192, 168, 0, 1);
byte redisHost[] = { 192, 168, 0, 100 };


EthernetClient client;

// Instantiate redis class
Redis redis;

// Define data which will be sent or recieved to Redis
// Simply define data in the normal way but also define
// a byte to hold an index refernece,
float exit_temp;
byte r_exit_temp;

float in_temp;
byte r_in_temp;

float engine_bat;
byte r_engine_bat;

float cabin_bat;
byte r_cabin_bat;


byte engine_on;
byte r_engine_on;

float head;
byte r_head;

float head_rate;
byte r_head_rate;

float comp_head;
byte r_comp_head;

float comp_head_rate;
byte r_comp_head_rate;

int heal;
byte r_heal;

int pitch;
byte r_pitch;

unsigned int auto_base;
byte r_auto_base;

// Define functions to send to Redis
// the libraray makes no assumption about the ethernet connection

void redis_print(String s){
    client.print(s);
}

void redis_println(String s){
    client.println(s);
}


void setup() {
  // put your setup code here, to run once:
  Ethernet.init(10); 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Init DHCP: ");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Failed");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Halt: No Ethernet Shield");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Connect Ethernet!");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
    Serial.print("IP: ");
  } else {
    Serial.print("DHCP IP: ");
  }
  Serial.println(Ethernet.localIP());
  // give the Ethernet shield a second to initialize:
  delay(1000);


  //connect to redis server on remote system eg a Raspberry Pi
  client.connect(redisHost,6379);

  //set up call backs to send redis strings
  redis.set_callbacks(redis_print, redis_println);

  // Set up mappings to define key name and location of data
  // Note for each variable we define the key name which will
  // be used to set or get the key value and the address of the
  // location to get or store the value.
  // The return value is a 1 byte index which will be used to
  // refer to the mapping in send and get requests,
  r_exit_temp = redis.data("exit_temp", &exit_temp);
  r_in_temp = redis.data("in_temp", &in_temp);
  r_engine_bat = redis.data("eng_bat", &engine_bat);
  r_cabin_bat = redis.data("cabin_bat", &cabin_bat);
  r_engine_on = redis.data("engine_on", &engine_on);
  r_head_rate = redis.data("head_rate", &head_rate);
  r_comp_head = redis.data("comp_head", &comp_head);
  r_comp_head_rate = redis.data("comp_head_rate", &comp_head_rate);
  r_heal = redis.data("heal", &heal);
  r_pitch = redis.data("pitch", &pitch);
  r_auto_base = redis.data("auto_base", &auto_base);

  
}

void loop() {
    // In the loop call redis sync sending in 1 rely char at a time
    // This is a contrieved example normally you would avoid the while
    // loop and delay and call or get from redis on events which happen
    // less frequently than the loop time. Alternatively redis.sync might
    // be placed in an timer interupt routine (eg .06 ms intervals)
    int x= 100;

    while(x>0){
        if (client.available()) {
            redis.sync(client.read());
        }
        x--;
        delay(1);
    }

    //example send each call is non blocking
    redis.set(r_exit_temp);
    redis.set(r_in_temp);
    //example get also non-blocking  - the value will be set some ms later
    //(approx. 2ms with RPI v2 )
    redis.get(r_engine_on);

    // do other stuff - normally do not use delay 

}