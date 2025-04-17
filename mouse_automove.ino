#include <BleMouse.h>

BleMouse bleMouse("Mouse Jose");

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
    ESP_LOGI(LOG_TAG, "Starting BLE work!");
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
