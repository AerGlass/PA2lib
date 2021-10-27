#include <nds.h>
#include <filesystem.h>
#include <fat.h>

// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Includes propios
#include "PA_basic.h"
#include "PA_2d.h"
#include "PA_tiledbg.h"
#include "PA_text.h"




// Define los buffers para almacenar las capas de texto
PA_TYPE_TEXT_IPAO PA_TEXT[2][4];



// Funcion PA_InitTextSys();
void PA_InitText(u8 screen) {

	u8 n = 0;

	// Reinicia las variables
	for (n = 0; n < 4; n ++) {
		PA_TEXT[screen][n].width = 0;		// Ancho del mapa de texto
		PA_TEXT[screen][n].height = 0;		// Alto del mapa de texto
		PA_TEXT[screen][n].rotation = 0;	// Rotacion a 0 (ninguna)
		PA_TEXT[screen][n].slot = 255;		// Slot donde esta el tileset
		PA_TEXT[screen][n].pal = 0;			// n� de paleta extendida (0 por defecto)
		PA_TEXT[screen][n].exist = false;	// Marcalo como no existente
		PA_TEXT[screen][n].update = false;	// No es necesario actualizarlo
	}
	
}



// Funcion PA_LoadTextFont();
void PA_LoadFont(const char* file, const char* name, u16 width, u16 height, u8 rotation) {

	// Busca un slot libre
	u16 n = 0;
	u8 slot = 255;
	for (n = 0; n < PA_SLOTS_TBG; n ++) {		// Busca en todos los slots
		if (PA_TILEDBG[n].available) {			// Si esta libre
			PA_TILEDBG[n].available = false;	// Marcalo como en uso
			slot = n;							// Guarda el slot a usar
			n = PA_SLOTS_TBG;					// Deja de buscar
		}
	}
	// Si no hay ningun slot libre, error
	if (slot == 255) {
		PA_Error(103, "Tiled Bg", PA_SLOTS_TBG);
	}

	// Verifica que el fondo sea multiplo de 256px (32 tiles)
	if (((width % 256) != 0) || ((height % 256) != 0)) {
		PA_Error(115, file, 0);
	}

	// Vacia los buffers que se usaran
	free(PA_BUFFER_BGMAP[slot]);		// Buffer para los mapas
	PA_BUFFER_BGMAP[slot] = NULL;
	free(PA_BUFFER_BGTILES[slot]);		// Buffer para los tiles
	PA_BUFFER_BGTILES[slot] = NULL;
	free(PA_BUFFER_BGPAL[slot]);		// Buffer para los paletas
	PA_BUFFER_BGPAL[slot] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .FNT
	sprintf(filename, "%s/%s.fnt", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		PA_TILEDBG[slot].tilesize = (PA_TEXT_FONT_CHARS << 6);		// 100 caracteres x 64 bytes
		// Reserva el espacio en RAM
		PA_BUFFER_BGTILES[slot] = (char*) calloc (PA_TILEDBG[slot].tilesize, sizeof(char));
		if (PA_BUFFER_BGTILES[slot] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_TILEDBG[slot].tilesize);
		}
		fread(PA_BUFFER_BGTILES[slot], 1, PA_TILEDBG[slot].tilesize, file_id);	// Lee el resto de caracteres de la fuente
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Rota los Gfx de los tiles si es necesario
	if (rotation > 0) {
		for (n = 0; n < PA_TEXT_FONT_CHARS; n ++) {
			PA_RotateTileGfx(slot, n, rotation);
		}
	}

	// Crea un archivo .MAP vacio en RAM 
	// ((ancho / 8) * (alto / 8)) * 2
	PA_TILEDBG[slot].mapsize = (((width >> 3) * (height >> 3)) << 1);
	// Reserva el espacio en RAM
	PA_BUFFER_BGMAP[slot] = (char*) calloc (PA_TILEDBG[slot].mapsize, sizeof(char));
	if (PA_BUFFER_BGMAP[slot] == NULL) {		// Si no hay suficiente RAM libre
		PA_Error(102, NULL, PA_TILEDBG[slot].mapsize);
	}
	// Y ponlo a 0
	memset(PA_BUFFER_BGMAP[slot], 0, PA_TILEDBG[slot].mapsize);

	// Carga el archivo .PAL
	sprintf(filename, "%s/%s.pal", PA_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		fseek(file_id, 0, SEEK_END);
		PA_TILEDBG[slot].palsize = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		PA_BUFFER_BGPAL[slot] = (char*) calloc (PA_TILEDBG[slot].palsize, sizeof(char));
		if (PA_BUFFER_BGPAL[slot] == NULL) {		// Si no hay suficiente RAM libre
			PA_Error(102, NULL, PA_TILEDBG[slot].palsize);
		}
		// Lee el archivo y ponlo en la RAM
		fread(PA_BUFFER_BGPAL[slot], 1, PA_TILEDBG[slot].palsize, file_id);
	} else {	// Si el archivo no existe...
		PA_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Guarda el nombre del Fondo
	sprintf(PA_TILEDBG[slot].name, "%s", name);

	// Y las medidas
	PA_TILEDBG[slot].width = width;
	PA_TILEDBG[slot].height = height;

}



// Funcion PA_UnloadTestFont();
void PA_UnloadFont(const char* name) {
	PA_UnloadBg(name);
}



// Funcion PA_CreateTextLayer();
void PA_CreateTextLayer(u8 screen, u8 layer, u8 rotation, const char* name) {

	u8 n = 0;			// Bucle
	u8 slot = 255;		// Slot seleccionado
	char bg[32];		// Nombre

	// Crea un  fondo para usarlo como capa de texto
	PA_CreateBg(screen, layer, name);

	// Busca el numero de slot donde esta cargada la fuente
	sprintf(bg, "%s", name);							// Obten el nombre del fondo a buscar
	for (n = 0; n < PA_SLOTS_TBG; n ++) {				// Busca en todos los slots
		if (strcmp(bg, PA_TILEDBG[n].name) == 0) {		// Si lo encuentras
			slot = n;									// Guarda el slot a usar
			n = PA_SLOTS_TBG;							// Deja de buscar
		}
	}

	// Guarda si el texto debe ser rotado
	PA_TEXT[screen][layer].rotation = rotation;

	// Guarda las medidas del fondo en tiles (ultimo numero de tile)
	PA_TEXT[screen][layer].width = ((PA_TILEDBG[slot].width >> 3) - 1);
	PA_TEXT[screen][layer].height = ((PA_TILEDBG[slot].height >> 3) - 1);

	// Almacena el slot donde esta cargada la fuente
	PA_TEXT[screen][layer].slot = slot;

	// Y marcalo como creado
	PA_TEXT[screen][layer].exist = true;

}



// Funcion PA_DeleteTextLayer();
void PA_DeleteTextLayer(u8 screen, u8 layer) {

	// Verifica si la capa de texto de destino existe
	if (!PA_TEXT[screen][layer].exist) {
		PA_Error(114, NULL, screen);
	}

	// Borra el fondo usado como capa de texto
	PA_DeleteBg(screen, layer);

	// Guarda si el texto debe ser rotado
	PA_TEXT[screen][layer].rotation = 0;

	// Guarda las medidas del fondo en tiles
	PA_TEXT[screen][layer].width = 0;
	PA_TEXT[screen][layer].height = 0;

	// Y marcalo como creado
	PA_TEXT[screen][layer].exist = false;

}

// Funcion PA_WriteText();
void PA_PrintText(u8 screen, u8 layer, u16 x, u16 y, const char* text) {

	// Verifica si la capa de texto de destino existe
	if (!PA_TEXT[screen][layer].exist) {
		PA_Error(114, NULL, screen);
	}

	u16 n = 0;					// Variable de uso general

	s16 value = 0;				// Valor

	u16 tsize = 0;				// Almacena el numero de letras
	tsize = strlen(text);		// Calcula el numero de letras del texto

	u8* string;					// Buffer temporal
	string = NULL;
	string = (u8*) calloc (tsize, sizeof(u8));

	// Almacena en el buffer temporal el valor de los caracteres
	for (n = 0; n < tsize; n ++) {
		value = ((int)(text[n])) - 32;	// Resta 32 al valor entrado
		if (value < 0) value = 0;
		string[n] = value;
		// Si es un caracter especial...
		if (string[n] > 95) {
			// Resetea el caracter...
			string[n] = 0;
			// Verifica caracteres especiales
			switch (((int)(text[n]))) {
				// Salto de linea
				case 10:	// \n
					string[n] = 200;
					break;
				// Caracteres especiales
				case 199:	// �
					string[n] = 96;
					break;
				case 231:	// �
					string[n] = 97;
					break;
				case 209:	// �
					string[n] = 98;
					break;
				case 241:	// �
					string[n] = 99;
					break;
				// Acentos Mayusculas
				case 193:	// �
					string[n] = 100;
					break;
				case 201:	// �
					string[n] = 101;
					break;
				case 205:	// �
					string[n] = 102;
					break;
				case 211:	// �
					string[n] = 103;
					break;
				case 218:	// �
					string[n] = 104;
					break;
				// Acentos Minusculas
				case 225:	// �
					string[n] = 105;
					break;
				case 233:	// �
					string[n] = 106;
					break;
				case 237:	// �
					string[n] = 107;
					break;
				case 243:	// �
					string[n] = 108;
					break;
				case 250:	// �
					string[n] = 109;
					break;
				// Dieresis
				case 239:	// �
					string[n] = 110;
					break;
				case 252:	// �
					string[n] = 111;
					break;
				// Admiracion e interrogante (Espa�ol)
				case 161:	// �
					string[n] = 112;
					break;
				case 191:	// �
					string[n] = 113;
					break;
				// Caracter invalido
				default:	
					string[n] = 0;
					break;
			}
		}
	}
}
	// Variable para calcular la posicion del texto
	s16 tx = 0;		// X
	s16 ty = 0;		// Y

	// Escribe los datos en el buffer de texto, segun la rotacion
	switch (PA_TEXT[screen][layer].rotation) {

		case 0:		// Sin rotacion
			// Traspasa las coordenadas virtuales a las reales
			tx = x;
			ty = y;
			// Copia el texto al buffer letra a letra
			for (n = 0; n < tsize; n ++) {
				// Si es un caracter valido
				if (string[n] <= PA_TEXT_FONT_LAST_VALID_CHAR) {
					// Escribe la letra correspondiente
					PA_SetTileOfMap(screen,layer, tx, ty, ((PA_TEXT[screen][layer].pal << 12) + string[n]));
					// Siguiente letra
					tx ++;
				}
				if ((tx > PA_TEXT[screen][layer].width) || (string[n] == 200)) {		// Si llegas al final de linea,
					tx = 0;			// salto de linea
					ty ++;
					if (ty > PA_TEXT[screen][layer].height) {	// Si estas en la ultima linea,
						ty = 0;		// vuelve a la primera
					}
				}
			}
			break;


		case 1:		// Rotacion 90� a la derecha
			// Traspasa las coordenadas virtuales a las reales
			tx = (PA_TEXT[screen][layer].width - y);
			ty = x;
			// Copia el texto al buffer letra a letra
			for (n = 0; n < tsize; n ++) {
				// Si es un caracter valido
				if (string[n] <= PA_TEXT_FONT_LAST_VALID_CHAR) {
					// Escribe la letra correspondiente
					PA_SetTileOfMap(screen,layer, tx, ty, ((PA_TEXT[screen][layer].pal << 12) + string[n]));
					// Siguiente letra
					ty ++;
				}
				if ((ty > PA_TEXT[screen][layer].height) || (string[n] == 200)) {		// Si llegas al final de linea,
					ty = 0;			// salto de linea
					tx --;
					if (tx < 0) {	// Si estas en la ultima linea,
						tx = PA_TEXT[screen][layer].width;	// vuelve a la primera
					}
				}
			}
			break;


		case 2:		// Rotacion 90� a la izquierda
			// Traspasa las coordenadas virtuales a las reales
			tx = y;
			ty = (PA_TEXT[screen][layer].height - x);
			// Copia el texto al buffer letra a letra
			for (n = 0; n < tsize; n ++) {
				// Si es un caracter valido
				if (string[n] <= PA_TEXT_FONT_LAST_VALID_CHAR) {
					// Escribe la letra correspondiente
					PA_SetTileOfMap(screen,layer, tx, ty, ((PA_TEXT[screen][layer].pal << 12) + string[n]));
					// Siguiente letra
					ty --;
				}
				if ((ty < 0) || (string[n] == 200)) {		// Si llegas al final de linea,
					ty = PA_TEXT[screen][layer].height;		// Salto de linea
					tx ++;
					if (tx > PA_TEXT[screen][layer].width) {	// Si llegas a la ultima linea
						tx = 0;		// vuelve a la primera
					}
				}
			}
			break;

	}

	// Marca esta capa de texto para actualizar
	PA_TEXT[screen][layer].update = true;

	// Libera el buffer
	free(string);

}



// Funcion PA_UpdateTextLayers();
void PA_UpdateTextLayers(void) {

	// Variables
	u8 screen = 0;
	u8 layer = 0;

	// Verifica si se tiene que actualizar la capa de texto
	for (screen = 0; screen < 2; screen ++) {		// Bucle de pantalla
		for (layer = 0; layer < 4; layer ++) {		// Bucle de capas
			if (PA_TEXT[screen][layer].update) {	// Si estas marcado para actualizar, hazlo
				PA_UpdateVramMap(screen, layer);
				// Y marcala como actualizada
				PA_TEXT[screen][layer].update = false;

			}
		}
	}
}


// Funcion PA_ClearTextLayer();
void PA_ClearTextLayer(u8 screen, u8 layer) {

	// Verifica si la capa de texto de destino existe
	if (!PA_TEXT[screen][layer].exist) {
		PA_Error(114, NULL, screen);
	}

	// Calcula el tama�o del buffer
	u32 size = (((PA_TEXT[screen][layer].width + 1) * (PA_TEXT[screen][layer].height + 1)) << 1);

	// Pon a 0 todos los bytes del mapa de la capa de texto
	memset(PA_BUFFER_BGMAP[PA_TEXT[screen][layer].slot], 0, size);

	// Marca esta capa de texto para actualizar
	PA_TEXT[screen][layer].update = true;

}



// Funcion PA_DefineTextColor();
void PA_DefineTextColor(u8 screen, u8 layer, u8 color, u8 r, u8 g, u8 b) {

	// Verifica si la capa de texto de destino existe
	if (!PA_TEXT[screen][layer].exist) {
		PA_Error(114, NULL, screen);
	}

	// Calcula el valor RGB
	u16 rgb = ((r)|((g) << 5)|((b) << 10));
	// Direccion en VRAM
	u32 address = 0;

	// Modifica la paleta
	if (screen == 0) {

		vramSetBankE(VRAM_E_LCD);
		address = (0x06880000) + (layer << 13) + (color << 9);		// Primer color de la paleta
		*((u16*)address) = (u16)0xFF00FF;
		address = (0x06880000) + (layer << 13) + (color << 9) + 2;	// Segundo color de la paleta
		*((u16*)address) = rgb;
		vramSetBankE(VRAM_E_BG_EXT_PALETTE);

	} else {	// Paletas de la pantalla 1 (VRAM_H)

		vramSetBankH(VRAM_H_LCD);
		address = (0x06898000) + (layer << 13)  + (color << 9);		// Primer color de la paleta
		*((u16*)address) = (u16)0xFF00FF;
		address = (0x06898000) + (layer << 13)  + (color << 9) + 2;	// Segundo color de la paleta
		*((u16*)address) = rgb;
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);

	}

}



// Function PA_SetTextColor();
void PA_SetTextColor(u8 screen, u8 layer, u8 color) {

	PA_TEXT[screen][layer].pal = color;

}
