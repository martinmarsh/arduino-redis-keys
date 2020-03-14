# arduino-redis-keys
Light weight non blocking Arduino library for exchanging keys with Redis. Supports smaller arduinos eg UNO

Redis is an open source (BSD licensed), in-memory data structure store, used as a database, cache and message broker.
It runs on small devices such as Raspberry Pi which can act as a Redis server.  Using this library arduinos can share data
in the form of key pairs. The Redis data can also be sahred by cleints running on linux and windows 10 and there is wode
language support. 

Simple low resource library to send and key data from Redis. UNO compatible

* Only supports setting and getting of keys from redis
* Supports multiple data types: byte, int, unsigned int, long, unsigned long, float, double, char[]
* Code does not need to wait for reply to get and set keys
* Data requested is saved ro pre-defined variable location without having to write call back funtions
* Option to call back on data received data
* Supports up to 100 keys (note to support UNO; header file is preset to 30 keys)
* 1 Byte reference to a key to conserves memory
* Suppots only short char key data (approx. <30chars limited by buffer size)
* key names should be less than 20 chars
* Speed is limited by SPI and board for example a test with UNO R£3 and ethernet 5100 shield send and get requets took 1.2ms, get values where set without blocking 1.3ms later 

# Notices/know Issues

1) This library Interface is now frozen after limited testing of first release v1.0.1
2) Limited Float and Double support - precision must be set - no supprt for large numbers in E format
3) Double and float are the same size/precision on AVR based Arduinos eg UNO
4) Char[] max length for associated variable must be given and must be small enough to be held in buffers. Due to memory limits this is envisaged only short string would be sent and recieved
5) Redis will drop the connection on error. Communication drop watchdog may be required.

# Prerequisites

1) Remote networked computer eg a Raspberry Pi running Redis.
2) Arduino Newtwork shield and Ethernet library installed
3) Ethernet switch (For small networks a cheap £10 switch)

Note: Use without Ethernet shield on Arduino should be possible since the library does not assume any type ofnote
connection.  The idea would be to use Serial.print instead of Client.print in your redis send function.
The Redis computer would the be connected to the Arduino via the usb cable. The serial data could be piped to
Redis locally but for a reliable connection an interface task would need to be written to interact with Redis.  


# HOW TO USE

Please see examples which comment the set up steps required and runs basic test with response timing


== License ==

Copyright (c) 2010 Arduino LLC. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

