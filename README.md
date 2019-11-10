# Home-Automation-System (HAS)
A home automation system with wireless sensors, actuators and a gateway to control the network and populate an internet server.

# Requirements:
1. **Access control**
    1. An *access control* with face recognition (e.g.: opening a 12V electric lock)
    2. A register interface, enabling access by face recognition for multiple users
    3. A datalog containing a list of previous access events (valid, invalid and panic) with the respective time and date
    
2. **Interface**
    1. A web interface for sensor data monitoring
    2. A web interface providing the access events datalog and sensor status
    3. A local data interface containing the access events datalog
  
3. **Data security** 
    1. Encrypted data in the web server
    
4. **Wireless network**
    1. The wireless sensors must communicate with a gateway their respectives states and sensors values
   
5. **Monitors and actuators**
    1. Camera (for face recognition/access control)
    2. Wireless temperature sensors (battery) 
    3. Opened/closed window and door sensors 
    4. Smart switches (smart outlets)
    5. Motor driver to control doors and shutters
    6. The system must have an real time clock to provide time and date logging

6. **Power consumption**
    1. Wireless sensors must have 2+ years lifespan
    
# Market analysis:  
The team gathered information about microprocessors with wireless interfaces. Here's the result:  

![Market analysis for microprocessors with wireless interfaces](docs/img/market_analysis.png)  

# Diagrama de blocos do sistema
  ![System Diagram](https://i.imgur.com/wHgt4r0.png)

# Servidor Web

Para a realização do projeto é necessária a utilização de um servidor web, onde serão armazenados os dados do sistema (valores dos sensores, imagens, etc). Alguns problemas giram em torno disso:

- Segurança no envio e recebimento dos dados e no servidor utilizado
- Armazenamento e acesso dos dados, onde fazer e como fazer?
- Tempo de desenvolvimento desse sistema
- Custo operacional

Como o projeto é de curto prazo e com equipe pequena sem expertise no assunto, a melhor solução foi encontrar um servidor web pronto, onde paga-se apenas pelo serviço utilizado, fazendo com que a equipe ganhe tempo para se preocupar com outras peculiaridades do projeto.

Foi feita uma pesquisa sobre alguma empresa que poderia disponibilizar esse tipo de serviço de base de dados online com algum serviço de acesso e as mais populares encontradas atualmente foram:

- AWS
- Microsoft Azure
- Google cloud

Cada um deles tem vários serviços web diferentes, que por sua vez podem ser integrados dentro da plataforma de cada fornecedor. Neste [link](https://intellipaat.com/blog/aws-vs-azure-vs-google-cloud/) há um vídeo que mostra diferenças básicas nos serviços, incluindo popularidade, variedade de serviços, preços, etc. 

![](http://i.imgur.com/vUvnjrz.png)

Estudando um pouco as empresas, e também o material disponível pela [espressif](https://github.com/espressif) foi possível constatar que tanto a AWS quanto o Microsoft Azure atendiam os nossos requisitos de serviços e também tinham algum tipo de material pronto para aceleração do desenvolvimento do projeto á um preço parecido. Os fatores que levaram a uma decisão foram:

- **Documentação**
  - O serviço com a melhor documentação, usualmente, torna o tempo de desenvolvimento mais rápido
- **Usabilidade**
  - Com uma boa usabilidade, o serviço permite que a equipe se concentre com o desenvolvimento de outras partes do projeto sem ter que se preocupar tanto em como tornar o serviço utilizavel
- **Popularidade**
  - Aprender a utilizar uma ferramenta popular facilita a realização de futuros projetos que tem mais chances de utilizar essa mesma ferramenta

Nesses quisitos o serviço da Amazon tem um pouco de vantagem em comparação com o serviço da Microsoft, fazendo com que esta seja a escolha para nosso projeto.

## AWS

A [AWS](https://aws.amazon.com/pt/) é uma plataforma que permite os usuários a acessarem vários serviços diferentes dessa plataforma, permitindo também a integração desses serviços.

![](http://i.imgur.com/Gaz85Jx.png)

Dentre esses serviços, os serviços que serão utilizados serão o AWS IoT (Internet das coisas) e o DynamoDB (Banco de dados). A escolha desses serviços está intrínsecamente ligada com os exemplos disponibilizados pela [espressif](https://github.com/espressif/esp-aws-iot) que disponibiliza um exemplo com uma implementação pronta para o serviço MQQT do [AWS IoT](https://docs.aws.amazon.com/pt_br/iot/latest/developerguide/aws-iot-how-it-works.html) que por sua vez pode ser integrado diretamente através de regras com a base de dados DynamoDB.

![](http://i.imgur.com/yJqbBqW.png)

Primeiramente para a utilização de serviços da amazon, primeiramente deve-se ter uma conta. E para isso utilizamos o AWS Educate que é uma conta na Amazon fornecida por uma parceria da instituição de ensino com a plataforma que disponibiliza alguns serviços e créditos para utilização de serviços com o intuito de familiarizar o estudante com as ferramentas.

## Espressif AWS IoT

Para a utilização do ESP-32 em conjunto com os serviços da Amazon, há um repositório fornecido pela empresa com um tutorial de como criar um dispositivo remoto na AWS e certificar o ESP-32 como esse dispositivo. Após isto, o exemplo fornecido pela espressif pode ser compilado e gravado no microcontrolador, em conjunto com a própria documentação do ESP-32.

Após todo o procedimento descrito anteriormente o microcontrolador, através de uma rede wifi manda uma requisição de escrita no MQQT client da AWS IoT, que por sua vez escreve no mesmo podendo ser observado na própria plataforma do AWS IoT.

![](http://i.imgur.com/5496wEx.png)	
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

