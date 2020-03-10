
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
// When defining variables also define an additional byte with a similar name
// eg in our example we use "r_" prefix.  This byte will be used to refer to
// the variable we wish to get or set in Redis.
// Now define a test variable for each type eg test_byte

byte test_byte;
byte r_test_byte;

float test_float;
byte r_test_float;

double  test_double;
byte r_test_double;

int test_int;
byte r_test_int;

unsigned int test_uint;
byte r_test_uint;

long test_long;
byte r_test_long;

unsigned long test_ulong;
byte r_test_ulong;

char test_char[10];
byte r_test_char;

// other variables
unsigned long timer_200;
unsigned long start_time;
unsigned long send_period;
unsigned long reply_period;
int send_once = 1;
int recieve_once = 0;
int tick;

// Define functions to send to Redis
// the libraray makes no assumption about the ethernet connection

void redis_send(String s){
    client.print(s);
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
  redis.set_callbacks(redis_send);

  // Set up mappings to define redis key name and link it to address of the sketch variable.
  // In Redis the key value will be set to the sketch variable's value on calling set
  // and vice versa on calling get.
  // Note the return value is a 1 byte index to the mapping and is used in calling get and set.
  // This means that gets and sets are resource efficient.
  
  r_test_byte = redis.data("test_byte", &test_byte);
  r_test_int = redis.data("test_int", &test_int);
  r_test_uint = redis.data("test_uint", &test_uint);
  r_test_long = redis.data("test_long", &test_long);
  r_test_ulong = redis.data("test_ulong", &test_ulong);
  r_test_float = redis.data("test_float", &test_float);
  r_test_double = redis.data("test_double", &test_double);
  r_test_char = redis.data("test_char", test_char);

}

void loop() {
  
    if(micros() - timer_200 > 200){
      //This runs every 200 mico seconds
      timer_200 = micros();
      tick++;
      //Consume any ethernet bytes (redis replies)
      //This must be called at much higher rate than sending or getiing redis keys
      while(client.available()) {
          redis.sync(client.read());
      }
      if(tick > 5000){
        tick = 0;
        Serial.println("Values are:");
        Serial.println( test_byte);
        Serial.println( test_float, 8);
        Serial.println( test_double, 8);
        Serial.println( test_int);
        Serial.println( test_uint );
        Serial.println( test_long);
        Serial.println( test_ulong);
        Serial.println( test_char);
        
        Serial.println("-----");
      }
      
    }
    if(send_once == 1){
        send_once = 0;
        
        test_byte = 27;
        test_float = 12345.1234567;
        test_double = 12345.1234567;
        test_int = -12345;
        test_uint = 12345;
        test_long = -1234567890;
        test_ulong = 1234567890;
        strncpy(test_char, "hello", 10);
        send_period = 0;
        start_time = micros();
        redis.set(r_test_byte);
        redis.set(r_test_float);
        redis.set(r_test_double);
        redis.set(r_test_int);
        redis.set(r_test_uint);
        redis.set(r_test_long);
        redis.set(r_test_ulong);
        redis.set(r_test_char);
        send_period = micros() - start_time;
        start_time = micros();
        
        test_byte = 0;
        test_float = 0;
        test_double = 0;
        test_int = 0;
        test_uint = 0;
        test_long = 0;
        test_ulong = 0;
        strncpy(test_char, "xxxxxxxx", 10);

        redis.get(r_test_byte);
        redis.get(r_test_float);
        redis.get(r_test_double);
        redis.get(r_test_int);
        redis.get(r_test_uint);
        redis.get(r_test_long);
        redis.get(r_test_ulong);
        redis.get(r_test_char);

        send_period += micros() - start_time;

        Serial.print("test char probably has not updated yet it is currently: ");
        Serial.println(test_char);

        Serial.print("Average set/get time is ");
        Serial.print(send_period/16);
        Serial.println(" micro secs");
        
        tick = 10000;
   }

}