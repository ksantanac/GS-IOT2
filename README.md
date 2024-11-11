## Descrição do Projeto
Este projeto utiliza duas placas ESP32 e sensores de entrada (um potenciômetro e um pino para simulação do nível de bateria) para medir a produção solar e o nível de bateria. A comunicação entre as placas ESP32 é realizada através do protocolo MQTT, utilizando um broker Mosquitto público. Além disso, o Node-RED é utilizado para criar um dashboard que exibe esses valores em tempo real.

## Integrantes 

- Kaue Matheus Santana - RM551732;
- Gustavo René dias Boamorte - RM551288 
- João Pedro Costa Feitosa - RM98442 
- Pedro Felipe Barros da Silva - RM98093 
- Igor Miguel Silva - RM99495

## Arquitetura

![Arquitetura do Sistema](images/arquitetura.png)

## Vídeo da Solução
<!-- Link: https://youtu.be/km0kCXDhtS0?si=8Hdd72nN5IADp1p1 -->

<!-- [![Assista ao vídeo](https://img.youtube.com/vi/78C82yDRMc4/0.jpg)](https://youtu.be/km0kCXDhtS0?si=8Hdd72nN5IADp1p1) -->

## Funcionamento

### Publicação da Produção Solar e Nível de Bateria
A primeira ESP32 está conectada a dois sensores de entrada:
- **Potenciômetro (para simular a produção solar)**: O valor do potenciômetro é lido e mapeado para um valor de 0 a 100, representando a produção de energia solar.
- **Nível de Bateria**: Um pino de entrada é utilizado para simular o nível de bateria, e o valor é mapeado de 0 a 100 para representar a porcentagem de carga.

A ESP32 publica os valores de produção solar e nível de bateria em dois tópicos MQTT separados a cada 2 segundos:
- Tópico `solarPower`: Contém o valor da produção solar.
- Tópico `batteryLevel`: Contém o valor do nível de bateria.

### Recepção e Visualização dos Dados
O Node-RED é utilizado para criar um dashboard de monitoramento em tempo real. O Node-RED se inscreve nos tópicos MQTT `solarPower` e `batteryLevel`, e exibe os valores recebidos em tempo real.

## Fluxo de Dados
- **Placa ESP32 (Publicação)**: A placa ESP32 lê os valores do potenciômetro e do pino de nível de bateria, e publica esses valores nos tópicos MQTT `solarPower` e `batteryLevel`.
- **Broker Mosquitto**: O broker Mosquitto recebe as mensagens publicadas pela primeira ESP32 e as repassa para os dispositivos que estiverem inscritos nesses tópicos.
- **Node-RED**: O Node-RED consome os dados dos tópicos MQTT e exibe os valores em um gráfico ou indicador visual.

## Tecnologias Utilizadas
- **ESP32**: Microcontrolador utilizado para capturar e transmitir os dados de produção solar e nível de bateria.
- **MQTT**: Protocolo de comunicação para IoT utilizado para transmitir dados entre dispositivos.
- **Mosquitto**: Broker MQTT que gerencia a publicação e inscrição de mensagens.
- **Node-RED**: Plataforma de automação utilizada para criar o dashboard de controle e visualização dos dados.

## Conexões
- **Placa ESP32 1 (Publicação):**
  - **Potenciômetro** conectado ao pino 34, para medir a produção solar.
  - **Simulação do nível de bateria** conectada ao pino 35.
  - Publica os dados de produção solar e nível de bateria via MQTT para o broker.

- **Placa ESP32 2 (Recepção):**
  - Se inscreve nos tópicos MQTT `solarPower` e `batteryLevel` para receber os dados de produção solar e nível de bateria.
  - Pode realizar ações com base nos dados recebidos, como acender um LED, exibir os dados em um display ou enviar comandos para outros dispositivos.

- **Node-RED (Recepção):**
  - Recebe os dados de produção solar e nível de bateria e exibe em tempo real no dashboard.


## Funcionamento

Publisher: https://wokwi.com/projects/414283382978871297

Subscriber: https://wokwi.com/projects/412509715139780609