# 📡 Tópicos MQTT — Sistema de Monitoramento Ambiental de Pets

## 📌 Visão Geral

Este projeto utiliza o protocolo MQTT para monitoramento em tempo real da temperatura ambiente e do nível de ração de animais domésticos mantidos em ambientes externos, utilizando ESP32, sensor DHT11 e sensor ultrassônico HC-SR04.

O sistema permite monitoramento contínuo e geração de alertas automáticos ao tutor quando as condições saem das faixas seguras.

---

## 🏗️ Arquitetura do Sistema

```text
ESP32 + DHT11 + HC-SR04
        ↓
Broker MQTT (HiveMQ)
        ↓
Node-RED / Sistema de Monitoramento
        ├──→ InfluxDB (armazenamento)
        ├──→ Grafana (dashboards)
        └──→ API WhatsApp (alertas)
                    ↓
               Tutor do Animal
```

---

## 📡 Broker MQTT Utilizado

```text
broker.hivemq.com
```

Porta utilizada:

```text
1883
```

---

## 📂 Tópicos MQTT Utilizados

### Tópico 1 — Temperatura

```text
pets/casinha/temperatura
```

Responsável pelo envio dos dados de temperatura coletados pelo sensor DHT11.

### Tópico 2 — Nível de Ração

```text
pets/comedouro/nivel
```

Responsável pelo envio do percentual de ração disponível no comedouro, calculado a partir das leituras do sensor HC-SR04.

---

## 📊 Estrutura dos Payloads

### Payload de Temperatura

```json
{
  "temperatura": 29.5,
  "status": "laranja"
}
```

### Payload de Nível de Ração

```json
{
  "nivel_percentual": 18.3,
  "status": "vermelho"
}
```

---

## 🔎 Assinatura MQTT

### Escutar dados de temperatura

```text
pets/casinha/temperatura
```

### Escutar dados de nível de ração

```text
pets/comedouro/nivel
```

### Escutar todos os tópicos do projeto

```text
pets/#
```

---

## ⚙️ QoS Utilizado

| QoS   | Finalidade                          |
|-------|-------------------------------------|
| QoS 0 | Envio contínuo de telemetria        |
| QoS 1 | Alertas críticos (entrega garantida)|

---

## 💾 Retained Messages

Mensagens retidas podem ser utilizadas para manter o último valor conhecido de temperatura e nível de ração disponível para novos clientes conectados ao broker, garantindo que o dashboard exiba sempre a leitura mais recente mesmo após reconexão.

---

## 📈 Funcionalidades

- Monitoramento em tempo real de temperatura e nível de ração
- Classificação por faixas de status: verde, laranja e vermelho
- Integração com Node-RED para processamento das regras de negócio
- Armazenamento histórico no InfluxDB
- Dashboards gerenciais no Grafana
- Alertas automáticos via WhatsApp (CallMeBot)

---

## 🛠️ Tecnologias Utilizadas

- ESP32
- DHT11
- HC-SR04
- MQTT
- HiveMQ
- Node-RED
- InfluxDB
- Grafana
- WhatsApp API (CallMeBot)

---

## 📄 Fluxo de Funcionamento

```text
ESP32 lê temperatura (DHT11) e nível de ração (HC-SR04)
        ↓
Publica nos tópicos:
pets/casinha/temperatura
pets/comedouro/nivel
        ↓
Broker HiveMQ recebe as mensagens
        ↓
Node-RED processa os dados e aplica as regras de negócio
        ↓
InfluxDB armazena as leituras com timestamp
        ↓
Grafana atualiza os dashboards (status atual e histórico)
        ↓
WhatsApp envia alertas automáticos ao tutor
(somente em transição de faixa de status)
```

---

## 🚦 Regras de Status por Tópico

### `pets/casinha/temperatura`

| Status     | Condição                                  |
|------------|-------------------------------------------|
| 🟢 Verde   | 15 °C a 28 °C                             |
| 🟡 Laranja | Acima de 28 °C até 32 °C, ou abaixo de 15 °C |
| 🔴 Vermelho| Acima de 32 °C ou abaixo de 10 °C        |

### `pets/comedouro/nivel`

| Status     | Condição            |
|------------|---------------------|
| 🟢 Verde   | Acima de 50%        |
| 🟡 Laranja | Entre 20% e 50%     |
| 🔴 Vermelho| Abaixo de 20%       |