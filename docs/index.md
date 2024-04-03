# Inicio

Informe de avances.

## Introducción y Objetivos

### Jornada de Salud

Con el objeto de favorecer las competencias específicas planteadas en el perfil del egresado de la Licenciatura en Bioinformática (FIUNER), el presente trabajo busca abordar un  problema de asignación de especies crípticas de interés epidemiológico en la región del litoral argentino. 


## Desarrollo y Resultados

### Análisis en 2 Dimensiones (2D)

En base a caracteres morfológicos discretos o el análisis univariado de variables escalares, una nueva especie de murciélago del género Molossus -definida genéticamente - no podía ser operativamente diferenciada de otros 3 molóssidos que cohabitan en la región central. A partir de este planteo, en base a criterios de factibilidad, los estudiantes lograron identificar, seleccionar críticamente, implementar y testear comparativamente una batería de métodos de adquisición y análisis de datos multidimensionales de morfometría craneal basados en medidas lineales (Fig1) y landmarks en imágenes 2D (Fig2) y objetos 3D obtenidos mediante fotogrametría digital.


![Fig1: Análisis de Morfometría Multidimensional 2D](/docs/img/fig_1.png){ .image50percent }


![Fig2: Análisis de Morfometría con Landmarks 2D](/docs/img/fig_2.png){ .image50percent }


### Análisis en 3 Dimensiones (3D)

Para proceder con el estudio morfométrico en 3D se recurrió a técnicas de fotometría digital para obtener un modelo tridimensional del cráneo. 

#### Fotometría digital

Como modelo de pruebas para ajustar el procedimiento, se empleó el modelo de un cráneo de murciélago pálido (Antrozous pallidus) de libre uso. El mismo se encuentra disponible en [thingiverse.com](https://www.thingiverse.com/thing:5380972). El modelo fue escalado para ajustarse al tamaño del cráneo de Molossus. 

![Modelos de pruebas](/docs/img/craneos.png)

Se requirió de un Setup de pruebas que permitiera tomar fotografías de manera automática. Se realizaron las pruebas de concepto con motores paso a paso y una placa de desarrollo Arduino:

![Prueba de concepto](/docs/img/prueba_concepto_1.png)

Posteriormente se probó emplear una cámara web USB:

![Prueba de cámara web](/docs/img/prueba_concepto_2.png)

Pruebas con caja de iluminación:

![Prueba del setup](/docs/img/prueba_concepto_3.png)


#### Escaner 3D

Para mejorar la repetibilidad del escaneo de los cráneos, se optó por modificar un modelo de escaner 3D por fotometría de OpenScan en su versión clásica. El mismo puede ser impreso mediante una impresora 3D con tecnología FDM, y los archivos se encuentran disponibles en [thingiverse.com](https://www.thingiverse.com/thing:2755968).

Para el modelado de piezas personalizadas se empleó el software Open Source FreeCAD.

![Modelado Adaptador](/docs/img/freecad.png)


Las piezas fueron impresas en una impresora 3D Hellbot Magna 1v2 usando filamento PLA.

![Piezas Impresas](/docs/img/piezas.png)


El ensamblado final de todas las partes:

![Escaner Ensamblado](/docs/img/ensamble.png)


Durante la prueba del escáner, se ejecuta una rutina de escaneo diseñada para capturar imágenes tridimensionales con precisión. Esta rutina comienza con un giro completo de 360° de la pieza en intervalos de 15°, deteniéndose en cada paso para la toma de las fotografías correspondientes. Posteriormente, el brazo lateral del escáner se inclina en un ángulo de 60° en ambas direcciones, con incrementos de 20° en cada movimiento. Cada vez que el brazo lateral se inclina, se realiza un giro completo de la pieza en escaneo, asegurando una cobertura exhaustiva del objeto en estudio.

![Escaner Ensamblado](/docs/img/rutina_escaneo.png)

### Procesado de Imágenes

#### Acondicionamiento

Para el preprocesamiento y acondicionamiento de las imágenes del escaneo, se empleó el programa de manipulación de imágenes de GNU, GIMP. Se añadió la extensión Batch Image Manipulation para aplicar el procesamiento en masa.

El acondicionamiento incluye etapas de:

- Recorte
  - cortar a relación de aspecto estándar (1:1)
  
- Corrección de color
  - convertir a escala de grises
  - corrección automática de los niveles de color
  
- Renombrar con un patrón

![Modelado Adaptador](/docs/img/acondicionamiento.png)

#### Reconstrucción 3D

Se trabajó con el programa de reconstrucción 3D Open Source Meshroom. Se cargaron las imágenes de cada escaneo al programa y se ajustan los parámetros del flujo de trabajo.

![Workflow Meshroom](/docs/img/workflow_2.png)

![Workflow Meshroom](/docs/img/workflow_1.png)






## Código arduino

El siguiente código se emplea para controlar todo el setup de escaneo:

``` c++ title="Escaner-fotometría.ino" linenums="1" 
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

```