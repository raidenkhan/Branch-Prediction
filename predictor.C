// A Perceptron Based predictor.

// Number of weights
#define N 28

// Total budget of the predictor
#define Budget (32*1024)

// Threshold values
#define ThetaMax (N*1.93+14)
#define ThetaMin ( -1*ThetaMax )

// Number of bits in a weight
#define BitsInWeight 8
#define MAXVAL 127
#define MINVAL -128

// calculation of entries of perceptron table
#define SizeOfPerceptron ((N+1)*BitsInWeight)
#define NumberPerceptron ((int)(Budget / SizeOfPerceptron))

int perceptron[NumberPerceptron][N];
int bias[NumberPerceptron];
int GHR;
int index;
int y;
bool prediction;

void init_predictor ()
{
  int i,j;
  for(i=0; i<NumberPerceptron; i++)
	for (j=0 ; j <= N ; j++)
		perceptron[i][j] = 0;
  GHR = 0;
}

bool make_prediction (unsigned int pc)
{
  int i,j,x;
  prediction = false;

  // hash function to map pc to a row in the perceptron table
  index = pc % NumberPerceptron;
  j = 1;
  y = bias[index];
  for(i=0; i < N; i++){
	  // checking jth bit of GHR 
	  if((GHR&j) == 0) 
		x = -1;
	  else
		x = 1;

	  // dot product calculation
	  y += perceptron[index][i]*x;

	  // used in finding out jth bit of GHR above
	  j = j << 1;
	}

  // making a prediction
  if( y >= 0 )
	prediction = true;
  else
	prediction = false;

 return prediction;
}

void train_predictor (unsigned int pc, bool outcome)
{
 int i,j,x,t, numericalOutcome;

 // t used to update bias and numerical outcome is used to update GHR
 if(outcome == true){
	t = 1;
	numericalOutcome = 1;
 }
 else{
	t = -1;
	numericalOutcome = 0;
 }
 
 j = 1;

 // updating weights and bias
 if( outcome != prediction || (y < ThetaMax && y > ThetaMin) ){

	// checking whether bias is within a value that is 8 bit long
	if ( bias[index] > MINVAL && bias[index] < MAXVAL) {
		// updating bias
		bias[index] = bias[index] + t;
	}

	for(i=0; i < N; i++){	

		// checking whether correlation is positive or negative
		if( ( ( (GHR&j) != 0) && (numericalOutcome == 1) ) ||  ( ((GHR&j) == 0) && (numericalOutcome == 0) ) ) 
			x = 1;
		else
			x = -1;

		// checking whether ith weight at index is within a value that is 8 bit long
		if ( perceptron[index][i] > MINVAL && perceptron[index][i] < MAXVAL ) {
			// updating weight 
			perceptron[index][i] = perceptron[index][i] + x;
		}

		// j used in finding out jth bit of GHR
		j = j << 1;
	}

 }
 
 // updating .. we take care of number of bits in GHR by doing an & with j everywhere we use GHR
 // in any case since it is an int it is just 32 bits well under 256 bits reserved for GHR
 GHR = ( GHR << 1 ) ;
 GHR = (GHR | numericalOutcome);
}





