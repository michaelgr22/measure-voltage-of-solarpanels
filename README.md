## **Measure Voltage of Solarpanels**

*Requirements:*
Hardware:
1x Arduino Uno
1x ESP8266-01S
1x Voltage Sensor
1x DS3231
3x Solar Panels (1,5 V)
1x External Power Supply (3,3 V)

Software:
1x Webserver
1x PostgresDB

-------
*Wiring:*

![enter image description here](https://i.ibb.co/Kj8YVJC/wiring-bb.png)

------

*How it works:*
1. Arduino collects the data (collect_data_on_arduino.ino)
	Voltage from voltage sensor
	Current time from DS3231
2. Arduino sends the data via SoftwareSerial to ESP8266 (collect_data_on_arduino.ino)
3. ESP8266 formats the data and sends it via POST request to webserver (esp8266-push_data_to_webserver.ino)
4. Webserver  inserts data into Postgres database (insert-data-in-postgres.php)
5. Now you can run different queries on the data e.g. with pgAdmin
![enter image description here](https://i.ibb.co/hMW7V1C/solar-data.png)
