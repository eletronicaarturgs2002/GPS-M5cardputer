#include "M5Cardputer.h"
#include <TinyGPS++.h>

#define RX_PIN 2
#define TX_PIN 1

const int UTC_OFFSET = -3; 
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

int paginaAtual = 1;

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
    
    // Suas bordas originais
    M5Cardputer.Display.drawRect(5,  5, 225, 120, TFT_MAGENTA);
    M5Cardputer.Display.drawRect(10, 10, 215, 25, TFT_ORANGE);
    M5Cardputer.Display.drawRect(10, 40, 215, 80, TFT_ORANGE);

    // Título dinâmico baseado na página
    M5Cardputer.Display.setTextColor(TFT_WHITE);
    M5Cardputer.Display.setTextSize(1);
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

    while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
            exibirDados();
        }
    }
}

void exibirDados() {
    M5Cardputer.Display.setTextSize(1);
    
    if (paginaAtual == 1) {
        // PAGINA 1: HORA, LAT, LON, ALT
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
        // PAGINA 2: VEL, DIR, LAT, LON, ALT
        M5Cardputer.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 50);
        M5Cardputer.Display.printf("VELOCIDADE: %.1f km/h", gps.speed.kmph());
        
        M5Cardputer.Display.setCursor(20, 65);
        M5Cardputer.Display.printf("RUMO: %s", obterDirecao(gps.course.deg()).c_str());

        M5Cardputer.Display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 85);
        M5Cardputer.Display.printf("L:%.3f / L:%.3f", gps.location.lat(), gps.location.lng());
        M5Cardputer.Display.setCursor(20, 100);
        M5Cardputer.Display.printf("ALT: %.1fm", gps.altitude.meters());

    } else if (paginaAtual == 3) {
        // PAGINA 3: SATS E HDOP
        M5Cardputer.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        M5Cardputer.Display.setCursor(20, 50);
        M5Cardputer.Display.printf("SATELITES: %d", gps.satellites.value());
        
        M5Cardputer.Display.setCursor(20, 70);
        M5Cardputer.Display.printf("PRECISAO (HDOP): %.2f", gps.hdop.hdop());
        
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