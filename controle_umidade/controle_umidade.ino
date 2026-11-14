// Controle de umidade - ar Samsung AR18BSHZ via infravermelho
// Placa: Arduino UNO (RoboCore) na COM5 | DHT11 no pino 4 | LED IR (2N2222) no pino 3
// Versão em execução em 2026-11-10 (ver PENDENCIAS.md)

#define DISABLE_CODE_FOR_RECEIVER
#include <IRremote.hpp>
#include <DHT.h>

// ===== CONFIGURAÇÃO =====
#define MODO_TESTE false  // true = ao ligar, aciona o ar por 20 s e desliga
#define PINO_DHT 4
#define PINO_IR  3

const float UMIDADE_LIGA    = 55.0;
const float UMIDADE_DESLIGA = 45.0;
const unsigned long MIN_LIGADO    = 20UL * 60 * 1000;  // 20 min
const unsigned long MIN_DESLIGADO = 10UL * 60 * 1000;  // 10 min
// ========================

// Sinais capturados do controle Samsung (Dry, 27 °C)
const uint16_t CMD_LIGAR[] PROGMEM = {580,12770, 2930,8970, 480,470, 530,1470, 480,520, 480,520, 430,520, 530,470, 480,520, 480,520, 480,520, 430,1570, 430,520, 480,520, 480,1520, 480,470, 480,520, 480,1520, 430,1520, 480,1520, 480,1470, 480,1520, 480,520, 480,520, 480,520, 430,520, 480,520, 480,520, 480,520, 480,470, 530,470, 530,470, 480,520, 480,520, 430,520, 480,520, 480,520, 480,520, 430,570, 480,520, 430,520, 480,520, 480,520, 430,570, 430,570, 430,520, 480,520, 480,520, 480,520, 430,520, 530,470, 480,520, 480,520, 480,520, 430,1520, 480,1520, 430,1570, 430,1520, 530,2920, 2980,8920, 480,1470, 530,470, 480,520, 480,520, 480,520, 430,520, 530,470, 480,520, 480,520, 430,1520, 480,520, 480,520, 430,1570, 480,520, 430,1520, 480,1520, 430,1520, 530,1470, 430,1520, 480,1520, 480,520, 430,520, 480,520, 480,520, 480,520, 480,470, 480,570, 430,520, 480,520, 430,570, 480,520, 480,520, 430,520, 530,470, 480,520, 480,520, 430,520, 530,470, 480,520, 480,520, 480,470, 480,520, 480,570, 430,520, 480,520, 480,520, 430,570, 430,520, 480,520, 480,520, 480,520, 480,470, 530,470, 480,520, 480,520, 480,520, 430,3020, 2930,8920, 480,1520, 480,520, 480,520, 430,570, 430,520, 480,520, 430,570, 480,520, 430,520, 480,1520, 480,520, 480,470, 480,1520, 480,520, 480,1470, 480,1520, 480,520, 480,1470, 480,1520, 430,1570, 480,1470, 480,1520, 480,520, 480,1470, 480,1520, 480,520, 430,520, 530,470, 480,1520, 430,1520, 480,1520, 480,520, 430,570, 430,570, 430,520, 480,520, 480,1520, 480,1470, 480,520, 480,1470, 480,1520, 480,520, 480,520, 480,470, 480,570, 430,1520, 430,570, 430,570, 430,520, 480,520, 480,520, 480,520, 480,1470, 480,1520, 480,1470, 480,1520, 480};

const uint16_t CMD_DESLIGAR[] PROGMEM = {580,12770, 2980,8920, 530,420, 530,1470, 480,520, 480,520, 480,470, 530,470, 480,520, 530,470, 480,520, 480,1470, 430,570, 480,520, 480,1470, 530,1470, 530,470, 530,1420, 530,1470, 480,1470, 530,1470, 480,1520, 480,520, 480,520, 380,570, 430,570, 430,570, 480,520, 480,470, 530,470, 530,470, 480,520, 530,470, 480,470, 530,470, 530,470, 480,520, 480,470, 530,470, 530,470, 530,470, 530,470, 430,570, 380,620, 480,470, 530,470, 480,520, 480,520, 530,420, 530,470, 480,520, 480,520, 530,420, 530,470, 530,470, 530,470, 480,1470, 530,1470, 530,2920, 2980,8920, 430,1570, 430,520, 530,470, 480,520, 480,520, 530,420, 530,470, 530,470, 530,470, 530,1420, 530,470, 530,470, 480,1470, 530,520, 430,1520, 430,1570, 430,1520, 530,1470, 530,1420, 530,1470, 530,470, 480,470, 530,470, 530,470, 530,470, 480,520, 480,470, 530,520, 430,520, 430,570, 430,570, 480,520, 480,470, 530,470, 530,470, 480,520, 480,520, 480,470, 530,470, 480,520, 480,520, 480,470, 530,470, 530,470, 530,470, 480,520, 480,520, 380,620, 380,570, 530,470, 480,520, 530,470, 530,420, 530,470, 530,470, 530,470, 530,2920, 2980,8870, 530,1470, 480,520, 530,420, 530,470, 530,470, 480,520, 480,520, 430,570, 430,570, 480,1470, 530,470, 530,470, 530,1420, 530,1470, 480,1470, 530,1470, 530,470, 480,1470, 530,1470, 480,1520, 380,1570, 530,1470, 530,470, 480,1470, 530,1470, 480,470, 530,470, 530,470, 480,1520, 480,1470, 480,1520, 480,520, 430,570, 380,570, 480,520, 480,520, 480,1520, 480,1470, 480,520, 480,1470, 530,1470, 480,520, 530,470, 480,470, 530,470, 480,1520, 480,520, 430,570, 380,570, 530,470, 480,520, 480,520, 530,470, 480,470, 530,1470, 480,1470, 530};

DHT dht(PINO_DHT, DHT11);
bool arLigado = false;
unsigned long ultimaTroca = 0;
unsigned long numLeitura = 0;

void enviarComando(bool ligar) {
  if (ligar)
    IrSender.sendRaw_P(CMD_LIGAR, sizeof(CMD_LIGAR) / sizeof(CMD_LIGAR[0]), 38);
  else
    IrSender.sendRaw_P(CMD_DESLIGAR, sizeof(CMD_DESLIGAR) / sizeof(CMD_DESLIGAR[0]), 38);
  arLigado = ligar;
  ultimaTroca = millis();
  Serial.println(ligar ? F(">> AR LIGADO (Dry)") : F(">> AR DESLIGADO"));
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  IrSender.begin(PINO_IR);
  delay(2000);

  if (MODO_TESTE) {
    Serial.println(F("TESTE: ligando o ar..."));
    enviarComando(true);
    delay(20000);
    Serial.println(F("TESTE: desligando o ar..."));
    enviarComando(false);
  } else {
    enviarComando(false);
    ultimaTroca = millis() - MIN_DESLIGADO;  // permite ligar já na primeira leitura
  }
}

void loop() {
  delay(30000);  // lê a cada 30 s

  float umidade = dht.readHumidity();
  float temp    = dht.readTemperature();
  if (isnan(umidade)) { Serial.println(F("Falha no sensor")); return; }

  numLeitura++;
  Serial.print(F("#"));           Serial.print(numLeitura);
  Serial.print(F("  Umidade: ")); Serial.print(umidade, 0);
  Serial.print(F("%  Temp: "));   Serial.print(temp, 0);
  Serial.print(F("C  Ar: "));     Serial.println(arLigado ? F("ligado") : F("desligado"));

  unsigned long decorrido = millis() - ultimaTroca;
  if (!arLigado && umidade >= UMIDADE_LIGA && decorrido > MIN_DESLIGADO) enviarComando(true);
  if (arLigado && umidade <= UMIDADE_DESLIGA && decorrido > MIN_LIGADO)  enviarComando(false);
}
