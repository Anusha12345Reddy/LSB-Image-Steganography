#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
/* Perform the encoding process */
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO: Encoding process started.....\n");

    printf("INFO: opening files\n");
    if (open_files(encInfo) == e_failure) 
    {
        printf("ERROR: Unable to open files\n");
        return e_failure;
    }
    printf("INFO: opening files is done...\n");
    printf("INFO: Checking capacity...\n");
    if(check_capacity(encInfo)==e_failure)
    {
        printf("ERROR: Failed to check capacity\n");
        return e_failure;
    }
    printf("INFO: Checking capacity is Done.\n");
    printf("INFO:Copy header started...\n");

    // Copy BMP Header (First 54 bytes)
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Failed to copy BMP header\n");
        return e_failure;
    }
    printf("INFO:Copy header is done\n");
    printf("INFO:Encode magic string started.......\n");
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode magic string\n");
        return e_failure;
    }
    printf("INFO:Encode magic string is done\n");
    printf("INFO: Encode secret file size started....\n");
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode file size\n");
        return e_failure;
    }
    printf("INFO: Encode secret file size done\n");
    printf("INFO: Encode secret file extension started....\n");
    strcpy(encInfo->extn_secret_file,".txt");
    // Encode Secret File Extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode file extension\n");
        return e_failure;
    }
    printf("INFO: Encode secret file extension done\n");
    // Encode Secret File Data

    if (encode_secret_file_data(encInfo) == e_failure) 
    {
        printf("ERROR: Failed to encode file data\n");
        return e_failure;
    }
    // Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Failed to copy remaining image data\n");
        return e_failure;
    }
    printf("INFO: Encoding completed successfully!......\n");
    return e_success;
}

/* Open input and output files */
Status open_files(EncodeInfo *encInfo)
{
    // Open Source Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }

    // Open Secret File
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL) 
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        return e_failure;
    }

    // Open Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    FILE *fptr =fopen(encInfo->secret_fname,"r");
    if(fptr ==NULL)
    {
        fprintf(stderr,"ERROR:unable to open file\n");
        return e_failure;
    }
  uint img_size = get_image_size_for_bmp(encInfo->fptr_src_image);
    uint m_string = 2*8;

    uint file_extension_size = 4*8;
    uint file_extension = 4*8;
    
    rewind(fptr);
    encInfo->size_secret_file = fseek(fptr,0,SEEK_END);
    encInfo->size_secret_file=ftell(fptr);
    uint filecontent = encInfo->size_secret_file;
    rewind(fptr);
    uint sizeof_encoding = m_string+file_extension+file_extension_size+(filecontent)*8;
    //printf("size = %d\n",sizeof_encoding);
    if(img_size > sizeof_encoding)
    {
        return e_success;
    }
    else{
        return e_failure;
    }
    fclose(fptr);

}
/* Copy BMP header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];

    // Read header from source image
    
    rewind(fptr_src_image);
    fread(header, 54, 1, fptr_src_image);
    // Write header to destination image
    printf("pointer : %ld\n", ftell(fptr_dest_image));
    fwrite(header, 54, 1, fptr_dest_image);
    printf("pointer : %ld\n", ftell(fptr_dest_image));

    return e_success;
}

/* Encode a string into the image */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    return encode_data_to_image((char *)magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image);
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    

    return encode_data_to_image((char *)file_extn, MAX_FILE_SUFFIX, encInfo->fptr_src_image, encInfo->fptr_stego_image);
}

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char data_buffer[32];
    fread(data_buffer, 32, 1, encInfo->fptr_src_image);

    for(int i = 0; i < 32; i++)
    {
        *(data_buffer+i) = (*(data_buffer+i) & (~1)) | ((file_size>>i) & 1);
        
    }

    fwrite(data_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("Encoding secret file data is started....\n");
    char data;
    fseek(encInfo->fptr_secret,0,SEEK_END);
    int size = ftell(encInfo->fptr_secret);

    // int size = strlen(encInfo->fptr_secret);
    rewind(encInfo -> fptr_secret);
    while (fread(&data, sizeof(char), 1, encInfo->fptr_secret) > 0)
    {
        fread(encInfo->image_data,1,8,encInfo->fptr_src_image);
        if (encode_byte_to_lsb(data, encInfo->image_data,size) == e_failure)
       	{
            return e_failure;
        }
        fwrite(encInfo->image_data, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    printf("INFO: Encode secret file data is done\n");
    return e_success;
}

/* Encode a byte into LSB of 8 bytes */
Status encode_byte_to_lsb(char data, char *image_buffer,int size)
{
    for (int i = 0; i < 8; i++)
    {
        *(image_buffer+i) = ((*(image_buffer+i)) & (~1)) | (((data) >> i) & 1);
    }
    return e_success;
}
/* Encode data to image */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[MAX_IMAGE_BUF_SIZE];
    // fseek(fptr_src_image,54,SEEK_SET);
    for (int i = 0; i < size; i++)
    {
        fread(buffer, sizeof(char), 8, fptr_src_image);
        if (encode_byte_to_lsb(data[i], buffer,size) == e_failure)
       	{
            return e_failure;
        }
        fwrite(buffer, sizeof(char), 8, fptr_stego_image);
    }
    return e_success;
}
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // char buffer[1024];
    printf("Copy remaining data started.....\n");
    char ch;

    while ((fread(&ch, sizeof(char), 1, fptr_src)) > 0)
    {
       fwrite(&ch, sizeof(char), 1, fptr_dest);
    }
    printf("INFO: Copy remaining data Done.....\n");

    return e_success;
}

