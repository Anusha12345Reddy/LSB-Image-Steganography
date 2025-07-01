#ifndef   DECODE_H
#define   DECODE_H

#include<stdio.h>
#include"types.h"

typedef struct _decodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char extn_secret_file[10];
    char output_name[256];
    int extension_size;    // To store the extension size
    int file_size;         // To store the decoded file size
} DecodeInfo;

// Function prototypes
Status do_decoding(DecodeInfo *decInfo);
Status open_files_decode(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);   // Ensure this prototype is present
Status decode_file_extension(DecodeInfo *decInfo);
Status decode_file_size(DecodeInfo *decInfo);
Status decode_secret_data(DecodeInfo *decInfo);      // Ensure this prototype is present
Status decode_file_data(DecodeInfo *decInfo);
char decode_byte_from_image(FILE *fptr_stego_image);

#endif
