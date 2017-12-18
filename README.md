# Code128 Encoder

This repo contains the algorithm to encode some text into **Code128 code set B** and the mathematic algorithm to calculate the checksum bit.

The following results was reached by reverse engineering in 2012. Since 2015 I use this algorithm in several commercial projects and it's worked very well.


### How the encoder works?

Code128 is a high-density linear barcode symbology. It is used for alphanumeric or numeric-only barcodes. It can encode all 128 characters of ASCII and, by use of an extension symbol (FNC4), the Latin-1 characters defined in ISO/IEC 8859-1.

A Code 128 barcode has four sections with two quiet sections:

  *  Quiet zone
  *  Start symbol
  *  Data
  *  Check symbol
  *  Stop symbol
  *  Quiet zone

The check symbol is calculated from a weighted sum (modulo 103) of all the symbols.


#### Why B norm?

| Code set | Start char | Start ASCII code | Stop char | Stop ASCII code | Symbols supported |
| ------ | ------ | ------ | ------ | ------ | ------ |
| A | Ð | 208 | Ò | 211 | 00 to 95 (0–9, A–Z and control codes), special characters, and FNC 1–4 |
| A | Ñ | 209 | Ò | 211 | ASCII characters 32 to 127 (0–9, A–Z, a–z), special characters, and FNC 1–4 |
| A | Ó | 210 | Ò | 211 | 00–99 (encodes two digits with a single code point) and FNC1 |

So, the B norm is more useful since can support uppercase, lowercase, numbers, symbols and special characters.


### Algorithm

At this point we have the following things:
 * Text to encode (Data)
 * Start symbol
 * End symbol

But we need to calculate the check symbol. How we do that?

The form is:

<kbd>Start symbol</kbd><kbd>**Data**</kbd><kbd>Checksum</kbd><kbd>End symbol</kbd>


#### Mathematics
Using the weighted sum model, sum each character subtracting 32 (so it's start from 0 to 95, the number of valid chars that can be represented by Code128-B) and multiply by the position of each character. Finally add 104 to this number.

In equation

<img src="https://latex.codecogs.com/gif.latex?\LARGE&space;S_{p}=104&plus;\sum_{i=1}^n{[(a_{i}-32).i]}" title="\LARGE S_{p}=104+\sum_{i=1}^n{[(a_{i}-32).i]}" />

Where:

 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;S_{p}" title="\small S_{p}" /> is the result of the weighted sum
 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;i" title="\small i" /> is the current position
 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;a_{i}" title="\small a_{i}" /> is the char in the <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;i" title="\small i" /> position.
 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;n" title="\small n" /> is the total number of chars

The next step is obtain the modulo (division remainder) between <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;S_{p}" title="\small S_{p}" /> and `103`.

<img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;R_{check}=(S_{p})MOD(103)" title="\LARGE R_{check}=(S_{p})MOD(103)" />

Where

 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\tiny&space;R_{check}" title="\small R_{check}" /> is the remainder between <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\tiny&space;\frac{S_{p}}{103}" title="\small \frac{S_{p}}{103}" />.
 * `MOD` is the mathematic expression to obtain the remainder, and no the division result.

Finally, to obtain the ASCII code to the checksum bit, evaluate the <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;R_{check}" title="\small R_{check}" /> result. If it's less than 95, then add 32. Otherwise add 105.

<img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\large&space;Bit_{c}=\left\{\begin{matrix}&space;R_{chek}&space;<&space;95&space;\rightarrow&space;R_{chek}&plus;32&space;\\&space;R_{chek}&space;\geqslant&space;95&space;\rightarrow&space;R_{chek}&plus;105&space;\end{matrix}\right." title="\large Bit_{c}=\left\{\begin{matrix} R_{chek} < 95 \rightarrow R_{chek}+32 \\ R_{chek} \geqslant 95 \rightarrow R_{chek}+105 \end{matrix}\right." />

Why we do that? Because this way avoid get undesired ASCII values (between 127 and 199), which are control chars and have not graphic representation.

----


#### Notes about the efficiency of the code
`Under test`



### About

The code and algorithm here provided was developed by Cristian Bottazzi

The font are not mine.



