# Monitoramento Ambiental de Animais Domésticos em Ambientes Externos

---

## Visão Geral

Este projeto implementa um **sistema de Internet das Coisas (IoT)** voltado ao monitoramento contínuo das condições ambientais de animais domésticos mantidos em áreas externas, com foco na prevenção de situações de risco à saúde dos pets.

O sistema foi desenvolvido com dispositivo físico, utilizando ESP32, sensor de temperatura DHT11 e sensor ultrassônico HC-SR04, com comunicação via MQTT e processamento em tempo real com Node-RED, InfluxDB e Grafana.

Em caso de variações críticas de temperatura ou nível crítico de ração, o sistema gera alertas automáticos via WhatsApp API.

O projeto está alinhado aos **Objetivos de Desenvolvimento Sustentável (ODS) 3** - Saúde e Bem-Estar - e **ODS 11** - Cidades e Comunidades Sustentáveis, da Agenda 2030 da ONU.

---

## 🎥 Vídeo de Apresentação

Assista à explicação completa do projeto:

> ⚠️  https://www.youtube.com/watch?v=5fRXgu9us0I 

---

## Objetivos

- Monitorar temperatura ambiente e nível de ração da casinha do pet em tempo real
- Classificar as condições em faixas de status: normal (verde), atenção (laranja) e crítica (vermelho)
- Gerar alertas automáticos via WhatsApp quando parâmetros saírem das faixas seguras
- Armazenar histórico de leituras em banco de dados de série temporal
- Visualizar dados em dashboards gerenciais via Grafana

---

## Arquitetura do Sistema

Fluxo de dados:

```
ESP32 (DHT11 + HC-SR04) → MQTT Broker → Node-RED → InfluxDB → Grafana
                                                  ↓
                                         WhatsApp API (Alertas)
```

---

## Tecnologias Utilizadas

### Hardware (Dispositivo Físico)

- ESP32 DevKit V1
- Sensor de temperatura DHT11
- Sensor ultrassônico HC-SR04

### Software

- MQTT
- Node-RED (hospedado em instância EC2 da AWS)
- InfluxDB (banco de dados de série temporal em nuvem)
- Grafana (dashboards gerenciais)
- WhatsApp API (CallMeBot)
- Arduino IDE

---

## Estrutura do Repositório

```text
ProjetodeObjetos-/
│
├── esp32/
│   ├── sketch.ino
│  
├── node-red/
│   └── fluxo.json
│
├── grafana/
│   └── dashboard.jpeg
│
├── mqtt/
│   └── mqtt.md
│
├── influxdb/
│   └── db.jpeg
│
└── README.md
```

---

## Guia de Instalação e Execução

### 1. Clonar o repositório

```bash
git clone https://github.com/seu-usuario/ProjetodeObjetos-.git
cd ProjetodeObjetos-
```

---

### 2. Configurar o ESP32 (dispositivo físico)

**Materiais necessários:**
- ESP32 DevKit V1
- Sensor DHT11
- Sensor ultrassônico HC-SR04
- Cabos jumper
- Cabo USB (para conectar o ESP32 ao computador)

**Conexão dos componentes:**

| Componente | Pino do componente | Pino do ESP32 |
|---|---|---|
| DHT11 | VCC | 3.3V |
| DHT11 | GND | GND |
| DHT11 | DATA | GPIO 15 |
| HC-SR04 | VCC | 5V (VIN) |
| HC-SR04 | GND | GND |
| HC-SR04 | TRIG | GPIO 32 |
| HC-SR04 | ECHO | GPIO 35 |

**Configuração do ambiente:**

1. Instalar a [Arduino IDE](https://www.arduino.cc/en/software)
2. Abrir a Arduino IDE e ir em **File → Preferences** e adicionar a URL abaixo em *Additional boards manager URLs*:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json 
3. Ir em **Tools → Board → Boards Manager**, buscar por `esp32` e instalar
4. Instalar as bibliotecas necessárias em **Tools → Manage Libraries**:
   - `PubSubClient` (Nick O'Leary)
   - `DHT sensor library` (Adafruit)

**Configurar e gravar o código:**

1. Abrir o arquivo `esp32/sketch.ino` na Arduino IDE
2. Editar as linhas com suas credenciais:
```cpp
const char* ssid      = "SEU_WIFI";
const char* password  = "SUA_SENHA";
const char* mqtt_user = "SEU_USUARIO_MQTT";
const char* mqtt_pass = "SUA_SENHA_MQTT";
```
3. Conectar o ESP32 ao computador via cabo USB
4. Em **Tools → Board**, selecionar `ESP32 Dev Module`
5. Em **Tools → Port**, selecionar a porta COM correspondente ao ESP32
6. Clicar em **Upload** (seta →) para gravar o firmware
7. Abrir o **Serial Monitor** (Tools → Serial Monitor, 115200 baud) para confirmar que o ESP32 conectou ao Wi-Fi e ao broker MQTT
---

### 3. Configurar o Node-RED

- Importar o arquivo `node-red/flows.json`
- Configurar o endereço do broker MQTT e as credenciais de autenticação
- Configurar a integração com a WhatsApp API (CallMeBot)
- Realizar o deploy do fluxo

---

### 4. Configurar o InfluxDB

- Criar o banco de dados conforme instruções em `influxdb/setup.md`
- Configurar as *measurements*, *fields* e *tags* conforme o projeto
- Conectar ao Node-RED

---

### 5. Configurar o Grafana

- Adicionar o InfluxDB como fonte de dados
- Importar o dashboard disponível em `grafana/dashboard.jpeg`
- Verificar a visualização dos painéis de status atual e histórico

---

## Tópicos MQTT

| Tópico                     | Sensor        | Descrição                          |
|----------------------------|---------------|------------------------------------|
| `pets/casinha/temperatura` | DHT11         | Temperatura ambiente em °C         |
| `pets/comedouro/nivel`     | HC-SR04       | Nível de ração em percentual (%)   |

---

## Regras de Negócio

### Temperatura (DHT11)

| Faixa     | Condição                          | Ação do sistema                              |
|-----------|-----------------------------------|----------------------------------------------|
| 🟢 Verde  | 15 °C a 28 °C                     | Registro no InfluxDB, sem alerta             |
| 🟡 Laranja | Acima de 28 °C até 32 °C, ou abaixo de 15 °C | Notificação via WhatsApp         |
| 🔴 Vermelho | Acima de 32 °C ou abaixo de 10 °C | Alerta urgente via WhatsApp                |

### Nível de Ração (HC-SR04)

| Faixa     | Condição              | Ação do sistema                              |
|-----------|-----------------------|----------------------------------------------|
| 🟢 Verde  | Acima de 50%          | Registro no InfluxDB, sem alerta             |
| 🟡 Laranja | Entre 20% e 50%      | Notificação via WhatsApp                     |
| 🔴 Vermelho | Abaixo de 20%       | Alerta urgente via WhatsApp                  |

As notificações são disparadas apenas na **transição de faixa**, evitando excesso de alertas ao tutor.

---

## Sistema de Alertas

Quando uma condição de atenção ou crítica é detectada:

1. ESP32 envia dados via MQTT em tempo real
2. Node-RED processa as regras de negócio
3. WhatsApp API (CallMeBot) envia alerta automático ao tutor
4. InfluxDB registra o evento com timestamp
5. Grafana atualiza os dashboards

---

## Dashboards - Grafana

### Dashboard de Status Atual

Painéis:
- Nível de ração (indicador verde/laranja/vermelho)
- Temperatura ambiente (indicador verde/laranja/vermelho)
- Status de conectividade do sistema (online/offline)

### Dashboard de Histórico
<img width="931" height="430" alt="Captura de tela 2026-06-03 204656" src="https://github.com/user-attachments/assets/30783705-0045-4614-a8e4-0aabadc95b4e" />

Painéis:
- Registro do último abastecimento do comedouro
- Média semanal de consumo de ração
- Média semanal de temperatura registrada

---

## Dados Armazenados no InfluxDB

- **Fields (valores numéricos):** temperatura (°C) e percentual do nível de ração (%)
- **Tags (metadados):** status da temperatura e status do nível de ração (verde, laranja ou vermelho)

---

## Autores

- Bruna Zakaib
- Mariana Chiorboli
- Murilo Francisco
- Yasmin Mendes

**Orientado por:** Prof. Wallace Rodrigues de Santana — Universidade Presbiteriana Mackenzie (FCI)
