void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(freemem());
}

// this function will return the number of bytes currently free in RAM      
extern int  __bss_end; 
extern int  *__brkval; 
int freemem()
{ 
int free_memory; 
if((int)__brkval == 0) 
   free_memory = ((int)&free_memory) - ((int)&__bss_end); 
else 
   free_memory = ((int)&free_memory) - ((int)__brkval); 
return free_memory; 
}
