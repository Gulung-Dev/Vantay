#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

bool hasFinger = false; // Trạng thái vân tay trước đó

void setup() {
    Serial.begin(9600);
    Serial.println("Cảm biến vân tay AS608 - Đọc liên tục");

    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Kết nối cảm biến thành công!");
    } else {
        Serial.println("Lỗi! Không tìm thấy cảm biến vân tay!");
        while (1);
    }
}

void loop() {
    uint8_t p = finger.getImage();

    if (p == FINGERPRINT_OK) {
        if (!hasFinger) {
            Serial.println("📍 Phát hiện vân tay! Đang xử lý...");
        }
        hasFinger = true;
        processFingerprint();
    } 
    else if (p == FINGERPRINT_NOFINGER) {
        if (hasFinger) {
            Serial.println("❌ Không còn vân tay.");
        }
        hasFinger = false;
    }

    delay(300); // Giảm tải CPU, tránh spam
}

void processFingerprint() {
    uint8_t p = finger.image2Tz();
    if (p == FINGERPRINT_OK) {
        Serial.println("✅ Ảnh vân tay hợp lệ, đang tìm kiếm...");
    } else if (p == FINGERPRINT_IMAGEMESS) {
        Serial.println("⚠ Ảnh vân tay quá mờ, thử lại!");
        return;
    } else {
        return; // Không in thông báo lỗi không cần thiết
    }

    p = finger.fingerSearch();
    if (p == FINGERPRINT_OK) {
        Serial.print("🎯 Tìm thấy ID: "); 
        Serial.print(finger.fingerID);
        Serial.print(" - Độ tin cậy: ");
        Serial.println(finger.confidence);
    } else {
        Serial.println("❌ Không tìm thấy trong cơ sở dữ liệu.");
    }
}
