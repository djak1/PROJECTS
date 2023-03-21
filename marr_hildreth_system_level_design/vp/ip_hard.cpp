#ifndef _IP_HARD_C
#define _IP_HARD_C

#include "ip_hard.hpp"
#include "address.hpp"

#include <tlm>

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

SC_HAS_PROCESS(IP_HARD);

IP_HARD::IP_HARD(sc_module_name name): sc_module(name)
{
	ip_target.register_b_transport(this, &IP_HARD::b_transport);
	
	SC_REPORT_INFO("Hard", "Constructed.");
	SC_THREAD(process);

}
IP_HARD::~IP_HARD()
{
	SC_REPORT_INFO("Ip_hard", "Destroyed.");
}

void IP_HARD::b_transport(pl_t &pl, sc_time &offset)
{
	tlm_command cmd = pl.get_command(); 
	uint64 addr = pl.get_address(); 
	unsigned int len = pl.get_data_length(); 
 	unsigned char *buf = pl.get_data_ptr(); 
  	//pl.set_response_status( tlm::TLM_OK_RESPONSE );

 	switch(cmd)
 	{
 		case TLM_WRITE_COMMAND:
 			switch(addr)
 			{
 				case ADDR_HEIGHT:
 					height = toInt(buf);
 					cout << "Ip:Core: Height := "<< height << endl;
 					//offset+=sc_time(10, SC_NS);

 				break;
 				case ADDR_WIDTH:
 					width = toInt(buf);
 					cout << "Ip:Core: Width := "<< width << endl;
 					//offset+=sc_time(10, SC_NS);

 				break;

 				case IP_HARD_START:
 					start = *((sc_uint<2>*)buf);
 					cout << "IP_HARD_START (data) := " << start << endl;
 					s0.notify();
                    //process();
 					pl.set_response_status(TLM_OK_RESPONSE);
					offset += sc_time(2000, SC_NS);

 				break;
 			}
 		break;
 		case TLM_READ_COMMAND:
        switch(addr)
        {
          case IP_HARD_START:
              
               memcpy(buf, &stop, sizeof(stop));
    
               pl.set_response_status(TLM_OK_RESPONSE);
                offset += sc_time(10, SC_NS);
          break;
         }
 		break;
        
        default:
            pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
 	}
 	offset += sc_time(10, SC_NS);



}



void IP_HARD::process()
{
	wait(s0);
	cout << "**IP_HARD::-> Welcome to process !" << endl;
 	sc_int<11> t;
 	sc_int<11> v;
    unsigned char* output_picture;
    unsigned char* test;
    int k = 0;

    im = new unsigned char*[height];
        
        for (int i = 0; i < height; ++i)
        {
            im[i] = new unsigned char[width];
        }

   		for (int i = 0; i < height*width; ++i)
   		{
   			v = pfifo -> read();
   			read_pic.push_back(v);
			//cout << name()  << " i = " << i << " : read :=" << v << endl;
   		}
        cout << "  ->**IP_HARD: -> Calling a createPicture function" << endl <<  endl;
   		createPicture(read_pic);
        cout << "  ->**IP_HARD: -> Calling a LaplacianHW function" <<  endl << endl;
   		LaplacianHW(newinp, height, width, matrix);
        
        output_picture = new unsigned char[height*width];
        
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                output_picture[k] = im[i][j];
                k++;
            }
        }

        for (int i = 0; i < height*width; ++i)
        {
           // cout << "Upisano u fifo" << endl;
             dfifo ->write(output_picture[i]);
           // cout << i << endl;
        }
        cout << "  ->**IP_HARD : Write to fifo" << endl << endl;

        stop = 0x2;
        cout << stop <<endl;


		
		wait(1, SC_NS);	
}

void IP_HARD::createPicture(vector<int> vect)
{
   int t = 0;
   matrix = new unsigned char*[height];
   newinp = new int*[height];

   for (int i = 0; i < height; ++i)
   {
       matrix[i] = new unsigned char[width];
       newinp[i] = new int[width];
   }

   for (int i = 0; i < height; ++i)
   {
       for (int j = 0; j < width; ++j)
       {
            matrix[i][j] = vect[t];
            newinp[i][j] = 0;
            t++;
       }
      
   }

}

void IP_HARD::LaplacianHW(int** newinp, int height, int width, unsigned char** matrix)
{
	    sc_int<11> l1,l2=3; //3
        sc_int<11> temp;l1=5; //5

        sc_int<11> j=0;
        sc_int<11> l=0;
        sc_int<11> border1=(l1/2);
        sc_int<11> border2=(l2/2);
        
        cout <<"LoG  Convolution" << endl;
        sc_int<11> m;
        sc_int<11> spix;
        //
        sc_int<11> i;
        sc_int<11> h = height;
        sc_int<11> w = width;
        sc_int<11> k;
        for (i=border1;i<(h-border1);i++)
        {
            for (j=border1;j<(w-border1);j++)
            {
                spix=0;
                for (k=0;k<l1;k++)
                {           
                    for (m=0;m<l1;m++)
                    { 
                        spix = spix+(logmask[k][m]*(matrix[i-border1+k][j-border1+m]));
                    }

                }   
               
                   newinp[i][j]=spix; // nova slika 
            }
        }
       
        // zero crossing i thresholding
        cout << "zero crossing and thresholding" << endl;
        sc_int<11> max=0, th;
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
        cout << "Komentar" << endl;
       th=int(.075*max);
       
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
int IP_HARD::toInt(unsigned char *buf)
{
    int val = 0;
    val += ((int)buf[0]) << 24;
    val += ((int)buf[1]) << 16;
    val += ((int)buf[2]) << 8;
    val += ((int)buf[3]);
    return val;
}

#endif