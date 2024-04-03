#include <AccelStepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// stepper motor 28byj-48 
const int STEPS_PER_REVOLUTION = 2038;  // change this to fit the number of steps per revolution

// Base Giratoria
const int PHOTOSTEP = 85; // 2048 steps / 360° = 5.68 steps/° --> 15° = 85.2 steps
const int FULL_TURN = 24;
const int ANGLE_BASE = 15;

// Brazo
const int ARMSTEP = 114; // 2048 steps / 360° = 5.68 steps/° --> 20° = 113.6 steps
const int ANGLE_ARM = 20;

// Motor 1 --> Base giratoria
// Motor 2 --> Brazo

// ULN2003 Motor 1 Driver Pins
#define IN1_1 8
#define IN2_1 9
#define IN3_1 10
#define IN4_1 11

// ULN2003 Motor 2 Driver Pins
#define IN1_2 4
#define IN2_2 5
#define IN3_2 6
#define IN4_2 7

// Joystick Pins
#define JOYSTICK_X A1
#define JOYSTICK_Y A0
#define JOYSTICK_THRESHOLD 50

// initialize the motor interface
AccelStepper Motor_1(AccelStepper::HALF4WIRE, IN1_1, IN3_1, IN2_1, IN4_1);
AccelStepper Motor_2(AccelStepper::HALF4WIRE, IN1_2, IN3_2, IN2_2, IN4_2);

// Button
#define BUTTON_JOYSTICK 12


// Define la dirección I2C del LCD
#define I2C_ADDR 0x27

// Define las dimensiones del LCD
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// Inicializa el objeto LiquidCrystal_I2C
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_WIDTH, LCD_HEIGHT);

void setup() {
  // set the maximum speed, acceleration factor,
	// initial speed and the target position
  Motor_1.setMaxSpeed(1000);
  Motor_1.setAcceleration(200);
  //Motor_1.setSpeed(200);

  Motor_2.setMaxSpeed(1000);
  Motor_2.setAcceleration(500);
  //Motor_2.setSpeed(200);

  // initialize the button pin as input
  pinMode(BUTTON_JOYSTICK, INPUT_PULLUP);

  // initialize the serial port
  Serial.begin(9600);

  // Inicializa el LCD utilizando I2C
  lcd.init();
  lcd.backlight(); // Enciende la retroiluminación

  PantallaInicio();

}

// ==============================================================

void loop() {
  // Leer los valores del joystick
  int joyX = analogRead(JOYSTICK_X);
  //delay(100);
  int joyY = analogRead(JOYSTICK_Y);
  //delay(100);
  bool buttonValue = digitalRead(BUTTON_JOYSTICK);

  // Mover el brazo hacia adelante o hacia atrás
  if (abs(joyX - 512) > JOYSTICK_THRESHOLD) {
    if (joyX < 512) {
      MoverBrazo(-ANGLE_ARM);
      PantallaInicio();
    } else {
      MoverBrazo(ANGLE_ARM);
      PantallaInicio();
    }
  }

  // Girar la base hacia la izquierda
  if (abs(joyY - 512) > JOYSTICK_THRESHOLD && joyY > 512) {
    GirarBaseCompleta();
    PantallaInicio();
  }

  // Espera a que se presione el botón
  if ( buttonValue == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("   Realizar");
    lcd.setCursor(0, 1); 
    lcd.print("  Escaneo...");
    delay(1000);

    RealizarEscaneo(); // Función para realizar el escaneo completo
    
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("   Escaneo");
    lcd.setCursor(0, 1); 
    lcd.print("  Completado!");
  }

  

} // fin loop

// ==============================================================

// Función para realizar los movimientos
void RealizarEscaneo() {
// Se mueve para atrás
  for (int i = 0; i < 3; i++) {
    GirarBaseCompleta();
    MoverBrazo(-ANGLE_ARM);
  }

// Vuelve al medio
  for (int i = 0; i < 3; i++) {
    MoverBrazo(ANGLE_ARM);
  }

// Se mueve para adelante
  for (int i = 0; i < 3; i++) {
    MoverBrazo(ANGLE_ARM);
    GirarBaseCompleta();
  }

// Vuelve al medio
  for (int i = 0; i < 3; i++) {
    MoverBrazo(-ANGLE_ARM);
  }
}

// Función para girar la base completa
void GirarBaseCompleta() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Girando base...");

  for (int i = 0; i <= FULL_TURN; i++) {
    lcd.setCursor(0, 1); 
    lcd.print(String("vuelta: ") + String(i) + String(" de 24"));
    Motor_1.move(ANGLE_BASE*2 * (STEPS_PER_REVOLUTION / 360)); //el 2 es para compensar por manejar en medio paso el motor
    Motor_1.runToPosition();
    delay(500);
  }
}

// Función para mover el brazo
void MoverBrazo(int angle) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(angle > 0 ? "Girando +brazo..." : "Girando -brazo...");
  lcd.setCursor(0, 1); 
  lcd.print(String("paso: ") + String(ANGLE_ARM) + String(" grados"));

  Motor_2.move(angle * (STEPS_PER_REVOLUTION / 360));
  Motor_2.runToPosition();
  delay(1000);
}

// Función para reinciar LCD

void PantallaInicio(){
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0)
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("   Fotometria");
  lcd.setCursor(0, 1); 
  lcd.print("Elija opcion...");
}
