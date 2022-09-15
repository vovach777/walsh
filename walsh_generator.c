//based on soucecode:  
//  https://github.com/asad82/2D-Signal-Image-Transforms/blob/master/Code/TransformsView.cpp
#include <math.h>
#include <string.h>
#include <stdio.h>

void DWalshTForward(const int* imageData,  int * magnitudeImage, int N)
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

		// clear the buffer
      		memset(magnitudeImage,0, sizeof(int) * N*N);

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
						magnitudeImage[u*N+v] += ( imageData[x*N+y] * ( pow(-1,summation) )  );
                 				printf( "magnitudeImage[%d] %c= imageData[%d];\n",u*N+v, (pow(-1,summation) > 0) ? '+' : '-',  x*N+y);

					}
				}
				magnitudeImage[u*N+v]=magnitudeImage[u*N+v]/N;
            			printf( "magnitudeImage[%d] /= %d;\n",u*N+v,N);
			}
		}// end of outer for loop

}// end of function

