#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"
#include <stdlib.h>

// Function definition for read and validate decode args
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
        decInfo->d_src_image_fname = argv[2];
    else
        return e_failure;

    if (argv[3] != NULL)
        decInfo->d_secret_fname = argv[3];
    else
        decInfo->d_secret_fname = "decode.txt";

    return e_success;
}

// Function definition for open files for decoding
Status open_files_dec(DecodeInfo *decInfo)
{
    // Opening Stego Image file
    decInfo->fptr_d_src_image = fopen(decInfo->d_src_image_fname, "r");
    if (decInfo->fptr_d_src_image == NULL)
    {
        printf("ERROR: Unable to open file %s\n", decInfo->d_src_image_fname);
        return e_failure;
    }

    // Opening Destination file
    decInfo->fptr_d_secret = fopen(decInfo->d_secret_fname, "w");
    if (decInfo->fptr_d_secret == NULL)
    {
        printf("ERROR: Unable to open file %s\n", decInfo->d_secret_fname);
        return e_failure;
    }

    // If no failure then return e_success
    return e_success;
}

// Function definition for decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_d_src_image, 54, SEEK_SET);
    int i = strlen(MAGIC_STRING);
    decInfo->magic_data = malloc(strlen(MAGIC_STRING) + 1);
    decode_data_from_image(strlen(MAGIC_STRING), decInfo->fptr_d_src_image, decInfo);
    decInfo->magic_data[i] = '\0';

    if (strcmp(decInfo->magic_data, MAGIC_STRING) == 0)
      {
        printf("INFO: Decoded magic string data successful\n");
          return e_success;
      }
    else
    {
        printf("ERROR: Decoded magic string data unsuccessful\n");
        return e_failure;
    }
}

// Function definition for decoding data from image
Status decode_data_from_image(int size, FILE *fptr_d_src_image, DecodeInfo *decInfo)
{
    int i;
    char str[8];
    for (i = 0; i < size; i++)
    {
        fread(str, 8, sizeof(char), fptr_d_src_image);
        decode_byte_from_lsb(&decInfo->magic_data[i], str);

    }
    printf("INFO: Decoded magic string length\n");
    return e_success;
}

// Function definition for decode byte from lsb
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    int bit = 7;
    unsigned char ch = 0x00;
    for (int i = 0; i < 8; i++)
    {
        ch = ((image_buffer[i] & 0x01) << bit--) | ch;
    }
    *data = ch;
    return e_success;
}

// Function definition for decode file extn size
Status decode_file_extn_size(int size, FILE *fptr_d_src_image)
{
    char str[32];
    int length;
    fread(str, 32, sizeof(char), fptr_d_src_image);
    decode_size_from_lsb(str, &length);
    if (length == size)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

// Function definition decode size from lsb
Status decode_size_from_lsb(char *buffer, int *size)
{
    int j = 31;
    int num = 0x00;
    for (int i = 0; i < 32; i++)
    {
        num = ((buffer[i] & 0x01) << j--) | num;
    }
    *size = num;
}

// Function definition for decode secret file extn
Status decode_secret_file_extn(char *file_ext, DecodeInfo *decInfo)
{
    file_ext = ".txt";
    int i = strlen(file_ext);
    decInfo->d_extn_secret_file = malloc(i + 1);
    decode_extension_data_from_image(strlen(file_ext), decInfo->fptr_d_src_image, decInfo);
    decInfo->d_extn_secret_file[i] = '\0';

    if (strcmp(decInfo->d_extn_secret_file, file_ext) == 0)
    {
        printf("INFO: Decoded secret file extension successfull\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Decoded secret file extension unsuccessfull\n");
        return e_failure;
    }
}

// Function definition decode extension data from image
Status decode_extension_data_from_image(int size, FILE *fptr_d_src_image, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        fread(decInfo->d_src_image_fname, 8, 1, fptr_d_src_image);
        decode_byte_from_lsb(&decInfo->d_extn_secret_file[i], decInfo->d_src_image_fname);
    }
    return e_success;
}

// Function definition for decode secret file size
Status decode_secret_file_size(int file_size, DecodeInfo *decInfo)
{
    char str[32];
    fread(str, 32, sizeof(char), decInfo->fptr_d_src_image);
    decode_size_from_lsb(str, &file_size);
    decInfo->size_secret_file = file_size;
    return e_success;
}

// Function definition for decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(decInfo->d_src_image_fname, 8, sizeof(char), decInfo->fptr_d_src_image);
        decode_byte_from_lsb(&ch, decInfo->d_src_image_fname);
        fputc(ch, decInfo->fptr_d_secret);
    }
    printf("INFO: Decoded secret file data successfull\n");
    return e_success;
}

// Function definition for do decoding
Status do_decoding(DecodeInfo *decInfo)
{
    // Open files and check for success
    if (open_files_dec(decInfo) != e_success)
    {
        printf(COLOR_RED "ERROR: Open files is a failure\n" COLOR_RESET);
        return e_failure;
    }
    printf(COLOR_GREEN "INFO: Open files is successful\n" COLOR_RESET);

    // Decode magic string and check for success
    if (decode_magic_string(decInfo) != e_success)
    {
        printf(COLOR_RED "ERROR: Decoding of magic string is a failure\n" COLOR_RESET);
        return e_failure;
    }
    printf(COLOR_GREEN "INFO: MAGIC STRING DATA DECODED SUCCESSFULLY\n" COLOR_RESET);

    // Decode file extension size and check for success
    if (decode_file_extn_size(strlen(".txt"), decInfo->fptr_d_src_image) != e_success)
    {
        printf(COLOR_RED "ERROR: Decoding of file extension size is a failure\n" COLOR_RESET);
        return e_failure;
    }
    printf("INFO: Decoded file extension size successfully\n");

    // Decode secret file extension and check for success
    if (decode_secret_file_extn(decInfo->d_extn_secret_file, decInfo) != e_success)
    {
        printf(COLOR_RED "ERROR: Decoding of secret file extension is a failure\n" COLOR_RESET);
        return e_failure;
    }
    printf(COLOR_GREEN "INFO: SECRET FILE EXTENSION DECODED SUCCESSFULLY\n" COLOR_RESET);

    // Decode secret file size and check for success
    if (decode_secret_file_size(decInfo->size_secret_file, decInfo) != e_success)
    {
        printf(COLOR_RED "ERROR: Decoding of secret file size is a failure\n" COLOR_RESET);
        return e_failure;
    }
    printf("INFO: Decoded secret file size successfully\n" );

    // Decode secret file data and check for success
    if (decode_secret_file_data(decInfo) != e_success)
    {
        printf(COLOR_RED "ERROR: Decoding of secret file data is a failure\n" COLOR_RESET);
        return e_failure;
    }
    printf(COLOR_GREEN "INFO: SECRET FILE DATA DECODED SUCCESSFULLY\n" COLOR_RESET);

    return e_success;
}