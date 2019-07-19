//Programa : Modulo Arduino Bluetooth HC-05 - Microprocessadores
//Autor : Matheus Nogueira Martino

//Carrega a biblioteca SoftwareSerial
#include <SoftwareSerial.h>

const int Lamp4 = 3;
const int Lamp3 = 4;
const int Lamp2 = 5;
const int Lamp1 = 6;
const int BT_habilita = 8; //Pino habilita Módulo BT.
const int AT_habilita = 9; //Pino habilita Modo AT no Módulo BT.
const int Botao = 7; //Um botão. Normalmente nivel 1, nível 0 quando precionado.
bool BT_habilita_status;
bool AT_habilita_status;
bool led = false;
bool Lamp1_estado = false;
bool Lamp2_estado = false;
bool Lamp3_estado = false;
bool Lamp4_estado = false;

String command = "";  // Stores response of bluetooth device
// which simply allows n between each
// response.

//Define os pinos para a SoftwareSerial
SoftwareSerial btSerial(10, 11); // RX, TX

void setup()
{
  //Inicialização dos pinos do Arduino.
  pinMode(AT_habilita, OUTPUT);
  pinMode(BT_habilita, OUTPUT);
  pinMode(Botao, INPUT);
  pinMode(Lamp1, OUTPUT);
  pinMode(Lamp2, OUTPUT);
  pinMode(Lamp3, OUTPUT);
  pinMode(Lamp4, OUTPUT);


  //Inicia a serial
  Serial.begin(57600);
  //Inicia a SoftwareSerial configurada nas portas 10 e 11
  btSerial.begin(38400); //Para comandos AT usar 38400 9600

  //Serial.println("Digite os comandos AT :");

  //Inicializa Módulo BT em modo AT.
  ATmode(true);
  enviaBTcomando("AT+NAME=Automacao\r\n");
  enviaBTcomando("AT+UART=9600,1,2\r\n");
  ATmode(false);
 // btSerial.end;

  btSerial.begin(9600);
  /*
    btSerial.write("AT+NAME=Automacao\r\n");

    btSerial.write("AT+INQM=0,9,40\r\n");
    btSerial.write("AT+PSWD=1234\r\n");
    btSerial.write("AT+UART=9600,1,2\r\n");
    btSerial.write("AT+CMODE=1\r\n"); //conecta qualquer endereço
    btSerial.write("AT+ROLE=0\r\n"); //0=modo Slave 1=modo Master
  */
  
}

void loop()
{
  // Read device output if available.
  if (btSerial.available())
  {
    while (btSerial.available())
    {
      delay(100);
      // While there is more to be read, keep reading.
      command += (char)btSerial.read();
    }
    Serial.println(command);
    
    if (command == "a")
    {
      Serial.println("Lâmpada 1");
      if (Lamp1_estado == false)
      {
        digitalWrite(Lamp1, HIGH);
        Lamp1_estado = true;
      }
      else
      {
        digitalWrite(Lamp1, LOW);
        Lamp1_estado = false;
      }
    }
    else if (command == "b")
    {
      Serial.println("Lâmpada 2");
      if (Lamp2_estado == false)
      {
        digitalWrite(Lamp2, HIGH);
        Lamp2_estado = true;
      }
      else
      {
        digitalWrite(Lamp2, LOW);
        Lamp2_estado = false;
      }
    }
    else if (command == "c")
    {
      Serial.println("Lâmpada 3");
      if (Lamp3_estado == false)
      {
        digitalWrite(Lamp3, HIGH);
        Lamp3_estado = true;
      }
      else
      {
        digitalWrite(Lamp3, LOW);
        Lamp3_estado = false;
      }
    }
    else if (command == "d")
    {
      Serial.println("Lâmpada 4");
      if (Lamp4_estado == false)
      {
        digitalWrite(Lamp4, HIGH);
        Lamp4_estado = true;
      }
      else
      {
        digitalWrite(Lamp4, LOW);
        Lamp4_estado = false;
      }
    }
    command = ""; // No repeats
  }

  // Read user input if available.
  if (Serial.available())
  {
    delay(10); // The DELAY!
    btSerial.write(Serial.read());
  }

  //Botão que mudo o modo de operação entre Programação/Operação
  if (digitalRead(7) == LOW)
  {
    if (AT_habilita_status == true)
    {
      ATmode(false);
    }
    else
    {
      ATmode(true);
    }
    delay(500); //Debouncing do botão!
  }

}

//Função que faz a troca do modo de operação do módulo BT
//Modo normal com pino 9 do Arduino em LOW, modo de configuração quando em HIGH
//
//ATmode(true) ou ATmode(false) -> Liga ou desliga o modo de configuração
//
void ATmode (bool mode)
{
  if (mode == true)
  {
    digitalWrite(BT_habilita, LOW);
    delay(100);
    Serial.println("Ativando modo de comandos AT");
    digitalWrite(AT_habilita, HIGH);
    AT_habilita_status = true;
    digitalWrite(BT_habilita, HIGH);
    delay(1000);
    delay(1000);
    delay(1000);
  }
  else
  {
    digitalWrite(BT_habilita, LOW);
    delay(100);
    Serial.println("Dasativando modo de comandos AT");
    digitalWrite(AT_habilita, LOW);
    AT_habilita_status = false;
    digitalWrite(BT_habilita, HIGH);
    delay(1000);
    delay(1000);
    delay(1000);
  }
}


void enviaBTcomando (String comando)
{
  char comandoBuff[20];
  comando.toCharArray(comandoBuff, 20);
  btSerial.write(comandoBuff);

  String resposta = "";
  int in_dado = 0x0;

  while ((resposta == "" || " " || "\r" || " \r" || "\n" || " \n" || "\r\n" || " \r\n" || "0")) //|| (in_dado == 0x0))
  {
    // Read device output if available.
    if (btSerial.available())
    {
      while (btSerial.available())
      {
        delay(10);
        // While there is more to be read, keep reading.
        in_dado = btSerial.read();
        resposta += (char)in_dado;
        Serial.println(in_dado);
      }
        if (resposta == "OK")
        {
          Serial.println("Resposta 1:");
          Serial.println(resposta);
          resposta = ""; // No repeats
          return;
        }
        else
        {
          Serial.println("Resposta 2:");
          Serial.println(resposta);
          Serial.println("ERRO: A resposta não é OK!");
          resposta = ""; // No repeats
          return;
        }
    }
  }
}
