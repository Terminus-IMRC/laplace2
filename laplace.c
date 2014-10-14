#include "laplace.h"

int fc, fn;

void laplace_init()
{
	field_init();

	return;
}

void laplace()
{
	long int turn=0;
	int i, j;
	uint8_t field_changed;

	for(;;){
		printf("turn: %ld\n", turn);
		field_output_null(turn);

		fc=turn%2;
		fn=(fc+1)%2;
		field_changed=0;
		for(i=1; i<1+M; i++){
			for(j=1; j<1+N; j++){
				field[fn][i][j]=(field[fc][i-1][j]+field[fc][i][j-1]+field[fc][i+1][j]+field[fc][i][j+1])/4;
				if((!field_changed) && field[fc][i][j]!=field[fn][i][j])
					field_changed=1;
			}
		}
		if(!field_changed)
			break;

		turn++;
	}
	return;
}
