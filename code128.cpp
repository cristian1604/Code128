#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    int pos = 1;
    int i = 1;
    int checksum = 104;       // Verification char (104 to B norm)
    char _start_bit = 'Ñ';
    char _end_bit = 'Ó';
    char aux[50] = {'\0'};
    int char_to_int;
    char str_conv[500] = {'\0'};
    char end_line = '\0';

    cout<<"Code128 Encoder - Developed by Cristian Bottazzi\n  * March 2012 * Last revision: May 2015.\n   * Uploaded to GitHub 2017\n  http://github.com/cristian1604 - cbottazzi@ful.unl.edu.ar\n---------------------------------------------\n\n";
    if (argc == 1) {
        cout<<" >>> Give me any string to encode!\n\n";
		return 1;
    } else {
        cout<<endl<<" >>> String provided: ";
        puts(argv[1]);
        cout<<endl;
        strcpy(str_conv,argv[1]);
    }

    // Copy the original string to auxiliar array bit to bit
    while (str_conv[pos - 1] != end_line) {
        aux[pos] = str_conv[pos - 1];
        pos++;
    };

    // At the original string put the start bit and the original string
    str_conv[0] = start_bit;
    pos = 1;
     while (aux[pos] != end_line) {
        str_conv[pos] = aux[pos];
        pos++;
    };


    // The pos variable point to the position where goes the parity bit (checksum)
    while (str_conv[i] != end_line) {
        char_to_int = str_conv[i];
        checksum = checksum + ((char_to_int-32)*i);
        i++;
    };

    checksum = (checksum % 103);        // 103 is for Code128.
    if (checksum < 95)
		checksum = checksum + 32;
    else
		checksum = checksum + 105;

    str_conv[pos] = checksum;
    pos++;
    str_conv[pos] = _end_bit;           // Stop bit
    pos++;
    str_conv[pos] = end_line;           // End of string

    fstream file;
    file.open("code128.txt",ios::out);
    file<<str_conv;
    file.close();

    cout<<"String generated at \"code128.txt\" file. In the same directory of this app\n\n";

    return 0;
}

