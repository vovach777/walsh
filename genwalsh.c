//based on soucecode:  
//  https://github.com/asad82/2D-Signal-Image-Transforms/blob/master/Code/TransformsView.cpp
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void GenWalsh( int N, FILE * f)
{
		int x,y,u,v;
		// NOTE: Input image must be square cx=cy
		// compute the value of small n from the value of Capital N
		int n=0;
		int value=0;
		while(value!=N)
		{
			n++;
			value=pow(2,n);
		}

		int tempX,tempY,tempU,tempV,bix,biy,piu,piv,b1u,b2u,b1v,b2v;
		int summation;

      fputs( "#include <stdalign.h>\n", f );
      fputs( "#include <string.h>\n", f);
      fprintf(f, "void walsh%d(int * imageData ) {\n", N);
      fprintf(f, "alignas(32) int magnitudeImage[%d] = {0};\n",N*N);

		// clear the buffer
      fprintf(f, "memset(magnitudeImage,0, sizeof(int) * %d);\n",N*N);

		for (u=0; u<N; u++)
		{
			for (v=0; v<N; v++)
			{
				for (x=0; x<N; x++)
				{
					for (y=0; y<N; y++)
					{
						summation=0;
						for(int i=0;i<n;i++)
						{
							tempX=x; tempY=y; tempU=u,tempV=v;
							tempX >>= i;
							tempY >>= i;
							bix = tempX & 1;
							biy = tempY & 1;
							// compute the value of p for u
							if(i==0)
							{
								tempU >>= n-1;
								piu = tempU & 1;
							}
							else
							{
								tempU >>= n-i;
								b1u = tempU & 1;
								tempU=u; // reset the value and compute again
								tempU >>= (n-(i+1));
								b2u = tempU & 1;
								piu = b1u + b2u;
							}

							// compute the value of p for v
							if(i==0)
							{
								tempV >>= n-1;
								piv = tempV & 1;
							}
							else
							{
								tempV >>= n-i;
								b1v = tempV & 1;
								tempV=v; // reset the value and compute again
								tempV >>= (n-(i+1));
								b2v = tempV & 1;
								piv = b1v + b2v;
							}

							int sum=(bix*piu) + (biy*piv);
							summation+=sum;

						}// end of for loop on i
                  fprintf(f, "magnitudeImage[%d] %c= imageData[%d];\n",u*N+v, (pow(-1,summation) > 0) ? '+' : '-',  x*N+y);

					}
				}
            fprintf(f, "magnitudeImage[%d] /= %d;\n",u*N+v,N);
			}
		}// end of outer for loop
      fprintf(f,"memcpy(imageData, magnitudeImage, %d * sizeof(int));\n",N*N);
      fputs("}\n",f);

}// end of function


int main(int argc, char* argv[]) {
   int sz = 8;
   char * filename = "";
   // parse args
   for (int i = 1; i < argc; i++) {
      if (strncmp(argv[i], "--",2)==0) {
         if (strncmp(argv[i], "--size=",7) == 0) {
            sz = strtoul( argv[i] + 7,NULL, 10 );
         }
      } else {
         filename = argv[i];
      }
   }
   //generate
   if (sz & (sz-1) != 0) {
      fprintf(stderr,"size must be power of two!!!");
      exit(1);
   }
   FILE * f = NULL;
   if (strlen(filename) > 0) {
      f = fopen(filename, "w");
   }
   GenWalsh(sz, f ? f : stdout);
   if (f) fclose(f);

}
