#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

/*The function f(x,y) = x**7 + 3*y**7 is conjectured to be a bijection.
This program searches for rationals x_1,y_1,x_2,y_2 such that
(x_1,y_1) != (x_2,y_2) and f(x_1,y_1) == f(x_2,y_2).
If a counterexample is found, f is not injective.*/

int pow(int x, int y){ //x**y by squaring
	if(y == 0){
		return 1;
	}else if(y == 1){
		return x;
	}else{
		if(y % 2 == 0){
			int root = pow(x, y>>1);
			return root * root;
		}else{
			return pow(x, y) * x;
		}
	}
}

void setQEnum(mpq_t r, int n){ //Sets r to the nth rational in an enumeration of positive rationals, n>0
	//Enumeration used is of the Stern-Brocot tree, from https://doisinkidney.com/posts/2019-12-14-stern-brocot.html
	//This enumeration only works when n is an odd positive integer
	
	//TODO: Is this only enumerating rationals >=1?
	int num = 1;
	int denom = 1;
	while(n > 0){
		if(n % 2 == 1){ //GT
			num += denom;
		}else{ //LT
			denom += num;
		}
		n /= 2;
	}
	mpq_set_ui(r, num, denom);
	mpq_canonicalize(r);
	return;
}

/*void setQEnum(mpq_t r, int n){ //Sets r to the nth rational in an enumeration of positive rationals
	//Not a good enumeration, quick to write but poor efficiency
	int num = 1;
	int denom = n;
	while(denom % 2 == 0){
		num++;
		denom = denom >> 1;
	}
	mpq_set_ui(r, num, denom);
	mpq_canonicalize(r);
	return;
}*/

void seventhPower(mpq_t x){ //Raise x to seventh power
	mpq_t xOld;
	//mpq_init(xOld);
	mpq_init(xOld);
	mpq_set(xOld, x);
	mpq_mul(x, xOld, xOld); //x is now xOld**2
	mpq_mul(x, x, xOld); //x is now xOld**3
	mpq_mul(x, x, x); //x is now xOld**6
	mpq_mul(x, x, xOld); //x is now xOld**7
	mpq_clear(xOld);
	return;
}

void f(mpq_t r, mpq_t x, mpq_t y){ //Sets r to x**7 + 3*y**7
	mpq_t xSeventh;
	mpq_t ySeventh;
	mpq_init(xSeventh);
	mpq_set(xSeventh, x);
	mpq_init(ySeventh);
	mpq_set(ySeventh, y);
	seventhPower(xSeventh);
	seventhPower(ySeventh);
	mpq_add(r, xSeventh, ySeventh); //r is now x**7 + y**7
	mpq_add(r, r, ySeventh);
	mpq_add(r, r, ySeventh); //r is now x**7 + 3*y**7
	mpq_clear(xSeventh);
	mpq_clear(ySeventh);
}

int main(){
	mpq_t x1, y1, x2, y2, f1, f2;
	mpq_init(x1);
	mpq_init(x2);
	mpq_init(y1);
	mpq_init(y2);
	mpq_init(f1);
	mpq_init(f2);
	//Looping through enumeration
	for(int x1code = 0; x1code < 1000; x1code++){
		for(int x2code = 0; x2code <= x1code; x2code++){
			for(int y1code = 0; y1code < 1000; y1code++){
				for(int y2code = 0; y2code <= y1code; y2code++){
					setQEnum(x1, x1code);
					setQEnum(x2, x2code);
					setQEnum(y1, y1code);
					setQEnum(y2, y2code);
					if(mpq_cmp(x1, x2) == 0 && mpq_cmp(y1, y2) == 0){ //Skip x1 == x2 && y1 == y2
						continue;
					}
					f(f1, x1, y1);
					f(f2, x2, y2);
					//gmp_printf("f(%Qd, %Qd) = %Qd\n", x1, y1, f1);
					//gmp_printf("f(%Qd, %Qd) = %Qd\n", x2, y2, f2);
					if(mpq_cmp(f1, f2) == 0){
						gmp_printf("Counterexample found! f(%Qd,%Qd) == f(%Qd,%Qd)\n", x1, y1, x2, y2);
						return 0;
					}
				}
			}
			gmp_printf("x1=%Qd (x1code=%d), x2=%Qd (x2code=%d)\n", x1, x1code, x2, x2code);
		}
	}
	return 0;
}
