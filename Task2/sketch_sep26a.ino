void setup() {
  Serial.begin(9600);  // Налаштування серіальної передачі на швидкості 9600 бод
  while (!Serial) {
    ; // Чекаємо на підключення серіального порту
  }
  Serial.println("Arduino ready to receive data");  // Стартове повідомлення
}

void loop() {
  // Перевіряємо, чи є доступні дані для читання
  if (Serial.available() > 0) {
    // Читаємо повідомлення від клієнта (до символу '\n')
    String receivedMessage = Serial.readStringUntil('\n');
    
    // Логіка обробки повідомлення (додамо відповідь)
    String response = "Hello Client!";  // Відповідь для клієнта
    
    // Виводимо на серіальний порт отримане повідомлення та відповідь
    Serial.println(response);         // Відправляємо відповідь клієнту
  }
}
