/*
Name - Ankit Kumar Mishra
Roll Number - 202051028
*/

#include <stdio.h> 
#include <string.h>


//Look-up table for Subbyte
unsigned char sub[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}};

// This will store the extra bit during subbyte operation for all 11 round keys
__uint128_t flagSub[11][16];
int all = 0;

//This is used for Rotation of the word, (Helps in creating other words)
void RotWord(__uint32_t temp){
 __uint32_t t0 = temp, t1 = temp, t2 = temp, t3 = temp;
 t0 = t0 >> 24;
 t1 = (t1 >> 16)&(0x000000ff);
 t2 = (t2 >> 8)&(0x000000ff);
 t3 = t3&(0x000000ff);
 temp = (t1 << 24) | (t2 << 16) | (t3 << 8) | t0;
}

//Subbyte used in finding word
void SubWord(__uint32_t temp){
   //  temp = (temp << 1) ^ 1;
 __uint32_t t[4] = {temp, temp, temp, temp};
 t[0] = t[0] >> 24;
    t[0] = (t[0] << 1) ^ 1;
 t[1] = (t[1] >> 16)&(0x000000ff);
    t[1] = (t[1] << 1) ^ 1;
 t[2] = (t[2] >> 8)&(0x000000ff);
    t[2] = (t[2] << 1) ^ 1;  
 t[3] = t[3]&(0x000000ff);
    t[3] = (t[3] << 1) ^ 1;

  for(int i = 0; i < 4; i++){
    int t_f,t_s;
    t_f = t[i] & 15;
    t_s = t[i] >> 4;
    t[i] = sub[t_s][t_f]; 
  }
  temp = (t[0] << 24) | (t[1] << 16) | (t[2] << 8) | t[3];
}


//This Kind of do modulus of the number a with our primitive polynomial (x^8 + x^4 + x^3 + x + 1)
//Helps in getting only required 8-bits for further use
__uint128_t Inline(__uint128_t a){
    __uint128_t mod = 0x11b, t = 0;
    for(int i = 15; i > 7; i--){
        t = a;
        if((t >> i) == 1){
        a = (a ^ (mod << (i-8)));
        }
    }
    
    return a;
}

//function used for Mixcolumn
__uint128_t MixCol(__uint128_t finPlainText){
// In Mixcol insead of using matrix multiplication we simply used bit manipulation by shifting text by 2
// in left direction for 4 and for 5 also we XORed the text with text after shift of 2 to left
// similar technique we used in Inverse Mixcolumn which has many shifting and xoring involved but helped  
// in avoiding matrix multiplication 

 __uint128_t temp[16], t[16] = {finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText, finPlainText};
__uint128_t col[4][4], anscol[4][4];
 
 //I am seperating the text in 8-bits and storing in t[i]'s
 for(int i = 0; i < 16; i++){
    t[i] = (t[i] >> (128 - (8*(i+1))))&(0xff);
 }
 
 int k = 0;
 for(int i = 0; i < 4; i++){
   for(int j = 0; j < 4; j++){
      col[i][j] = t[k];
      k++;
   }
 }
 
 //Main Idea used in MixCol
 for(int i = 0; i < 4; i++){
  anscol[0][i] = Inline(col[0][i]) ^ Inline((col[1][i] << 2)) ^ Inline(col[2][i] << 2) ^ Inline((col[3][i] << 2) ^ col[3][i]);
  anscol[1][i] = Inline((col[0][i] << 2) ^ col[0][i]) ^ Inline(col[1][i]) ^ Inline(col[2][i] << 2) ^ Inline(col[3][i] << 2);
  anscol[2][i] = Inline(col[0][i] << 2) ^ Inline((col[1][i] << 2) ^ col[1][i]) ^ Inline(col[2][i]) ^ Inline(col[3][i] << 2);
  anscol[3][i] = Inline(col[0][i] << 2) ^ Inline(col[1][i] << 2) ^ Inline((col[2][i] << 2) ^ col[2][i]) ^ Inline(col[3][i]); 
  }
 
 k = 0;
 finPlainText = 0;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
       finPlainText = finPlainText | (anscol[i][j] << (128 - (8*(k+1))));
       k++;
    }
  }
      return finPlainText;
}

//Function for Finding inverse of MixColumn
__uint128_t Inv_MixCol(__uint128_t finPlainText){
 __uint128_t temp[16], t[16] = {finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText, finPlainText};
__uint128_t anscol[4][4], newcol[4][4];
 
 for(int i = 0; i < 16; i++){
    t[i] = (t[i] >> (128 - (8*(i+1))))&(0xff);
 }
 
 int k = 0;
 for(int i = 0; i < 4; i++){
   for(int j = 0; j < 4; j++){
      anscol[i][j] = t[k];
      k++;
   }
 }
 
 for(int i = 0; i < 4; i++){
  newcol[0][i] = Inline((anscol[0][i] << 7) ^ (anscol[0][i] << 5) ^ (anscol[0][i] << 2) ^ anscol[0][i]) ^ Inline((anscol[1][i] << 2) ^ (anscol[1][i] << 1) ^ anscol[1][i]) ^ Inline((anscol[2][i] << 4) ^ (anscol[2][i] << 3) ^ (anscol[2][i] << 1)) ^ Inline((anscol[3][i] << 6) ^ (anscol[3][i] << 5) ^ (anscol[3][i] << 4) ^ (anscol[3][i] << 1) ^ anscol[3][i]);
  newcol[1][i] = Inline((anscol[1][i] << 7) ^ (anscol[1][i] << 5) ^ (anscol[1][i] << 2) ^ anscol[1][i]) ^ Inline((anscol[2][i] << 2) ^ (anscol[2][i] << 1) ^ anscol[2][i]) ^ Inline((anscol[3][i] << 4) ^ (anscol[3][i] << 3) ^ (anscol[3][i] << 1)) ^ Inline((anscol[0][i] << 6) ^ (anscol[0][i] << 5) ^ (anscol[0][i] << 4) ^ (anscol[0][i] << 1) ^ anscol[0][i]);
  newcol[2][i] = Inline((anscol[2][i] << 7) ^ (anscol[2][i] << 5) ^ (anscol[2][i] << 2) ^ anscol[2][i]) ^ Inline((anscol[3][i] << 2) ^ (anscol[3][i] << 1) ^ anscol[3][i]) ^ Inline((anscol[0][i] << 4) ^ (anscol[0][i] << 3) ^ (anscol[0][i] << 1)) ^ Inline((anscol[1][i] << 6) ^ (anscol[1][i] << 5) ^ (anscol[1][i] << 4) ^ (anscol[1][i] << 1) ^ anscol[1][i]);
  newcol[3][i] = Inline((anscol[3][i] << 7) ^ (anscol[3][i] << 5) ^ (anscol[3][i] << 2) ^ anscol[3][i]) ^ Inline((anscol[0][i] << 2) ^ (anscol[0][i] << 1) ^ anscol[0][i]) ^ Inline((anscol[1][i] << 4) ^ (anscol[1][i] << 3) ^ (anscol[1][i] << 1)) ^ Inline((anscol[2][i] << 6) ^ (anscol[2][i] << 5) ^ (anscol[2][i] << 4) ^ (anscol[2][i] << 1) ^ anscol[2][i]);
  }

  k = 0;
 finPlainText = 0;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
       finPlainText = finPlainText | (newcol[i][j] << (128 - (8*(k+1))));
       k++;
    }
  }
      return finPlainText;
}


//Function used for Subbyte
__uint128_t SubBytes(__uint128_t finPlainText){
 __uint128_t t[16] = {finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText, finPlainText};

 for(int i = 0; i < 16; i++){
    t[i] = (t[i] >> (128 - (8*(i+1))))&(0xff);
 }                     
 
 //Appling modified subbyte function here
 for(int i = 0; i < 16; i++){
    t[i] = (t[i] << 1) ^ 1;
 }

//Here storing extra bit in flag
 for(int i = 0; i < 16; i++){
    flagSub[all][i] = (t[i] >> 8);
 }
    all++;
 
 for(int i = 0; i < 16; i++){
  t[i] = t[i] & 0xff;
 }
 
//Finding Subbyte of the 8-bit using below code explained by sir
 for(int i = 0; i < 16; i++){
    int t_f,t_s;
    t_f = t[i] & 15;
    t_s = t[i] >> 4;
    t[i] = sub[t_s][t_f]; 
  }

 finPlainText = 0;
 for(int i = 0; i < 16; i++){
 finPlainText = finPlainText | (t[i] << (128 - (8*(i+1))));
 }
 return finPlainText;
}

//Function for inverse of Subbyte
__uint128_t Inv_Subbyte(__uint128_t finPlainText){
 __uint128_t t[16] = {finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText, finPlainText};

 for(int i = 0; i < 16; i++){
    t[i] = (t[i] >> (128 - (8*(i+1))))&(0xff);
 }

//Finding Inverse of Subbyte of the 8-bit using below code explained by sir
for(int k = 0; k < 16; k++){
    __uint128_t x = 0;
 for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
      if(t[k] == sub[i][j]){
                x = i;
                x = (x << 4)|j;
                break;
            }
    }
  }
  t[k] = x;
}

all--;
for(int i = 0; i < 16; i++){
 if(flagSub[all][i] == 1){
 t[i] = t[i] | 0x00000000000000000000000000000100;
 }
}

for(int i = 0; i < 16; i++){
     t[i] = (t[i] ^ 1) >> 1;
}
 finPlainText = 0;
 for(int i = 0; i < 16; i++){
 finPlainText = finPlainText | (t[i] << (128 - (8*(i+1))));
 }

 return finPlainText;
}

//Using the Shift row we will shift the bits of text in particular order
__uint128_t ShiftRow(__uint128_t finPlainText){
 __uint128_t temp[16], t[16] = {finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText, finPlainText};

 for(int i = 0; i < 16; i++){
    t[i] = (t[i] >> (128 - (8*(i+1))))&(0xff);
 }
 
  for(int i = 0; i < 16; i++){
    temp[i] = t[i];
 }
 
t[4] = temp[5];
t[5] = temp[6];
t[6] = temp[7];
t[7] = temp[4];
t[8] = temp[9];
t[9] = temp[10];
t[10] = temp[11];
t[11] = temp[8];
t[12] = temp[13];
t[13] = temp[14];
t[14] = temp[15];
t[15] = temp[12];
 
finPlainText = 0;
 for(int i = 0; i < 16; i++){
 finPlainText = finPlainText | (t[i] << (128 - (8*(i+1))));
 }
 return finPlainText;
}


//Function to regain the shift row previously done
__uint128_t InvShiftRow(__uint128_t finPlainText){
 __uint128_t temp[16], t[16] = {finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText,
                      finPlainText, finPlainText, finPlainText, finPlainText, finPlainText, finPlainText};

 for(int i = 0; i < 16; i++){
    t[i] = (t[i] >> (128 - (8*(i+1))))&(0xff);
 }
 
  for(int i = 0; i < 16; i++){
    temp[i] = t[i];
 }
 
t[4] = temp[7];
t[5] = temp[4];
t[6] = temp[5];
t[7] = temp[6];
t[8] = temp[11];
t[9] = temp[8];
t[10] = temp[9];
t[11] = temp[10];
t[12] = temp[15];
t[13] = temp[12];
t[14] = temp[13];
t[15] = temp[14];
 
finPlainText = 0;
 for(int i = 0; i < 16; i++){
 finPlainText = finPlainText | (t[i] << (128 - (8*(i+1))));
 }
 return finPlainText;
}


void main(){
__uint32_t key[16], decText[16], plainText[16], R_con[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
__uint128_t word[44], AnsKey[11], finPlainText = 0;

printf("Enter the Plaintext: ");
for(int i = 0; i < 16; i++){
    scanf("%x", &plainText[i]);
}

for(int i = 0; i < 16; i++){ 
 __uint128_t PT = plainText[i];     
 finPlainText = finPlainText | (PT << (128 - (8*(i+1))));
}

printf("Enter the key: ");
for(int i = 0; i < 16; i++){
    scanf("%x", &key[i]);
}

//Finding the first 4 words here
for(int i = 0; i < 4; i++){
    word[i] = (key[15 - 4*i] << 24) | (key[15 - (4*i + 1)] << 16) | (key[15 - (4*i + 2)] << 8) | key[15 - (4*i + 3)];   
}

//Finding the remaining words here
for(int i = 4; i < 44; i++){
    __uint32_t temp = word[i-1];
    if(i % 4 == 0){
        RotWord(temp);
        SubWord(temp);
        temp = temp ^ R_con[i/4];
    }
    word[i] = word[i-4] ^ temp;
}
 

 //Getting all the 11 round keys here using the words generated
 for(int i = 0; i < 11; i++){
    AnsKey[i] = (word[i] << 96) | (word[i+1] << 64) | (word[i+2] << 32) | (word[i+3]);
 }

printf("\n\n----------------------------------------------------------------------------\n");
//AES Encryption for 10 rounds
for(int i = 0; i < 10; i++){
 finPlainText = finPlainText ^ AnsKey[i];
 finPlainText = SubBytes(finPlainText);
 finPlainText = ShiftRow(finPlainText);
 finPlainText = MixCol(finPlainText);
}

//AES Encryption for 11th rounds
 finPlainText = finPlainText ^ AnsKey[10];
 finPlainText = SubBytes(finPlainText);
 finPlainText = ShiftRow(finPlainText);

__uint128_t tk = finPlainText;

printf("After Encryption, Text is : ");
 for(int i = 0; i < 16; i++){
    decText[i] = (tk >> (128 - (8*(i+1))))&(0xff);
tk = finPlainText;
printf("%x ", decText[i]);
 }

printf("\n----------------------------------------------------------------------------\n\n");

printf("\n-----------------------------------------------------------------------------\n");
//AES Decryption for 11th rounds
 finPlainText = InvShiftRow(finPlainText);
 finPlainText =  Inv_Subbyte(finPlainText);
 finPlainText = finPlainText ^ AnsKey[10];

//AES Decryption for 10 rounds
for(int i = 9; i >= 0; i--){
 finPlainText = Inv_MixCol(finPlainText);
 finPlainText = InvShiftRow(finPlainText);
 finPlainText =  Inv_Subbyte(finPlainText);
 finPlainText = finPlainText ^ AnsKey[i];
}

tk = finPlainText;

printf("After Decryption, Text is : ");
 for(int i = 0; i < 16; i++){
    decText[i] = (tk >> (128 - (8*(i+1))))&(0xff);
tk = finPlainText;
printf("%x ", decText[i]);
 }
printf("\n-----------------------------------------------------------------------------\n");

}