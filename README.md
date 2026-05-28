# Sistema Inteligente para Monitoramento Ambiental de Animais Domésticos em Ambientes Externos

## Integrantes
- Bruna Zakaib
- Mariana Chiorboli
- Murilo Francisco
- Yasmin Mendes
- Wallace Santana

---

# Descrição do Projeto

O projeto consiste em uma casinha inteligente para pets mantidos em ambientes externos, utilizando IoT para monitorar continuamente a temperatura ambiente e o nível de ração disponível no comedouro.

Os sensores conectados ao ESP32 enviam os dados via MQTT para o Node-RED, responsável pelas regras de negócio e envio de notificações automáticas ao tutor. Os dados também são armazenados no InfluxDB e exibidos em dashboards no Grafana.

O sistema busca melhorar o bem-estar animal e está alinhado aos ODS 3 e ODS 11.

---

# Tecnologias Utilizadas

- ESP32
- MQTT
- Node-RED
- InfluxDB
- Grafana
- AWS EC2
- WhatsApp API / Telegram API

---

# Arquitetura do Sistema

```text
Sensores → ESP32 → Broker MQTT → Node-RED → InfluxDB → Grafana → Smartphone
```

---

# Regras de Negócio

## Temperatura
- Verde: 15°C a 28°C
- Laranja: acima de 28°C até 33°C ou abaixo de 15°C
- Vermelho: acima de 33°C ou abaixo de 10°C

## Nível de Ração
- Verde: acima de 50%
- Laranja: entre 20% e 50%
- Vermelho: abaixo de 20%

---

# Dashboard

## Status Atual
- Temperatura ambiente
- Nível da ração
- Status do sistema

## Histórico
- Último abastecimento
- Média semanal de temperatura
- Média semanal de consumo de ração

---

# Referências

