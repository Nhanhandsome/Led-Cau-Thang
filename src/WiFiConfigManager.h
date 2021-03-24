#include <WiFi.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>
#include <CauThang.h>
char auth[] = "ttB1wQmTd-dTGBlITTW5P5RQwl-vrlyx";
String WIFI_NETWORK = "HAVICOM_T2";
String WIFI_PASSWORD = "havicom2021";
char ssid[50];
char pass[50];
#define maxEEPROM 100;
#define ssid_add 0
#define pass_add 50
#define num_led_on_step_add 98
#define num_step_add 99
uint16_t num_connect = 0;
WiFiManager wifiManager;
bool shouldSaveConfig = false;
char num_led_on_step[5] = "60";
char num_step[5] = "30";
void configModeCallback(WiFiManager *myWiFiManager)
{
    //  Serial.println("Entered config mode");
    Serial.println("configModeCallback");
    Serial.println(WiFi.softAPIP());                      //imprime o IP do AP
    Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede</p><p>}</p><p>//callback que indica que salvamos uma nova rede para se conectar (modo estação)
}
void saveConfigCallback()
{
    //  Serial.println("Should save config");
    Serial.println("Should save config");
    shouldSaveConfig = true;
}
void wifi_setup()
{
    WiFiManagerParameter custom_so_led_1bac("Num led on step","Num LED",num_led_on_step,5);
    WiFiManagerParameter custom_so_bac("Num step","Num step",num_step,5);
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    //wifiManager.setConfigPortalTimeout(600);
    wifiManager.addParameter(&custom_so_led_1bac);
    wifiManager.addParameter(&custom_so_bac);
    wifiManager.startConfigPortal("Led cau thang", "12345678");
    strcpy(num_led_on_step,custom_so_led_1bac.getValue());
    strcpy(num_step,custom_so_bac.getValue());
    so_led_1_bac = String(num_led_on_step).toInt();
    so_bac = String(num_step).toInt();
    if (shouldSaveConfig == true)
    {
        Serial.println("Saving config...");
        WIFI_NETWORK = wifiManager.getSSID();
        WIFI_PASSWORD = wifiManager.getPassword();
        Serial.print("SSID : ");
        Serial.println(WIFI_NETWORK);
        Serial.print("PASS : ");
        Serial.println(WIFI_PASSWORD);
        WIFI_NETWORK.toCharArray(ssid,WIFI_NETWORK.length()+1);
        WIFI_PASSWORD.toCharArray(pass,WIFI_PASSWORD.length()+1);
        EEPROM.write(ssid_add,WIFI_NETWORK.length()+1);
        for(char i = 1;i<=WIFI_NETWORK.length()+1;i++){
            EEPROM.write(ssid_add+i,ssid[i-1]);
        }
        EEPROM.commit();
        EEPROM.write(pass_add,WIFI_PASSWORD.length()+1);
        for(char i = 1;i<=WIFI_PASSWORD.length()+1;i++){
            EEPROM.write(pass_add+i,pass[i-1]);
        }
        EEPROM.write(num_led_on_step_add,so_led_1_bac);
        EEPROM.write(num_step_add,so_bac);
        EEPROM.commit();
    }
}
void wifi_begin()
{

    if (!EEPROM.begin(100))
    {
        Serial.println("EEPROM begin false");
    }
    //  EEPROM.write(ssid_add,0);
    //  EEPROM.write(pass_add,0);
    //  EEPROM.commit();
    Serial.println(EEPROM.read(ssid_add));
    Serial.println(EEPROM.read(pass_add));
    so_led_1_bac = EEPROM.read(num_led_on_step_add);
    so_bac = EEPROM.read(num_step_add);
    Serial.println(so_led_1_bac);
    Serial.println(so_bac);
    if(EEPROM.read(ssid_add)== 0 || EEPROM.read(ssid_add)== 255){
        WIFI_NETWORK.toCharArray(ssid,WIFI_NETWORK.length()+1);
        WIFI_PASSWORD.toCharArray(pass,WIFI_PASSWORD.length()+1);
        EEPROM.write(ssid_add,WIFI_NETWORK.length()+1);
        for(char i = 1;i<=WIFI_NETWORK.length()+1;i++){
            EEPROM.write(ssid_add+i,ssid[i-1]);
        }
        EEPROM.commit();
        EEPROM.write(pass_add,WIFI_PASSWORD.length()+1);
        for(char i = 1;i<=WIFI_PASSWORD.length()+1;i++){
            EEPROM.write(pass_add+i,pass[i-1]);
        }
        EEPROM.commit(); 
    }
}
void WiFi_Process()
{
    
    if (WiFi.status() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect
        char temp1 = EEPROM.read(ssid_add);
        if(temp1>0){
            for(uint16_t i = 0;i<=temp1;i++){
                ssid[i] = EEPROM.read(ssid_add+i+1);
            }
        }
        else{
            WIFI_NETWORK.toCharArray(ssid, WIFI_NETWORK.length() + 1);
        }
        char temp2 = EEPROM.read(pass_add);
        if(temp2>0){
            for(uint16_t i = 0;i<=temp2;i++){
                pass[i] = EEPROM.read(pass_add+i+1);
            }
        }else{
            WIFI_PASSWORD.toCharArray(pass, WIFI_PASSWORD.length() + 1);
        }
        // WIFI_NETWORK.toCharArray(ssid, WIFI_NETWORK.length() + 1);
        // WIFI_PASSWORD.toCharArray(pass, WIFI_PASSWORD.length() + 1);
        Serial.print("SSIS EEPROM: ");
        Serial.println(ssid);
        Serial.print("PASS EEPROM: ");
        Serial.println(pass);        
        WiFi.begin(ssid, pass);
        Serial.print("Connecting to ");
        Serial.print(WIFI_NETWORK);
        delay(500);
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(200);
            num_connect++;
            if (num_connect >= 50)
            {
                wifi_setup();
            }
        }
        // Connect to the network
        Serial.print("Connected ");
        Serial.println(WiFi.SSID());
        Serial.print("IP local: ");
        Serial.println(WiFi.localIP());
        Blynk.begin(auth,ssid,pass);
        //Serial.print('.');
    }
    else
    {
        Blynk.run();
        num_connect = 0;
    }
}