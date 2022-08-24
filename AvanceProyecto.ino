/*
 * Nombre:PROTOTIPO FUNCIONAL DE VEHICULO AUTÓNOMO EN PEQUEÑA ESCALA PARA EL TRANSPORTE DE MATERIALES E INSUMOS EN BODEGAS
 * Autores: Carlos Eduardo Becerra Lizarazo,Jhon Stewar Nuñez Castellanos,Carlos Alfonso Estevez Carvajal
 * Fecha:
 * Funcionalidad:
 */

//librería para uso del módulo bluetooth
#include <SoftwareSerial.h> // Incluimos la librería  SoftwareSerial  

SoftwareSerial BTserial(6, 7);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

 // Definición de variables y constantes relacionadas con el motor Derecho
const int IN1 = 13;  // Pin digital 13 para controlar sentido giro motor Derecho
const int IN2 = 12;  // Pin digital 12 para controlar sentido giro motor Derecho
//const int ENA = 6;
 
// Definición de variables y constantes relacionadas con el motor Izquierdo
const int IN3 = 11;  // Pin digital 11 para controlar sentido giro motor izquierdo
const int IN4 = 10;  // Pin digital 10 para controlar sentido giro motor izquierdo
//const int ENB = 5;
 
//const int velM = 255;
//const int velm=150;
 
// Definición de variables y constantes relacionadas con los sensores IR
int lecturaSensorIzq; // Almacena el valor de la lectura del sensor izquierdo
int lecturaSensorDer; // Almacena el valor de la lectura del sensor derecho
int lecturaSensorPos; // Almacena el valor de la lectura del sensor del centro 

const int sensorIzqPin = A1; // El sensor izq irá conectado al pin analógico A0
const int sensorDerPin = A0; // El sensor derecho irá conectado al pin analógico A1
const int sensorPosPin = A2; // El sensor del centro irá conectado al pin analógico A2

void setup()
{
  // Se declaran todos los pines como salidas
  // Pines asociados a los motores
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  //pinMode (ENA, OUTPUT);
  //pinMode (ENB, OUTPUT);
  //pinMode( sensorIzqPin  , INPUT) ;
  //pinMode( sensorDerPin  , INPUT) ;

  BTserial.begin(9600); // transmisión por bluetooth
  Serial.begin(9600); // Se inicia el puerto de comunicaciones en serie
}
 
void loop()
{

  lecturaSensorIR(); // Se lee el valor de los sensores IR
  // Se analiza el resultado de los sensores para hacer que el robot siga la línea negra
  
  // si los 3 sensores se encuentran afuera el robot se detiene (Todos zona blanca)
  if(lecturaSensorIzq <= 600 and lecturaSensorDer <= 600 and lecturaSensorPos <=600)
  {
    robotParar();
    delay(200);
    robotRetroceso(); 
  }
  else {
    
    //verificamos que el sensor de posición se encuentre sobre a linea negra
    if(lecturaSensorPos>600)
    {
      // Si el izquierdo retorna 0 (zona blanca) y el derecho 1 (negra) el robot gira derecha
      if (lecturaSensorIzq <=600 and lecturaSensorDer >= 601)
      {
        robotDerecha();
         
      }
  
      // Si el izquierdo retorna 1 (zona negra) y el derecho 0 (blanca) el robot gira izquierda
      if (lecturaSensorIzq >= 601 and lecturaSensorDer <=600)
      {
       robotIzquierda();
     
      }
  
      // Si ambos sensores retornan 0 (zona negra) el robot sigue recto
      if (lecturaSensorIzq > 600 and lecturaSensorDer > 600)
      {
        robotAvance(); // El robot avanza
        //Serial.println("robot avanza");
      }
      
    }
    else
    {
      //detenemos el robot por unos instantes
      robotParar();
      delay(500);
      
      //mientras no obtengamos una respuesta, spamear "stop"
      BTserial.print("1");
      //Serial.println(BTserial.readStringUntil('\n'));

      String ruta = BTserial.readStringUntil('\n');
      if(ruta=="I")
      {
        //Serial.println("Here I am");
        robotIzquierda();
        //de momento se usa delay, pero lo ideal es usar otro método
        delay(700);
      }
      if(ruta=="D")
      {
        //Serial.println("Here I am");
        robotDerecha();
        //de momento se usa delay, pero lo ideal es usar otro método
        delay(700);
      }
      if(ruta=="A")
      {
        //Serial.println("Here I am");
        robotAvanza();
        //de momento se usa delay, pero lo ideal es usar otro método
        delay(700);
      }
        
    }
    
  }
 

    /* Si el izquierdo retorna 0 (zona blanca) y el derecho 1 (negra) el robot gira derecha
    if (lecturaSensorIzq <=600 and lecturaSensorDer >= 601)
    {
      robotDerecha();
       
    }
    // Si el izquierdo retorna 1 (zona negra) y el derecho 0 (blanca) el robot gira izquierda
    if (lecturaSensorIzq >= 601 and lecturaSensorDer <=600)
    {
     robotIzquierda();
   
    }
    // Si ambos sensores retornan 0 (zona negra) el robot sigue recto
    if (lecturaSensorIzq > 600 and lecturaSensorDer > 600)
    {
      robotAvance(); // El robot avanza
      Serial.println("robot avanza");
    }
  }
  //cuando el sensor de posición encuentre un punto blanco sobre la cartulina
  else {
    robotIzquierda();
  }
 */
}
/*
  Función lecturaSensorIR: leerá el valor del sensor de infrarrojos TCRT5000
  y lo almacena en una variable. Dicho sensor retornará el valor 0 (LOW) si
  el sensor está en zona blanca y el valor 1 (HIGH) si el sensor está en zona
  negra.
*/
void lecturaSensorIR()
{
  lecturaSensorIzq = analogRead(sensorIzqPin); // Almacena la lectura del sensor izquierdo
  lecturaSensorDer = analogRead(sensorDerPin); // Almacena la lectura del sensor derecho

  lecturaSensorPos = analogRead(sensorPosPin); // Almacena la lectura del sensor del centro
  /*
  Serial.println("El valor del sensor izquierdo es ");
  Serial.println(lecturaSensorIzq);
  Serial.println("");
 
  Serial.println("El valor del sensor derecho es ");
  Serial.println(lecturaSensorDer);
  Serial.println("");

  Serial.println("El valor del sensor del centro es ");
  Serial.println(lecturaSensorPos);
  Serial.println("");
  */
}
/*
  Función robotAvance: esta función hará que ambos motores se activen a máxima potencia
  por lo que el robot avanzará hacia delante
*/
void robotAvance()
{
  // Motor Derecho
  // Al mantener un pin HIGH y el otro LOW el motor gira en un sentido
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  //analogWrite (ENA, velM); //Velocidad motor Derecho
 
  // Motor Izquierdo
  // Al mantener un pin HIGH y el otro LOW el motor gira en un sentido
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  //analogWrite (ENB, velM); //Velocidad motor Izquierdo 
}
/*
  Función robotRetroceso: esta función hará que ambos motores se activen a máxima potencia 
  en sentido contrario al anterior por lo que el robot avanzará hacia atrás
*/
void robotRetroceso()
{
  // Motor Derecho
 
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, HIGH);
  //analogWrite (ENA, velm); //Velocidad motor Derecho 
 
  // Motor Izquierdo
  
  digitalWrite (IN3, LOW);
  digitalWrite (IN4, HIGH);
  //analogWrite (ENB, velm); //Velocidad motor Izquierdo 
}
 
/*
  Función robotDerecha: esta función acccionará el motor izquierdo y parará el derecho
  por lo que el coche girará hacia la derecha (sentido horario)
*/
void robotDerecha()
{
  //  Motor Derecho

  digitalWrite (IN1, LOW);
  digitalWrite (IN2, HIGH);
  //analogWrite (ENA, velM); //Velocidad motor A  
 
  // Motor Izquierdo
  
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  //analogWrite (ENB, velm); //Velocidad motor A  
 
}
/*
  Función robotIzquierda: esta función acccionará el motor derecho y parará el izquierdo
  por lo que el coche girará hacia la izquierda (sentido antihorario)
*/
void robotIzquierda ()
{
   //  Motor Derecho
 
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  //analogWrite (ENA, velm); //Velocidad motor A  
 
  // Motor Izquiero
  
  digitalWrite (IN3, LOW);
  digitalWrite (IN4, HIGH);
  //analogWrite (ENB, velM); //Velocidad motor A  
}
/*
  Función robotParar: esta función parará ambos motores
  por lo que el robot se parará.
*/
void robotParar()
{
  // Motor Derecho
 
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, LOW);
 
  // Motor Izquierdo
  
  digitalWrite (IN3, LOW);
  digitalWrite (IN4, LOW);
 
}
