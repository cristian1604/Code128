# Codificador Code128

English version [here][link]

Este repositorio contiene un ejemplo para codificar texto en **code128 norma B**. También se provee el **algoritmo matemático** para calcular el bit de paridad.

Este código ha sido resultado de aplicar ingeniería inversa para un trabajo que tuve que realizar en 2012. Desde el 2015 (última revisión del algoritmo matemático) lo he utilizado en varios proyectos y ha funcionado muy bien.

También he subido la fuente tipográfica `Code128.ttf`


### Introducción


   * El Code 128 es una simbología de muy alta densidad. Es empleado para la representación de caracteres alfanuméricos o sólo numéricos.
   * Éste estandar puede codificar todos los 128 caracteres de la tabla ASCII, entre otros estándares (incluso caracteres latinos).
   * Es empleado ampliamente alrededor del mundo para el rastreo de paquetes, controles de stock industriales, identificación de contenedores y pallets en la cadena de abastecimiento.
   * La simbología fue definida como ISO/IEC 15417:2007.


Está compuesto de cuatro secciones, y sus extremos (*quiet zones*) no deben tener ninguna inscripción.

  *  Quiet zone
  *  Símbolo de inicio
  *  Dato
  *  Bit de paridad
  *  Bit de fin
  *  Quiet zone

El símbolo de paridad está calculado mediante una suma ponderada (modulo 103) de todos los demás símbolos.


#### Por qué empleamos la norma B?

| Code set | Inicio | Código ASCII inicio | Bit fin | Código ASCII fin | Símbolos soportados |
| ------ | ------ | ------ | ------ | ------ | ------ |
| A | Ð | 208 | Ò | 211 | 00 a 95 (0–9, A–Z y códigos de control), caracteres especiales y FNC 1–4 |
| A | Ñ | 209 | Ò | 211 | Caracteres ASCII del 32 al 127 (0–9, A–Z, a–z), caracteres especiales, y FNC 1–4 |
| A | Ó | 210 | Ò | 211 | 00–99 (codifica dos dígitos en un único punto) y FNC1 |

Es por ello que la norma B es más útil, ya que soporta mayúsculas, minúsculas, números y símbolos.


### Algoritmo

En este punto tenemos los siguientes datos:
 * Texto a codificar (Datos)
 * Símbolo de inicio
 * Símbolo de fin

Pero es necesario hallar el bit de paridad (o símbolo de verificación). Cómo lo hallamos?

Recordemos que la forma final debe ser:

<kbd>Inicio</kbd><kbd>**Dato**</kbd><kbd>**Paridad**</kbd><kbd>Fin</kbd>


#### Matemática
Utilizando el modelo de suma ponderada, debemos sumar el valor de cada caracter (restado 32, de ese modo comenzamos a contar desde 0 a 95, que corresponde al número total de caracteres que se pueden representar con el código 128B) y multiplicar esa resta por la posición del caracter. Finalmente se debe sumar `104` a ese resultado.

En ecuación:

<img src="https://latex.codecogs.com/gif.latex?\LARGE&space;S_{p}=104&plus;\sum_{i=1}^n{[(a_{i}-32).i]}" title="\LARGE S_{p}=104+\sum_{i=1}^n{[(a_{i}-32).i]}" />

Donde:

 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;S_{p}" title="\small S_{p}" /> es el resultado de la suma ponderada
 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;i" title="\small i" /> es la posición actual
 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;a_{i}" title="\small a_{i}" /> es el caracter en la posición <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;i" title="\small i" />.
 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;n" title="\small n" /> es el número total de caracteres

El próximo paso es obtener el resto de la división de la suma ponderada <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;S_{p}" title="\small S_{p}" /> y `103`.

<img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;R_{check}=(S_{p})MOD(103)" title="\LARGE R_{check}=(S_{p})MOD(103)" />

Donde

 * <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\tiny&space;R_{check}" title="\small R_{check}" /> es el resto de <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\tiny&space;\frac{S_{p}}{103}" title="\small \frac{S_{p}}{103}" />.
 * `MOD` es la expresión matemática para obtener el resto de una división.

Finalmente, para obtener el número del caracter ASCII que representará el bit de paridad, evaluamos el resultado <img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\small&space;R_{check}" title="\small R_{check}" />. Si es menor que 95 entonces se le suma 32. En cualquier otro caso se le suma 105.

<img src="https://latex.codecogs.com/png.latex?\dpi{150}&space;\large&space;Bit_{c}=\left\{\begin{matrix}&space;R_{chek}&space;<&space;95&space;\rightarrow&space;R_{chek}&plus;32&space;\\&space;R_{chek}&space;\geqslant&space;95&space;\rightarrow&space;R_{chek}&plus;105&space;\end{matrix}\right." title="\large Bit_{c}=\left\{\begin{matrix} R_{chek} < 95 \rightarrow R_{chek}+32 \\ R_{chek} \geqslant 95 \rightarrow R_{chek}+105 \end{matrix}\right." />

¿Por qué hacer esto? Porque de este modo se evita obtener códigos ASCII no deseados. Éstos son los caracteres comprendidos entre 127 y 199, que no poseen ninguna representación gráfica.

----

### Ejemplo

El ejemplo es uno que originalmente armé para un blog que tuve hace un tiempo llamado [Binarios][binarios]. Aquí transcribo:

Codificaremos la palabra `Binari-OS` en code128, variante `B`. Para ello ya se ha visto que la forma básica del armado es el siguiente:
<kbd>Inicio</kbd><kbd>**Dato**</kbd><kbd>**Paridad**</kbd><kbd>Fin</kbd>


   * [INICIO] = Ñ ó ASCII 209
   * [FIN] = Ó ó ASCII 211
   * Dato: `Binari-OS`

Para hallar el único dato que queda, se debe comenzar obteniendo la correspondencia entre caracteres y su código ASCII. Aplicar la suma ponderada a estos valores y obtener el valor de esa sumatoria:

<a href="https://www.codecogs.com/eqnedit.php?latex=\left.\begin{matrix}&space;B&space;&&space;\rightarrow&space;&&space;66&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;n&space;&&space;\rightarrow&space;&&space;110&space;\\&space;a&space;&&space;\rightarrow&space;&&space;96&space;\\&space;r&space;&&space;\rightarrow&space;&&space;114&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;-&space;&&space;\rightarrow&space;&&space;45&space;\\&space;O&space;&&space;\rightarrow&space;&&space;79&space;\\&space;S&space;&&space;\rightarrow&space;&&space;83&space;\end{matrix}\right\}&space;104&plus;\sum_{i=1}^{9}(a_{i}-32)*i=2552" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\left.\begin{matrix}&space;B&space;&&space;\rightarrow&space;&&space;66&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;n&space;&&space;\rightarrow&space;&&space;110&space;\\&space;a&space;&&space;\rightarrow&space;&&space;96&space;\\&space;r&space;&&space;\rightarrow&space;&&space;114&space;\\&space;i&space;&&space;\rightarrow&space;&&space;105&space;\\&space;-&space;&&space;\rightarrow&space;&&space;45&space;\\&space;O&space;&&space;\rightarrow&space;&&space;79&space;\\&space;S&space;&&space;\rightarrow&space;&&space;83&space;\end{matrix}\right\}&space;104&plus;\sum_{i=1}^{9}(a_{i}-32)*i=2552" title="\left.\begin{matrix} B & \rightarrow & 66 \\ i & \rightarrow & 105 \\ n & \rightarrow & 110 \\ a & \rightarrow & 96 \\ r & \rightarrow & 114 \\ i & \rightarrow & 105 \\ - & \rightarrow & 45 \\ O & \rightarrow & 79 \\ S & \rightarrow & 83 \end{matrix}\right\} 104+\sum_{i=1}^{9}(a_{i}-32)*i=2552" /></a>


Luego este valor se divide por `103` y obtenemos el **resto**.

<a href="https://www.codecogs.com/eqnedit.php?latex=\frac{2552}{103}=&space;24&space;(mod&space;\rightarrow&space;80)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\frac{2552}{103}=&space;24&space;(mod&space;\rightarrow&space;80)" title="\frac{2552}{103}= 24 (mod \rightarrow 80)" /></a>

Finalmente, realizamos la última comprobación:  `80 < 95`, por lo que se debe sumar 32.

`32 + 80 = 112`

Observando la tabla ASCII, el caracter `112` corresponde a `p`.

Por lo que finalmente el código quedará compuesto como:

<kbd>Ñ</kbd><kbd>Binari-OS</kbd><kbd>p</kbd><kbd>Ó</kbd>

ó `ÑBinari-OSpÓ`

Si a este texto se le aplica la fuente `code128.ttf` provista en este mismo repositorio podrá ser leído por cualquier escáner de códigos de barra o teléfono móvil con la app correspondiente.


#### Notas acerca de la eficiencia del código propuesto

 * **Código propuesto**: El archivo `code128.cpp` escribe en un archivo de texto el resultado de la cadena codificada. Este método es muy ineficiente debido a que el acceso a la unidad de disco genera latencia que no debe ser despreciada al codificar concurrentemente gran cantidad de información.

 * **Mejoralo!**: Este código permite muchas modificaciones que pueden incrementar notablemente su performance. Por ejemplo, en lugar de copiar bit a bit la cadena a convertir, puede agregarse separadamente los bit de inicio, paridad y finalización sin realizar esta copia. Esto reduce el orden de ejecución de O(n²) a O(n).

No pierda de vista que el código que se proporciona tiene fines educativos y académicos.


### Acerca de

El código y la documentación fueron desarrolladas por Cristian Bottazzi

La tipografía no es de mi autoría.

[//]: #References
[link]: <https://github.com/cristian1604/Code128/blob/master/README.md>
[binarios]: <https://web.archive.org/web/20140111080144/http://binari-os.com.ar:80/index.php/cs-computacion/122-generar-codigos-de-barra-con-el-estandar-code128>



