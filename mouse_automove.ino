// # this example is using the library https://github.com/joseluu/ESP32-BLE-Mouse
// Go to ghttps://github.com/joseluu/ESP32-BLE-Mouse.
// Click the green Code button and select Download ZIP.
// Save the ZIP file to your computer.
// Install the Library in Arduino IDE:
// Open the Arduino IDE.
// Go to Sketch > Include Library > Add .ZIP Library.
// Navigate to the downloaded ESP32-BLE-Mouse-master.zip file and select it.
// The IDE will import the library.
// Verify Installation:
// Go to Sketch > Include Library > Contributed Libraries.
// You should see ESP32-BLE-Mouse listed.
// You can now include it in your sketch with #include <BleMouse.h>.

// a good generic BLE tutorial is available here: https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/

// IMPORTANT:
// Works with esp32 (by Espressif Systems) version 3.2
// make sure you have selected the proper board
// if serial messages in the serial monitor becomes garbled it means that the 
// selected board does not have the same xtal frequency as the actual board
// Heltec Wifi Kit 32 has a 26MHz xtal
// TTGO Lora32-OLED has a 40MHz xtal
//
// under Windows 11, open the dialog "Bluetooth & Devices" and do "Add device"
// the mouse will simply appear as Mouse

#include <esp_log.h>
#include <BleMouse.h>

static const char* LOG_TAG = "MyESP32Mouse";

std::string name = "Mouse";

BleMouse bleMouse(name);

class MySecurity : public BLESecurityCallbacks
{
  ~MySecurity()
  {
  }

  uint32_t onPassKeyRequest()
  {
    ESP_LOGI(LOG_TAG, "PassKeyRequest");
    return 123456;
  }

  void onPassKeyNotify(uint32_t pass_key)
  {
        ESP_LOGI(LOG_TAG, "On passkey Notify number:%d", pass_key);
  }

  bool onSecurityRequest()
  {
      ESP_LOGI(LOG_TAG, "On Security Request");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
  {
    ESP_LOGI(LOG_TAG, "Authent done let's have BLE work!");
    if(cmpl.success)
    {
      uint16_t length;
      esp_ble_gap_get_whitelist_size(&length);
      ESP_LOGD(LOG_TAG, "size: %d", length);
    }
  }
};

void setup() {
  Serial.begin(115200);
  esp_log_level_set(LOG_TAG, ESP_LOG_DEBUG); // Set log level to INFO
  delay(1000); // Small delay to ensure Serial is ready
  ESP_LOGI(LOG_TAG, "log: ESP32 initialized successfully!");

  Serial.println("Starting BLE work!");
  bleMouse.begin();

  BLESecurity *pSecurity = new BLESecurity(); 
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  pSecurity->setCapability(ESP_IO_CAP_IN); 
}


void loop() {
  if(bleMouse.isConnected()) {

    unsigned long startTime;

    Serial.println("Move mouse pointer up");
    bleMouse.move(0,-1);

    delay(5000);

    Serial.println("Move mouse pointer down");
    bleMouse.move(0,1);
    delay(500000);

  }
}
