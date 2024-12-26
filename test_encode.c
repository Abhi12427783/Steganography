/*Name         : Abhiram A M
  Date         : 10/10/2024
  Description  : Stegnography
  
*/




#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

/* Passing arguments through command line arguments */
int main(int argc, char *argv[])
{
    // Check for minimum number of arguments
    if (argc < 2) 
    {
        printf("ERROR: Not enough arguments provided.\n");
        printf("INFO: Usage:\n");
        printf("  Encoding: ./a.out -e <source_image> <secret_file> <stego_image>\n");
        printf("  Decoding: ./a.out -d <stego_image> <output_file>\n");
        return e_failure;
    }

    // Function call for check operation type
    if (check_operation_type(argv) == e_encode)
    {
        printf("INFO: Selected encoding\n");

        // Check for the number of arguments required for encoding
        if (argc < 4) 
        { 
            // At least 4 arguments are needed for encoding
            printf("ERROR: Insufficient arguments for encoding.\n");
            printf("INFO: Usage for encoding: ./a.out -e <source_image> <secret_file> <stego_image>\n");
            return e_failure;
        }
       // if(argc == )

        // Declare structure variable
        EncodeInfo encInfo;

        // Read and validate encode arguments
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("INFO: Read and validate encode arguments is a success\n");
            printf("<--------Started encoding-------->\n");

            // Function call for encoding
            if (do_encoding(&encInfo) == e_success)
            {
                printf("<--------Completed encoding-------->\n");
            }
            else
            {
                printf("ERROR: Failed to encode\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Read and validate encode arguments is a failure\n");
            return e_failure;
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        printf("INFO: Selected decoding\n");

        // Check for the number of arguments required for decoding
        if (argc < 3) 
        { 
            // At least 3 arguments needed for decoding
            printf("ERROR: Insufficient arguments for decoding.\n");
            printf("INFO: Usage for decoding: ./a.out -d <stego_image> <output_file>\n");
            return e_failure;
        }

        // Declare structure variable
        DecodeInfo decInfo;

        // Read and validate decode arguments
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("INFO: Read and validate decode arguments is a success\n");
            printf("<---------Started decoding--------->\n");

            // Function call for do decoding
            if (do_decoding(&decInfo) == e_success)
            {
                printf("<---------Completed decoding--------->\n");
            }
            else
            {
                printf("ERROR: Failed to decode\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Read and validate decode arguments is a failure\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Invalid option\n");
        printf("INFO: Usage:\n");
        printf("  Encoding: ./a.out -e <source_image> <secret_file> <stego_image>\n");
        printf("  Decoding: ./a.out -d <stego_image> <output_file>\n");
        return e_failure;
    }

    return 0;
}
