#define BLYNK_TEMPLATE_ID "TMPL3zSLjE_IB"
#define BLYNK_TEMPLATE_NAME "Smart Home"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi & Blynk
char auth[] = "OWQ0U-hhuiwmaMdzOjOqVK3ot_Goj233";
char ssid[] = "vivo Y28s 5G";
char pass[] = "rinku62s";

// Pins
int ledPin1 = 2;
int irPin1  = 5;
int gasPin = 7;
int buzzerPin = 35;
int buttonPin = 10;
int irPin2 = 8;
int ledPin2 = 12;
int ledPin3 = 4;

bool manualMode = false;

// To avoid repeated notifications
bool gasAlertSent = false;
bool emergencySent = false;

// Dashboard Button
BLYNK_WRITE(V0)
{
  int state = param.asInt();

  if(state == 1)
  {
    manualMode = true;
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  }
  else
  {
    manualMode = false;
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(ledPin1, OUTPUT);
  pinMode(irPin1, INPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLDOWN);

  pinMode(gasPin, INPUT);

  pinMode(irPin2, INPUT);

  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  Blynk.begin(auth, ssid, pass);

  Serial.println("System Started");
}

void loop()
{
  Blynk.run();

  int irValue1 = digitalRead(irPin1);
  int irValue2 = digitalRead(irPin2);
  int gasValue = digitalRead(gasPin);

  // ---------------- GAS ALERT ----------------
  Blynk.virtualWrite(V2, gasValue);
  if(gasValue == 1)
  {
    digitalWrite(ledPin3, HIGH);
    Serial.println("Gas detected");
    Blynk.virtualWrite(V5, "🚨 GAS LEAK!");

    // Send notification only once
    if(!gasAlertSent)
    {
      Blynk.logEvent("gas_alert", "🚨 Gas Leakage Detected!");
      gasAlertSent = true;
    }
  }
  else
  {
    digitalWrite(ledPin3, LOW);
    Serial.println("Gas not detected");
    Blynk.virtualWrite(V5, "✅ SAFE");

    gasAlertSent = false;
  }

  // ---------------- IR AUTOMATION ----------------

  if(manualMode == false)
  {
    if(irValue1 == 0)
    {
      digitalWrite(ledPin1, HIGH);
    }
    else
    {
      digitalWrite(ledPin1, LOW);
    }

    if(irValue2 == 0)
    {
      digitalWrite(ledPin2, HIGH);
    }
    else
    {
      digitalWrite(ledPin2, LOW);
    }
  }

  // ---------------- EMERGENCY BUTTON ----------------

  int buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH)
  {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Emergency");
    Blynk.virtualWrite(V4, "🚨 EMERGENCY ALERT!");

    // Send notification only once
    if(!emergencySent)
    {
      Blynk.logEvent("emergency_alert", "🚨 Emergency Button Pressed!");
      emergencySent = true;
    }
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
  

    Blynk.virtualWrite(V4, "SAFE");

    emergencySent = false;
  }

  delay(200);
}