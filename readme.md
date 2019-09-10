# home-automation-system (has)
A home automation system with wireless sensors, actuators and a gateway to control the network and populate an internet server.

# Requirements:
1. **Power consumption**
    a. Wireless sensors must have 2+ years lifespan
    
2. **Interface**
    a. A web interface for sensor data monitoring
    b. A web interface providing the access events datalog and sensor status
    c. A local data interface containing the access events datalog
    
3. **Access control**
    a. An *access control* with face recognition (e.g.: opening a 12V electric lock)
    b. A register interface, enabling access for multiple users

4. **Security**
    a. A datalog containing a list of previous access events (valid, invalid and panic) with the respective time and date
    b. Encrypted data in the web server
    
5. **Network**
    a. The wireless sensors must communicate with the gateway their respectives states and sensors values
    
6. **Sensors**
    a. Camera (for face recognition)
    b.
    c.
    d.  
    
7. **Actuators**
    a.
    b.
    c.
    
# solution/hardware requirements
* **BLE/WI-FI Gateway**
  - ESP32-CAM module
    - ESP-IDF
      - FreeRTOS 8 Espressif's fork
    - ESP-FACE
    - ESP-WHO
    - Azure-IoT Espressif API
    
* **Wireless BLE sensors**
  - Battery powered
  - Nordic Semiconductor's NRF52832 microcontroller
    - ARM Cortex-M4F (with floating point ALU)
  - **Must design hardware module with NRF52832 + sensors**
  - Two basic sensors:
    - ADC sensor for temperature reading (e.g: LM35)
    - GPIO ON/OFF sensor for open/closed door event
  - Transistors controlling current drain in sensors - "sensor sleep mode"
* **Connected BLE actuators**
  - 220Vac-5Vdc powered
  - Relay with normally closed phase
  - Nordic Semiconductor's NRF52832 microcontroller
    - ARM Cortex-M4F (with floating point ALU)
    ![NRF52832 module](docs/img/nrf52832_module.jpg)

# system datapath
  ![System Diagram](https://i.imgur.com/wHgt4r0.png)
  
# tools
  - 1x ESP32-CAM
  - 3x NRF52832
  - 1x Segger J-Link
  - 1x JSN-SR04T
  - 1x LM35
  - 1x AC 220V/DC 5V converter
  - 1x Relay 5V
 
# contributors
Rodrigo Belisário Ramos (rodbelisario)<br/>Rodigo Luiz da Costa (rodrigo-lc)<br/>Tarcis Aurélio Becher (tarciszera)

