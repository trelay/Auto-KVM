#include "string.h"
#include "stdio.h"
#include "ctype.h" 
#include "assert.h"

#define MAX_RX          0xFF
#define ENTER           0x0D
#define OFFSET          1         //offset from the Digital PIN(start from digi-2)


char rxbuff[MAX_RX];
int buffin;    //define counter for Serial buffer
char cmd_s[MAX_RX];
boolean Serial_data_available;  //indicate if an enter key received
int ports = 8;   //Total ports will be supported.



static int process_Serial_input_data () 
{
  byte tmp_rcv_data;
  if (Serial.available())
  {
        tmp_rcv_data = Serial.read();
        
        if(tmp_rcv_data==ENTER)          //received Enter key
        {
            strcpy(cmd_s, rxbuff);
            memset(rxbuff, 0, MAX_RX);
            buffin=0;
            Serial_data_available = true;
        }
        else{
      rxbuff[buffin++]=tmp_rcv_data;
    }
  }
}

static int validate_cmd(int *cmd){
  if ((*cmd>=1) &&(*cmd<=ports)) {
    return 0;
  } 
  return -1;
} 
static int exe_cmd(int *cmd)
{
    *cmd=*cmd+OFFSET;
    digitalWrite(*cmd, LOW);
    delay(500);
    digitalWrite(*cmd, HIGH); 
    return 0;
}

void setup() {
    Serial.begin(9600);
    buffin=0;
    for(int i=(1+OFFSET);i<=(ports+OFFSET); i=i+1)
    {
       pinMode(i, OUTPUT);
    }
}


void loop()
{
  for(int i=(1+OFFSET);i<=(ports+OFFSET); i=i+1)
  {
       digitalWrite(i, HIGH); 
  }
  
  process_Serial_input_data();
  if (Serial_data_available == true )
  {
    int cmd;
    cmd=atoi(cmd_s);
  
    if (validate_cmd(&cmd))
    {
      Serial.println("Your cmd is not right!");
      Serial.print("Only the first ");
      Serial.print(ports);
      Serial.println(" supported!");
      Serial_data_available= false; 
    }
    else
    {
        Serial.print("Sending CMD:");
        Serial.println(cmd);
        if (exe_cmd(&cmd)) Serial.println("Executed failed.");
        else Serial.println("Switched.");
        Serial_data_available= false;    
    }

  }
}

