//MiniProject 1: Encryption/Decryption Utility
//Group 10: Ken Phanthavong and Nathan Chung

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void encrypt(char decryptedChar, FILE *fp);

void decrypt(int char1, int char2, FILE *in, FILE *out);

int main(int argc, char *argv[])
{

    //points to string of file name taken as arguement
    char *filename, *extension;

    int removeAddress;
    char noExtension[100] = {' '};
    char newFile[100] = {' '};

    char inChar, inChar2;
    FILE *input, *output;

    //If a key is not entered, the second arguement will be taken as the file name instead of the third
    if (argv[2] != NULL)
    {
        filename = argv[2];
    }
    else
    {
        filename = argv[1];
    }

    /*
    ***Preperation for file renaming***
    */

    //strchr finds the characters after and including the first instance of '.'

    //This checks if the file name contains '.', or if the file name ends with .txt
    if (strchr(filename, '.') != NULL)
    {
        //this finds the characters that comes after and includes '.' in the filename
        extension = strchr(filename, '.');

        //Takes address of where '.' occurs and subtracts from address of the string and adds 1 to get index
        removeAddress = extension - filename + 1;

        //copies file name without whatever extension it has by copying up to the index with the '.' character
        strncpy(noExtension, filename, removeAddress);
    }
    else //if no extension is included / no '.'
    {
        strcpy(noExtension, filename);
        strcat(noExtension, ".");

        //allows passage through the extension-adding if statements
        //this is so the program doesn't get stuck on strcmp
    }

    /*
    ***file reading and writing along with decrypting and encrypting***
    */

    //checks if the arguement is the decrypt or encrypt key
    //this is decrypt
    if (strcmp(argv[1], "-decrypt") == 0)
    {
        //renames file to appropriate extension
        strcpy(newFile, noExtension);
        strcat(newFile, "crp");
        rename(filename, newFile);

        input = fopen(newFile, "r");

        if (input == NULL)
        {
            printf("Can't open the file\n");
            exit(0);
        }

        //creates output txt file
        output = fopen(strcat(noExtension, "txt"), "w");

        //decrypts per 2 characters until the end of the file
        inChar = fgetc(input);
        inChar2 = fgetc(input);
        while (inChar != EOF)
        {
            decrypt(inChar, inChar2, input, output);
            inChar = fgetc(input);
            inChar2 = fgetc(input);
        }

        fclose(input);
        fclose(output);
    }

    //encrypt
    else
    {
        //renames file with proper extension
        strcpy(newFile, noExtension);
        strcat(newFile, "txt");
        rename(filename, newFile);

        input = fopen(newFile, "r");

        if (input == NULL)
        {
            printf("Can't open the file\n");
            exit(0);
        }

        //creates output crp file
        output = fopen(strcat(noExtension, "crp"), "w");

        //encrypts per character until end of the file
        inChar = fgetc(input);
        while (inChar != EOF)
        {
            encrypt(inChar, output);
            inChar = fgetc(input);
        }

        fclose(input);
        fclose(output);
    }

    return 0;
}

void encrypt(char decryptedChar, FILE *fp)
{
    int outChar;

    if (decryptedChar == '\t')
    {
        fputs("TT", fp); //If char i is \t, print "TT"
    }
    else if (decryptedChar == '\n' || decryptedChar == '\r')
    {
        fputc(decryptedChar, fp); //If char i is \n or \r, print it as it is
    }
    else
    {
        outChar = decryptedChar - 16;
        if (outChar < 32)
        {
            outChar = (outChar - 32) + 144;
        }

        //48 is the ASCII value for '0', int 0 -> char 0
        fputc(outChar / 16 + 48, fp); //outputs 1st character of the hexadecimal by /16

        outChar = outChar % 16; //converts to 2nd character by %16
        if (outChar <= 9)
        {
            //If remainder of outChar%16 is less than 9, print it as it is
            fputc(outChar + 48, fp);
        }
        else
        {
            //Converts decimal number above 9 (2 digit number) to a letter by assigning 10
            // as the ascii value of 'A' (65) and counting up | 10 - 15 is A - F
            outChar = outChar - 10 + 65;
            fputc(outChar, fp); //Prints the 2nd character of the hexadecimal
        }
    }
}

void decrypt(int char1, int char2, FILE *in, FILE *out)
{
    int dec1, dec2, outChar;

    if (char1 == 10 || char1 == 13)
    {
        fputc(char1, out); //If char1 is \n or \r, output the same as it was before
        if (char2 != EOF) //Checks if \r or \n is the last character
        { 
            decrypt(char2, fgetc(in), in, out);
        }
    }
    else if (char1 == 84 && char2 == 84)
    {
        fputc('\t', out); //If the pair of characters is "TT", print \t into the file
    }
    else
    {
        dec1 = (char1 - 48) * 16; //Multiply decimal of char1 by 16
        if (char2 <= 57)
        {
            dec2 = char2 - 48; //Shifts decimal to ascii value of character
        }
        else
        {
            dec2 = char2 - 55; //Shifts decimal to ascii value of letter
        }
        outChar = dec1 + dec2;

        outChar = outChar + 16;
        if (outChar > 127)
        {
            outChar = (outChar - 144) + 32;
        }
        fputc(outChar, out);
    }
}