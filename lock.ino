/*
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA: Pin 10
* RST: Pin 9
*/

#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN);

int led1 = 7;
int led2= 6;
int led3 = 5;
int buz = 4;
int power = 8;
int serNum[5];
int cards[50][5];
int master[5] = {88, 242, 72, 16, 242};
bool access1 = false;
bool access2 = false;
bool master_access = false;
int card_counter = 0;

void setup()
{

    Serial.begin(9600);
    SPI.begin();
    rfid.init();

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(buz, OUTPUT);

    digitalWrite(led, LOW);
}

void loop()
{
    digitalWrite(led1,HIGH);
    if (rfid.isCard())
    {
        digitalWrite(buz,HIGH);
        delay(500);
        digitalWrite(buz,LOW);
        if (rfid.readCardSerial())
        {
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");

                for (int i = 0; i < sizeof(rfid.serNum); i++)
                {
                    if (rfid.serNum[i] != master[i])
                    {
                        access1 = false;
                        break;
                    }
                    else
                    {
                        access1 = true;
                    }
                }

        }
        if (access1 == true)
        {
            master_access = !master_access;

        }
        else
        {
            if (master_access == true)
            {
                for(int i = 0; i < 5; i++) cards[card_counter][i] = rfid.serNum[i];
                card_counter++;
                Serial.println("Tag Added Successfully");
            }
            else
            {
                for(int i = 0; i < card_counter; i++)
                {
                    for(int j = 0; j < 5; j++)
                    {
                        if(cards[i][j] == rfid.serNum[j])
                        {
                            access2 = true;
                        }
                        else
                        {
                            access2 = false;
                            break;
                        }
                    }
                    if(access2) break;
                }
                if (access2 == true)
                {
                    Serial.println("Welcome!");
                    digitalWrite(led2, HIGH);
                    delay(1000);
                    digitalWrite(led2, LOW);
                    digitalWrite(power, HIGH);
                    delay(1000);
                    digitalWrite(power, LOW);
                }
                else
                {
                    Serial.println("Not allowed!");
                    digitalWrite(led3, HIGH);
                    delay(500);
                    digitalWrite(led3, LOW);
                    delay(500);
                    digitalWrite(led3, HIGH);
                    delay(500);
                    digitalWrite(led3, LOW);
                }
            }
        }
    }
    rfid.halt();
}
/*
if (master_access == true)
            {
                Serial.println("Welcome!");
                digitalWrite(led, HIGH);
                delay(1000);
                digitalWrite(led, LOW);
                digitalWrite(power, HIGH);
                delay(1000);
                digitalWrite(power, LOW);
            }
            else
            {
                Serial.println("Not allowed!");
                digitalWrite(led, HIGH);
                delay(500);
                digitalWrite(led, LOW);
                delay(500);
                digitalWrite(led, HIGH);
                delay(500);
                digitalWrite(led, LOW);
            }*/
