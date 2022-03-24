#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define MEMORYSIZE 256 // Memory Layout (256 Bytes)
#define REGISTERS 6 // A - B - C - D - SP - PC

typedef struct
{
  char MEM[MEMORYSIZE] ;
  char REG[REGISTERS];
  char *E;

} OS;

char tok[500], *ptr = tok;
int j = 0, d;
size_t f = 0, f2 = 0 ; // flag for --> 00 mov r imm8 

void execut(OS *D);
void Fread(OS *D ,FILE *fPtr); // read the code as text from a text file
void printM(OS *D, int c); // Disassemble, print memory and registers
int toInt(char *s, int j); // Hex or octal to integer
void readBinary(OS *D ,FILE *fPtr); // read the code as binary from a text file
int binarytoInt(char *s); // binary to integer 

int main(void)//---------------------main function----------------------------------------
{   	

  FILE *cfPtr; // code.txt or binray file pointer

  size_t i ;

  OS data = {"", {(char)0, (char)0, (char)0, (char)0, (char)192, (char)192}, NULL}; // Initial Value

  data.E = &data.MEM[192]; // E --> Opcode,  E+1 --> Operand 1 ,   E+2 --> Operand 2

  void (*f[3])(OS*, FILE *) = {readBinary, Fread};
  int choice; // variable to hold user's choice
  printf("Enter 1 to read the code as binary from binary.txt\nEnter 2 to read the code as assembly code from code.txt\n---->");
  scanf("%d", &choice);
  // 1 read the code as binary
  // 2 read the code as assembly code
  (*f[choice-1])(&data,cfPtr);
  
  
  puts("Code raw bytes");
  printf("| ");
  for( i = 0 ; i < j-192 ; ++i)
  {
    printf("%02X",(unsigned char)data.MEM[192+i]);
  }
  printf(" |\n");
  puts("");
  
  execut(&data);

  puts("-----------------------------------------------");
  for( i = 0 ; i < 4 ; ++i)
  {
    printf("Register %c --> 0x%02X\n", (char)(i+65),(unsigned char)data.REG[i]);
  }
  printf("Register %s --> 0x%02X\n", "SP",(unsigned char)data.REG[i]);
  printf("Register %s --> 0x%02X\n", "PC",(unsigned char)data.REG[i+1]);
  puts("-----------------------------------------------");
  puts("");
  
} //------------------------------End of main()------------------------------------------------------------------------------



void printM(OS *D, int c){ //---------------------Disassemble, print memory and registers--------------------------------------
  int n = (int)(unsigned char)(D->REG[4]);
  int i ;

  // Disassemble--------------------------------------------
  puts("");
  printf("Disassembly: ");
    printf("%02X: %02X  %02X  %02X ", (unsigned char)D->REG[5]-3, (unsigned char)*(D->E-3), (unsigned char)*(D->E-2), (unsigned char)*(D->E-1));                                          

 if((int)*(D->E-3) == 0 && f2 !=0 ) // Move immediate byte to register
  {//printf("------ 0x%02X\n",(unsigned char)*(D->E-1));
    --f2;
   if((int)*(D->E-2)<4){
   printf("mov %c, 0x%02X\n", (char)((int)*(D->E-2)+65),(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 4){
     printf("mov SP, 0x%02X\n",(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 5){
     printf("mov PC, 0c%02X\n",(unsigned char)*(D->E-1));
   }


  }else if((int)*(D->E-3) == 1)// Move memory address value to register
  {
   if((int)*(D->E-2)<4){
   printf("mov %c, [0x%02X]\n", (char)((int)*(D->E-2)+65),(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 4){
     printf("mov SP, [0x%02X]\n",(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 5){
     printf("mov PC, [0x%02X]\n",(unsigned char)*(D->E-1));
   }
   

  }else if((int)*(D->E-3) == 2) // Move register value to memory address
  {
  
   if((int)*(D->E-1)<4){
   printf("mov [0x%02X], %c\n",(unsigned char)*(D->E-2), (char)((int)*(D->E-1)+65));
   }
   else if((int)*(D->E-1) == 4){
     printf("mov [0x%02X], SP\n",(unsigned char)*(D->E-2));
   }
   else if((int)*(D->E-1) == 5){
     printf("mov [0x%02X], PC\n",(unsigned char)*(D->E-2));
   }

  }else if((int)*(D->E-3) == 3) // Push immediate byte to stack
  {
  
  printf("push 0x%02X\n",(unsigned char)*(D->E-2));

  }else if((int)*(D->E-3) == 4) // Pop byte from stack to register
  {
   if((int)*(D->E-2)<4){
   printf("pop %c\n", (char)((int)*(D->E-2)+65));

   }else if((int)*(D->E-2) == 5){
     puts("pop PC");
   }

  }else if((int)*(D->E-3) == 5) // Add immediate byte value to register
  {
   if((int)*(D->E-2)<4){
   printf("add %c, 0x%02X\n", (char)((int)*(D->E-2)+65),(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 4){
     printf("add SP, 0x%02X\n",(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 5){
     printf("add PC, 0x%02X\n",(unsigned char)*(D->E-1));
   }
    
  }else if((int)*(D->E-3) == 6) // Decrease register by immediate byte
  {
   if((int)*(D->E-2)<4){
   printf("dec %c, 0x%02X\n", (char)((int)*(D->E-2)+65),(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 4){
     printf("dec SP, 0x%02X\n",(unsigned char)*(D->E-1));
   }
   else if((int)*(D->E-2) == 5){
     printf("dec PC, 0x%02X\n",(unsigned char)*(D->E-1));
   }
    
  }else if((int)*(D->E-3) == 7) // Print string at memory address and stopping the execution
  {
    printf("prn [0x%02X]\n",(unsigned char)*(D->E-2)); 
    
  }

  // Print registers-------------------------------------------------------
  puts("");

  puts("-----------------------------------------------");
  for( i = 0 ; i < 4 ; ++i)
  {
    printf("Register %c --> 0x%02X\n", (char)(i+65),(unsigned char)D->REG[i]);
  }
  printf("Register %s --> 0x%02X\n", "SP",(unsigned char)D->REG[i]);
  printf("Register %s --> 0x%02X\n", "PC",(unsigned char)D->REG[i+1]-3);
  puts("-----------------------------------------------");
  puts("");

  // Print memory-------------------------------------
  puts("Memory Layout (256 Bytes):");
  puts("-----------------------------------------------");
  printf("%02X  %02X  %02X <-- %02X = SP   |   PC = %02X --> %02X  %02X  %02X\n",(unsigned char)(D->MEM[n-2]), (unsigned char)(D->MEM[n-1]), (unsigned char)(D->MEM[n]), (unsigned char)D->REG[4], (unsigned char)D->REG[5]-3, (unsigned char)*(D->E-3), (unsigned char)*(D->E-2), (unsigned char)*(D->E-1));                                          
  puts("-----------------------------------------------");
  puts("");
  

}//---------------------End of printM()-----------------------------------------------------------------------------------

void execut(OS *D)//--------------------- Execut the hex code in data.MEM[192..] ----------------------------------------
{
  int k = 192 , SP = 191, n = k; // E go from 0xC0 to 0xFF   ----  SP go from 0xBF to 0x00
 
 while (n < j )
 {
   
   
  SP = (int)((unsigned char)D->REG[4]) - 1;
  if((int)*(D->E) == 0 && f >0 ) // Move immediate byte to register
  {
    --f ;
    
   D->REG[(int)*(D->E+1)] = *(D->E+2) ;
   d = 3 ;

  }else if((int)*(D->E) == 1)// Move memory address value to register
  {
   D->MEM[(int)*(D->E+2)] = (char)8 ;
   D->REG[(int)*(D->E+1)] = D->MEM[(int)*(D->E+2)] ;
   d = 3 ;
   

  }else if((int)*(D->E) == 2) // Move register value to memory address
  {
  
   D->MEM[(int)*(D->E+1)] = D->REG[(int)*(D->E+2)] ;
   d = 3 ;
  
  }else if((int)*(D->E) == 3) // Push immediate byte to stack
  {
  
  if(SP < 0)
  {   puts("We exceeded memory limit :( "); 
  ++SP ;  
  }

  D->MEM[SP] = (char)*(D->E+1) ; 
  --SP ; 
  D->REG[4] = (char)(SP+1) ;
  d = 2 ;

  }else if((int)*(D->E) == 4) // Pop byte from stack to register
  {
    ++SP ; 

  if(SP == 192)
  {   puts("Stack is empty :) "); 
  --SP ;  
  }

  D->REG[(int)*(D->E+1)] = D->MEM[SP] ; 

  D->REG[4] = (char)(SP+1) ;
  d = 2 ;

  }else if((int)*(D->E) == 5) // Add immediate byte value to register
  {
  D->REG[(int)*(D->E+1)] += (char)*(D->E+2) ; 
  d = 3 ;
    
  }else if((int)*(D->E) == 6) // Decrease register by immediate byte
  {
  D->REG[(int)*(D->E+1)] -= (char)*(D->E+2) ; 
  d = 3 ;
    
  }else if((int)*(D->E) == 7) // Print string at memory address and stopping the execution
  { 
  int l = (int)(unsigned char)*(D->E+1) ;
   char *m = &(D->MEM[l]);
   printf("String ------>%s\n",m);
   puts("");
   puts("-------Program execution completed-------");
   puts("");
  
   return ; // stopping the execution after the first prn instruction
    
  }

  D->E += 3 ;
  n += 3;
  D->REG[5] = (char)(n) ;
  printf("<<<<<<<<<<<<<<<<<<<<<<<< Line: %d >>>>>>>>>>>>>>>>>>>>>>>>\n",(n-192)/3);
  printM(D,d);
  printf("<<<<<<<<<<<<<<<<<<<<<<<< End of Line: %d >>>>>>>>>>>>>>>>>>>>>>>>\n",(n-192)/3);
  puts("");
  puts("");
  //sleep(1);

 }
puts("");
puts("-------Program execution completed-------");
puts("");

}//--------------------- End of execut() -------------------------------------------------------------------------------




void readBinary(OS *D ,FILE *fPtr)//----------------Read from a binary text file and loads the memory with code-----------------
{

  unsigned char  p[9]; // 9 -> null
  unsigned int ins = 192;
  
  if ((fPtr = fopen("binary.txt", "r")) == NULL) {
      puts("File does not exist, create a new one");
      return;
   }

   // read record from file
   while (!feof(fPtr)) { 
     
        fscanf(fPtr, "%8c",p );
         p[8] = '\0' ; 
         D->MEM[ins] = (char)binarytoInt(p);
         ++ins;
         if(binarytoInt(p) == 0 && ins%3 == 0 )
         {
           ++f2 ; // Initial Value of data.MEM is 0 so, we have to know how many times opcode 0x00 appeared
           ++f;
         }

      } 
      fclose(fPtr);
      j = ins -1 ;
 


}//------------------------------End of readBinary()-----------------------------------------------------------------------





void Fread(OS *D ,FILE *fPtr)//----------Read from a text file , tokenization, parsing and loads the memory with code-----------
{ 
  
  unsigned char temp, *y, *p;
  unsigned int i = 0, ins = 192, h;
  
  if ((fPtr = fopen("code.txt", "r")) == NULL) {
      puts("File does not exist, create a new one");
      return;
   }

   // read record from file
   while (!feof(fPtr)) { 
     
        fscanf(fPtr, "%c",&temp );
         tok[i] = temp ; 
         
        ++i;
      } 
      fclose(fPtr);
      tok[i-1] = '\0';
      
  //Loads the memory with the code-------------------------------------------
   y = strtok(tok," ,\n");

   while (y != NULL )
   { 
     
     if( strcmp(y,"mov") == 0 ){
       
         y = strtok(NULL," ,\n");
         if((int)*y >=65 && (int)*y <=68 || (int)*y >=97 && (int)*y <=100 ){
            p = y ;
            y = strtok(NULL," ,\n");
           if( strchr(y,'[') == NULL )
           {    
                ++f; // Initial Value of data.MEM is 0 so, we have to know how many times opcode 0x00 appeared
                ++f2; 
                
                D->MEM[ins] = (char)(0) ; // for example: 192 --> C0  &&  NULL --> 0x00 one byte
                D->MEM[ins+1] = (char)(toupper(*p)-65) ; // for example:if p = B or b --> D->MEM[193] = 0x01
                h = toInt(y,1); // y --> imm8  and 1 to skip  '\0'
                temp = (char)h ; // immediate will not exceed one byte 
                D->MEM[ins+2] = temp ; 
           }else
           {
                D->MEM[ins] = (char)(1) ; // for example: 192 --> C0  &&  0000 0001 --> 0x01 one byte
                D->MEM[ins+1] = (char)(toupper(*p)-65) ; 
                h = toInt((y+1),2); // y --> m]  and 2 to skip ']' && '\0'
                temp = (char)h ;
                D->MEM[ins+2] = temp ;
                

           }
           ins += 3 ; // for example: 192 + 3 --> C3 
         }else if(strcmp(y,"SP") != 0  && strcmp(y,"PC") != 0 )
         {
                D->MEM[ins] = (char)(2) ; 
                h = toInt((y+1),2); // y --> m]  and 2 to skip ']' && '\0'
                temp = (char)h ;
                D->MEM[ins+1] = temp ;
                y = strtok(NULL," ,\n");
                if(strcmp(y,"SP") != 0  && strcmp(y,"PC") != 0 ){D->MEM[ins+2] = (char)(toupper(*y)-65) ; 
                }else if(strcmp(y,"SP") == 0   ){D->MEM[ins+2] = (char)(4) ;
                }else if(strcmp(y,"PC") == 0   ){D->MEM[ins+2] = (char)(5) ;
                }
               ins += 3 ; // for example: 192 + 3 --> C3

         }else if(strcmp(y,"SP") == 0  )
         {
            y = strtok(NULL," ,\n");
           if( strchr(y,'[') == NULL )
           {

                D->MEM[ins] = (char)(0) ; 
                D->MEM[ins+1] = (char)(4) ; 
                h = toInt(y,1); // y --> imm8  and 1 to skip  '\0'
                temp = (char)h ; // immediate will not exceed one byte 
                D->MEM[ins+2] = temp ; 
           }else
           {
                D->MEM[ins] = (char)(1) ; 
                D->MEM[ins+2] = (char)(4) ;
                h = toInt((y+1),2); // y --> m]  and 2 to skip ']' && '\0'
                temp = (char)h ;
                D->MEM[ins+1] = temp ;
  

           }
           ins += 3 ; // for example: 192 + 3 --> C3 

         }else if(strcmp(y,"PC") == 0  )
         {
            y = strtok(NULL," ,\n");
           if( strchr(y,'[') == NULL )
           {

                D->MEM[ins] = (char)(0) ; 
                D->MEM[ins+1] = (char)(5) ; 
                h = toInt(y,1); // y --> imm8  and 1 to skip  '\0'
                temp = (char)h ; // immediate will not exceed one byte 
                D->MEM[ins+2] = temp ; 
           }else
           {
                D->MEM[ins] = (char)(1) ; 
                D->MEM[ins+1] = (char)(5) ;
                h = toInt((y+1),2); // y --> m]  and 2 to skip ']' && '\0'
                temp = (char)h ;
                D->MEM[ins+2] = temp ;
                 



           }
           ins += 3 ; // for example: 192 + 3 --> C3 

         }else if(strchr(y,'[') == NULL  )
         {
            p = y ; // [m]
            y = strtok(NULL," ,\n"); // --> SP or PC ?
           if( strcmp(y,"SP") == 0 )
           {

                D->MEM[ins] = (char)(2) ; 
                h = toInt(++p,2); // y --> m]  and 2 to skip ']' && '\0'
                temp = (char)h ;
                D->MEM[ins+1] = temp ;
                D->MEM[ins+2] = (char)(4) ; 

           }else if(strcmp(y,"PC") == 0)
           {
                D->MEM[ins] = (char)(2) ; 
                h = toInt(++p,2); // y --> m]  and 2 to skip ']' && '\0'
                temp = (char)h ;
                D->MEM[ins+1] = temp ;
                D->MEM[ins+2] = (char)(5) ; 

           }
           ins += 3 ; // for example: 192 + 3 --> C3
           
   

         }
     }
   
     if( strcmp(y,"push") == 0 ){
          
         y = strtok(NULL," ,\n");
          D->MEM[ins] = (char)(3) ; 
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+1] = temp;
           D->MEM[ins+2] = (char)0;

          ins += 3 ; // for example: 192 + 3 --> C3 

     }

     if( strcmp(y,"pop") == 0 ){

         y = strtok(NULL," ,\n");
         if(strcmp(y,"PC") == 0)
          {
          D->MEM[ins] = (char)(4) ; 
          D->MEM[ins+1] = (char)(5);  // for example: it will pop 0xff to PC
          D->MEM[ins+2] = (char)0 ;

          }else if((int)*y >=65 && (int)*y <=68 || (int)*y >=97 && (int)*y <=100)
          {


                D->MEM[ins] = (char)(4) ; 
                D->MEM[ins+1] = (char)(toupper(*y)-65) ; // for example: it will pop 0xff to A
                D->MEM[ins+2] = (char)(0) ; 

          }

          ins += 3 ; // for example: 192 + 3 --> C3 

     }

     if( strcmp(y,"add") == 0 ){

         y = strtok(NULL," ,\n");
        if(strchr(y,'P') == NULL)
        {
          D->MEM[ins] = (char)(5) ; 
          D->MEM[ins+1] = (char)(toupper(*y)-65) ;
          y = strtok(NULL," ,\n");
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+2] = temp ;
        }else if(strcmp(y,"SP") == 0)
        {
          D->MEM[ins] = (char)(5) ; 
          D->MEM[ins+1] = (char)(4) ;
          y = strtok(NULL," ,\n");
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+2] = temp ;

        }else if(strcmp(y,"PC") == 0)
        {
          D->MEM[ins] = (char)(5) ; 
          D->MEM[ins+1] = (char)(5) ;
          y = strtok(NULL," ,\n");
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+2] = temp ;
        }
          ins += 3 ; // for example: 192 + 3 --> C3 

     }

     if( strcmp(y,"dec") == 0 ){

         y = strtok(NULL," ,\n");
        if(strchr(y,'P') == NULL)
        {
          D->MEM[ins] = (char)(6) ; 
          D->MEM[ins+1] = (char)(toupper(*y)-65) ;
          y = strtok(NULL," ,\n");
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+2] = temp ;
        }else if(strcmp(y,"SP") == 0)
        {
          D->MEM[ins] = (char)(6) ; 
          D->MEM[ins+1] = (char)(4) ;
          y = strtok(NULL," ,\n");
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+2] = temp ;

        }else if(strcmp(y,"PC") == 0)
        {
          D->MEM[ins] = (char)(6) ; 
          D->MEM[ins+1] = (char)(5) ;
          y = strtok(NULL," ,\n");
          h = toInt(y,1); 
          temp = (char)h ;
          D->MEM[ins+2] = temp ;
        }
          ins += 3 ; // for example: 192 + 3 --> C3 

     }

     if( strcmp(y,"prn") == 0 ){

         y = strtok(NULL," ,\n");

          D->MEM[ins] = (char)(7) ; 
          h = toInt((y+1),2); // y --> m]  and 2 to skip ']' && '\0'
          temp = (char)h ;
          D->MEM[ins+1] = temp ;
          D->MEM[ins+2] = (char)(0) ;

          ins += 3 ; // for example: 192 + 3 --> C3 

     }
     
    y = strtok(NULL," ,\n");
  
  if(ins > 255)
  { 
  puts("Your program exceed memory limit :( ") ; 
  return ;  
  }

   }
//Done loading--------------------------------------------------
   
   
   j = ins ;
  
}//------------------------------End of Fread()------------------------------------------------------------------------------


int toInt(char *s, int j)//------------------------------Hex or octal to integer--------------------------------------------------
{

if(strstr(s,"0x")!=NULL){

  int i = 0 ;
  int decimal = 0, base = 1, length;

  length = strlen(s);

  for(i = length--; i >= 0; i--)
  {
        if(s[i] >= '0' && s[i] <= '9')
        {
            decimal += (s[i] - 48) * base;
            base *= 16;
        }
        else if(s[i] >= 'A' && s[i] <= 'F')
        {
            decimal += (s[i] - 55) * base;
            base *= 16;
        }
        else if(s[i] >= 'a' && s[i] <= 'f')
        {
            decimal += (s[i] - 87) * base;
            base *= 16; 
        }
  }  

return decimal ;

}


if((int)s[0] == 48 ){

  int i = 0 ;
  int decimal = 0, base = 1, length;

  length = strlen(s)-j;

  for(i = length--; i >= 0; i--)
  {
        
    decimal += (s[i] - 48) * base;
    base *= 8;

  }  

return decimal ;

}

return atoi(s) ;
}//------------------------------End of toInt()----------------------------------------------------------------------------

int binarytoInt(char *s)//------------------------------Binary to integer to integer--------------------------------------------------
{
  int i = 0 ;
  int decimal = 0, base = 1, length;

  length = strlen(s)-1;

  for(i = length; i >= 0; i--)
  {
            decimal += (s[i]-48) * base;
            base *= 2;
  }  

return decimal ;


}//------------------------------End of binarytoInt()----------------------------------------------------------