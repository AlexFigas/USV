#include <Utils.h>

#include <stdarg.h>

char* format(const char* sFmt, ...) {
	char msgBuf[1024];

	va_list args;

	va_start( args, sFmt );
	int size;
	size = vsprintf( msgBuf, sFmt, args );
	va_end( args );

	char* result = new char[ size + 1 ];
	strcpy( result, msgBuf);

	return result;	
}
