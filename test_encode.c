#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

OperationType check_operation_type(char *argv[]) {
    if ((strcmp(argv[1], "-e")) == 0) {
        return e_encode;
    }
    else if ((strcmp(argv[1], "-d")) == 0) {
        return e_decode;
    }
    else {
        return e_unsupported;
    }
}

int Bmp_fileor_not(char *fileName) {
    int len = strlen(fileName);
    if ((len > 4 && strcasecmp(fileName + len - 4, ".bmp") == 0)) {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    EncodeInfo encInfo;
    OperationType ret_value = check_operation_type(argv);

    if (ret_value == e_encode) {
        if (argc < 5) {
            printf("you should give inputs like this <encode or decode ><source.bmp><secret_file><output_image.bmp>\n");
            return 1;
        }

        if ((Bmp_fileor_not(argv[2])) != 0 || (Bmp_fileor_not(argv[4])) != 0) {
            printf("ERROR: You should give source and destination file with .BMP extension\n");
            return 1;
        }

        encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
        encInfo.stego_image_fname = argv[4];

        if (do_encoding(&encInfo) == e_failure) {
            printf("Encoding failed\n");
            return 1;
        }
    }
    else if (ret_value == e_decode) {
        DecodeInfo decInfo;

        if (argc < 3) {
            printf("Usage: %s <mode> <stego_image_fname> [output_name]\n", argv[0]);
            return 1;
        }

        decInfo.stego_image_fname = argv[2];

        if (argc >= 4) {
            // Use strcpy to copy the string into the array
            strcpy(decInfo.output_name, argv[3]);
        } else {
            // Set it to empty string if no output file is provided
            decInfo.output_name[0] = '\0';
        }

        if (do_decoding(&decInfo) == e_failure) {
            printf("Decoding failed\n");
            return 1;
        }
    }
    else {
        printf("Error: Invalid input\n");
        return 1;
    }

    // Assuming you have a function to get the image size (implement it if not)
    uint img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);

    return 0;
}


