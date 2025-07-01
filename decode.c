#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<stdint.h>

// Function to perform decoding
Status do_decoding(DecodeInfo *decInfo) 
{
    if (open_files_decode(decInfo) == e_failure) 
    {
        printf("ERROR: unable to open files\n");
        return e_failure;
    }

    if (decode_magic_string(decInfo) == e_failure) 
    {
        printf("ERROR: Unable to decode magic string\n");
        return e_failure;
    }
    if (decode_file_size(decInfo) == e_failure) 
    {
        printf("ERROR: Unable to decode file size\n");
        return e_failure;
    }
    if (decode_secret_file_extn(decInfo) == e_failure) 
    {
        printf("ERROR: Unable to decode extension size\n");
        return e_failure;
    }


    snprintf(decInfo->output_name, sizeof(decInfo->output_name), "output_file%s", decInfo->extn_secret_file);

    if (decode_secret_data(decInfo) == e_failure) {
        printf("ERROR: Unable to decode secret data\n");
        return e_failure;
    }

    return e_success;
}

// Function to open files for decoding
Status open_files_decode(DecodeInfo *decInfo) 
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL) 
    {
        printf("ERROR: Unable to find file\n");
        return e_failure;
    }
    return e_success;
}

// Function to decode magic string
Status decode_magic_string(DecodeInfo *decInfo) 
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    for (int j = 0; MAGIC_STRING[j] != '\0'; j++) 
    {
        char magic_char = 0;
        for (int i = 0; i < 8; i++) 
        {
            char img_byte;
            if (fread(&img_byte, 1, 1, decInfo->fptr_stego_image) != 1) 
            {
                printf("ERROR: Unable to read byte during magic string decoding\n");
                return e_failure;
            }
            magic_char |= ((img_byte & 1) << i);
        }
        printf("Decoded magic character: %c\n", magic_char);

        if (magic_char != MAGIC_STRING[j]) 
        {
            printf("ERROR: Magic string mismatch at position %d\n", j);
            return e_failure; 
        }
    }
    printf("Info: decoded magic string is successfully\n");
    return e_success; // Magic string matched
}

// Function to decode extension size
Status decode_secret_file_extn(DecodeInfo *decInfo) 
{
    char extn[4]; 
    for (int j = 0; j < 4; j++) 
    {
        extn[j] = decode_byte_from_image(decInfo->fptr_stego_image);
    }
    // extn[4] = '\0';
    strcpy(decInfo->extn_secret_file, extn);
    return e_success;
}

// Function to decode the secret message byte by byte
char decode_byte_from_image(FILE *fptr_stego_image) 
{
    char byte = 0;
    for (int i = 0; i < 8; i++) {
        char img_byte;
        if (fread(&img_byte, 1, 1, fptr_stego_image) != 1) 
        {
            printf("ERROR: Unable to read byte from image\n");
            return '\0';
        }
        byte |= ((img_byte & 1) << i);
    }
    return byte;
}

// Function to decode the secret file size
Status decode_file_size(DecodeInfo *decInfo) 
{
    printf("pointer -> %ld\n",ftell(decInfo->fptr_stego_image));
    uint32_t file_size = 0;
    for (int i = 0; i < 32; i++) 
    {
        char img_byte;
        fread(&img_byte, 1, 1, decInfo->fptr_stego_image);
        uint32_t temp = 0; 
        temp = img_byte&1;
        file_size =  (file_size |(temp<<i));
    }
     printf("pointer -> %ld\n",ftell(decInfo->fptr_stego_image));
    decInfo->file_size = file_size;
    return e_success;
}
// Function to decode the secret data and store  in the output file
Status decode_secret_data(DecodeInfo *decInfo) 
{
    printf("Output file name is : %s\n",decInfo->output_name);
    FILE *fptr_output = fopen(decInfo->output_name, "w");
    if (fptr_output == NULL) 
    {
        printf("ERROR: Unable to open output file %s\n", decInfo->output_name);
        return e_failure;
    }

    int secret_data_size = decInfo->file_size;
    printf("Decoding Secret Data of Size: %d\n", secret_data_size);

    for (int i = 0; i < secret_data_size; i++) 
    {
        char decoded_byte = decode_byte_from_image(decInfo->fptr_stego_image);
        fwrite(&decoded_byte, sizeof(char), 1, fptr_output);
    }


    fclose(fptr_output);
    printf("Secret data decoded and written to %s successfully!\n", decInfo->output_name);

    return e_success;
}
