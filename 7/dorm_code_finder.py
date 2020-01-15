# Author: walid.daboubi@gmail.com

possible_codes = []
for num in range(1,7732):
    prime = True
    for i in range(2,num):
        if (num%i==0):
            prime = False
    if prime:
    	str_num=str(num)
    	if '1' in str_num and '3' in str_num and '7' in str_num:
    		if str_num.count('1') == 2 or str_num.count('3') == 2 or str_num.count('7') == 2:
    			possible_codes.append(num)	
print possible_codes