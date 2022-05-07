/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar 
 *  apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *   
 *	El numero 15 (0000001111) se representaria encendiendo los 
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos 
 *	los led anteriores y encenderse uno de los leds VERDES. 
 *	Notese, en el ejemplo los 0 representan los led apagados 
 *	y los 1 los led encendidos). 
 *   
 *	-------------------------------------------------------
 *   
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga. 
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los 
 *	LEDS y de ser posible, todo el código para evitar repetir 
 *  lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador 
 *  para que el cambio de los leds encendidos sea perceptible 
 *  para el ojo humano y 
 *	documentar cada función creada en el código. 
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
*/

//##### Una Ayudita #####
//--- Defines ---//
#define ZERO_TO_HERO 0          //? To start the counter from 0
#define BUTTON_START 3
#define BUTTON_RESET 2
#define FIRST_LED 4             //? First pin of the leds
#define LAST_LED 13             //? Last pin of the leds
#define BASE_MILLI_SECONDS 1000 //? Secods expresed in miliseconds
#define MAX_SECONDS 1023        //! Max amount of secods to show
#define TAM_BINARIO 10
//--- End Defines ---//
void setPins(){
  //seteo de pins
  for(int i=FIRST_LED; i<=LAST_LED; i++){
    pinMode(i, OUTPUT);
  }
}

void setButtons(){
  //seteo de botones
  pinMode(BUTTON_RESET, INPUT);
  pinMode(BUTTON_START, INPUT);
}

void setup(){
  setPins();
  setButtons();
  Serial.begin(9600);
}

//Variables
short contador = ZERO_TO_HERO;
int arrayBinario[TAM_BINARIO];//Un array con los digitos del numero del contador en binario
unsigned long millisBefore = 0;
short buttonBefore = 0;//Estado del boton antes de se clickeado
short flagStart = 0;//bandera para determinar si un boton ya fue clickeado

void decimalAbinario(int cont, int array[]){
    short numeroBin = 0;
	int ultimoIndice = TAM_BINARIO-1;
    
    // Arma cadena binaria en el array
    while( cont > 0){
        numeroBin = cont % 2;
        cont /= 2;
        array[ultimoIndice] = numeroBin;
        ultimoIndice--;
    }

    // rellenar con 0 hacia la izquierda.
    while(ultimoIndice >= 0){
        array[ultimoIndice] = 0;
        ultimoIndice--;
    }
}

//Imprime los numeros en binario
void imprimirCadena(){
    for(int i = 0; i < TAM_BINARIO; i++){
        Serial.print(arrayBinario[i]);
    }
  	Serial.print("|");
    Serial.print(contador);
    Serial.println("");
}

//Enciende los led según el valor del digito
void turnLed(int arr[], int tam){
  for(int i = 0; i<tam; i++){
    digitalWrite(LAST_LED-i, arr[i]);
  }
}

//Esta funcion realiza el conteo
void conteo(){
  unsigned long millisNow = millis();
  	if(millisNow - millisBefore >= BASE_MILLI_SECONDS){
    decimalAbinario(contador, arrayBinario);
    imprimirCadena();
    turnLed(arrayBinario, TAM_BINARIO);
    contador++;
    millisBefore = millisNow;
  }
}

//Inicia y pausa el conteo
void start()
{
 int buttonNow = digitalRead(BUTTON_START);
 
  if(flagStart == 1 && contador <= MAX_SECONDS)
  {
    conteo();  
  }
  
  if(buttonNow == HIGH && buttonBefore == LOW  &&
     flagStart == 0)
  {
    flagStart = 1;
    
  }else if(buttonNow == HIGH && buttonBefore == LOW &&
           flagStart == 1)
  {
    flagStart = 0;
  }
  
  buttonBefore = buttonNow;
}

//Se reinicia el contador
void reset(){
  int buttonNow = digitalRead(BUTTON_RESET);
  if(buttonNow == HIGH && buttonBefore == LOW){
    contador = ZERO_TO_HERO;
  }
  buttonNow = buttonBefore;
}

void loop(){
  start();
  reset();
  if(contador>MAX_SECONDS){
    contador = ZERO_TO_HERO;
  }
  /*Aclaración: Puede ser que por motivos de latencia el circuito
  ande lento, de ser así revisar con un delay*/
}