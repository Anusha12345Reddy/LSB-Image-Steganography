# LSB-Image-Steganography
The Address Book is a command-line based application built entirely in the C language, designed to manage contact information like names, phone numbers, and email addresses. The core objective of this project was to create a simple, menu-driven utility that allows a user to add, view, search, edit, and delete contact records, with all data being persistently stored using file handling in C.

Under the hood, this application uses a singly linked list to dynamically manage contacts during runtime. Each contact is represented as a node in the list, containing fields for name, mobile number(s), email, etc. When the program is launched, it reads all previously saved contacts from a file into the linked list. When the user performs any operation — like adding or deleting a contact — the changes are first reflected in the list and then written back to the file.

The user interface is a text-based menu, where the user can:

Add new contacts (with validation to avoid duplicates)

Search for existing contacts by name, phone number, or email

Edit the details of a specific contact

Delete contacts from the list

View all saved contacts in a formatted display

Exit the application, saving all changes to file

This project involved extensive use of file I/O operations (fopen, fread, fwrite) and pointer manipulation for linked list traversal. It also required attention to memory management, string handling, and designing clean modular functions for each operation.

What This Project Showcases:

Strong understanding of dynamic memory allocation and data structures

Use of persistence in CLI applications using files

Ability to write clean, reusable, and modular code in C

Focus on real-world use cases, like managing structured records

