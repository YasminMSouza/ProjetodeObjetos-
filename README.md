# Monitoramento Ambiental de Animais Domésticos em Ambientes Externos

---

## Visão Geral

Este projeto implementa um **sistema de Internet das Coisas (IoT)** voltado ao monitoramento contínuo das condições ambientais de animais domésticos mantidos em áreas externas, com foco na prevenção de situações de risco à saúde dos pets.

O sistema foi desenvolvido com dispositivo físico, utilizando ESP32, sensor de temperatura DHT11 e sensor ultrassônico HC-SR04, com comunicação via MQTT e processamento em tempo real com Node-RED, InfluxDB e Grafana.

Em caso de variações críticas de temperatura ou nível crítico de ração, o sistema gera alertas automáticos via WhatsApp API.

O projeto está alinhado aos **Objetivos de Desenvolvimento Sustentável (ODS) 3** — Saúde e Bem-Estar — e **ODS 11** — Cidades e Comunidades Sustentáveis, da Agenda 2030 da ONU.

---

## 🎥 Vídeo de Apresentação

Assista à explicação completa do projeto:

> ⚠️ **[PREENCHER: inserir link do vídeo no YouTube após publicação]**

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
pet-monitoring-iot/
│
├── esp32/
│   ├── sketch.ino
│   └── bibliotecas.txt
│
├── node-red/
│   └── flows.json
│
├── grafana/
│   └── dashboard.jpeg
│
├── mqtt/
│   └── topics.md
│
├── influxdb/
│   └── setup.md
│
└── README.md
```

---

## Guia de Instalação e Execução

### 1. Clonar o repositório

```bash
git clone https://github.com/seu-usuario/pet-monitoring-iot.git
cd pet-monitoring-iot
```

---

### 2. Configurar o ESP32 (dispositivo físico)

- Abrir o arquivo `esp32/sketch.ino` na Arduino IDE
- Instalar as bibliotecas necessárias (listadas em `esp32/bibliotecas.txt`):
  - `WiFi.h`
  - `PubSubClient`
  - `DHT sensor library`
  - `NewPing` (para HC-SR04)
- Configurar as credenciais de Wi-Fi e o endereço do broker MQTT no sketch
- Realizar a calibração do HC-SR04 com o comedouro vazio (valor `d_max`)
- Compilar e gravar o firmware no ESP32

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

## Dashboards — Grafana

### Dashboard de Status Atual

Painéis:
- Nível de ração (indicador verde/laranja/vermelho)
- Temperatura ambiente (indicador verde/laranja/vermelho)
- Status de conectividade do sistema (online/offline)

### Dashboard de Histórico

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
