# 🛰️ M5Cardputer GPS Monitor

Este projeto transforma o **M5Stack Cardputer** em um monitor GPS multifuncional utilizando o módulo **GY-GPS6MV2**. O sistema possui uma interface dividida em três páginas navegáveis, permitindo visualizar desde dados de localização em tempo real até a qualidade da conexão com os satélites.

## 🚀 Funcionalidades

- **Página 1: Posicionamento e Hora**
  - Latitude e Longitude em tempo real.
  - Altitude em metros.
  - Horário local sincronizado via satélite (Ajustado para fuso UTC-3).

- **Página 2: Navegação**
  - Velocidade atual em km/h.
  - Rumo/Direção por extenso (Norte, Sul, Sudeste, etc.).
  - Resumo de coordenadas para referência rápida.
 
  - **Odômetro Digital (Novo!):**
  - Cálculo de distância percorrida em tempo real.
  - Alternância automática entre Metros e Quilômetros.
  - Função de Reset manual pressionando a tecla 'R'.

- **Página 3: Qualidade do Sinal**
  - Contador de satélites ativos.
  - Precisão horizontal (HDOP) com status visual (Excelente, OK ou Buscando).

- **Interface Personalizada:**
  - Sistema de troca de telas através da tecla **'M'** do teclado físico.
  - Molduras estilizadas nas cores Magenta e Laranja.

## 🛠️ Hardware Utilizado

- **M5Stack Cardputer** (ESP32-S3).
- **Módulo GPS GY-GPS6MV2**.
- **Modificação de Antena:** Soldagem de uma antena externa de maior ganho (extraída de modem) para melhorar a recepção de sinal em ambientes internos ou com nuvens pesadas.
- **Conexão:** Porta Grove (Pinos G1 e G2).

## 📂 Bibliotecas Necessárias

- [M5Cardputer](https://github.com/m5stack/M5Cardputer)
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)

## 🔧 Instalação

1. Configure o ambiente do Arduino IDE para o **M5Stack Cardputer**.
2. Instale as bibliotecas listadas acima através do Library Manager.
3. Conecte o módulo GPS na porta Grove.
4. Carregue o código fornecido no repositório.

## 📸 Demonstração da Interface

> [!TIP]
> Use a tecla **'M'** para alternar entre as visualizações de dados, navegação e status do sinal.

![WhatsApp Image 2026-02-16 at 21 00 30](https://github.com/user-attachments/assets/973fa05e-570c-426c-90de-4deb9fa5743d)
