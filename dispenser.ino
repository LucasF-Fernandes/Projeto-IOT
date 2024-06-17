#include <NewPing.h> // Biblioteca para sensor de ultrassom

// Definição dos pinos
const int triggerPin = 3; // Pino de trigger do sensor de ultrassom
const int echoPin = 4;   // Pino de echo do sensor de ultrassom
const int pumpPin = 2;   // Pino de controle da bomba de água
const int buzzerPin = 5;  // Pino do buzzer

// Criação da instância do sensor de ultrassom
NewPing sonar(triggerPin, echoPin, 4000); // Max distance: 4 meters

// Duração do pulso da bomba (em segundos)
const int pumpDuration = 2; // 2 segundos de acionamento da bomba

// Variável para controle de ativação
bool dispenserActivated = false;

// Temporizador para redefinir dispenserActivated
unsigned long lastActivationTime = 0;
const unsigned long activationCooldown = 1000; // Tempo de espera em milissegundos (5 segundos)

void setup() {
  // Configuração dos pinos como saída
  pinMode(pumpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Desliga a bomba e o buzzer inicialmente
  digitalWrite(pumpPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  int distance = sonar.ping_cm();

  // Se o objeto estiver a uma distância de até 3 cm e o dispensador não estiver ativado
  if (distance <= 2 && !dispenserActivated) {
    digitalWrite(pumpPin, HIGH);
    delay(pumpDuration * 200); // Ativa a bomba por 2 segundos
    digitalWrite(pumpPin, LOW);

    digitalWrite(buzzerPin, HIGH);
    delay(500); // Ativa o buzzer por 0.5 segundos
    digitalWrite(buzzerPin, LOW);

    dispenserActivated = true;
    lastActivationTime = millis(); // Registra o tempo de ativação

    // Aguarde até que o objeto não esteja mais próximo antes de reativar o sensor
    while (distance <= 3) {
      distance = sonar.ping_cm();
      delay(200); // Espera para evitar a verificação contínua
    }
  }

  // Se o tempo de espera passou e o dispensador ainda está ativado, redefina-o
  if (dispenserActivated && millis() - lastActivationTime >= activationCooldown) {
    dispenserActivated = false;
  }

  delay(200); // Pequeno atraso para evitar a verificação contínua
}
