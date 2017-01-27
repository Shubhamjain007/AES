//
//  main.cpp
//  AES
//
//  Created by Shubham Jain on 05/05/16.
//  Copyright © 2016 Shubham Jain. All rights reserved.
//

#include <iostream>
#include <opencv/cv.h>
#include<string.h>

unsigned char in[32], out[32],state[4][8];
int debug;
int length;


int Rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

int subword(int num)
{
    int sbox[256] =   {
        //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };//F
    return sbox[num];
}

int isubword(int num)
{
    int sbox[256] =   {
        //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, //0
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, //1
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, //2
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, //3
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, //4
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, //5
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, //6
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, //7
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, //8
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, //9
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, //A
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, //B
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, //C
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, //D
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, //E
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };//F
    return sbox[num];
}
void rotate(unsigned char (*rkey)[4],int s,unsigned char *tmp)
{unsigned char  temp;
    
    temp=rkey[s][0];
    tmp[0]=rkey[s][1];
    tmp[1]=rkey[s][2];
    tmp[2]=rkey[s][3];
    tmp[3]=temp;
    
};
void subkeyx(unsigned char *key)
{
    key[0]=subword(key[0]);
    key[1]=subword(key[1]);
    key[2]=subword(key[2]);
    key[3]=subword(key[3]);
    
}
void subkey()
{
    for(int i=0;i<length;i++)
        for(int j=0;j<4;j++)
    state[j][i]=subword(state[j][i]);
    
}
void isubkey(unsigned char *key)
{
    for(int i=0;i<length;i++)
        key[i]=isubword(key[i]);
    
}

void key_generation(unsigned char (*rkey)[4],int round,unsigned char *hkey)
{   unsigned char temp[4];
    for(int i=0;i<=length-1;i++){
        rkey[i][0]=hkey[0+4*i];
        rkey[i][1]=hkey[1+4*i];
        rkey[i][2]=hkey[2+4*i];
        rkey[i][3]=hkey[3+4*i];
        
    }
    
    for(int i=length;i<=(round*(length-1))-1;i++)
    {
        if(i%length!=0)
        {
            rkey[i][0]=rkey[i-1][0]^rkey[i-length][0];
            rkey[i][1]=rkey[i-1][1]^rkey[i-length][1];
            rkey[i][2]=rkey[i-1][2]^rkey[i-length][2];
            rkey[i][3]=rkey[i-1][3]^rkey[i-length][3]; //done
        }
        else
        {  // rotate(rkey,i,temp);
            temp[0]=rkey[i-1][1];
            temp[1]=rkey[i-1][2];
            temp[2]=rkey[i-1][3];
            temp[3]=rkey[i-1][0];
            
            subkeyx(temp);
            //printf("%02X    %02X    %02X    %02X",temp[0],temp[1],temp[2],temp[3]);
            //std::cin>>debug;
            
            rkey[i][0]=(temp[0]^Rcon[i/4])^rkey[i-length][0];
            rkey[i][1]=temp[1]^rkey[i-length][1];
            rkey[i][2]=temp[2]^rkey[i-length][2];
            rkey[i][3]=temp[3]^rkey[i-length][3];//done
            
        }
        
    }
    
};

void printer(unsigned char(*rkey)[4],int round)
{
    for(int i=0;i<=(length*(round+1))-1;i++)
    {//std::cout<<i<<" == "<<(char)rkey[i][0]<<" "<<(char)rkey[i][1]<<" "<<(char)rkey[i][2]<<" "<<(char)rkey[i][3]<<"\n";
        printf("%02X    %02X    %02X    %02X",rkey[i][0],rkey[i][1],rkey[i][2],rkey[i][3]);
        std::cout<<"\n";
    }
    
}
void AddRoundKey(unsigned char (*rkey)[4],int round)
{
    int i , j;
    
    for(i=0;i<length;i++)
        for(j=0;j<4;j++)
            state[j][i]^=rkey[i+(round*length)][j]; //done
}

void ShiftRows()
{unsigned char temp;int x=0;
    for(int i=1;i<4;i++)
    {   x=1;
        while(x<=i)
        {   int z=0;
            temp=state[i][0];
            for(z=0;z<length-1;z++)
                state[i][z]=state[i][z+1];
            
            //state[i][0]=state[i][1];
            //state[i][1]=state[i][2];
            //state[i][2]=state[i][3];  //done
            state[i][z]=temp;
            x++;
        }
        
    }
    
}
void LShiftRows()
{unsigned char temp;int x=0;
    for(int i=1;i<4;i++)
    {   x=0;
        while(x<i)
        {int z= length-1;
            temp=state[i][length-1];
            for(z=length-1;z>0;z--)
                state[i][z]=state[i][z-1];
            //state[i][3]=state[i][2];
            //state[i][2]=state[i][1];
            //state[i][1]=state[i][0];      //done
            
            state[i][z]=temp;
            x++;
        }
        
    }
    
}

void MixColumn()
{
    unsigned char temp[4];
    
    for(int i=0;i<length;i++)
    {
        for(int x=0;x<4;x++)
            temp[x]=state[x][i];
        
        state[0][i]=(0x02*temp[0])^(0x03*temp[1])^temp[2]^temp[3];
        state[1][i]=(0x01*temp[0])^(0x02*temp[1])^(0x03*temp[2])^temp[3];
        state[2][i]=(0x01*temp[0])^(0x01*temp[1])^(0x02*temp[2])^(0x03*temp[3]);
        state[3][i]=(0x03*temp[0])^(0x01*temp[1])^temp[2]^(0x02*temp[3]);
        
    }
    
    
}

#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

// MixColumns function mixes the columns of the state matrix
void MixColumns()
{
    int i;
    unsigned char Tmp,Tm,t;
    for(i=0;i<length;i++)
    {
        t=state[0][i];
        Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i] ;
        Tm = state[0][i] ^ state[1][i] ; Tm = xtime(Tm); state[0][i] ^= Tm ^ Tmp ;
        Tm = state[1][i] ^ state[2][i] ; Tm = xtime(Tm); state[1][i] ^= Tm ^ Tmp ;
        Tm = state[2][i] ^ state[3][i] ; Tm = xtime(Tm); state[2][i] ^= Tm ^ Tmp ;
        Tm = state[3][i] ^ t ; Tm = xtime(Tm); state[3][i] ^= Tm ^ Tmp ;
    }
}

void IMixColumns()
{
    unsigned char temp[4];
    
    for(int i=0;i<length;i++)
    {
        for(int x=0;x<4;x++)
            temp[x]=state[x][1];
        
        state[0][i]=(0x0e*temp[0])^(0x0b*temp[1])^(0x0d*temp[2])^(0x09*temp[3]);
        state[1][i]=(0x09*temp[0])^(0x0e*temp[1])^(0x0b*temp[2])^(0x0d*temp[3]);
        state[2][i]=(0x0d*temp[0])^(0x09*temp[1])^(0x0e*temp[2])^(0x0b*temp[3]);
        state[3][i]=(0x0b*temp[0])^(0x0d*temp[1])^(0x09*temp[2])^(0x0e*temp[3]);
        
    }
    
    
}
#define Multiply(x, y)                                \
(  ((y & 1) * x) ^                              \
((y>>1 & 1) * xtime(x)) ^                       \
((y>>2 & 1) * xtime(xtime(x))) ^                \
((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \



// MixColumns function mixes the columns of the state matrix.

static void InvMixColumns(void)
{
    int i;
    uint8_t a,b,c,d;
    for(i=0;i<length;++i)
    {
        a = state[0][i];
        b = state[1][i];
        c = state[2][i];
        d = state[3][i];
        
        state[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        state[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        state[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        state[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}


void cipher(unsigned char(*rkey)[4],int round)
{
    int i,j;
    
    //Copy the input PlainText to state array.
    for(i=0;i<length;i++)
    {
        for(j=0;j<4;j++)
        {
            state[j][i] = in[j*length + i];
        }
    }
    AddRoundKey(rkey,0);
    //printf("%02X    %02X    %02X    %02X",state[0][0],state[1][0],state[2][0],state[3][0]);
    //std::cin>>debug;
    
    for(int x=1;x<round;x++)
    {
        //for(int z=0;z<=4;z++)
        subkey();
        
        ShiftRows();
        //printf("%02X    %02X    %02X    %02X @shift ",state[0][0],state[1][1],state[2][2],state[3][3]);
        //std::cin>>debug;
        MixColumns();
        //printf("%02X    %02X    %02X    %02X @mix",state[0][0],state[1][1],state[2][2],state[3][3]);
        //std::cin>>debug;
        
        AddRoundKey(rkey,x);
        //printf("%02X    %02X    %02X    %02X @add",state[0][0],state[1][1],state[2][2],state[3][3]);
        //std::cin>>debug;
        
    }
    
    // The last round is given below.
    // The MixColumns function is not here in the last round.
    //for(int z=0;z<4;z++)
        subkey();
    
    ShiftRows();
    AddRoundKey(rkey,round);
    
    // The encryption process is over.
    // Copy the state array to output array.
    for(i=0;i<length;i++)
    {
        for(j=0;j<4;j++)
        {
            out[j*length+i]=state[j][i];
        }
    }
    
    
    
}

void decipher(unsigned char(*rkey)[4],int round)
{
    int i,j;
    
    //Copy the input cipherText to state array.
    for(i=0;i<length;i++)
    {
        for(j=0;j<4;j++)
        {
            state[j][i] = in[j*length + i];
        }
    }
    //printf("%02X    %02X    %02X    %02X @add",state[0][0],state[1][1],state[2][2],state[3][3]);
    //std::cin>>debug;
    AddRoundKey(rkey,round);
    
    
    for(int x=round-1;x>0;x--)
    {
        LShiftRows();
        isubkey(state[0]);
        isubkey(state[1]);
        isubkey(state[2]);
        isubkey(state[3]);
        AddRoundKey(rkey,x);
        InvMixColumns();
        
    }
    
    // The last round is given below.
    // The MixColumns function is not here in the last round.
    LShiftRows();
    isubkey(state[0]);
    isubkey(state[1]);
    isubkey(state[2]);
    isubkey(state[3]);
    AddRoundKey(rkey,0);
    
    // The encryption process is over.
    // Copy the state array to output array.
    for(i=0;i<length;i++)
    {
        for(j=0;j<4;j++)
        {
            out[j*length+i]=state[j][i];
        }
    }
    
    
    
}


int main(int argc, const char * argv[]) {
    FILE *fp,*fp1,*fp2;
    // insert code here...
    int lkey;int round=10;
    //unsigned char *hkey;
    unsigned char (*rkey)[4];
    unsigned char  key[50]={ (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
    
    //{0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54 ,0x54};
    // for debug == key[50]={0x24 ,0x75 ,0xa2 ,0xb3 ,0x34 ,0x75 ,0x56 ,0x88 ,0x31 ,0xe2 ,0x12 ,0x00 ,0x13 ,0xaa ,0x54 ,0x87};
    int flag=0;
    char c;
    int runner=0; //check how many block of states were made after padding
    
    std::cout << "Please Enter the key 16/24/32 alphabets\n";
    std::cin>>key;
    
    
    for(lkey=0;key[lkey]!='\0';lkey++)
    {};//my strlen
    
    
    
    if(lkey!=16 && lkey!=24 && lkey!=32)
    {
        std::cout<<"Worng length of key ..Exiting..."<<lkey;
        exit(1);
    }
    
    /* hkey=(unsigned char*)malloc(lkey*sizeof(int));
     for(int i=0;i<lkey;i++)
     {
     
     if(int(key[i])>47&&int(key[i])<58) //0 to 9
     { hkey[i]=int(key[i])-22;
     };
     if(int(key[i])>64&&int(key[i])<91)    //A to Z
     { hkey[i]=int(key[i])-65;
     };
     if(int(key[i])>96&&int(key[i])<123)   //a to z
     { hkey[i]=int(key[i])-61;
     };
     }*/
    for(int x=0;x<lkey;x++)
    { std::cout<<"\n"<<key[x]<<"==";
        printf("%02x ",(int)key[x]);
        
    }std::cout<<"\n";
    
    if(lkey==16){
        round=10;
        length=4;
    }
    if(lkey==24){
        round=12;
        length=6;
    }
    if(lkey==32){
        round=14;
        length=8;
    }
    
    
    rkey=(unsigned char(*)[4])malloc((round+1) *length* sizeof *rkey);
    
    
    key_generation(rkey,round, key);
    
    fp=fopen("/rar/license.txt","r");
    fp1=fopen("/rar/out.txt","w");
    fp2=fopen("/rar/outcheck.txt","w");
    int hx=0;
    
    while(flag==0)
    {
        for(int i=0;i<length*4;i++)
        {
            if(!flag)
            {
                c=fgetc(fp);
                if(c==EOF)
                {   if(i==0)
                    hx=1;
                    
                    flag=1;
                    in[i]='\0';
                }
                else
                    in[i]=c;
                
            }
            else
            {   in[i]='\0';
                
            }
            printf("%02x ",(int)in[i]);
            
        }
        
        if(hx==0){
            runner++;
            cipher(rkey,round);
            std::cout<<"\n encrypted text==";
            for(int i=0;i<length*4;i++)
            {
                fputc(out[i],fp1);
                printf("%02x ",(int)out[i]);
                
            }
        }
    }
    fclose(fp1);
    fclose(fp);
    fp1=fopen("/rar/out.txt","r");
    while(runner!=0)
    {
        for(int i=0;i<length*4;i++)
        {
            c=fgetc(fp1);
            in[i]=c;
            //debug printf("%02X",c);
            
        }
        
        decipher(rkey,round);
        std::cout<<"\n decrypted text==";
        for(int i=0;i<length*4;i++)
        {
            fputc(out[i],fp2);
            printf("%02X ",(int)out[i]);
            
        }
        runner--;
        
    }
    fclose(fp1);
    fclose(fp2);
    
    //printer(rkey,round);
    return 0;
}
