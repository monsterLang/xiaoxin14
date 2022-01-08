/************************
  func 1 analogRead sound
*************************/
# include <Arduino.h>
# define A_SOUND 36
# define A_LIGHT 39
//sound is IO36
//light is IO39

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin 0:
  //读取A0脚输入值大小（0-5V对应0~1023）
  int sensorValue = analogRead(A_SOUND);
  
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1); // delay 1 ms in between reads for stability

}

///************************
//  func 2 light led
//*************************/
//#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
//Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
//
//bool status_led = false;
//
//void led_on(){
//  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
//  pixels.show();
//  Serial.println("led_on");
//}
//
//void led_off(){
//  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
//  pixels.show();
//  Serial.println("led_off");
//}
//
//void change_led_status(){
//  if (status_led == false){
//    led_on();
//    status_led = true;
//  }
//  else if (status_led == true){
//    led_off();
//    status_led = false;
//  }
//  else
//    Serial.println("status_led error.");
//}
//
//
//void setup() {
//  Serial.begin(9600);
//  pixels.begin();
//
//  // set time
//  
//}
//void loop() {
//  // judge time IRQ
//  change_led_status();
//  delay(500);
//}


/////************************
////  func 3 time irq -- mstimers
////  error:  MsTimer2 library only works on AVR architecture
////*************************/
//#include <MsTimer2.h>               //定时器库的 头文件
//
//
//#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
//Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
//
//static bool status_led = false;
//
//void led_on(){
//  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
//  pixels.show();
//  Serial.println("led_on");
//}
//
//void led_off(){
//  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
//  pixels.show();
//  Serial.println("led_off");
//}
//
//void change_led_status(){
//  if (status_led == false){
//    led_on();
//    status_led = true;
//  }
//  else if (status_led == true){
//    led_off();
//    status_led = false;
//  }
//  else
//    Serial.println("status_led error.");
//}
//
//void setup() 
//{
//  Serial.begin(9600);
//  pixels.begin();
//  
//  MsTimer2::set(1, change_led_status);        // 中断设置函数，每 500ms 进入一次中断
//  MsTimer2::start();                //开始计时
//
//}
// 
//void loop()
//{
// 
//}

/////************************
////  func 3 time irq -- mstimers 2 
////  error:  MsTimer2 library only works on AVR architecture
////*************************/
//#include <MsTimer2.h>     //定时器库的头文件
//int tick = 0; //计数值
// 
////中断服务程序
//void onTimer()
//{
//  Serial.print("timer ");
//  Serial.println(tick++);
//}
// 
//void setup()
//{
//  Serial.begin(9600); //初始化串口
// 
//  MsTimer2::set(1000, onTimer); //设置中断，每1000ms进入一次中断服务程序 onTimer()
//  MsTimer2::start(); //开始计时_开启定时器中断
//  //MsTimer2::stop(); //关闭定时器中断
//}
// 
//void loop()
//{
//}
//


/////************************
////  func 3 time irq -- hw_timer_t
////  error: reboot
////*************************/
//
//#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
//Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
//# define A_SOUND 36
//
//volatile bool status_led = false;
//volatile int irq_count = 0;
//int count_sound = 0;
//
//void led_on(){
//  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
//  pixels.show();
//  Serial.println("led_on");
//}
//
//void led_off(){
//  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
//  pixels.show();
//  Serial.println("led_off");
//}
//
//void change_led_status(){
//  if (status_led == false){
//    led_on();
//    status_led = true;
//  }
//  else if (status_led == true){
//    led_off();
//    status_led = false;
//  }
//  else
//    Serial.println("status_led error.");
//}
//
// 
//hw_timer_t * timer = NULL;            //声明一个定时器
//
////void IRAM_ATTR onTimer() {            //中断函数
//void IRAM_ATTR onTimer() {            //中断函数
////  change_led_status();
//
//  Serial.println(irq_count);
//  irq_count = irq_count+1;
//}
// 
// 
//void setup() {
//  Serial.begin(115200);                        
//  timer = timerBegin(0, 80, true);                //初始化
//  timerAttachInterrupt(timer, &onTimer, true);    //调用中断函数
//  timerAlarmWrite(timer, 1000000, true);        //timerBegin的参数二 80位80MHZ，这里为1000000  意思为1秒
//  timerAlarmEnable(timer);                //定时器使能
// 
////timerDetachInterrupt(timer);            //关闭定时器
//}
// 
// 
//void loop() {
////    int sensorValue = analogRead(A_SOUND);
////    
////    // print out the value you read:
//////    Serial.println(sensorValue);
////    Serial.println(count_sound);
//////    Serial.print('\t');
//////    Serial.println(sensorValue);
////    count_sound = count_sound +1;
//}

//
///************************
//  func 4 analogRead sound + time irq
//*************************/
//# include <Arduino.h>
//# define A_SOUND 36
//# define A_LIGHT 39
////sound is IO36
////light is IO39
//
//int count_sound = 0;
//volatile int status_time_irq = 0;
//
//hw_timer_t * timer = NULL;            //声明一个定时器
//
////void IRAM_ATTR onTimer() {            //中断函数
//void IRAM_ATTR onTimer() {            //中断函数
////  change_led_status();
////  status_time_irq = 1;
//  Serial.println("xxxxxx");
//}
//
//void setup() {
//  // initialize serial communication at 9600 bits per second:
//  Serial.begin(115200);                        
//  timer = timerBegin(0, 80, true);                //初始化
//  timerAttachInterrupt(timer, &onTimer, true);    //调用中断函数
//  timerAlarmWrite(timer, 1000000, true);        //timerBegin的参数二 80位80MHZ，这里为1000000  意思为1秒
//  timerAlarmEnable(timer);                //定时器使能
// 
//}
//
//void loop() {
//  
//  if (status_time_irq == 1)
////    return;
//    Serial.println("irq");
//  else{
//    // read the input on analog pin 0:
//    //读取A0脚输入值大小（0-5V对应0~1023）
//    int sensorValue = analogRead(A_SOUND);
//    
//    // print out the value you read:
//    Serial.println(count_sound);
////    Serial.print('\t');
////    Serial.println(sensorValue);
//    count_sound = count_sound +1;
//  }
//  
////  delay(1); // delay 1 ms in between reads for stability
//
//}

/////************************
////  func 5 test 2 cpu
////*************************/
//void setup() {
//    Serial.begin(9500);
//    xTaskCreatePinnedToCore(appCpuLoop,    //具体实现的函数
//                            "APP_CPU_LOOP",  //任务名称
//                            8192,       //堆栈大小
//                            NULL,       //输入参数
//                            1,          //任务优先级
//                            NULL,       //
//                            1           //核心  0\1
//                            );
//}
//
//void loop() {
//  Serial.println("main core");
//  delay(1000);
//}
//void appCpuLoop(void *pvParameters){
//  while(1){
//    Serial.println("appCpu");
//    delay(1000);
//  }
////当你想结束它的时候break()
//  vTaskDelete(NULL);
//}


/////************************
////  func 6 test 2 cpu + getsound + time irq 1s
////*************************/
//#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
//Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
//# define A_SOUND 36
//
//volatile bool status_led = false;
//volatile DRAM_ATTR int irq_count = 0;
//int count_sound = 0;
// 
//hw_timer_t * timer = NULL;            //声明一个定时器
//
//
////void IRAM_ATTR onTimer() {            //中断函数
//void IRAM_ATTR onTimer() {            //中断函数
//    Serial.println(irq_count);
//    irq_count = irq_count+1;
//}
//
//void setup() {
//  Serial.println("start setup");
//  Serial.begin(9500);
//
//  //time irq 1s
//  timer = timerBegin(0, 80, true);                //初始化
//  timerAttachInterrupt(timer, &onTimer, true);    //调用中断函数
//  timerAlarmWrite(timer, 1000000, true);        //timerBegin的参数二 80位80MHZ，这里为1000000  意思为1秒
//  timerAlarmEnable(timer);                //定时器使能
//  //timerDetachInterrupt(timer);            //关闭定时器
//
//  
//  xTaskCreatePinnedToCore(appCpuLoop,    //具体实现的函数
//                          "APP_CPU_LOOP",  //任务名称
//                          8192,       //堆栈大小
//                          NULL,       //输入参数
//                          1,          //任务优先级
//                          NULL,       //
//                          1           //核心  0\1
//                          );
//  Serial.println("end setup");
//}
//
//void loop() {
//  Serial.println("main core");
//}
//void appCpuLoop(void *pvParameters){
//  Serial.println("enter appCpuLoop");
//  while(1){
//    int sensorValue = analogRead(A_SOUND);
//    
//    // print out the value you read:
//    Serial.println(count_sound);
////    Serial.print('\t');
////    Serial.println(sensorValue);
//    count_sound = count_sound +1;
//  }
////当你想结束它的时候break()
//  vTaskDelete(NULL);
//}
//




 
