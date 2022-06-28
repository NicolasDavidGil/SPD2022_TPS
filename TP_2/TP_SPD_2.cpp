// C++ code
//
#include <LiquidCrystal.h> //Libreria lcd
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Libreria cadenas

//Defines-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NEXT_BUTTON 8 //Boton para pasar a la siguiente letra
#define BEFORE_BUTTON 10 //Boton para pasar a la letra anterior
#define SELECT_BUTTON 9 //Boton para verificar la letra
#define BASE_MILISEG 3000 //Milisegundos de delay entre cada partida

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
//Se crea un constructor lcd de 4 bits
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//Variables---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

unsigned long millisBefore = 0,
			  millisNow;

char abecedario[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'}, //Vector de los caracteres elegibles durante el juego
	 palabraSecreta[] = "SISTEMAS",   //Cadena donde se guarda la palabra a descubrir.
	 palabraOculta[] = "________",    //Cadena donde se guarda lo descubierto hasta el momento de la palabra secreta.
	 letrasIngresadas[27],            //Vector donde se guarda todas las letras ingresadas hasta el momento, para evitar la repeticion.
	 letraActual;                     //Caracter donde se guarda la letra elegida en ese momento.

int searchIndex = 0,                  //Indice utilizado en la funcion buscarLetra para recorrer la palabra secreta
    letterIndex = 0,                  //Indice utilizado en las funciones elegirLetra y seleccionarLetra para recorrer el abecedario.
    printIndex = 0;                   //Indice utilizado en la funcion mostrarPalabra para recorrer la palabra oculta.

int vidasAhora = 3,                   //Valor que indica cuantas vidas le quedan al jugador, luego del ultimo ingreso de la letra.
	vidasAntes = 3;                   //Valor que indica cuantas vidas le quedaban al jugador, antes del ultimo ingreso de la letra.

int nextButtonBefore = 0,             //Valor que indica si en el loop anterior, el boton de "siguiente letra" estaba apagado. Se utiliza para evitar el rebote.
    beforeButtonBefore = 0,           //Valor que indica si en el loop anterior, el boton de "letra anterior" estaba apagado. Se utiliza para evitar el rebote.
    selectButtonBefore = 0;           //Valor que indica si en el loop anterior, el boton de "seleccionar letra" estaba apagado. Se utiliza para evitar el rebote.

int flagReinicio = 0,		          //Bandera utilizada para indicar el fin del juego, y hacer llamado a la funcion Reinicio.
    flagMillis = 1,                   //Bandera utilizada al mostrar el mensaje de fin, para contar a partir de alli los 3 segundos, antes del reinicio.
    flagBuscar = 0,                   //Bandera utilizada para hacer llamado a la funcion buscarLetra, una vez ya elegida la letra.
	flagLetraEncontrada = 0;          //Bandera utilizada para indicar si la letra ingresada se encuentra en la palabra secreta.

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//MAIN--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  pinMode(NEXT_BUTTON,INPUT);
  pinMode(SELECT_BUTTON,INPUT);
  pinMode(BEFORE_BUTTON,INPUT);
  

  lcd.begin(16,2);
  mostrarVidas();
  flechasLetra();
}

void loop()
{
  if(flagReinicio != 1)
  {
    elegirLetras();
    seleccionarLetra();
    
    if(flagBuscar == 1)
      buscarLetra();
    
    mostrarPalabra();
  }
  
  finJuego();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//Funciones---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

///brief Funcion que lee los estados de los botones "siguiente" y "anterior" letra, los cuales incrementan y decrementan el 
///      indice de letra, y printea en el display la letra de la cadena "abecedario" correspondiente a ese indice.
///
void elegirLetras()
{
  lcd.setCursor(3,0);
  lcd.print(abecedario[letterIndex]);
  if(digitalRead(NEXT_BUTTON) && nextButtonBefore == LOW)
  {
    letterIndex = letraSiguiente(letterIndex);
  }
  else
  {
    if(digitalRead(BEFORE_BUTTON) && beforeButtonBefore == LOW)
	{
      letterIndex = letraAnterior(letterIndex);
    }
    beforeButtonBefore = digitalRead(BEFORE_BUTTON);
  }
  
  nextButtonBefore = digitalRead(NEXT_BUTTON);
}

///brief Funcion que incrementa el indice de letra, y lo retorna. Si dicho indice es mayor a 25, lo reinicia a 0.
///param letterIndex Indice a incrementar.
///
///return Devuelve el indice incrementado
int letraSiguiente(int letterIndex)
{
  letterIndex++;
  if(letterIndex > 25)
  {
    letterIndex = 0;
  }
  
  return letterIndex;
}

///brief Funcion que decrementa el indice de letra, y lo retorna. Si dicho indice es menor a 0, lo convierte en 25.
///param letterIndex Indice a decrementar.
///
///return Devuelve el indice decrementado
int letraAnterior(int letterIndex)
{
  letterIndex--;
  if(letterIndex < 0)
  {
    letterIndex = 25;
  }
  
  return letterIndex;
}

///brief Funcion que lee el estado del boton "seleccionar letra", y verifica si la letra seleccionada ya habia sido seleccionada
///      con anterioridad. De ser no ser asi, se agrega dicha letra a la lista de letras ingresadas y se activa el flag de busqueda.
///
void seleccionarLetra()
{
  if(digitalRead(SELECT_BUTTON) && selectButtonBefore == LOW)
  {
    if(abecedario[letterIndex] != letrasIngresadas[letterIndex])
    {
      flagBuscar = 1;
      letrasIngresadas[letterIndex] = abecedario[letterIndex];
      letraActual = abecedario[letterIndex];
    }
  }
  
  selectButtonBefore = digitalRead(SELECT_BUTTON);
}

///brief Funcion que verifica si la letra seleccionada forma parte de la palabra secreta. Si es el caso, se reemplaza el "_" por la 
///      letra, en todas las coincidencias, en la cadena de la palabra oculta. Si luego de recorrer toda la palabra secreta no se 
///      encontraron coincidencias, se le resta una vida al jugador.
///
void buscarLetra()
{
  if(searchIndex<strlen(palabraSecreta))
  {
    if(letraActual == palabraSecreta[searchIndex])
	{
      palabraOculta[searchIndex] = letraActual;
      Serial.println("Se encontro el caracter:");
      Serial.println(letraActual);
      
      flagLetraEncontrada = 1;
    }
    
    searchIndex++;
  }
  else if(searchIndex == strlen(palabraSecreta))
  {
    if(flagLetraEncontrada == 0)
    {
      vidasAhora--;
      quitarVidas();
    }
    
    flagLetraEncontrada = 0;
    flagBuscar = 0; 
    searchIndex = 0; 
  }
}

///brief Funcion que imprime en el display las vidas iniciales del jugador.
///
void mostrarVidas()
{
  lcd.setCursor(7,0);
  lcd.print("Vidas:");
  
  byte corazon[] = 
  {
    B00000,
    B01010,
    B11111,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000
  };
  
  lcd.createChar(byte(6), corazon); 
  
  lcd.setCursor(13,0);
  lcd.write(byte(6));
  lcd.setCursor(14,0);
  lcd.write(byte(6));
  lcd.setCursor(15,0);
  lcd.write(byte(6));
  
}

///brief Funcion que, en caso de haberse equivocado al ingresar una letra, borra del display una vida.
///
void quitarVidas()
{
  int vidasPerdidas = 2 - vidasAhora;
  lcd.setCursor(13 + vidasPerdidas ,0);
  if(vidasAhora < vidasAntes)
  {
    lcd.print(" ");
    vidasAntes = vidasAhora;
  }
}

///brief Funcion que printea en el display la palabra secreta, con las letras descubiertas hasta el momento, y las desconocidas
///      representadas por un "_".
///
void mostrarPalabra()
{
  if(printIndex < strlen(palabraOculta))
  {
    lcd.setCursor(printIndex+4,1);
    lcd.print(palabraOculta[printIndex]);
    printIndex++;
  }
  else if (printIndex == strlen(palabraOculta))
    printIndex = 0;
}

///brief Funcion que imprime el mensaje final del juego, ya sea por haberse quedado sin vidas, o por haber descubierto la palabra secreta.
///      Luego de 3 segundos, se reinicia el juego, borrando los datos del juego anterior.
///
void finJuego()
{
  if(vidasAhora == 0 && flagReinicio == 0)
  {
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("GAME");
    lcd.setCursor(6,1);
    lcd.print("OVER");
    
    flagReinicio = 1;
    ahorcado();
  }
  else if(strcmp(palabraOculta,palabraSecreta) == 0 && flagReinicio == 0)
  {
    lcd.clear();  
    lcd.setCursor(4,0);
    lcd.print("Ganaste"); 
    lcd.setCursor(5,1);
    lcd.print("Papa"); 
    
    flagReinicio = 1;
  }
  
  if(flagReinicio)
  {
    if(flagMillis)
    {
      millisBefore = millis();
      flagMillis = 0;
    }
    
  	Reiniciar(); 
  }
}

///brief Funcion que calcula el tiempo desde el fin del juego, y llegados los 3 segundos reinicia todos los parametros, dejandolo listo
///      para otro juego.
///
void Reiniciar()
{
  millisNow = millis();
  if (millisNow - millisBefore >= BASE_MILISEG)
  {
    millisBefore = millisNow;
    strcpy(letrasIngresadas,"..........................");
    strcpy(palabraOculta,"________");
           
    vidasAhora = 3;
    vidasAntes = 3;
    letterIndex = 0;
    
    flagMillis = 1;
    flagReinicio = 0;
    
    lcd.clear();
    mostrarVidas();
    flechasLetra();
  }    
}

///brief Funcion que, al perder, imprime en en display un dibujo de un muñeco ahorcado.
///
void ahorcado()
{
	byte ahorcado1[] = {
      B01111,
      B01000,
      B01000,
      B01000,
      B01000,
      B01000,
      B01000,
      B01000
    };
  
 	byte ahorcado2[] = {
      B11111,
      B00100,
      B00100,
      B01110,
      B01110,
      B00100,
      B01110,
      B10101
    };
  
 	byte ahorcado3[] = {
      B00100,
      B00100,
      B01110,
      B01010,
      B01010,
      B11011,
      B00000,
      B00000
    };
  
  	byte ahorcado4[] = {
      B01001,
      B01000,
      B01000,
      B01000,
      B01000,
      B01000,
      B11100,
      B11100
    };
  
  	byte ahorcado5[] = {
      B11111,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    };
  
  	byte ahorcado6[] = {
      B10000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    };

  lcd.createChar(byte(0), ahorcado1);
  lcd.setCursor(13, 0);
  lcd.write(byte(0));
  
  lcd.createChar(byte(1), ahorcado2);
  lcd.setCursor(14, 0);
  lcd.write(byte(1));
  
  lcd.createChar(byte(2), ahorcado3);
  lcd.setCursor(14, 1);
  lcd.write(byte(2));
  
  lcd.createChar(byte(3), ahorcado4);
  lcd.setCursor(13, 1);
  lcd.write(byte(3));
  
  lcd.createChar(byte(4), ahorcado5);
  lcd.setCursor(15, 0);
  lcd.write(byte(4));
  
  lcd.createChar(byte(5), ahorcado6);
  lcd.setCursor(15, 1);
  lcd.write(byte(5));
}

///brief Funcion que imprime en el display, a los costados de la letra actual, 2 flechas.
///
void flechasLetra()
{
    byte flechaIzq[] = 
  {
    B10000,
    B11000,
    B11100,
    B11110,
    B11100,
    B11000,
    B10000,
    B00000
  };
  
  byte flechaDer[] = 
  {
    B00001,
    B00011,
    B00111,
    B01111,
    B00111,
    B00011,
    B00001,
    B00000
  };
  
  lcd.createChar(byte(7), flechaIzq); 
  lcd.createChar(byte(8), flechaDer); 
  
  lcd.setCursor(2,0);
  lcd.write(byte(7));
  
  lcd.setCursor(4,0);
  lcd.write(byte(8));
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
