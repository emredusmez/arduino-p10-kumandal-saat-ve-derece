#include <SPI.h>
#include <Wire.h> // wire kütüphanesini ekledim sende yoksa indir...yoksa derlerken hata verir...
#include <TimerOne.h>
#include <Droid_Sans_16.h>
#include <EEPROM.h>
#include <TrueRandom.h>
#include <DMD.h>
#include <IRremote.h>
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
/*-----( Declare Constants )-----*/
#define ONE_WIRE_BUS 2 /*-(Connect to Pin 2 )-*/
//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
/*-----( Declare objects )-----*/
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire ourWire(ONE_WIRE_BUS);

/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature sensors(&ourWire);
IRrecv irrecv(3);
decode_results results;
String saat1;
String dakika1;
String saniye1;
String defaultparlaklik = "254";
//TUŞ TANIMLARI
String ackapa = "16753245";// açma kapatma
String arti = "16712445";// parlaklık yükselt
String eksi = "16750695";//parlaklik düşür
String prev = "16769055";//saat ayarı
String next = "16748655";//dakika ayarı
String mode = "16769565";// ayar modu
String stac = "16738455"; //saat aç
String scac = "16756815"; // sıcaklık aç
String stkpt = "16724175"; // saat kapat
String sckpt = "16743045";//sıcaklık kapat
String stsa = "16716015";// saat süre arttır
String stsd = "16728765";//saat süre düşür
String scsa = "16734885";// sıcaklık süre arttır
String scsd = "16732845";// sıcaklıksüre düşür
String saac = "16754775";
String yedi = "";
String sekiz = "55A59747";
String dokuz = "";
String sifir = "DA6C72F1";
int saatayar = 0;
int dakikaayar = 0;
// TUŞ TANIMLARI BİTİŞ
String basilantus = "";
//parlakliklar
int parlakliklar[] = {0,15, 1020, 255};
byte parlakliksecim = 2;
byte saatacikmi = 1;
byte sicaklikacikmi = 1;
//parlakliklar
int ayarlamami = 0;
int kactane = 0;
String deger = "";
byte dizi[50];
byte acikmi = 1;
int sicakliksure = 20;
int saatsure = 20;
int sayac = 0;
int sicaklikkilit = 1;
int saatkilit = 0;
byte saatsure2 = 3;
byte sicakliksure2 = 3;
int sureayarlamami = 0;
int sicakliksilmi = 0;
int saatsilmi = 0;
RTC_DS1307 rtc;

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}
void test()
{

}
void setup() {
pinMode(10,OUTPUT);
  attachInterrupt(1, test, CHANGE);
  delay(1000);
  Timer1.initialize( 1000);           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
  dmd.selectFont(Droid_Sans_16);
  irrecv.enableIRIn();
  //  Wire.begin();
  Serial.begin(9600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  rtc.begin(); // Start the RTC library code

  // This line sets the RTC with an explicit date & time, for example to set
  // May 21, 2015 at 6pm you would call: (use 24 hour time)


  sensors.begin();
  pinMode(9, OUTPUT);
  if(EEPROM.read(55)!=5)
  {
    EEPROM.write(10,3);
    delay(200);
    EEPROM.write(11,1);
    delay(200);
    EEPROM.write(12,1);
    delay(200);
    EEPROM.write(13,30);
    delay(200);
    EEPROM.write(14,30);
    EEPROM.write(55,5);
    
    
    }
    
  parlakliksecim = EEPROM.read(10);
  saatacikmi = EEPROM.read(11);
  sicaklikacikmi = EEPROM.read(12);
  saatsure = EEPROM.read(13);
  sicakliksure = EEPROM.read(14);
}





void loop()
{
  digitalWrite(10,HIGH);
  delay(100);

  if (irrecv.decode(&results)) {


    basilantus = String(results.value);
    Serial.println(basilantus);
    irrecv.resume(); // Receive the next value
  }

  if (acikmi == 1)
  {
    dmd.setBrightness(parlakliklar[parlakliksecim]);
    if (basilantus == arti)
    {
      if(parlakliksecim!=3)
      {
        parlakliksecim+=1;
        }
      //parlakliksecim=2;
      EEPROM.write(10, parlakliksecim);
      dmd.setBrightness(parlakliklar[parlakliksecim]);

    }
    if (basilantus == eksi)
    {
      if(parlakliksecim!=1)
      {
        parlakliksecim-=1;
        }
      //parlakliksecim=1;
      EEPROM.write(10, parlakliksecim);
      dmd.setBrightness(parlakliklar[parlakliksecim]);

    }

    if (basilantus == stac)
    {

saatsure = EEPROM.read(13);
      EEPROM.write(11, 1);
      saatacikmi = 1;
      saatkilit=1;
      sicaklikkilit=0;
      sayac=0;
      dmd.clearScreen( true );
    }
    if (basilantus == scac)
    {
      sicakliksure = EEPROM.read(14);
      EEPROM.write(12, 1);
      sicaklikacikmi = 1;
      saatkilit=0;
      sicaklikkilit=1;
      sayac=0;
      dmd.clearScreen( true );
    }
    if (basilantus == stkpt)
    {
      if(sicaklikacikmi==1)
      {
      EEPROM.write(11, 0);
      saatacikmi = 0;
      saatsure = 0;
      sicaklikkilit=1;
      saatkilit=0;
      dmd.clearScreen( true );
      }
    }
    if (basilantus == sckpt)
    {
      if(saatacikmi==1)
      {
      EEPROM.write(12, 0);
      sicaklikacikmi = 0;
      sicakliksure = 0;
      sicaklikkilit=0;
      saatkilit=1;
      dmd.clearScreen( true );
      }
    }

    if (basilantus == stsa)
    {
      if (saatsure == 200)
      {
        saatsure == 10;
        EEPROM.write(13, saatsure);
      }
    }
    if (basilantus == scsa)
    {
      if (sicakliksure == 200)
      {
        sicakliksure == 10;
        EEPROM.write(14, sicakliksure);
      }
    }




    if (ayarlamami == 1)
    {

      if (basilantus == mode)
      {
        // setDS3231time(saatayar, dakikaayar, 00, 5, 5, 5, 5);
        ayarlamami = 0;
      }


      if (basilantus == prev)
      {
        if (saatayar == 59)
        {
          saatayar = 00;
        }
        else
        {
          saatayar++;
        }


      }



      if (basilantus == next)
      {
        if (dakikaayar == 59)
        {
          dakikaayar = 00;
        }
        else
        {
          dakikaayar++;
        }

      }
      saatayarla();


    }
    else
    {
      if (basilantus == mode)
      {
        ayarlamami = 1;
        Serial.println("mod");
      }


      if (sureayarlamami == 1)
      {

        if (basilantus == stsa)
        {
          if (saatsure == 200)
          {
            saatsure = 10;
            EEPROM.write(13, saatsure);
          }
          else
          {
            saatsure += 10;
            EEPROM.write(13, saatsure);
            }
        }
        if (basilantus == scsa)
        {
          if (sicakliksure == 200)
          {
            sicakliksure = 10;
            EEPROM.write(14, sicakliksure);
          }
          else
          {
            sicakliksure += 10;
            EEPROM.write(14, sicakliksure);
            }
          
         
        }
       dmd.clearScreen( true );
          delay(10);
         int ys=sicakliksure/10;
          int yss=saatsure/10;
          String ys1;
          String yss1;
          if(String(ys).length()==1)
          {
           ys1="0"+String(ys);
            }
            else
            {
              ys1=String(ys);
              }
            if(String(yss).length()==1)
            {
              yss1="0"+String(yss);
              }
              else
              {
                yss1=String(yss);
                }
          dmd.drawChar(1, -1, ys1[0], GRAPHICS_NORMAL );
          dmd.drawChar(8, -1, ys1[1], GRAPHICS_NORMAL );

          dmd.drawChar(18, -1, yss1[0], GRAPHICS_NORMAL );
          dmd.drawChar(25, -1, yss1[1], GRAPHICS_NORMAL );
 if (basilantus == saac)
          {
            sureayarlamami = 0;
            dmd.clearScreen( true );
          }
      }
       else
        {

          if (basilantus == saac)
          {
            sureayarlamami = 1;
          }
          ekrandagoster ();
        }

    }
  }

  if (basilantus == ackapa)
  {
    if (acikmi == 1)
    {
      acikmi = 0;
      parlakliksecim = 0;
      dmd.setBrightness(parlakliklar[parlakliksecim]);

    }
    else
    {
      acikmi = 1;
      parlakliksecim = EEPROM.read(10);
      dmd.setBrightness(parlakliklar[parlakliksecim]);
    }

  }






  basilantus = "";
}
//----------------------------------


void ekrandagoster ()
{

  if (sicaklikkilit == 1)
  {
    sicaklikgoster();
    Serial.println("sc");
  }





  if (saatkilit == 1)
  {
    saatgoster();
    Serial.println("st");
  }

}


void sicaklikgoster ()
{
  if (sayac == sicakliksure)
  {
    saatkilit = 1;
    sicaklikkilit = 0;
    sayac = 0;
    if (saatsure == 0)
    {}
    else
    {
      if (sicakliksure != 0)
      {

        saatsilmi = 0;
        sicakliksilmi = 0;
        sayac = 0;
      }
    }
  }
  else
  {
    if (sicakliksilmi == 0)
    { sicakliksilmi = 1;
      sensors.requestTemperatures();
      // dmd.clearScreen();
      dmd.clearScreen( true );
    }
    sayac++;
    //sicaklikolc();
    sicaklikekran();
Serial.print(sayac);
Serial.print(sicakliksure);
Serial.print(saatsilmi);
Serial.print(sicakliksilmi);
  }
}

void saatgoster ()
{
  if (sayac == saatsure)
  {

    sicaklikkilit = 1;
    saatkilit = 0;
    sayac = 0;
    if (sicakliksure == 0)
    {}
    else
    {
      if (saatsure != 0)
      {


        saatsilmi = 0;
        sicakliksilmi = 0;

        sayac = 0;
      }
    }
  }
  else
  {
    if (saatsilmi == 0)
    { saatsilmi = 1;
      dmd.clearScreen( true );
    }
    sayac++;

    saatgoster1();

  }

}

//-------------------------------------------------
String sanyk = "0";
int oncekisanyk = 0;
//---------------------------------------------------------EKRANDA SAAT GÖSTERME YAZILIM------------------------------------------------


void sicaklikekran()
{
  String sicaklik;
  String sicaklik0;
  String sicaklik1;

  sicaklik1 = String(sensors.getTempCByIndex(0));
  if (sicaklik1.substring(0, 1) == "-")
  {
    if (sicaklik1.length() == 6)
    {
      sicaklik = sicaklik1.substring(1, 2);
      sicaklik0 = sicaklik1.substring(2, 3);

    }
    else if (sicaklik1.length() == 5)
    {
      sicaklik0 = sicaklik1.substring(1, 2);
      sicaklik = "0";


    }

    dmd.drawChar(0, 0, '-', GRAPHICS_NORMAL );
    dmd.drawChar(5, 0, sicaklik[0], GRAPHICS_NORMAL );
    dmd.drawChar(12, 0, sicaklik0[0], GRAPHICS_NORMAL );
    dmd.drawChar( 19, 0, 'C', GRAPHICS_NORMAL );
  }
  else
  {
    if (sicaklik1.length() == 5)
    {
      sicaklik = sicaklik1.substring(0, 1);
      sicaklik0 = sicaklik1.substring(1, 2);

    }
    else if (sicaklik1.length() == 4)
    {
      sicaklik = sicaklik1.substring(0, 1);

      sicaklik0 = "0";


    }


    dmd.drawChar(3, 0, sicaklik[0], GRAPHICS_NORMAL );
    dmd.drawChar(10, 0, sicaklik0[0], GRAPHICS_NORMAL );
    dmd.drawChar( 17, 0, 'C', GRAPHICS_NORMAL );

    sensors.requestTemperatures();
  }
}
//-----------------------------------

int yandimi = 0;
void saatayarla()
{
  dmd.clearScreen( true );

  String saatk = String(saatayar);
  String dakik = String(dakikaayar);


  if (dakik.length() == 1)
  {
    dakik = "0" + dakik;
  }
  if (saatk.length() == 1)
  {
    saatk = "0" + saatk;
  }
  String d1 = dakik.substring(0, 1);
  String d2 = dakik.substring(1, 2);;
  String s1 = saatk.substring(0, 1);;
  String s2 = saatk.substring(1, 2);;

  String parlak = defaultparlaklik;



  if (yandimi == 0)
  {

    yandimi = 1;



    dmd.drawChar(1, -1, s1[0], GRAPHICS_NORMAL );
    dmd.drawChar(8, -1, s2[0], GRAPHICS_NORMAL );
    dmd.drawChar(15, -3, ':', GRAPHICS_NORMAL );
    dmd.drawChar(18, -1, d1[0], GRAPHICS_NORMAL );
    dmd.drawChar(25, -1, d2[0], GRAPHICS_NORMAL );
    
    
  }
  else
  {

    yandimi = 0;
    //dmd.clearScreen();
  }
  rtc.adjust(DateTime(2015, 5, 21, saatayar, dakikaayar, 0));


}
// ------------------------------ -

void saatgoster1() {

  DateTime now = rtc.now();
  String dakik = String(now.minute(), DEC);
  String saatk = String(now.hour(), DEC);
  dakikaayar = dakik.toInt();
  saatayar = saatk.toInt();
  saatayar = saatk.toInt();
  dakikaayar = dakik.toInt();

  sanyk = String(now.second(), DEC);
  if (dakik.length() == 1)
  {
    dakik = "0" + dakik;
  }
  if (saatk.length() == 1)
  {
    saatk = "0" + saatk;
  }
  String d1 = dakik.substring(0, 1);
  String d2 = dakik.substring(1, 2);;
  String s1 = saatk.substring(0, 1);;
  String s2 = saatk.substring(1, 2);;

  String parlak = defaultparlaklik;

if(s1[0]=='1')
{
   dmd.drawChar(0, -1, s1[0], GRAPHICS_NORMAL );
  dmd.drawChar(7, -1, s2[0], GRAPHICS_NORMAL);
  if (yandimi == 0)
  {
    dmd.drawChar(14, -3, ':', GRAPHICS_NORMAL );
    yandimi = 1;
  }
  else
  {
    dmd.drawChar(14, -3, ';', GRAPHICS_NORMAL );
    yandimi = 0;
  }

  dmd.drawChar(17, -1, d1[0], GRAPHICS_NORMAL );
  dmd.drawChar(24, -1, d2[0], GRAPHICS_NORMAL );
  }
  else
  {
    dmd.drawChar(1, -1, s1[0], GRAPHICS_NORMAL );
  dmd.drawChar(8, -1, s2[0], GRAPHICS_NORMAL);
  if (yandimi == 0)
  {
    dmd.drawChar(15, -3, ':', GRAPHICS_NORMAL );
    yandimi = 1;
  }
  else
  {
    dmd.drawChar(15, -3, ';', GRAPHICS_NORMAL );
    yandimi = 0;
  }
  dmd.drawChar(18, -1, d1[0], GRAPHICS_NORMAL );
  dmd.drawChar(25, -1, d2[0], GRAPHICS_NORMAL );
    }
 

}
//---------------------------------------------------------EKRANDA SAAT GÖSTERME YAZILIM BİTİŞ------------------------------------------------



