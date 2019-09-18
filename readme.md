# Home-Automation-System (HAS)
A home automation system with wireless sensors, actuators and a gateway to control the network and populate an internet server.

# Requirements:
1. **Access control**
    1. An *access control* with face recognition (e.g.: opening a 12V electric lock)
    2. A register interface, enabling access by face recognition for multiple users
    
2. **Interface**
    1. A web interface for sensor data monitoring
    2. A web interface providing the access events datalog and sensor status
    3. A local data interface containing the access events datalog
  
3. **Data security**
    1. A datalog containing a list of previous access events (valid, invalid and panic) with the respective time and date
    2. Encrypted data in the web server
    
4. **Wireless network**
    1. The wireless sensors must communicate with a gateway their respectives states and sensors values
   
5. **Monitors and actuators**
    1. Camera (for face recognition/access control)
    2. Temperature sensors
    3. Open/closed window and door sensors 
    4. Wireless switches (smart outlets)
	5. Motor driver to control doors and shutters
    5. The system must have an real time clock to provide time and date logging

6. **Power consumption**
    1. Wireless sensors must have 2+ years lifespan
    
# Market analysis:  
The team gathered information about microprocessors with wireless interfaces. Here's the result:  

![Market analysis for microprocessors with wireless interfaces](docs/img/market_analysis.png)  
	
# Solutions:
* **BLE/WI-FI Gateway**
  - ESP32-CAM module
    - ESP-IDF
      - FreeRTOS 8 Espressif's fork
    - ESP-FACE
    - ESP-WHO
    - Azure-IoT Espressif API
    
* **Wireless BLE sensors**
  - Battery powered
  - Espressif'S ESP32 microcontroller
    - Two cores 
  - **Must design hardware module with ESP32 + sensors**
  - Two basic sensors:
    - ADC sensor for temperature reading (e.g: LM35)
    - GPIO ON/OFF sensor for open/closed door event
  - Transistors controlling current drain in sensors - "sensor sleep mode"
* **Connected BLE actuators**
  - 220Vac-5Vdc powered
  - Relay with normally closed phase


# System datapath
  ![System Diagram](https://i.imgur.com/wHgt4r0.png)
  
# Tools
  - 1x ESP32-CAM module
  - 3x ESP32-01 module
  - 1x Segger J-Link
  - 1x JSN-SR04T
  - 1x LM35
  - 1x AC 220V/DC 5V converter
  - 1x Relay 5V
 
# Contributors
Rodrigo Belisário Ramos (rodbelisario)<br/>Rodigo Luiz da Costa (rodrigo-lc)<br/>Tarcis Aurélio Becher (tarciszera)

