# arduino-redis-keys
Light weight non blocking Arduino library for exchanging keys with Redis. Tested with UNO R3.


Simple low resource library to send and key data from Redis. UNO compatible

* Only supports setting and getting of keys from redis
* Supports multiple data types 
* Non-blocking get and set
* Data send and saved without further intervention
* Option to call back on data received data
* Supports up to 244 variables (note memory limits this much less on UNO)
* 1 Byte reference to data mapping conserves memory when repeating calls to redis

ISSUES

Char strings must be dimensioned to 10 bytes
Float and Double have default precisions


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

