#ifndef _H_CKADAI5SUB_
#define _H_CKADAI5SUB_


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define RESULT_OK 0
#define RESULT_ERROR -1
#define RESULT_MISMATCH -2
#define FILESIZE_INVALID -1
#define BM 0x4D42
#define EXTENSION_BMP ".bmp"
#define EXTENSION_BMP_SIZ sizeof(EXTENSION_BMP)
#define NEWLINE_SIZ 1
#define COMMA_SIZ 1
#define MAX_STR_SIZ 256
#define MAX_4byte_char_SIZ 11
#define MAX_2byte_char_SIZ 6
#define MAX_BMPITEMLIST_SIZ MAX_STR_SIZ + MAX_4byte_char_SIZ * 2 + MAX_2byte_char_SIZ + COMMA_SIZ * 3 + NEWLINE_SIZ

/* BMPファイル構造体 */
#pragma pack(push, 1) // パディングを１byteに変更

typedef struct
{
    unsigned short bfType;      /* ファイルタイプ */
    unsigned long bfSize;       /* ファイルサイズ */
    unsigned short bfReserved1; /* 予約領域１ */
    unsigned short bfReserved2; /* 予約領域２ */
    unsigned long bfOffBits;    /* 先頭から画像データまでのオフセット */
} BITMAPFILEHEADER;

typedef struct
{
    unsigned long biSize;         /* ヘッダサイズ */
    long biWidth;                 /* 画像の幅(px) */
    long biHeight;                /* 画像の高さ(px) */
    unsigned short biPlanes;      /* プレーン数 常に１ */
    unsigned short biBitCount;    /* １画素あたりのデータサイズ */
    unsigned long biCompression;  /* 圧縮形式 */
    unsigned long biSizeImage;    /* 画像データ部のサイズ */
    long biXPixPerMeter;          /* 横方向解像度(dot/m) */
    long biYPixPerMeter;          /* 縦方向解像度(dot/m) */
    unsigned long biClrUsed;      /* 格納されているパレット数(使用色数) */
    unsigned long biClrImportant; /* 重要なパレットのインデックス */
} BITMAPINFOHEADER;

typedef struct
{
    BITMAPFILEHEADER BITMAPFILEHEADER;
    BITMAPINFOHEADER BITMAPINFOHEADER;
} BITMAP_HEADER;

#pragma pack(pop)

/* プロトタイプ宣言 */
int32_t GetExtension_4char(char *filename, char *extension);
int32_t IsBmpfile(BITMAP_HEADER *pb);
int32_t CheckFilesize_over54byte(char *filename);
int32_t GetFilesize(char *filename);

#endif
