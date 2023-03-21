#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <systemc>

#include </usr/include/opencv4/opencv2/opencv.hpp>
#include </usr/include/opencv4/opencv2/core/core.hpp>
#include </usr/include/opencv4/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv4/opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

typedef sc_dt::sc_int <13> cst8;

int inp[833][833];
int newinp[833][833];
unsigned char im[833][833];

typedef unsigned char BYTE;


struct PIC
{
    unsigned int nChannel;
    bool InterLeaved;
    unsigned int Width, Height, Maxval;
    BYTE *img;
};
bool LoadP5Header(ifstream &infile, PIC &pic)
{
    bool rtv = true;
    char buf[16];
    int bufIndex;
    int width, height, maxval;
    infile.read(buf, 2); 
    buf[2]='\0';

    if(buf[0] == 'P' && buf[1] == '5')
    {
        infile.read(buf, 1);
        while(isspace(buf[0])) 
        {
            infile.read(buf,1);
        }

        
        bufIndex = 0;
        while(bufIndex < 15 && !isspace(buf[bufIndex]))
        {
            bufIndex++;
            infile.read(buf+bufIndex, 1);
        }
        buf[bufIndex] = NULL;  
        width = atoi(buf);

       
        infile.read(buf,1);
        while(isspace(buf[0])) 
        {
            infile.read(buf,1);
        }
        bufIndex = 0;
        while(bufIndex < 15 && !isspace(buf[bufIndex]))
        {
            bufIndex++;
            infile.read(buf+bufIndex, 1);
        }
        buf[bufIndex] = NULL;  
        height = atoi(buf);

    
        infile.read(buf,1);
        while(isspace(buf[0])) 
        {
            infile.read(buf,1);
        }
        bufIndex = 0;
        while(bufIndex < 15 && !isspace(buf[bufIndex]))
        {
            bufIndex++;
            infile.read(buf+bufIndex, 1);
        }
        buf[bufIndex] = NULL;  
        maxval = atoi(buf);

        
        infile.read(buf,1);

        
        pic.InterLeaved = false;
        pic.Width = width;
        pic.Height = height;
        pic.Maxval = maxval;

    }
    else rtv = false;

    return rtv;
}; 


void LoadImage(ifstream &infile, PIC &pic)
{
    infile.read(reinterpret_cast<char *>(pic.img), pic.Width*pic.Height);
}
template <typename T> void LaplacianHW(int newinp[][833], T i, T h, T w, T k, T j)
{
        T logmask[5][5]={ {0, 0, -1, 0, 0},
                            {0, -1, -2, -1, 0},
                            {-1, -2, 16, -2, -1},
                            { 0, -1, -2, -1, 0},
                            {0, 0, -1, 0, 0} };
      
        T l1,l2=3; //3
        T temp;l1=5; //5

        j=0;
        T l=0;
        T border1=(l1/2);
        T border2=(l2/2);

      
        T m;
        T spix;
        //
        for (i=border1;i<(h-border1);i++)
        {
            for (j=border1;j<(w-border1);j++)
            {
                spix=0;
                for (k=0;k<l1;k++)
                {           
                    for (m=0;m<l1;m++)
                    { 
                        spix = spix+(logmask[k][m]*(inp[i-border1+k][j-border1+m]));
                    }
                }
                      
               
                   newinp[i][j]=spix; 

                
            }
        }
       

        T max=0, th;
        for (k=0;k<h;k++)
        {
            for (i=0;i<w;i++)
            {
                if (newinp[k][i]>max)
                {
                    max= newinp[k][i];
                }
            }

        } 
       th=T(.075*max);
       
       // int th = 0;
        i=0;j=0;

       
        
        for (i=border2;i<(h-border2);i++)
        {    //j=0;
            for (j=border2;j<(w-border2);j++)
            {
                if ( newinp[i][j]!=0)
                {
                    if ((newinp[i][j+1]>=0 && newinp[i][j-1]<0) || (newinp[i][j+1]<0 && newinp[i][j-1]>=0))
                          {    
                              if ((newinp[i][j]>=th))
                                   { 
                                       im[i][j]=255;
                                    } 
                          }
                    
                    else if ((newinp[i+1][j]>=0 && newinp[i-1][j]<0) || (newinp[i+1][j]<0 && newinp[i-1][j]>=0))
                            { if (newinp[i][j]>=th)
                                 {  
                                    im[i][j]=255;
                                 }
                             }

                    else if ((newinp[i+1][j+1]>=0 && newinp[i-1][j-1]<0) || (newinp[i+1][j+1]<0 && newinp[i-1][j-1]>=0))
                             { if (newinp[i][j]>=th)
                                   {
                                    im[i][j]=255;
                                   }
                             }
                    
                    else if ((newinp[i-1][j+1]>=0 && newinp[i+1][j-1]<0) || (newinp[i-1][j+1]<0 && newinp[i+1][j-1]>=0))
                            { if (newinp[i][j]>=th)
                                 {
                                    im[i][j]=255;
                                 }
                            }
                }
            }
        }       
 }

int sc_main(int argc, char* argv[])
{

    ifstream infile_Pin(argv[1], std::ios::in);                
    
    int scale;
    
    int Npixels,l;
    int i;    
    
    PIC Pin;            

    

    if (LoadP5Header(infile_Pin, Pin)) // load pgm (Pin) image header information
    {
 
        Pin.img = new BYTE[Pin.Width*Pin.Height];

        LoadImage(infile_Pin, Pin);
    
    
        int w,h;
        w=(Pin.Width);
        h=(Pin.Height);
       
        
        int j=0,k;
    
       for (k=0;k<h;k++)
       {
          for (i=0;i<w;i++)
           {
            inp[k][i]=Pin.img[j];

            newinp[k][i]=0;
            im[k][i]=0;
                j++;                 
           } 
       }
      
     LaplacianHW<cst8>(newinp ,i,h, w, k, j);

     for (int i = 0; i < h ; ++i)
     {
        for (int j = 0; j < w; ++j)
        {
         cout << +im[i][j] << endl;   
        }
         
     }
     }

     
      cv::Mat Laplacian = cv::Mat(833, 833, CV_8UC1, im);
      //Laplacian.data = im[0];

      //im[0][0] = uchar(0);

      cv::imshow("Laplacian", Laplacian);
     

     cv::waitKey(0);


    return 0;
}

