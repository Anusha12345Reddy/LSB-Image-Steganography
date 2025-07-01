# LSB-Steganography

📘 **LSB Steganography Project – Explanation**

This project is a command-line based implementation of LSB (Least Significant Bit) Steganography using the C programming language. The goal of the project is to hide and retrieve secret messages or files within image files (specifically BMP format), without visibly altering the image. It demonstrates how digital data can be concealed in the lowest bits of pixel values — a common technique used in digital watermarking, security, and information hiding.

The encoding process involves reading the secret message or file, converting it into a binary stream, and then embedding those bits into the least significant bits of pixel data in the image. During decoding, the program extracts the embedded bits from the image and reconstructs the original message. The image quality remains visually unaffected, making the hidden data imperceptible to the human eye.

This project is entirely menu-driven and supports:

- Encoding secret text or file into an image using LSB technique
- Decoding the hidden message/file from the stego-image
- Verification of image format (supports 24-bit uncompressed BMP)
- Error handling for capacity overflow, file mismatches, and missing inputs
- Modular file structure and separation of logic for encoding and decoding

The project required deep understanding of file formats (especially BMP headers), bitwise operations, and binary data manipulation. It showcases efficient handling of byte-level operations and use of system-level programming in C to perform real-world data hiding techniques.

### 🔹 What This Project Showcases:

- Knowledge of file format parsing and binary data handling
- Implementation of real-world steganography using bitwise manipulation
- Strong grasp of pointers, buffers, and modular function design in C
- Attention to security, data integrity, and command-line automation
