#include "RF24.h"
RF24 myRadio (7, 8);
byte addresses[][6] = {"0", "1","2"};

/*Variables*/
const int LED = 4;
const int BOTON = 2;
int val = 0;
int state1 = 0;
int old_val = 0;

struct package
{
  int led1 = 0;
  int led2 = 0;
  bool state = 0;

};
typedef struct package Package;
Package data;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTON, INPUT);

  Serial.begin(115200);
  delay(100);
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.openWritingPipe(addresses[0]);

}

void loop() {

  delay(5);
  myRadio.stopListening();
  val = digitalRead(BOTON); // Read the state of the button
  if ((val == HIGH) && (old_val == LOW)) {
    data.led1 = 1 - data.led1;
     myRadio.write(&data, sizeof(data));
     delay(10);
  }

  old_val = val; // old state value

  if (data.led1 == 1) {
  digitalWrite(LED, HIGH); // Turn on the LED

  }
  if(data.led1==0 and !data.state)
    digitalWrite(LED, LOW); // Turn off LED


  myRadio.startListening();

     if (myRadio.available())
  {
    while (myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
    }

    if(!data.state){
       digitalWrite(LED, HIGH);
       Serial.println("Gas detected");
    }

    Serial.println(data.state);
  }
}
