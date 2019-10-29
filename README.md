# Dado Electrónico

Este pequeño proyecto consiste en un dado electrónico en el que los números aparecen en el display de forma sucesiva mientras el jugador mantiene dos botones pulsados. Al soltar estos botones el último número que se mostró en el dado es el seleccionado y se muestra de forma parpadeante. La velocidad de aparición de los números es variable y por tanto es posible dificultar hacer trampas para soltar los botones en el número que se desee. Además, por si el usuario no recuerda el número que le salió en el dado, pulsando únicamente uno de los dos botones es posible mostrar el dígito las veces necesarias pero sin parpadear para indicar que el número ya se mostró. 

Por otro lado, si el dado no se ha lanzado y se pulsa sólo uno de los botones, se representa el número 0 para indicar que no se ha tirado el dado en esta ronda.

Además, este dado tiene un segundo modo de funcionamiento en el que se puede variar la luminosidad o brillo de los leds mientras los números se sudecen pero en este modo no es posible parar el dado. 

Cada vez que se cambia del modo cambiar brillo al modo normal, se entiende que se ha comenzado una nueva ronda y por tanto el dado no recuerda el último valor representado y si se pulsa alguno de los botones, mostrará el número 0 como si se hubiese reiniciado. 


Este dado programado consta de los siguientes componentes: 
* Display de 7 segmentos donde se mostrarán los dígitos
* 3 Pulsadores: cambiar modo, pulsación simultánea y control del brillo
* Transistor S8050: ayuda a regular la diferencia de potencial del display para así poder modificar su brillo
* Potenciómetro: señal de entrada analógica para regular la velocidad de funcionamiento


## Especificaciones del funcionamiento
Voy a concretar algunos detalles del funcionamiento del dado en sus dos diferentes modos. 

### Modo normal o modo1
En este modo, el display se utiliza para representar los dígitos y el led del punto del display se muestra apagado para diferenciar visualmente en qué modo nos encontramos. Además, por hacer una doble comprobación, el led de la placa, también conocido como LED_BUILTIN, se encuentra apagado. 

### Modo variación del brillo
Los números se suceden a la velocidad establecida y tanto el led punto del display como el led LED_BUILTIN se encuentran encendidos mientras los números se suceden. El brillo se establece dentro de este modo con ayuda de una variable. Esta variable es la que se utiliza para cambiar la tensión del pin (pin PWM) que se conecta como base del transistor para modificar la luminosidad de los leds del display. Para incrementar o disminuir el valor de esta variable basta con entrar en este modo y pulsar el botón **+** o **-** respectivamente. 
