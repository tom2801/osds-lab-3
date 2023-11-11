#include "sha-256.h"
#include <stdio.h>
#include <stdlib.h>

char* call_test(char * argv, char * result)
{
	FILE * f = NULL;
	unsigned int i = 0;
	unsigned int j = 0;
	char buf[4096];
	uint8_t sha256sum[32];
	printf("sunt in test \n");
	if( ! ( f = fopen( argv, "rb" ) ) )
        {
            perror( "fopen" );
            return( "1" );
        }

	sha256_context ctx;
        sha256_starts( &ctx );

        while( ( i = fread( buf, 1, sizeof( buf ), f ) ) > 0 )
        {
            sha256_update( &ctx, buf, i );
        }

        sha256_finish( &ctx, sha256sum );

        
        unsigned char *str_to_ret = malloc (sizeof (unsigned char) * 1000);
        
        for( j = 0; j < 32; j++ )
        {
           str_to_ret[j] = sha256sum[j];
            
        }

        return str_to_ret;
}
