#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PA_LIB_H__
#define __PA_LIB_H__





// NightFox LIB - Include General
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



/*

Notas sobre BITSHIFT

(n >> x)	Divide			n / x
(n << x)	Multiplica		n * x

Valores de X
2	  =	1
4	  =	2
8	  =	3
16	  =	4
32	  =	5
64	  =	6
128	  =	7
256	  =	8
512	  =	9
1024  =	10
2048  =	11
4096  =	12
8192  =	13
16384 =	14
32768 =	15
65536 =	16

Dado que la DS no tiene unidad de coma flotante, siempre que dividas o
multipliques por numeros de base 2, usa el bitshift
Por ejemplo:
a = (512 / 8);
seria equivalente a
a = (512 >> 3);
Multiplicando
b = (3 * 2048);
seria con bitshift
b = (3 << 11);

*/




// Definiciones comunes
#include "PA_defines.h"

// Libreria de funciones basicas y comunes
#include "PA_basic.h"

// Libreria de funciones 2D comunes
#include "PA_2d.h"

// Libreria de fondos con Tiles
#include "PA_tiledbg.h"

// Libreria de fondos Affine
#include "PA_affinebg.h"

// Libreria de fondos en modo Bitmap
#include "PA_bitmapbg.h"

// Libreria de fondos en modo mixto (Tiled / Bitmap 8 bits)
#include "PA_mixedbg.h"

// Libreria de sprites de 256 colores
#include "PA_sprite256.h"

// Libreria de textos
#include "PA_text.h"

// Libreria de textos de 16 pixeles
#include "PA_text16.h"

// Libreria de colisiones
#include "PA_colision.h"

// Libreria de sonido
#include "PA_sound.h"

// Libreria de archivos multimedia
#include "PA_media.h"

// Libreria 3D, funciones comunes
#include "PA_3d.h"

// Libreria 3D, Sprites
#include "PA_sprite3d.h"





#endif

#ifdef __cplusplus
}
#endif
