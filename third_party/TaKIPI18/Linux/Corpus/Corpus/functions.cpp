#include "../corpuslib_headers.h"

#ifdef LINUX
#include <stdio.h>
#include <stdio.h>
#include <string.h>

char* itoa(int value, char* str, int radix){
	snprintf(str, 10, "%d", value);
	return str;
}

char* ultoa(unsigned long value, char* str, int radix) {
	snprintf(str, 10, "%ld", value);
	return str;
}

char *lltoa(unsigned long long value, char* buffer, int radix){
	if (radix < 2 || radix > 36) return NULL;
	if (buffer == NULL) return NULL;

	long long tmpVal = (value<0)?-value:value;
	int       sign   = (value & (long long)1<<63)==0?0:1;

	unsigned int maxPower = 0;
	while (tmpVal>=radix){
		tmpVal/=radix;
		++maxPower;
	}

	memset(buffer, 0x00, sizeof(char)*(maxPower+sign+2));
	if (sign) buffer[0] = '-';

	tmpVal = (value < 0? -value:value);

	unsigned long long digit = 0;
	for (unsigned int i = 0; i <= maxPower; i++){
		digit = tmpVal%radix;
		tmpVal/=radix;

		if (0<=digit && digit <=9){
			buffer[maxPower-i+sign]='0'+ (char)digit;
		}
		else if (10 <= digit && digit <= 35){
			buffer[maxPower-i+sign]='A'+ (char)digit;
		}
		else {
			return  NULL;
		}
	}

	return buffer;
}



char *ulltoa(long long value, char* buffer, int radix){
	if (radix < 2 || radix > 36) return NULL;
	if (buffer == NULL) return NULL;

	long long tmpVal = value;

	unsigned int maxPower = 0;
	while (tmpVal>=radix){
		tmpVal/=radix;
		++maxPower;
	}

	memset(buffer, 0x00, sizeof(char)*(maxPower+2));

	tmpVal =value;

	unsigned long long digit = 0;
	for (unsigned int i = 0; i <= maxPower; i++){
		digit = tmpVal%radix;
		tmpVal/=radix;

		if (0<=digit && digit <=9){
			buffer[maxPower-i]='0'+ (char)digit;
		}
		else if (10 <= digit && digit <= 35){
			buffer[maxPower-i]='A'+ (char)digit;
		}
		else {
			return  NULL;
		}
	}

	return buffer;
}


char* _i64toa(int value, char* bufor, int radix){
	return lltoa((long long)value, bufor, radix);
}

int strnicmp(const char *string1, const char *string2, size_t size)
{
	return strncasecmp(string1,string2,size);
}

int stricmp(const char *string1, const char *string2)
{
	return strcasecmp(string1,string2);
}

#endif // ifdef linux


