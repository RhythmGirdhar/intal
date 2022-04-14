#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "intal.h"
/*
void* intal_create(const char* str); done
void intal_destroy(void* intal); done
char* intal2str(void* intal); done
void* intal_increment(void* intal); done
void* intal_decrement(void* intal); done
void* intal_add(void* intal1, void* intal2); done
void* intal_diff(void* intal1, void* intal2); done
void* intal_multiply(void* intal1, void* intal2);
void* intal_divide(void* intal1, void* intal2);
int intal_compare(void* intal1, void* intal2); done
void* intal_pow(void* intal1, void* intal2);
add border cases.
*/

typedef struct number{
	char *s;
	int n; //number of digits
}number;

static char * revstr(char *str);
static number deep_copy(number a1, number *a);
static number* padarray(number* a, int num);
static number* multiply_byten(number *a, int exp);

static char * revstr(char *str){
	if (str==NULL){
   		return NULL;
    }
    int i,k=0;
    char * a = (char *)malloc(sizeof(char) * (strlen(str)+1));
	for(i=strlen(str)-1; i>=0; i--){
		a[k++] = str[i];
	}
	a[k++] = '\0';
  	return a;
}


static number deep_copy(number a1, number *a){
	int i;
	a1.n = a->n;
	a1.s = (char *)malloc(sizeof(char)*(a1.n + 1));
	for(i = 0; i <= a1.n; i++){
		a1.s[i] = a->s[i];
	}
	return a1;
}

static number* padarray(number* a, int num){
	int padval = num - a->n;
	number *new1 = (number *)malloc(sizeof(number));
	new1->s = (char *)malloc(sizeof(char)*(num + 1));
	new1->s[0] = a->s[0];
	new1->n = num;
	int i = 1, j = 1;
	while(i <= new1->n){
		while(padval != 0){
			new1->s[i++] = '0';
			padval--;
		}
		new1->s[i++] = a->s[j++];
	}
	new1->s[new1->n + 1] = '\0';
	return new1;
}


static number* multiply_byten(number *a, int exp){
    int  i,j;
	number * x = (number *)a;
	number *result = (number *)malloc(sizeof(number));
	result->s = (char *)malloc(sizeof(char) * ((x->n + exp) + 1));
	result->n = x->n + exp;

	for (i = 0; i < x->n; i++){
		result->s[i] = x->s[i];
	}
	//strcpy(result->s, x->s);
	for(j = x->n  ; j < result->n; j++){
		result->s[j] = '0';
	}
	result->s[j] = '\0';
	return result;
}

void* intal_create(const char* str){
	if(strcmp(str,"")==0)
		return NULL;

	int i;
	for(i=0; str[i]!='\0'; i++){			//to check if each character is a digit, if not return NULL
		if(str[i] < '0' || str[i] > '9')
			return NULL;
	}
	char *temp=(char*)malloc(sizeof(char) * (1 + strlen(str)));		//copy str to temp for removing leading 0's
	strcpy(temp,str);

	for(i=0; str[i]!='\0'; i++){			//removes leading 0's
		if(str[i] == '0'){
			temp++;
		}
		else								//first occurrence of a digit
			break;
	}
	if(i==strlen(str))
		temp="0";

	number * x=(number *)malloc(sizeof(number));
	if(x == NULL){
		return NULL; 					//malloc failed
	}

	x->s=(char *)malloc(sizeof(char) * (1 + strlen(str)));
	if(x->s == NULL) {
		free(x);
		return NULL; 					//malloc failed
	}

	strcpy(x->s, temp);
	x->n=strlen(x->s);
	//printf("%d\n",x->n);

	//free(temp);
	//free(x);
	return x;

}


//display
char* intal2str(void* intal){
	if(intal==NULL)
		return NULL;
	return ((char*)((number*)intal)->s);
}


//addition
void* intal_add(void* intal1, void* intal2){
	if(intal1==NULL || intal2==NULL)
		return NULL;

	number *a=(number *)intal1;
	number *b=(number *)intal2;

	if(strcmp(a->s,"0")== 0)						//redundant but faster if one of the numbers is 0
		return intal2;
	if(strcmp(b->s,"0")== 0)
		return intal1;

	if(a->n > b->n){
		number temp;
		temp = *a;
		*a = *b;
		*b = temp;
	}

	char * str = (char *)malloc((b->n + 2) *sizeof(char));		//empty string for storing result

	int diff = b->n - a->n;
	int i,carry = 0,k = 0;

	for(i= a->n - 1; i>=0; i--){
		int sum = ((a->s[i] - '0') + (b->s[i+diff] - '0') + carry);
		str[k++] = sum%10 + '0';
		carry = sum/10;
	}

	for(i= b->n - a->n - 1 ; i>=0; i--){
		int sum = ((b->s[i] - '0') + carry);
		str[k++] = sum%10 + '0';
		carry = sum/10;
	}

	if (carry)
		str[k++] = carry + '0';

	char * res = revstr(str);
	void * sum = intal_create(res);

	//free(str);
	return sum;


}



//Difference
void* intal_diff(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return NULL;

	number *a = (number *)intal1;
	number *b = (number *)intal2;

	if(strcmp(a->s,"0") == 0)						//redundant but faster if one of the numbers is 0
		return intal2;
	if(strcmp(b->s,"0") == 0)
		return intal1;

	if(intal_compare(intal1, intal2) == -1){
		number temp;
		temp = *a;
		*a = *b;
		*b = temp;
	}

	char * str = (char *)malloc((a->n + 1) * sizeof(char));

	int diff = a->n - b->n;
	int i, k = 0, carry =  0;

	for(i=b->n -1; i>=0; i--){
		int sub = ((a->s[i+diff] - '0') - (b->s[i] - '0') - carry);
		if(sub < 0){
			sub = sub + 10;
			carry = 1;
		}
		else{
			carry = 0;
		}
		str[k++] = sub + '0';
	}

	for(i=(diff - 1); i>=0; i--){
		if(a->s[i] == '0' && carry == 1){
			str[k++]='9';
			continue;
		}
		int sub = ((a->s[i] - '0') - carry);
		if(i>0 || sub>0)
			str[k++] = sub + '0';
		carry = 0;
	}

	char * res=revstr(str);
	void * difference=intal_create(res);
	return difference;
}



//Increment
void* intal_increment(void* intal){
	if(intal == NULL)
		return NULL;

	char *a = "1";
	void * x = intal_create(a);
	void *sum = intal_add(intal, x);
	return sum;
}

//Decrement
void* intal_decrement(void* intal){
	if(intal == NULL)
		return NULL;
	if(strcmp( ((char*)((number*)intal)->s), "0" ) == 0)
		return intal;


	void * x = intal_create("1");
	void *diff = intal_diff(intal, x);
	return diff;
}


//Compare
int intal_compare(void* intal1, void* intal2){
	int i;

	number *a = (number *)intal1;
	number *b = (number *)intal2;

	if(a->n < b->n){							//if b has more digits b > a
		return -1;
	}
	if(a->n > b->n){							//if a has more digits a > b
		return 1;
	}
	for(i = 0; i < a->n ; i++){					//comparing first digit of the numbers
		if(a->s[i] < b->s[i])
			return -1;
		else if(a->s[i] > b->s[i])
			return 1;
	}
	return 0;
}

//Destroy the created "object"
void intal_destroy(void* intal){
	if(intal == NULL)
		return;
	else
		intal = NULL;
}

//
//void* intal_multiply(void* intal1, void* intal2)
//{
//    number *a=(number *)intal1;
//	number *b=(number *)intal2;
//
//	if(a->n == 1 && b->n == 1){		//base case if number has only 1 digit
//		number *result = (number *)malloc(sizeof(number));
//		int res = (a->s[1] - '0')*(b->s[1] - '0');		//multiply the numbers
//		int rescpy = res;
//		int len = 0, i;
//		char temp;
//		if(res == 0){
//			len++;
//		}
//		while(rescpy != 0 ){
//			len++;
//			rescpy = rescpy/10;
//		}
//		result->s = (char *)malloc(sizeof(char)*(len + 2));
//		result->n = len;
//		for(i = 1; i <= len; i++){
//			result->s[i] = ((res % 10) + '0');
//			res = res/10;
//		}
//		if(len == 2){
//			temp = result->s[1];
//			result->s[1] = result->s[2];
//			result->s[2] = temp;
//		}
//		return result;
//	}
//	int i;
//	number *result = (number *)malloc(sizeof(number));	//result to be returned
//	number a1;	//copies to work with
//	number b1;
//
//	a1.n = 0;
//	a1.s = NULL;
//
//	b1.n = 0;
//	b1.s = NULL;
//
//	a1 = deep_copy(a1, a);	//create deep copies
//	b1 = deep_copy(b1, b);
//
//	if(a1.n > b1.n){
//		b1 = *padarray(&b1, a1.n);	//equalize the array sizes
//	}
//	else if(b1.n > a1.n){
//		a1 = *padarray(&a1, b1.n);
//	}
//	if(a1.n % 2 == 1){				//if length is odd, padd 1 zero to make the array size even
//		a1 = *padarray(&a1, a1.n+1);
//		b1 = *padarray(&b1, b1.n+1);
//	}
//
//	int m = a1.n/2, j, tlen;	//calculate the mid value
//	if(m == 1){
//		tlen = m + 1;
//	}
//	else{
//		tlen = m + 2;
//	}
//	number *c1 = (number*)malloc(sizeof(number));
//	number *c2 = (number*)malloc(sizeof(number));
//	number *d1 = (number*)malloc(sizeof(number));
//	number *d2 = (number*)malloc(sizeof(number));
//	number *c3 = (number*)malloc(sizeof(number));
//	number *d3 = (number*)malloc(sizeof(number));
//
//	c1->s = (char *)malloc(sizeof(char)*(m + 2));	//create the sub-arrays
//	c2->s = (char *)malloc(sizeof(char)*(m + 2));
//	d1->s = (char *)malloc(sizeof(char)*(m + 2));
//	d2->s = (char *)malloc(sizeof(char)*(m + 2));
//	c3->s = (char *)malloc(sizeof(char)*(tlen + 2));
//	d3->s = (char *)malloc(sizeof(char)*(tlen + 2));
//
//	for(i = 0; i <= m; i++){
//		c1->s[i] = a1.s[i];
//		d1->s[i] = b1.s[i];
//	}
//	c1->s[i] = '\0';
//	d1->s[i] = '\0';
//	c1->n = m;
//	d1->n = m;
//
//	c2->s[0] = a1.s[0];
//	d2->s[0] = b1.s[0];
//	c2->n = m;
//	d2->n = m;
//
//	j = 1;
//
//	for(i = m+1; i < (a1.n + 2); i++){
//		c2->s[j] = a1.s[i];
//		d2->s[j++] = b1.s[i];
//	}
//
//	c3 = intal_add(c1, c2);		//c3 = c1 + c2
//	d3 = intal_add(d1, d2);		//d3 = d1 + d2
//
//	if(c3->n != d3->n){			//equalize array size
//		if(c3->n > d3->n){
//			d3 = padarray(d3, c3->n);
//		}
//		else{
//			c3 = padarray(c3, d3->n);
//		}
//	}
//
//	if(c3->n % 2 == 1 && c3->n != 1){	//if array size if odd, pad one zero to make array size even.
//		c3 = padarray(c3, c3->n+1);
//		d3 = padarray(d3, d3->n+1);
//	}
//
//	c3->s[0] = c1->s[0];
//	d3->s[0] = d1->s[0];
//
//	number* p1 = (number*)malloc(sizeof(number));
//	number* p2 = (number*)malloc(sizeof(number));
//	number* p3 = (number*)malloc(sizeof(number));
//
//	p1 = intal_multiply(c1, d1);	//p1 = c1[0...m] * b1[0...m]
//	p2 = intal_multiply(c2, d2);	//p2 = c2[m+1...n] * d2[m+1...n]
//	p3 = intal_multiply(c3, d3);	//p3 = c3[0...m+1] * d3[0...m+1]
//
//	number* p1c = (number *)malloc(sizeof(number));
//	number* p3c = (number *)malloc(sizeof(number));
//
//	p1c = multiply_byten(p1, (2*m));	//p1c = p1*(10^n)
//
//	p3c = intal_diff(p3, p2);		//p3c = (p3 - p2 - p1)*(10^m)
//	p3c = intal_diff(p3c, p1);
//	p3c = multiply_byten(p3c, m);
//	result = intal_add(p1c, p2);
//	result = intal_add(result, p3c);	//result = p1c + p3c + p2
//	int count = 0;
//	for(i = 0; i < result->n; i++){		//remove leading zeros
//		if(result->s[1] != '0'){
//			break;
//		}
//		if(result->s[1] == '0' && i != (result->n - 1)){
//			for(j = 1; j <= result->n; j++){
//				result->s[j] = result->s[j + 1];
//			}
//			count++;
//		}
//	}
//	result->s = (char *)realloc(result->s, sizeof(char)*(result->n + 2 - count)); //reallocate after removing leading zeros.
//	result->n -= (count);
//	return result;
//}


void* intal_divide(void* intal1, void* intal2) {
	/*
	if(intal1==NULL || intal2==NULL)
		return NULL;
	if(!intal_compare(intal2, intal_create("0")))
		return NULL;
	void *q=intal_create("0");
	void *i1=intal_create(intal2str(intal1));
	void *i2=intal_create(intal2str(intal2));
	while(intal_compare(i1, i2)!=-1) {
		q=intal_increment(q);
		i1=intal_diff(i1, i2);
	}

	return q;
	*/
	if(intal1 == NULL || intal2 == NULL)						//If one of the value or both the values is NULL
		return NULL;

    if(!intal_compare(intal2, intal_create("0")))
		return NULL;
	void *q=intal_create("0");
	void *i1=intal_create(intal2str(intal1));
	void *i2=intal_create(intal2str(intal2));
	while(intal_compare(i1, i2)!=-1) {
		q=intal_increment(q);
		i1=intal_diff(i1, i2);
	}

	return q;
}
