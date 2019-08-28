# ble-home-automation (bha)
A sensor network using modular BLE sensors and an ESP32 as a gateway and also a gatekeeper (literally), providing an access control system with face recognition.

# requirements:
1. **Power consumption**
    1. Wireless BLE sensors must have 2+ years lifespan
        - A Panasonic 3V CR2032 225.000uAh can provide 12,84uA mean current for 2 years.
    2. The gateway and all the access control peripherals
    
2. **Interface**
    1. A web interface using Microsoft Azure IoT Server for data monitoring
    2. A web interface providing the access events datalog and sensor status
    3. A SD card containing the access events datalog
    
3. **Access control**
    1. An *access control* with face recognition (opening a 12V electric lock, for example)
    2. A register interface, enabling access for multiple users
    3. A datalog containing a list of previous access events (valid, invalid and panic) with the respective time and date
    
4. **BLE Network**
    1. Star network topology, every RX/TX packet passes in the gateway

5. **Wireless sensors**
    1. Ultrassonic occupancy sensor JSN-SR04T to simulate open/closed windows and doors.
      - This sensor will be used to provide a event of the user getting close too, enabling the camera to start the image aquisition
           
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

# system datapath
  - _IMAGE_
  
# tools
  - 1x ESP32-CAM
  - 3x NRF52832
  - 1x Segger J-Link
  - 1x JSN-SR04T
  - 1x LM35
  - 1x AC 220V/DC 5V converter
  - 1x Relay 5V
 
# contributors
Rodrigo Belisário Ramos (rodbelisario)<br/>Rodigo Luiz da Costa (rodrigo-lc)<br/>Tarcis Aurélio Becher
