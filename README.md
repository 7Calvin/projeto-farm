# 🌿 Projeto Calvin - Automação Indoor com ESP32 + RuuviTag

Sistema de automação agrícola baseado em ESP32, com leitura de umidade do solo em 5 vasos, controle manual de relés (água, luz, aquecedor e ventilação) via interface web, e monitoramento ambiental (temperatura, umidade relativa e pressão atmosférica) utilizando um beacon BLE RuuviTag.

---

## 📦 Componentes Utilizados

### 🧠 Microcontrolador

* [x] **ESP32 Dev Module** (com Wi-Fi + BLE)

### 🌱 Sensores

* [x] 5 sensores de umidade do solo resistivos ou capacitivos (analógicos)

### 🔌 Relés

* [x] 4 canais de relé para controlar:

  * Água (RELAY\_AGUA) – GPIO 26
  * Luz (RELAY\_LUZ) – GPIO 25
  * Aquecedor (RELAY\_AQUECEDOR) – GPIO 33
  * Ventilação (RELAY\_VENTILACAO) – GPIO 32

### 🌡️ Monitoramento Ambiental

* [x] **RuuviTag (BLE Beacon)** – envia dados de temperatura, umidade e pressão via BLE Advertising

---

## 📚 Bibliotecas Necessárias

Instale via **Arduino IDE** > **Gerenciar Bibliotecas...** ou `platformio.ini` se usar PlatformIO.

```cpp
#include <WiFi.h>           // Conexão Wi-Fi
#include <WebServer.h>      // Servidor HTTP embarcado
#include <BLEDevice.h>      // Core BLE para ESP32
#include <BLEUtils.h>       // Utilitários BLE
#include <BLEScan.h>        // Scanner BLE para anúncios (advertising)
```

Essas bibliotecas são padrão no ESP32 core para Arduino:

* Instale o suporte ao ESP32 no Arduino IDE (via Board Manager):
  URL do gerenciador de placas:
  `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

---

## 🛠️ Pinos Utilizados

| Função          | GPIO     |
| --------------- | -------- |
| Umidade Solo 1  | 34 (ADC) |
| Umidade Solo 2  | 35 (ADC) |
| Umidade Solo 3  | 32 (ADC) |
| Umidade Solo 4  | 33 (ADC) |
| Umidade Solo 5  | 25 (ADC) |
| Relé Água       | 26       |
| Relé Luz        | 25       |
| Relé Aquecedor  | 33       |
| Relé Ventilação | 32       |

---

## 🌐 Configuração da Rede Wi-Fi

```cpp
const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA_WIFI";

IPAddress local_IP(192, 168, 1, 105);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
```

> Atribui IP estático ao ESP32.

---

## 📋 Como Usar

### 🔌 Etapas de Instalação

1. Instale o **Arduino IDE** com suporte ao **ESP32**
2. Instale as bibliotecas listadas acima
3. Faça upload do código para o ESP32
4. Ligue o ESP32 e acesse o IP local definido (ex: [http://192.168.1.105](http://192.168.1.105))
5. Na interface web você verá:

   * Leitura dos 5 sensores de solo
   * Temperatura, umidade e pressão do RuuviTag (se estiver por perto)
   * Botões para ligar/desligar Água, Luz, Aquecedor e Ventilação

Claro! Aqui está uma seção mais completa e profissional para incluir no seu **README**, explicando detalhadamente **como configurar do zero** a IDE Arduino para programar o ESP32, instalar as bibliotecas corretas e preparar tudo para rodar seu projeto com sucesso.

---

## ⚙️ Configuração Completa do Ambiente Arduino IDE para ESP32

Este projeto utiliza um ESP32 com conexão Wi-Fi e BLE, além de sensores analógicos e controle de relés. Abaixo está um passo a passo completo para configurar sua máquina para compilar e executar este código corretamente, mesmo em uma instalação **nova** da Arduino IDE.

---

### ✅ 1. Instalar a Arduino IDE

Se ainda não tiver:

* Acesse: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
* Baixe e instale a versão recomendada para o seu sistema operacional (Windows, macOS ou Linux)

---

### ✅ 2. Adicionar suporte à placa ESP32

**Passo 1 – Adicionar URL do gerenciador de placas:**

1. Abra a Arduino IDE
2. Vá em **`Arquivo` → `Preferências`**
3. Em **"URLs Adicionais para Gerenciadores de Placas"**, adicione:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

> Caso já haja outra URL, separe por vírgulas.

**Passo 2 – Instalar a plataforma ESP32:**

1. Vá em **`Ferramentas` → `Placa` → `Gerenciador de Placas...`**
2. No campo de busca digite: `esp32`
3. Instale o pacote: **`esp32 by Espressif Systems`**

Após a instalação:

* Selecione a placa correta em **Ferramentas → Placa**:
  ➤ `ESP32 Dev Module` ou equivalente à sua placa ESP32

---

### ✅ 3. Selecionar Parâmetros da Placa

Ajuste os seguintes parâmetros em **Ferramentas → ...**

| Parâmetro        | Valor Recomendado                                |
| ---------------- | ------------------------------------------------ |
| Placa            | ESP32 Dev Module                                 |
| Flash Frequency  | 80 MHz                                           |
| Upload Speed     | 115200                                           |
| Flash Mode       | QIO                                              |
| Partition Scheme | Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS) |
| Porta            | Selecione a COM correta                          |

---

### 📚 4. Instalar as bibliotecas necessárias

Abra o **Gerenciador de Bibliotecas**:

**Menu:** `Sketch → Incluir Biblioteca → Gerenciar Bibliotecas...`

Depois, instale as bibliotecas abaixo (pesquise pelo nome exato e clique em **Instalar**):

| Biblioteca    | Nome no Gerenciador                  | Autor                           |
| ------------- | ------------------------------------ | ------------------------------- |
| **WiFi**      | `WiFi` (incluída no core ESP32)      | Espressif                       |
| **WebServer** | `WebServer` (incluída no core ESP32) | Espressif                       |
| **BLEDevice** | `ESP32 BLE Arduino`                  | Neil Kolban, Evandro Luis, etc. |

> **Importante**: A `ESP32 BLE Arduino` é essencial para que o ESP32 funcione como scanner BLE e consiga interpretar os dados do RuuviTag.

---

### 🧪 5. Verificar Compilação

Com tudo instalado:

1. Copie e cole o código principal no seu sketch Arduino.
2. Conecte o ESP32 via USB.
3. Vá em `Ferramentas → Porta` e selecione a porta COM do seu ESP32.
4. Clique em **"Verificar"** (ícone ✓) para compilar.
5. Clique em **"Upload"** (seta →) para enviar ao ESP32.

---

### 🖥️ 6. Monitor Serial e IP da Interface Web

Após o upload:

1. Abra o **Monitor Serial** em 115200 baud
2. Verifique se a conexão com a rede Wi-Fi foi feita com sucesso.
3. O ESP32 usará o IP `192.168.1.105` (IP fixo configurado no código).
4. Abra um navegador e acesse:
   👉 **[http://192.168.1.105](http://192.168.1.105)**

---

### 📌 Resumo das Dependências

Essas bibliotecas são automaticamente carregadas ao instalar o suporte ao ESP32:

```cpp
#include <WiFi.h>         // Conexão com rede Wi-Fi
#include <WebServer.h>    // Servidor HTTP embarcado
#include <BLEDevice.h>    // BLE base para scanner e GAP
#include <BLEUtils.h>     // Utilitários adicionais BLE
#include <BLEScan.h>      // Scanner BLE para advertising (RuuviTag)
```

---

Se desejar, posso também gerar um script `.sh` para automação da instalação no Linux, ou um `platformio.ini` se você preferir migrar para PlatformIO no VSCode.

Deseja isso também?





### 🟢 Interface Web

* A interface é responsiva e mostra dados em tempo real após cada acesso.
* Os botões enviam comandos GET para `/toggle`.

---

## 📡 Detecção do RuuviTag

* O RuuviTag deve estar anunciando (BLE Advertising ativo)
* O ESP32 verifica o `MAC` do RuuviTag (`CE:77:73:81:BD:D5`) e decodifica os dados do **ManufacturerData**.
* Cálculos aplicados:

```cpp
ruuviTemp = temp_raw * 0.005;
ruuviHum = hum_raw * 0.0025;
ruuviPress = (press_raw + 50000) / 100.0;
```

---

## ⚠️ Observações e Limitações

* O uso extensivo de bibliotecas e BLE pode ocupar >90% da memória flash. Evite adicionar mais recursos sem otimização.
* Sensores resistivos desgastam com o tempo; prefira sensores capacitivos de solo.
* O sistema usa **pull polling** dos sensores a cada carregamento da página.
* Verifique o mapeamento correto de pinos caso use relés com lógica inversa (ativa em LOW).

---

## 🧪 Teste Rápido do RuuviTag

1. Verifique no monitor serial se o MAC do Ruuvi aparece.
2. Cheque a linha:

   ```
   Ruuvi: temp=18.9C hum=68.8% press=1012.5hPa
   ```
3. Se não aparecer, verifique:

   * Distância do beacon
   * Bateria
   * Se está enviando via modo RAW ou normal (modo Advertising)

---

## 📸 Exemplo de Interface Web

![Exemplo da interface](inserir_captura_aqui.png)

---

## 🔧 Futuras Melhorias

* Adicionar OTA para atualização remota do firmware
* Gravar dados em cartão SD ou Firebase
* Controle automático com base nas leituras
* Alertas por e-mail/Telegram

---

Se quiser, posso gerar um `.pdf` completo com screenshots e diagrama de ligação dos sensores e relés.

Deseja que eu gere esse PDF ou adicione ao Canvas para documentar junto ao código?
