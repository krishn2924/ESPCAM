
#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER 
#define myled 4
#include "camera_pins.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>



const char * ssid = "crear-campus";
const char * password  = "crearworld09";


const char* host = "maker.ifttt.com";

const int httpsPort = 443;

void startCameraServer();

int countON =0;
int countOFF =0; 


void setup() {
  Serial.begin(115200);
    Serial.begin(115200);
Serial.setDebugOutput(true);
 
  Serial.println();  
  pinMode(4, OUTPUT);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

 



#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  Serial.begin(115200);

  Serial.println();

  Serial.print("connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("");

  Serial.println("WiFi connected");

  Serial.println("IP address: ");

  Serial.println(WiFi.localIP());

 

  WiFiClientSecure client;

  Serial.print("connecting to ");

  Serial.println(host);

  if (!client.connect(host, httpsPort)) {

    Serial.println("connection failed");

    return;

  }

  startCameraServer();
 

  String url = "/trigger/ESP32/with/key/pBpHP8bxKNUCrbmIo_0CVnDoDpazgzL9Wx1jl0EB3pM";

  Serial.print("requesting URL: ");

  Serial.println(url);

 

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +

               "Host: " + host + "\r\n" +

               "User-Agent: BuildFailureDetectorESP32\r\n" +

               "Connection: close\r\n\r\n");

 

  Serial.println("request sent");

 while (client.connected()) {

    String line = client.readStringUntil('\n');

    if (line == "\r") {

    Serial.println("headers received");

      break;

   }

  }

  String line = client.readStringUntil('\n');

 

  Serial.println("reply was:");

  Serial.println("==========");

 Serial.println(line);

  Serial.println("==========");

  Serial.println("closing connection");

}
  
 
  

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);

}
