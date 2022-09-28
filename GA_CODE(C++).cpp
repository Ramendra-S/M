#include<bits/stdc++.h>

using namespace std;


double f(double , double );


int main(){
	int n = 6, l1 = 5, l = 10, no_of_variable = 2;
	double xmin = 0, xmax = 0.5;
	int e = 0; // no of element for elitsm (keep even number if n is even, and odd if n is odd)
	int toursize = 3; // no. of element select for tournament
	double pc = 0.9, pm = 0.1; // crossover and mutation probability
	int max_generation = 100000;
	
	
	vector <vector <int> > POPULATION( n+1, vector<int> (l+1));
	vector <vector <int> > DECODE_VALUE( n+1, vector<int> (no_of_variable+1));
	vector <vector <double> > X( n+1, vector<double> (no_of_variable+1));
	vector <vector <double> > FUNCTION_VALUE( n+1, vector<double> (2));
	vector <vector <double> > FITNESS( n+1, vector<double> (2));
	vector <vector <int> > POOL( n+1, vector<int> (l+1));
	vector <vector <int> > CROSSOVER( n+1, vector<int> (l+1));
	vector <vector <int> > MUTATION( n+1, vector<int> (l+1));
	vector <vector <int> > NEW_GENERATION( n+1, vector<int> (l+1));
	
	
	int i, j, k, generation;
	double x1,x2, maxfit, prob;
	vector <double> temp( n+1 );
	vector <double> best_ele( e+1 );
	vector <int> best_position( e+1 );
	int sum=0, p , r, maxpos;
	double avgfit;
	

	ofstream OUT;
    OUT.open("OUTPUT.txt");
	
	// generating random population
	cout<<"Initial population : \n";
	OUT<<"Initial population : \n";
	srand(time(0));
	for(i=1;i<=n;i++){
		cout<<i<<".  ";
		OUT<<i<<".  ";
		for(j=1;j<=l;j++){
			POPULATION[i][j] = (rand()%(2));
			cout<<POPULATION[i][j];
			OUT<<POPULATION[i][j];
		}
		cout<<endl;
		OUT<<endl;
	}
	cout<<endl<<endl;
	OUT<<endl<<endl;
	
	for(i=1;i<=n;i++){
		for(j=1;j<=l;j++){
			NEW_GENERATION[i][j] = POPULATION[i][j];	
		}
	}
	
	
	for(generation=1;generation<=max_generation;generation++){
	
		for(i=1;i<=n;i++){
			for(j=1;j<=l;j++){
				POPULATION[i][j] = NEW_GENERATION[i][j];	
			}
		}
		
		// finding decord value and x value
		for(i=1;i<=n;i++){
			sum=0;
			for(j=1;j<=l1;j++){
				sum=sum+pow(2,5-j)*POPULATION[i][j];
			}
			DECODE_VALUE[i][1] = sum;
			sum=0;
			for(j=l1+1;j<=l;j++){
				sum=sum+pow(2,5+l1-j)*POPULATION[i][j];
			}
			DECODE_VALUE[i][2] = sum;
		}
		for(i=1;i<=n;i++){
	
			X[i][1] = xmin+(xmax-xmin)*DECODE_VALUE[i][1]/(pow(2,l1)-1);
			X[i][2] = xmin+(xmax-xmin)*DECODE_VALUE[i][2]/(pow(2,l1)-1);
		}
		
		// calculating function value to find fitness value
		for(i=1;i<=n;i++){
	
			x1 = X[i][1];
			x2 = X[i][2];
			FUNCTION_VALUE[i][1] = f(x1,x2);
		}
		for(i=1;i<=n;i++){
	
			FITNESS[i][1] = -1*FUNCTION_VALUE[i][1]; // to conver minimization problem into maximization
		}
		
		// Selection of best e solution by Elitism and other by tournament selection for mating pool
		for(i=1;i<=n;i++){
			temp[i] = FITNESS[i][1];
		}
		sort(temp.begin()+1,temp.end());
		
		// for elitism of e no. of element
		for(i=1;i<=e;i++){
			best_ele[i] = temp[n-i+1];
		}
		for(j=1;j<=e;j++){
			for(i=1;i<=n;i++){
				if(FITNESS[i][1]==best_ele[j]){
					best_position[j] = i;
				}
			}	
		}
		
		// tranfering elitism solution to new generation
		for(i=1;i<=e;i++){
			p=best_position[i];
			for(j=1;j<=l;j++){
				
				NEW_GENERATION[e][j] = POPULATION[p][j];
			}
		}
		
		//tournament selection of n-e no. of element
		for(i=e+1;i<=n;i++){
			r = 1+(rand()%(n));	// random number between 1 to n
			maxfit=FITNESS[r][1];
			maxpos=r;
			for(j=2;j<=toursize;j++){
				r = 1+(rand()%(n));
				if(FITNESS[r][1]>maxfit){
					maxfit=FITNESS[r][1];
					maxpos=r;
				}
			}
			
			for(j=1;j<=l;j++){
				POOL[i][j] = POPULATION[i][maxpos];
			}
		}
		
		// CROSS-OVER
		for(i=e+1;i<=n;i=i+2){
			prob=fabs(sin(rand())); // random number between 0 and 1
			if(prob<=pc){
				r = 1+(rand()%(l-1));
				for(j=1;j<=r;j++){
					CROSSOVER[i][j] = POOL[i][j];
					CROSSOVER[i+1][j] = POOL[i+1][j];
				}
				for(j=r+1;j<=l;j++){
					CROSSOVER[i][j] = POOL[i+1][j];
					CROSSOVER[i+1][j] = POOL[i][j];
				}
			}
			else{
				for(j=1;j<=l;j++){
					CROSSOVER[i][j] = POOL[i][j];
					CROSSOVER[i+1][j] = POOL[i+1][j];
				}
			}
		}
		
		// MUTATION
		for(i=e+1;i<=n;i=i+2){
			for(j=1;j<=l;j++){
				MUTATION[i][j] = CROSSOVER[i][j];
			}
		}
		for(i=e+1;i<=n;i=i+2){
			prob=fabs(sin(rand())); // random number between 0 and 1
			if(prob<=pm){
				r = 1+(rand()%(l-1));
				if(CROSSOVER[i][r]==0){
					MUTATION[i][r]=1;
				}
				else{
					MUTATION[i][r]=0;
				}
			}
		}
		
		for(i=e+1;i<=n;i++){
			for(j=1;j<=l;j++){
				NEW_GENERATION[i][j] = MUTATION[i][j];	
			}
		}
	}
	
	cout<<"Final Population after "<<max_generation<<" generation : \n";
	OUT<<"Final Population after "<<max_generation<<" generation : \n";
	for(i=1;i<=n;i++){
		cout<<i<<".  ";
		OUT<<i<<".  ";
		for(j=1;j<=l;j++){
			cout<<POPULATION[i][j];
			OUT<<POPULATION[i][j];
		}
		cout<<endl;
		OUT<<endl;
	}
	cout<<endl<<endl;
	OUT<<endl<<endl;
	
	cout<<"Position and Function value of all population : \n";
	cout<<"Population#\tx1\tx2\tf(x1,x2)\n";
	cout<<setprecision(3);
	OUT<<"Position and Function value of all population : \n";
	OUT<<"Population#\tx1\tx2\tf(x1,x2)\n";
	OUT<<setprecision(3);
	for(i=1;i<=n;i++){
		x1 = X[i][1];
		x2 = X[i][2];
		FUNCTION_VALUE[i][1] = f(x1,x2);
		cout<<i<<"\t\t"<<x1<<"\t"<<x2<<"\t"<<FUNCTION_VALUE[i][1]<<endl;
		OUT<<i<<"\t\t"<<x1<<"\t"<<x2<<"\t"<<FUNCTION_VALUE[i][1]<<endl;
	}
	cout<<endl<<endl;
	OUT<<endl<<endl;
	
	p = best_position[1];
	cout<<"The minimum of given problem lies at x1 = "<<X[p][1]<<", x2 = "<<X[p][1]<<", and minimum function value is f(x1,x2) = "<<FUNCTION_VALUE[p][1]<<".";
	OUT<<"The minimum of given problem lies at x1 = "<<X[p][1]<<", x2 = "<<X[p][1]<<", and minimum function value is f(x1,x2) = "<<FUNCTION_VALUE[p][1]<<".";
		
	return 0;
}

double f(double x1, double x2){
	return x1+x2-2*x1*x1-x2*x2+x1*x2;
}
