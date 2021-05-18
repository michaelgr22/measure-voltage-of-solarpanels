## **Measure Voltage of Solarpanels self sufficient**

### Requirements:
Hardware:  

    1x ESP32 NodeMCU 
    1x Panasonic NCR18650B (Li-Ion 3.7 V 3400 mAh) 
    1x MCP1700 3.3 V regulator 
    1x TP4056 
    8x Solar Panels (1,5 V)   
    1x Capacitor (100 microfarads) 
    1x Resistor (220 Ohms)
    1x BH1750 Light Sensor

  
Software:  

    1x Webserver  
    1x PostgresDB  

-------
### Wiring: 
  
![enter image description here](https://i.ibb.co/gvGW30m/v2-fritzing-bb.png)

------

### How it works:

 *electronics:*

    - 2 panels for measuring open circuit voltage, 2 panels for measuring resistor voltage -> internal resistance can be calculated
    - 4 panels to operate the NodeMCU, excess energy can be stored in the battery
    - 3.3 V regulator to reduce the voltage comming from the TP4056
    - capacitor to smooth the voltage for 3.3 V Input of NodeMCU

*application-side:*

![enter image description here](https://i.ibb.co/ZVn75X7/v2-how-it-works.png)
