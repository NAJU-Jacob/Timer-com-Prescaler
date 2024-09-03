#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Pinos do display LCD
const int startButtonPin = 6; // Pino do botão de iniciar
const int stopButtonPin = 7; // Pino do botão de parar

unsigned long startTime = 0; // Variável para armazenar o tempo de início
unsigned long elapsedTime = 0; // Variável para armazenar o tempo decorrido
bool running = false; // Variável para verificar se o cronômetro está em execução
bool lastStartButtonState = LOW; // Estado anterior do botão de iniciar

void setup() {
  lcd.begin(16, 2); // Inicializa o display LCD
  pinMode(startButtonPin, INPUT_PULLUP); // Configura o pino do botão de iniciar como entrada com pull-up interno
  pinMode(stopButtonPin, INPUT_PULLUP); // Configura o pino do botão de parar como entrada com pull-up interno

  // Configura o Timer1 com pré-scaler de 256
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000100; // Pre-scaler de 256
  TIMSK1 |= B00000010; // Habilita a interrupção de overflow do Timer1
  sei(); // Habilita interrupções globais
}

void loop() {
  // Lê o estado atual do botão de iniciar
  bool currentStartButtonState = digitalRead(startButtonPin);
  
  // Verifica se houve uma transição de LOW para HIGH no botão de iniciar
  if (currentStartButtonState == HIGH && lastStartButtonState == LOW) {
    if (!running) {
      startTime = millis(); // Registra o tempo de início
      running = true; // Define o cronômetro como em execução
    } else {
      elapsedTime = millis() - startTime; // Calcula o tempo decorrido
      running = false; // Define o cronômetro como parado
    }
  }

  // Atualiza o estado anterior do botão de iniciar
  lastStartButtonState = currentStartButtonState;

  // Atualiza o display LCD
  updateLCD();
}

void updateLCD() {
  lcd.clear(); // Limpa o display LCD
  
  // Mostra "Aula 2 time" na primeira linha
  lcd.setCursor(0, 0);
  lcd.print("Aula 2 time");

  // Mostra o tempo decorrido no formato mm:ss na segunda linha
  unsigned long seconds = (elapsedTime / 1000) % 60; // Calcula os segundos restantes
  unsigned long minutes = (elapsedTime / 60000) % 100; // Calcula os minutos restantes
  lcd.setCursor(0, 1);
  lcd.print(minutes < 10 ? "0" : ""); // Adiciona um zero à esquerda se necessário
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds < 10 ? "0" : ""); // Adiciona um zero à esquerda se necessário
  lcd.print(seconds);
  
  delay(100); // Pequeno atraso para evitar atualizações rápidas demais
}

ISR(TIMER1_OVF_vect) {
  // Se o cronômetro está em execução, atualiza o tempo decorrido
  if (running) {
    elapsedTime += 65536 * 256 / F_CPU; // Adiciona o tempo do overflow do Timer1 ao tempo decorrido
  }
}
