String input;

void setup() {
  Serial.begin(115200);
  pinMode(7, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    input += c;
  }

  if (input.length() > 0) {
    Serial.println(input);
    
    if (input.equals("1")) {
      digitalWrite(7, HIGH);
      Serial.println("LED ON");
    } else if (input.equals("0")) {
      digitalWrite(7, LOW);
      Serial.println("LED OFF");
    }

    input = "";
  }
}
