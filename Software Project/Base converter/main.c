#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SPBufferset.h"
int powa(int base, int exp);

/*--------------------------------------------------------------------------------------*/
int powa(int base, int exp)
    {
      int r = 1;
      if(exp < 0)
        return -1;

        while (exp)
        {
            if (exp & 1)
                r *= base;
            exp >>= 1;
            base *= base;
        }

        return r;
    } /*end powa function*/
/*----------------------------------------------------------------------------------------*/

int main(){ /*main function*/

int original_base, desired_base, c, count, whild, num, i, j, decimal=0, decc, res, remainder,size=0, arr[1024], desired_chars[1024],desired_num[1024];
SP_BUFF_SET();
printf("Please enter the number's base:\n");
fflush(stdout);
if (scanf("%d",&original_base)<=0){
	printf("An error occurred!\n");
	fflush(stdout);	
    exit(0);
}

if (original_base>16 || original_base<2){

	printf("Invalid input base\n");
	fflush(stdout);	
	exit(0);		
	}

		printf("Please enter the desired base:\n");
		fflush(stdout);	
		if (scanf("%d",&desired_base)<=0){
			printf("An error occurred!\n");
			fflush(stdout);	
    		exit(0);
		}

		if (desired_base>16 || desired_base<2)
		{
			printf("Invalid desired base\n");
			fflush(stdout);	
			exit(0); 	
				}
		
		/*actual program starts*/
		
			printf("Please enter a number in base %d:\n",original_base);
			fflush(stdout);	
			fflush(stdin);	
	c=' ';		
	while (isspace(c)){
		c=getchar();
	}
			
			 /*c=getchar();*/
    		count = 0;
    		whild=0;
   /*-------------------------------------------------------------------------------------------------*/
    		while ((count <= 1024) && (!isspace(c) && c != EOF)) {
    		
    		/*check if number is invalid-----------------------*/
    		
    			if(original_base<=10){
    				if (c<48 || c>original_base+47){
    					printf("Invalid number!\n");
    					fflush(stdout);	
    					exit(0);
    						}
    					} /*end <=10*/
    					
    			else {   /*original_base>10*/
    				if ((c<48)||((c>57) && (c<65))||((c>original_base%10+64)&&(c<97))||(c>96+original_base%10)){
    					printf("Invalid number!\n");
    					fflush(stdout);	
    					exit(0);
    						}
    					} /*end invalid number*/
    		/*-----------------------------------------*/
    		 /* number is valid*/ 
    		 whild = 1;
    				if (c>=48 && c<=57){ /* 48<=c<=57 */
    					num = c-48;
	    						}
    				else if (c>=65 && c<=90){ /* c is A-Z */
    					 num = c-55;
    							}
    				else if (97<=c && c<=102){  /* c is a-z */
    					num = c-87; 
    	  					}
    	  			else{ /*some error occurred*/
    	  				printf("An error occurred!\n");
    	  				fflush(stdout);	
    	  				exit(0);
    	  				}
    	  	/* num has a value */			
        	arr[count] = num;
			c=getchar();
        	++count;
       	
       	 }     	 /*while getchar() ends*/   
       	 
       	 /*-------------------------------------------------------------------------------------------*/
	if (whild==0){
	printf("An error occurred!\n");
    	  				fflush(stdout);	
    	  				exit(0);
	}       	        	 
	count -=1;
	for(i=count;i>=0;--i){
		decimal+=arr[i]*powa(original_base,count-i);		
		}	

	
	if (desired_base == 10){
		printf("The result is: %d\n",decimal);
		fflush(stdout);	
		return 0;
	}
	
		decc=decimal;
		if (decc == 0){
			printf("The result is: 0\n");
			fflush(stdout);
			return 0;
		}
		else{ /*result isnt 0*/
			while (decc!=0){
				res = decc/desired_base;
				remainder = decc%desired_base;
				desired_num[size] = remainder;
				decc = res;
				++size;		
			}	
			
			for (j=0;j<size;++j){
				if (desired_num[j]<10){
					desired_chars[j] = 48+desired_num[j];
					
				}
				else{
					desired_chars[j] = 55+desired_num[j];
				} 
	
			} /*end array of chars*/
		
			
			
			--size;
			printf("The result is: ");
			fflush(stdout);
				
			for (;size>=0;size--){
	
				printf("%c",desired_chars[size]);
				fflush(stdout);	
					}
		
			fflush(stdout);	
			printf("\n");
			fflush(stdout);		
			return 0;
		
	} /*result isnt 0 ends*/
	
	/*actual program ends*/

	
} /*end main function*/

