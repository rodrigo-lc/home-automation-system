# Home-Automation-System (HAS)
O HAS é um sistema de automação residencial, ao qual existem alguns módulos que se comunicam através de um serviço de núvem.

Requerimentos:
1. **Controle de acesso**
    1. Um *controle de acesso* com reconhecimento de rosto (por exemplo: abrir uma trava elétrica de 12V)
    2. Uma interface de registro, permitindo acesso por reconhecimento de face para vários usuários
    3. Um registro de dados contendo uma lista de eventos de acesso anteriores (válidos, inválidos e pânico) com a respectiva hora e data
    
2. **Interface**
    1. Uma interface web para monitoramento de dados do sensor
    2. Uma interface da web que fornece o registro de dados de eventos de acesso e o status do sensor
    3. Uma interface de dados local que contém o registro de dados de eventos de acesso
  
3. **Segurança dos dados**
    1. Dados criptografados no servidor web
    
4. **rede sem fio**
    1. Os sensores sem fio devem comunicar com um gateway seus respectivos estados e valores de sensores
   
5. **Monitores e atuadores**
    1. Câmera (para reconhecimento de rosto / controle de acesso)
    2. Sensores de temperatura sem fio (bateria)
    3. Sensores de janela e porta abertos / fechados
    4. Switches inteligentes (tomadas inteligentes)
    5. Motorista para controlar portas e persianas
    6. O sistema deve ter um relógio de tempo real para fornecer registro de hora e data

6. **Consumo de energia**
    1. Os sensores sem fio devem ter mais de 2 anos de vida útil
    
# Análise de mercado:
A equipe reuniu informações sobre microprocessadores com interfaces sem fio. Aqui está o resultado:

![Market analysis for microprocessors with wireless interfaces](docs/img/market_analysis.png)  

# Diagrama de blocos do sistema
  ![System Diagram](docs/img/block_diagram.jfif)  

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

# ESP-32

O esp 32 é um microcontrolador relativamente barato com GPIOs, ADC, periférico wifi e BLE. Associado a isso, ele também tem baixo consumo de energia, sendo uma boa escolha de baixo de custo para aplicações IoT. Por esses motivos esse foi o microcontrolador escolhido para fazer o projeto.

![](docs/img/esp-wroom32.jpg)

[Especificações](https://www.filipeflop.com/produto/modulo-wifi-esp32-bluetooth/#tab-blogrelacionados):

– CPU: Xtensa® Dual-Core 32-bit LX6

– ROM: 448 KBytes

– RAM: 520 Kbytes

– Flash: 4 MB

– Clock máximo: 240MHz

– Wireless padrão 802.11 b/g/n

– Conexão Wifi 2.4Ghz (máximo de 150 Mbps)

– Antena embutida

– Conector micro-usb

– Wi-Fi Direct (P2P), P2P Discovery, P2P Group Owner mode e P2P Power Management

– Modos de operação: STA/AP/STA+AP

– Bluetooth BLE 4.2

– Portas GPIO: 11

– GPIO com funções de PWM, I2C, SPI, etc

– Tensão de operação: 4,5 ~ 9V

– Taxa de transferência: 110-460800bps

– Suporta Upgrade remoto de firmware

– Conversor analógico digital (ADC)

## ESP-IDF

Há um framework de desenvolvimento onde há varias aplicações já implementadas em exemplos, na documentação há todas as informações necessárias para rodar compilar e escrever na flash do microcontrolador

https://github.com/espressif/esp-idf
https://docs.espressif.com/projects/esp-idf/en/latest/

## [Espressif AWS IoT](https://github.com/espressif/esp-aws-iot)

Para a utilização do ESP-32 em conjunto com os serviços da Amazon, há um repositório fornecido pela empresa com um tutorial de como criar um dispositivo remoto na AWS e certificar o ESP-32 como esse dispositivo. Após isto, o exemplo fornecido pela espressif pode ser compilado e gravado no microcontrolador, em conjunto com a própria documentação do ESP-32.

Após todo o procedimento descrito anteriormente o microcontrolador, através de uma rede wifi manda uma requisição de escrita no MQQT client da AWS IoT, que por sua vez escreve no mesmo podendo ser observado na própria plataforma do AWS IoT.

![](http://i.imgur.com/5496wEx.png)	

https://github.com/espressif/esp-aws-iot

# Resultados

Há basicamente 3 módulos desenvolvidos no projeto:

-   Camera
-   Sensores
-   Atuadores

Sendo cada um desses módulos responsável por algum tipo de operação no sistema.

## Módulo camera

O módulo camera é responsável pelo cadastro de faces e reconhecimento facil do sistema, que por sua vez, ativa um atuador quando reconhecida uma face. A seguir uma imagem do módulo.

![](http://i.imgur.com/mtbfi2w.png)

O processamento de dados é baseado em dois frameworks [esp-who](https://github.com/espressif/esp-who) e [esp-face](https://github.com/espressif/esp-face). O módulo se conecta numa rede wifi, juntamente com um computador que tem uma interface html, onde é possível detectar-se uma pessoa não cadastrada (intruder alert), apertando "enroll face" é possível o cadastro de uma face e então após o cadastro o rosto é detectado como "sample" e quando ocorre uma detecção o módulo envia uma string para o servidor AWS que é lida por um módulo atuador, abrindo a porta de entrada da residencia. A seguir 3 imagens mostrando o processo de cadastramento.

![](http://i.imgur.com/JPnewno.png)

![](http://i.imgur.com/zF3MYnT.png)

![](http://i.imgur.com/haRdEfe.png)


## Módulo atuador

O módulo atuador é responsável por ligar um relé. Esse relé funciona como chave de uma tomada no nosso caso. A seguir duas imagens mostrando o módulo.

![](http://i.imgur.com/QQb5Upu.png)

![](http://i.imgur.com/Z5VsBkE.png)

Este módulo utiliza uma fonte retificadora de 220 AC para 5V DC, um ESP-32 e um Relé de 220V/10A.

O funcionamento é bastante simple, este módulo está inscrito em um tópico MQTT. Toda vez que algo é publicado neste tópico, o módulo analisa a mensagem e toma uma ação dependendo do conteúdo.

Nesse caso existem 3 mensagens possíveís:

-   Acesso concedido!
    Ativa o relé por um pequeno instante de tempo simulando uma tranca e depois o desativa
-   Liga tomada
    Ativa o relé
-   Desliga tomada
    Desativa o relé

## Módulo sensor

O módulo sensor trabalha com GPIOs e ADCs para o tratamento de dados de sensores analógicos e digitais. Para a exemplificação de ambos, foram utilizados:

-   LM35 (Analógico)
-   E18D80NK (Digital)

Para a adequação do sinal do LM35 na porta do ADC foi utlizado um circuito passa baixa com ganho demonstrado na próxima figura:

![](http://i.imgur.com/HRRy3G1.png)

De forma parecida com o módulo do atuador, o módulo do sensor envia mensagens com os valores dos sensores, e essas mensagens podem ser armazenadas em um serviço de banco de dados da aws através de [regras](https://docs.aws.amazon.com/pt_br/iot/latest/developerguide/iot-ddb-rule.html) aplicadas no serviço de IoT. Essas regras que manipulam o banco de dados segundo algum comando escrito em um tópico especifico. A seguir imagens da implementação:

![](http://i.imgur.com/PVxH9lD.png)
![](http://i.imgur.com/b0Co8fi.png)

A seguir algumas imagens do módulo, este alimentado a bateria

![](http://i.imgur.com/q6ELyZh.png)
![](http://i.imgur.com/xzrGe74.png)

# Ferramentas utilizadas
  - 1x ESP32-CAM 
  - 2x ESP32-01 
  - 1x Segger J-Link
  - 1x E18D80NK
  - 1x LM35
  - 1x LM324
  - 1x AC 220V/DC 5V 
  - 1x Relé 5V
  
# Considerações finais

Todos os módulos, mesmo tendo funções bem diferentes se comunicavam com a núvem, tanto para envio quanto recepção dos dados, além de suas funcionalidades locais. O sistema como um todo se integrava de forma simples e modular, uma vez feito de desenvolvimento da parte de IoT, esta poderia ser replicada para qualquer outro tipo de funcionalidade.

Os dados adquiridos pelo sensor de temperatura foram escritos numa tabela do banco de dados da amazon para possível acesso das informações posteriormente.

No porteiro (Módulo camera) é feito processamento de imagem, com cadastro e reconhecimento dos rostos cadastrados funcionando em tempo real, mandando informações a respeito desse reconhecimento para a internet atingindo os requisitos do projeto.

  
# Contribuições
Rodrigo Belisário Ramos (rodbelisario)<br/>Rodigo Luiz da Costa (rodrigo-lc)<br/>Tarcis Aurélio Becher (tarciszera)

