#ifndef _H_CKADAI5_
#define _H_CKADAI5_


#include "ckadai5sub.h"

/* BMPファイル構造体 */
#pragma pack(push, 1) // パディングを１byteに変更

typedef struct
{
    char filename[MAX_STR_SIZ];
    int32_t width;
    int32_t height;
    int16_t bitCount;
} BITMAP_ITEM;

#pragma pack(pop)

/* プロトタイプ宣言 */
int32_t Ckadai5(char *txtfilename, char *csvfilename);
int32_t Ckadai5_sub(FILE *fp, char *csvfilename);
int32_t ReadTxtfile_1line(FILE *fp, char *line);
int32_t ReadBmpfile_header(char *filename, BITMAP_HEADER *bmpheader);
int32_t ReadBmpfile_header_sub(char *filename, BITMAP_HEADER *bmpheader, FILE *fp);
int32_t DeleteCsvfile(char *csvfilename);
int32_t ExtractBmpfile_4item(char *bmpfilename, BITMAP_HEADER *bmpheader, BITMAP_ITEM *bmpitem);
int32_t CombineBmpfile_4item(BITMAP_ITEM *bmpitem, char *bmpitemlist);
int32_t WriteCsvfile_lasttail(char *csvfilename, char *bmpitemlist);
int32_t WriteCsvfile_lasttail_sub(char *bmpitemlist, FILE *fp);

#endif
