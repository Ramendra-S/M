#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double tf_hl(double x,double a1);
double tf_ol(double x,double a2);
int main()
{
	int p=0,i=0,j=0,k=0,P,L,M,N,TP;double l_r=0.2,m_c=0.3,max,min;					//l_r : Learning rate,m_c : momentum coefficient
	FILE *fpoint=fopen("Parameters.txt","r");
	if(fpoint==NULL)
	{
		printf("The file does not exists");
		exit(1);
	}
	fscanf(fpoint,"%d",&P);
	fscanf(fpoint,"%d",&L);
	fscanf(fpoint,"%d",&N);
	fscanf(fpoint,"%d",&TP);
	fclose(fpoint);
	M=5;													//No of Hidden Neurons
	FILE *fpo=fopen("inputfile.txt","r");
	if(fpo==NULL)
	{
		printf("The file does not exists");
		exit(1);
	}
	
	double I[L][P],Imax[L],Imin[L];							//Reading Input Data......No Need of Normalising since only 1 input is there
	for(p=0;p<P;p++)
	{
		for(i=0;i<L;i++)
		{
			fscanf(fpo,"%lf",&I[i][p]);
		}
	}
/*	for(p=0;p<P;p++)
	{
		max=I[0][p];
		min=I[0][p];	
		for(i=0;i<L;i++)
		{
			if(I[i][p]>max)
			{
				max=I[i][p];
			}
			if(I[i][p]<min)
			{
				min=I[i][p];
			}
		}
		Imax[p]=max;
		Imin[p]=min;
	}
	
	double Inorm[L][P];
	for(p=0;p<P;p++)
	{
		for(i=0;i<L;i++)
		{
			Inorm[i][p]=0.1+0.8*(I[i][p]-Imin[p])/(Imax[p]-Imin[p]);
		}
	}	
	*/
	fclose(fpo);	
	FILE *fptr=fopen("Targetvalues.txt","r");							//Reading Target Values
	if(fptr==NULL)
	{
		printf("The file does not exists");
		exit(1);
	}
	double T[N][P],Tmax[P],Tmin[P];double Tnorm[N][P];
	for(p=0;p<P;p++)
	{
		for(k=0;k<N;k++)
		{
			fscanf(fptr,"%lf",&T[k][p]);
		}
	}
	for(p=0;p<P;p++)												//Normalising Target Values
	{	
		max=T[0][p];
		min=T[0][p];
		for(k=0;k<N;k++)
		{
			if(T[k][p]>max)
			{
				max=T[k][p];
			}
			if(T[k][p]<min)
			{
				min=T[k][p];
			}
		}
		Tmax[p]=max;
		Tmin[p]=min;
		for(k=0;k<N;k++)
		{
			Tnorm[k][p]=0.1+0.8*(T[k][p]-Tmin[p])/(Tmax[p]-Tmin[p]);
		}
	}
	
	fclose(fptr);
	
	//intialising v,w,delv,delw
	double v[L+1][M],w[M+1][N],delv[L+1][M],delw[M+1][N],predelv[L+1][M],predelw[M+1][N];	
	for(i=0;i<=L;i++)
	{
		for(j=0;j<M;j++)
		{
			delv[i][j]=0.0;
		}
	}
	for(j=0;j<=M;j++)
	{
		for(k=0;k<N;k++)
		{
			delw[j][k]=0.0;
		}
	}
	
	for(i=0;i<=L;i++)
	{
		for(j=0;j<M;j++)
		{
			predelv[i][j]=0.0;
		}
	}
	for(j=0;j<=M;j++)
	{
		for(k=0;k<N;k++)
		{
			predelw[j][k]=0.0;
		}
	}
	for(i=0;i<=L;i++)					//Randomly initialising The V matrix
	{
		for(j=0;j<M;j++)
		{
			
			v[i][j]=(double)(rand()%11-5)/5;
		}
	}
	for(j=0;j<=M;j++)					//Randomly initialising The W matrix
	{
		for(k=0;k<N;k++)
		{
			w[j][k]=(double)(rand()%11-5)/5;
		}
	}
	
	double IH[M][P],OH[M][P],IO[N][P],OO[N][P],MSE=20,TOL,a1=1,a2=1,Onorm[N][P],Omax[P],Omin[P];
	TOL=pow(10,-5);
	FILE *fp1=fopen("iteration.txt","w");
	FILE *fp2=fopen("MSE.txt","w");
	fpoint=fopen("MSE_with_no_of_Iterations.txt","w");
	fprintf(fpoint,"Iteration| MSE\n");
	int count=0;
	while((MSE>=TOL))					//Training the Model
	{
		for(p=0;p<P;p++)
		{
			for(j=0;j<M;j++)
			{
				IH[j][p]=0;
			}	
		}
		for(p=0;p<P;p++)
		{
			for(j=0;j<M;j++)
			{
				for(i=0;i<L;i++)
				{
					IH[j][p]=IH[j][p]+I[i][p]*v[i+1][j];
				}
				IH[j][p]=IH[j][p]+1*v[0][j];
				OH[j][p]=tf_hl(IH[j][p],a1);
			}
		}
		for(p=0;p<P;p++)
		{
			for(k=0;k<N;k++)
			{
				IO[k][p]=0;
			}
		}
		for(p=0;p<P;p++)
		{
			for(k=0;k<N;k++)
			{
				for(j=0;j<M;j++)
				{
					IO[k][p]=IO[k][p]+OH[j][p]*w[j+1][k];
				}
				IO[k][p]=IO[k][p]+1*w[0][k];
				OO[k][p]=tf_ol(IO[k][p],a2);
			}
		}
		for(p=0;p<P;p++)
		{	
			max=OO[0][p];
			min=OO[0][p];
			for(k=0;k<N;k++)
			{
				if(OO[k][p]>max)
				{
					max=OO[k][p];
				}
				if(OO[k][p]<min)
				{
					min=OO[k][p];
				}
			}
			Omax[p]=max;
			Omin[p]=min;
			for(k=0;k<N;k++)
			{
				Onorm[k][p]=0.1+0.8*(OO[k][p]-Omin[p])/(Omax[p]-Omin[p]);
			}
		}
		for(j=0;j<=M;j++)
		{
			for(k=0;k<N;k++)
			{
				delw[j][k]=0;
			}
		}
		for(j=1;j<=M;j++)
		{
			for(k=0;k<N;k++)
			{
				for(p=0;p<P;p++)
				{
					delw[j][k]=delw[j][k]+(Tnorm[k][p]-Onorm[k][p])*a2*OO[k][p]*(1-OO[k][p])*OH[j-1][p]*0.8/(Omax[p]-Omin[p]);
				}
				delw[j][k]=l_r/P*delw[j][k];
			}
		}

		for(k=0;k<N;k++)
		{
			for(p=0;p<P;p++)
			{
				delw[0][k]=delw[0][k]+(Tnorm[k][p]-Onorm[k][p])*a2*OO[k][p]*(1-OO[k][p])*1*0.8/(Omax[k]-Omin[k]);
			}
			delw[0][k]=l_r/P*delw[0][k];
		}
		for(i=0;i<=L;i++)
		{
			for(j=0;j<M;j++)
			{
				delv[i][j]=0;
			}
		}
		for(i=1;i<=L;i++)
		{
			for(j=0;j<M;j++)
			{
				for(k=0;k<N;k++)
				{
					for(p=0;p<P;p++)
					{
						delv[i][j]=delv[i][j]+(Tnorm[k][p]-Onorm[k][p])*a2*OO[k][p]*(1-OO[k][p])*w[j][k]*a1*OH[j][p]*(1-OH[j][p])*I[i-1][p]*0.8/(Omax[k]-Omin[k]);	
					}
				}
				delv[i][j]=l_r/(N*P)*delv[i][j];
			}
		}
		for(j=0;j<M;j++)
		{
			for(k=0;k<N;k++)
			{
				for(p=0;p<P;p++)
				{
					delv[0][j]=delv[0][j]+(Tnorm[k][p]-Onorm[k][p])*a2*OO[k][p]*(1-OO[k][p])*w[j][k]*a1*OH[j][p]*(1-OH[j][p])*1*0.8/(Omax[k]-Omin[k]);	
				}
			}
			delv[0][j]=l_r/(N*P)*delv[0][j];
		}
		for(i=0;i<=L;i++)
		{
			for(j=0;j<M;j++)
			{
				v[i][j]=v[i][j]+delv[i][j]+m_c*predelv[i][j];
			}
		}
		for(j=0;j<=M;j++)
		{
			for(k=0;k<N;k++)
			{
				w[j][k]=w[j][k]+delw[j][k]+m_c*predelw[j][k];
			}
		}
		for(i=0;i<=L;i++)
		{
			for(j=0;j<M;j++)
			{
				predelv[i][j]=delv[i][j];
			}
		}
		for(j=0;j<=M;j++)
		{
			for(k=0;k<N;k++)
			{
				predelw[j][k]=delw[j][k];
			}
		}
		
		MSE=0;
		for(p=0;p<P;p++)
		{
			for(k=0;k<N;k++)
			{
				MSE=MSE+pow((Tnorm[k][p]-Onorm[k][p]),2)/2;
			}
		}
		MSE=MSE/P;
		count++;
		fprintf(fpoint," %d\t| %lf\n",count,MSE);
		fprintf(fp1,"%d;\n",count);
		fprintf(fp2,"%lf;\n",MSE);
		
	}
	fclose(fpoint);
	fclose(fp1);
	fclose(fp2);
	double inp[L][TP];
	fpoint=fopen("Test_inputs.txt","r");
	for(p=0;p<TP;p++)															//Calculating output of trained model
	{
		for(i=0;i<L;i++)
		{
			fscanf(fpoint,"%lf",&inp[i][p]);
			/*inp[i][p]=0.1+0.8*(inp[i][p]-Imin[p])/(Imax[p]-Imin[p]);*/
		}
	}
	fclose(fpoint);
	fpoint=fopen("Exact_values.txt","r");
	double EO[N][TP];
	for(p=0;p<TP;p++)
	{
		for(k=0;k<N;k++)
		{
			fscanf(fpoint,"%lf",&EO[k][p]);
		}
	}
	fclose(fpoint);
	fpoint=fopen("ouput.txt","w");
	double ih[M][TP],oh[M][TP],io[N][TP],oo[N][TP];
	for(p=0;p<TP;p++)
	{
		for(j=0;j<M;j++)
		{
			ih[j][p]=0;
		}	
	}
    for(p=0;p<TP;p++)
	{
		for(j=0;j<M;j++)
		{
			for(i=0;i<L;i++)
			{
				ih[j][p]=ih[j][p]+inp[i][p]*v[i+1][j];
			}
			ih[j][p]=ih[j][p]+1*v[0][j];
			oh[j][p]=tf_hl(ih[j][p],a1);
		}
	}
	fprintf(fpoint,"  Items to Produce  |  Predicted\t| Relative error|");	
	fprintf(fpoint,"         Sales      |  Predicted\t| Relative error|");
	fprintf(fpoint,"      Rating        |  Predicted\t| Relative error|");
	fprintf(fpoint,"       Acquisition  |  Predicted\t| Relative error|\n");
	for(p=0;p<TP;p++)
	{
		for(k=0;k<N;k++)
		{
			io[k][p]=0;
		}
	}
	for(p=0;p<TP;p++)
	{
		for(k=0;k<N;k++)
		{
			for(j=0;j<M;j++)
			{
				io[k][p]=io[k][p]+oh[j][p]*w[j+1][k];
			}
			io[k][p]=io[k][p]+1*w[0][k];
			oo[k][p]=tf_ol(io[k][p],a2);
		}
	}
	double omax[TP],omin[TP],onorm[N][TP];
	for(p=0;p<TP;p++)
		{	
			max=oo[0][p];
			min=oo[0][p];
			for(k=0;k<N;k++)
			{
				if(oo[k][p]>max)
				{
					max=oo[k][p];
				}
				if(oo[k][p]<min)
				{
					min=oo[k][p];
				}
			}
			omax[p]=max;
			omin[p]=min;
			for(k=0;k<N;k++)
			{
				onorm[k][p]=0.1+0.8*(oo[k][p]-omin[p])/(omax[p]-omin[p]);
			}
		}
		
	int m,s,pos;double err;
	for(m=0;m<TP;m++)
	{
		min=0;
		for(s=0;s<L;s++)
		{
			min=min+fabs(I[s][0]-inp[s][m]);
		}
		pos=0;
		for(p=0;p<P;p++)
		{
			err=0;
			for(i=0;i<L;i++)
			{
				err=err+fabs(I[i][p]-inp[i][m]);
			}
			if(err<=min)
			{
				min=err;
				pos=p;
			}
		}
		for(k=0;k<N;k++)
		{
			oo[k][m]=(onorm[k][m]-0.1)/(0.8)*(Tmax[pos]-Tmin[pos])+Tmin[pos];
			fprintf(fpoint,"\t%lf  | %lf\t| %lf\t|",(EO[k][m]),(oo[k][m]),fabs(1-(oo[k][m])/(EO[k][m])));
		}
		fprintf(fpoint,"\n");
	}	
	
	fprintf(fpoint,"\n\n\nThe 2nd,5th,8th data are not used in Training...");
	fclose(fpoint);
	return 0;	
}
double tf_hl(double x,double a1)
{
	return ((double)1/(1+exp(-a1*x)));
}
double tf_ol(double x,double a2)
{
	return ((double)1/(1+exp(-a2*x)));
}

