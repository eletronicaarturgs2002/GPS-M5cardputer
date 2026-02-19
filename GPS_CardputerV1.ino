#include "M5Cardputer.h"
#include <TinyGPS++.h>

#define RX_PIN 2
#define TX_PIN 1

const int UTC_OFFSET = -3; 
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

int paginaAtual = 1;

// Variáveis para o Odômetro
double distanciaTotalKM = 0.0;
double ultimaLat = 0.0;
double ultimaLon = 0.0;
bool primeiraLeitura = true;

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    gpsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    desenharInterface();
}

String obterDirecao(double graus) {
    if (graus >= 337.5 || graus < 22.5)  return "NORTE";
    if (graus >= 22.5  && graus < 67.5)  return "NORDESTE";
    if (graus >= 67.5  && graus < 112.5) return "LESTE";
    if (graus >= 112.5 && graus < 157.5) return "SUDESTE";
    if (graus >= 157.5 && graus < 202.5) return "SUL";
    if (graus >= 202.5 && graus < 247.5) return "SUDOESTE";
    if (graus >= 247.5 && graus < 292.5) return "OESTE";
    if (graus >= 292.5 && graus < 337.5) return "NOROESTE";
    return "---";
}

void desenharInterface() {
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.drawRect(5,  5, 225, 120, TFT_MAGENTA);
    M5Cardputer.Display.drawRect(10, 10, 215, 25, TFT_ORANGE);
    M5Cardputer.Display.drawRect(10, 40, 215, 80, TFT_ORANGE);

    M5Cardputer.Display.setTextColor(TFT_WHITE);
    if (paginaAtual == 1) M5Cardputer.Display.drawString("P1: POSICAO E HORA", 55, 18);
    else if (paginaAtual == 2) M5Cardputer.Display.drawString("P2: NAVEGACAO", 75, 18);
    else if (paginaAtual == 3) M5Cardputer.Display.drawString("P3: QUALIDADE SINAL", 55, 18);
}

void loop() {
    M5Cardputer.update();

    if (M5Cardputer.Keyboard.isKeyPressed('m')) {
        paginaAtual++;
        if (paginaAtual > 3) paginaAtual = 1;
        desenharInterface();
        delay(200);
    }

    // Reset do odômetro se segurar a tecla 'R'
    if (M5Cardputer.Keyboard.isKeyPressed('r')) {
        distanciaTotalKM = 0;
        desenharInterface();
    }

    while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
            // Lógica do Odômetro
            if (gps.location.isValid() && gps.speed.kmph() > 1.5) { // Só conta se estiver se movendo > 1.5km/h para evitar erro parado
                if (primeiraLeitura) {
                    ultimaLat = gps.location.lat();
                    ultimaLon = gps.location.lng();
                    primeiraLeitura = false;
                } else {
                    double distanciaMetros = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), ultimaLat, ultimaLon);
                    distanciaTotalKM += (distanciaMetros / 1000.0);
                    ultimaLat = gps.location.lat();
                    ultimaLon = gps.location.lng();
                }
            }
            exibirDados();
        }
    }
}

void exibirDados() {
    M5Cardputer.Display.setTextSize(1);
    
    if (paginaAtual == 1) {
        // PAGINA 1: HORA E POSIÇÃO
        M5Cardputer.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
        if (gps.time.isValid()) {
            int hora = (gps.time.hour() + UTC_OFFSET + 24) % 24;
            M5Cardputer.Display.setCursor(20, 50);
            M5Cardputer.Display.printf("HORA: %02d:%02d:%02d", hora, gps.time.minute(), gps.time.second());
        }
        M5Cardputer.Display.setTextColor(TFT_GREEN, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 65);
        M5Cardputer.Display.printf("LAT: %.6f", gps.location.lat());
        M5Cardputer.Display.setCursor(20, 80);
        M5Cardputer.Display.printf("LON: %.6f", gps.location.lng());
        M5Cardputer.Display.setTextColor(TFT_CYAN, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 100);
        M5Cardputer.Display.printf("ALTITUDE: %.1fm", gps.altitude.meters());

    } else if (paginaAtual == 2) {
        // PAGINA 2: VELOCIDADE, RUMO E DISTÂNCIA PERCORRIDA
        M5Cardputer.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 50);
        M5Cardputer.Display.printf("VEL: %.1f km/h", gps.speed.kmph());
        
        M5Cardputer.Display.setCursor(20, 65);
        M5Cardputer.Display.printf("RUMO: %s", obterDirecao(gps.course.deg()).c_str());

        // EXIBIÇÃO DO ODÔMETRO
        M5Cardputer.Display.setTextColor(TFT_ORANGE, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 85);
        if (distanciaTotalKM < 1.0) {
            M5Cardputer.Display.printf("DIST: %.1f metros", distanciaTotalKM * 1000.0);
        } else {
            M5Cardputer.Display.printf("DIST: %.2f km", distanciaTotalKM);
        }

        M5Cardputer.Display.setTextColor(TFT_DARKGREY, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 105);
        M5Cardputer.Display.printf("Segure 'R' para resetar");

    } else if (paginaAtual == 3) {
        // PAGINA 3: QUALIDADE DO SINAL
        M5Cardputer.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 50);
        M5Cardputer.Display.printf("SATELITES: %d", gps.satellites.value());
        M5Cardputer.Display.setCursor(20, 70);
        M5Cardputer.Display.printf("HDOP: %.2f", gps.hdop.hdop());
        M5Cardputer.Display.setCursor(20, 95);
        if (gps.hdop.hdop() < 2.5 && gps.hdop.hdop() > 0) {
            M5Cardputer.Display.setTextColor(TFT_GREEN, TFT_BLACK);
            M5Cardputer.Display.print("SINAL: EXCELENTE");
        } else {
            M5Cardputer.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
            M5Cardputer.Display.print("SINAL: OK / BUSCANDO");
        }
    }
}
