# Code128 Encoder

Versión en español [aquí][link]

This repo contains an example algorithm to encode some text into **Code128 code set B** and the **mathematic algorithm** to calculate the checksum bit.

The following results was reached by reverse engineering in 2012. Since 2015 I use this algorithm (enhanced) in several commercial projects and it's worked very well.

I have also uploaded the ttf source.


### Introduction

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

Why do that? Because this way avoid get undesired ASCII values (between 127 and 199), which are control chars and have not graphic representation.

----

### Example

This is an example that I wrote to my old blog [Binarios][binarios] on 2012.

The idea is calculate the checksum bit of the word `Binari-OS` (binary and "OS" in spanish) using the standard `code128-B`. Remember, the parts in the codification string are:
<kbd>Start</kbd><kbd>**Data**</kbd><kbd>**Checksum**</kbd><kbd>End</kbd>

   * `Start` is a Ñ (ASCII 209)
   * `End` = is an Ó or (ASCII 211)
   * `Data`: `Binari-OS`

So, to calculate the checksum bit, we have to obtain the weighted sum with the values of each char.

<a href="https://www.codecogs.com/eqnedit.php?latex=\left.\begin{matrix}&space;B&space;&&space;\rightarrow&space;&&space;66&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;n&space;&&space;\rightarrow&space;&&space;110&space;\\&space;a&space;&&space;\rightarrow&space;&&space;96&space;\\&space;r&space;&&space;\rightarrow&space;&&space;114&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;-&space;&&space;\rightarrow&space;&&space;45&space;\\&space;O&space;&&space;\rightarrow&space;&&space;79&space;\\&space;S&space;&&space;\rightarrow&space;&&space;83&space;\end{matrix}\right\}&space;104&plus;\sum_{i=1}^{9}(a_{i}-32)*i=2552" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\left.\begin{matrix}&space;B&space;&&space;\rightarrow&space;&&space;66&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;n&space;&&space;\rightarrow&space;&&space;110&space;\\&space;a&space;&&space;\rightarrow&space;&&space;96&space;\\&space;r&space;&&space;\rightarrow&space;&&space;114&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;-&space;&&space;\rightarrow&space;&&space;45&space;\\&space;O&space;&&space;\rightarrow&space;&&space;79&space;\\&space;S&space;&&space;\rightarrow&space;&&space;83&space;\end{matrix}\right\}&space;104&plus;\sum_{i=1}^{9}(a_{i}-32)*i=2552" title="\left.\begin{matrix} B & \rightarrow & 66 \\ i & \rightarrow & 105 \\ n & \rightarrow & 110 \\ a & \rightarrow & 96 \\ r & \rightarrow & 114 \\ i & \rightarrow & 105 \\ - & \rightarrow & 45 \\ O & \rightarrow & 79 \\ S & \rightarrow & 83 \end{matrix}\right\} 104+\sum_{i=1}^{9}(a_{i}-32)*i=2552" /></a>


At this moment we have the result: `2552`. Now we need the **remainder** between the result and `103`.

<a href="https://www.codecogs.com/eqnedit.php?latex=\frac{2552}{103}=&space;24&space;(mod&space;\rightarrow&space;80)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\frac{2552}{103}=&space;24&space;(mod&space;\rightarrow&space;80)" title="\frac{2552}{103}= 24 (mod \rightarrow 80)" /></a>

Finally, compare the result:  `80 < 95`, so we have to add 32 to 80.

`32 + 80 = 112`

If we take a look on the ASCII table, the char number `112` it's a `p`.

So, the final encoded string `Binari-OS` will be:

<kbd>Ñ</kbd><kbd>Binari-OS</kbd><kbd>p</kbd><kbd>Ó</kbd>

or `ÑBinari-OSpÓ`

Using this string (you can copy&paste) with the font `code128.ttf` provided in this repository, you can test quickly with any barcode scanner or your mobile phone. Test it!



#### Notes about the efficiency of the code

 * **Code as is**: The `code128.cpp` writes in a txt file on disk the result of the encoded data. But it's very inefficient because the constant access to HDD generates latency.

 * **Improve it!**: This code allows a huge improvement. For example, instead copy bit to bit the string, yo can add separately the start, checksum and end bits. This reduce one order the execution from O(n²) to O(n).

Remember: the code here provided is for demonstration :)


### About

All the source and documentation was developed by Cristian Bottazzi

The `ttf` font are not mine.

[//]: #References
[link]: <https://github.com/cristian1604/Code128/blob/master/README_ES.md>


