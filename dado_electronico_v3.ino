/*  Asignación de cada segmento en mis pines digitales 
        5==A
    6==F    4==B
        7==G
    0==E    1==C
        2==D        DP: No asignado

    Pin E del display conectado al pin digital 0
    ...
    Pin G del display conectado al pin digital 7


    Ejemplo para representar el número 3:

         A=1            ---
            B=1             |
         G=1            ---
            C=1             |
         D=1            ---
         
    Se deben poner a HIGH los pines A, B, C, G y D
    Siguiendo el orden de mis pines
    SEGMENTO: G F A B P D C E
    Número 3: 1   1 1   1 1

    Los demás pines del display se pondrán a LOW, por tanto el número 3 quedaría definido como
    Número 3: 1 0 1 1 0 1 1 0


    ORDEN MI PUERTO D:
         PIN: 7 6 5 4 3 2 1 0
    SEGMENTO: G F A B P D C E

    Número 0: 0 1 1 1 0 1 1 1
    Número 1: 0 0 0 1 0 0 1 0
    Número 2: 1 0 1 1 0 1 0 1
    Número 3: 1 0 1 1 0 1 1 0
    Número 4: 1 1 0 1 0 0 1 0
    Número 5: 1 1 1 0 0 1 1 0
    Número 6: 1 1 1 0 0 1 1 1
    Número 7: 0 0 1 1 0 0 1 0
    Número 8: 1 1 1 1 0 1 1 1
    Número 9: 1 1 1 1 0 1 1 0

    Valores a dar a cada pin para mostrar cada dígito
    La representación de cada número está sujeta al esquema de conexión realizado. 
    Según qué pin del puerto D hayamos conectado a cada pin del display
    Cada uno debe hacer la representación de sus números si modifica las conexiones.
*/


const byte potenciometro = A5;  //Pin donde conecto la patilla de entrada del potenciometro
byte espera; //Tiempo de espera - inversamente proporcional a la velocidad de aparición de los números

const byte transistor = 9;      //Pin donde está conectada la patilla Base del transistor
byte brillo = 255;              //Valor inicial del brillo del display

//Definición de las posiciones de las conexiones de los bbotones y variables para almacenar sus estados
const byte b_modo = A2;
bool modo1 = true;              //El estado del boton-modo no indicará el modo, sino que cada vez pulsado alternará el modo
const byte b_mas = A3;
bool mas = false;
const byte b_menos = A4;
bool menos = false;

//Booleanos que nos ayudarán a conocer el estado actual del programa 
bool parpadeante = false;
bool primera_ronda = true;


//Creación de un array con el byte que se debe asignar al puerto D
//Para obtener la representación del número en el visualizador
byte numeros[6] = {  B00010010, //1
                     B10110101, //2
                     B10110110, //3
                     B11010010, //4
                     B11100110, //5
                     B11100111, //6
                  };    
                  
//Este array es igual que el anterior pero incorpora el led del punto para su utilización en el modo 2   
byte numeros_p[6] = {B00011010, //1
                     B10111101, //2
                     B10111110, //3
                     B11011010, //4
                     B11101110, //5
                     B11101111, //6
                    };    

//Bytes que almacena el binaro que pondremos al puerto D para mostrar el display vacío y el número 0
byte vacio = B00000000;
byte cero = B01110111;

byte elementos = sizeof(numeros) / sizeof(numeros[0]);
byte n_actual = 0;   //Variable donde almacenaremos el valor actual del dado



void setup() {
  DDRD = B11111111;           //Digital [0 - 7] como pines de salida del puerto D para controlar los pines del display y mostrar los números
  DDRB = DDRB | B00100010;    //Digitales 9 y 13 como salida para el transistor y led_builtin (para indicar el modo), respectivamente
  DDRC = DDRC & B11000011;    //Analógicos 5,4,3 y 2 como entrada del puerto C sin dañar los valores de otros pines del puerto C
  analogWrite(transistor, brillo);
}



void loop() {
  espera = map(analogRead(potenciometro), 0, 1023, 0, 255); //Registra el valor del potenciometro mapeado entre 0 y 255
                                          //En la variable espera que se utiliza para controlar la velocidad de funcionamiento.

                                          //Realiza lectura de los tres pulsadores
  if (digitalRead(b_modo))                //Si pulsa el boton modo, cambia el valor de la variable modo
    modo1 = !modo1;
  mas = digitalRead(b_mas);
  menos = digitalRead(b_menos);


  //MODO 1
  if (modo1) {
    digitalWrite(LED_BUILTIN, LOW);       //El led apagado indica el modo 1
    if (mas && menos) {
      n_actual++;                         //Aumentamos el valor de n_actual en 1
      n_actual %= elementos;              //Siempre en los márgenes del vector
      PORTD = numeros[n_actual];          //Actualizamos los bits del puerto D con el número actual a representar
      delay(espera);
      parpadeante = true;                 //Actualiza parpadeante para cuando se suelten los botones, el numero se muestre parpadeante
      primera_ronda = false;              //Al rotarse los números, ya no debe mostrarse el valor 0 asociado al primer funcionamiento del dado en modo1
    }

    else {                                //Si los dos botones +/- no están pulsados simultáneamente
      if ((mas || menos) && primera_ronda) { //Si se pulsa uno de ellos y es la primera ronda del dado, se muestra el valor 0 parpadeante
        parpadeante = true;
      }

      if (parpadeante) {
        for (byte i = 0 ; i < 5 ; i++) {
          if (primera_ronda)
            PORTD = cero;                 //Si es la primera ronda el valor será 0
          else
            PORTD = numeros[n_actual];    //Si no, será el número actual del dado
          delay(2 * espera);
          PORTD = vacio;                  //Se muestra el display limpio entre parpadeos
          delay(espera);
        }
        parpadeante = false;
      }
      else {                              //Si estamos en modo 1 sin accionar ningún funcionamiento, el display se mostrará vacío
        PORTD = vacio;
        delay(espera);
        if (mas || menos) {               //Si se pulsa uno de los botones +/- y un valor ya se ha mostrado de forma parpadeante
          PORTD = numeros[n_actual];      // el valor del dado se mostrará de forma contínua
          delay(10 * espera);
        }
      }
    }
  }


  //MODO 2
  else {
    digitalWrite(LED_BUILTIN, HIGH);    //El modo 2 se indica con el punto en los valores pero además se enciende el led de la placa
    primera_ronda = true;
    n_actual++;                         //Aumentamos el valor de n_actual en 1
    n_actual %= elementos;              //Siempre en los márgenes del vector
    PORTD = numeros_p[n_actual];        //Actualizamos los bits del puerto D con el número actual a representar
    delay(espera);
    if (menos && brillo > 20)           //Si se pulsan los botones + o - en el modo 2, se cambia el valor de brillo
      brillo -= 20;                     //El valor mínimo al que puede llegar el brillo es 5
    if (mas && brillo < 235)
      brillo += 20;                     //El valor máximo al que puede llegar el brillo es 255
    analogWrite(transistor, brillo);        //Realiza una escritura analógica en el pin que va al transistor para controlar el brillo del display
  }
}
