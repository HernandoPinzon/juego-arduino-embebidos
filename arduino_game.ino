#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

byte player[] = {
  B01110,
  B01010,
  B01110,
  B00100,
  B11111,
  B00100,
  B01010,
  B10001
};

byte asteroide[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

bool endGame = false;

double frameTimeLimit = 200;
long frameTime = millis();
long velTime = millis();

const byte numPins = 2;
byte pins[numPins] = {3, 2};
volatile byte playerY = 0;
int obstacleX = 15;
int obstacleY = 0;
int obstacle2X = 23;
int obstacle2Y = 1;

unsigned long puntajeInicial = 0;
bool gameStarted = false;

void setup() {
  Serial.begin(9600);
  
  lcd.createChar(0, player);
  lcd.createChar(1, asteroide);
  
  for (byte i = 0; i < numPins; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(pins[0]), movePlayerUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(pins[1]), movePlayerDown, FALLING);

  lcd.begin(16, 2);
  lcd.print("Esquiva o muere!");
  delay(2000);
  lcd.clear();

  drawPlayer();
  drawObstacles();
}

void loop() {
  if (!gameStarted) {
    puntajeInicial = millis();
    gameStarted = true;
  }
  
  //Manejo del tiempo entre cada frame
  if(millis()>frameTime+frameTimeLimit && !endGame){
    frameTime=millis();
    obstacleX--;
    obstacle2X--;
    drawObstacles();
  }
  
  //Manejo de la velocidad
  if(millis()>velTime+5000 && frameTimeLimit>40 && !endGame){
    velTime=millis();
  	frameTimeLimit= frameTimeLimit*0.9;
  }
  if (obstacleX < 0 && !endGame) {
    // El obstáculo llegó al borde izquierdo de la pantalla
    // Reiniciar el movimiento
    obstacleX = 15;
    obstacleY = random(0, 2);
    Serial.print("obj1:");
    Serial.println(obstacleY);
  }
  if (obstacle2X < 0 && !endGame) {
    // El obstáculo llegó al borde izquierdo de la pantalla
    // Reiniciar el movimiento
    obstacle2X = 15;
    obstacle2Y = random(0, 2);
    Serial.print("obj2:");
    Serial.println(obstacle2Y);
  }

  if ((obstacleX == 0 && obstacleY == playerY) || (obstacle2X == 0 && obstacle2Y == playerY)) {
    // Colisión
    endGame=true;
    frameTimeLimit=200;
    unsigned long puntajeFinal = millis(); // Detener el temporizador
    unsigned long puntaje = puntajeFinal - puntajeInicial; // Calcular tiempo transcurrido
    lcd.clear();
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Puntaje: ");
    lcd.print(puntaje / 1000); // Mostrar tiempo en segundos
    obstacleX = 15;
    obstacle2X = 23;
    gameStarted = false;
  }

  
}

void movePlayerUp() {
  startGame();
  playerY = 0;
}

void movePlayerDown() {
  startGame();
  playerY = 1;
}

void drawPlayer() {
  lcd.clear();
  lcd.setCursor(0, playerY);
  lcd.write(byte(0));
}

void drawObstacles() {
  drawPlayer();
  lcd.setCursor(obstacleX, obstacleY);
  lcd.write(byte(1));
  lcd.setCursor(obstacle2X, obstacle2Y);
  lcd.write(byte(1));
}

void startGame(){
  if(endGame){
  	endGame=false;
  }
}