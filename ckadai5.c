#include "ckadai5.h"

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： main
  引数		： argc	ポインタ配列の個数
               argv	文字列へのダブルポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： コマンドライン引数が2つあるか確認
              C言語課題5の処理
  注意事項	： txtファイルには以下のような形式でbmpファイル名を書き込むこと
              <bmpファイル名>[改行]
              <bmpファイル名>[改行]
              <bmpファイル名>[改行]
              <bmpファイル名>[改行]
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t main(int32_t argc, char **argv)
{
    /* コマンドライン引数が2つあるか確認 */
    if (argc < 3)
    {
        printf("Usage[%s][%d]: ./5-5.exe <txtファイル名> <CSVファイル名>.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    /* C言語課題5の処理 */
    if (Ckadai5(argv[1], argv[2]) != RESULT_OK)
    {
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： Ckadai5
  引数		： txtfilename   char型配列へのポインタ変数
               csvfilename   char型配列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： txtファイルを開く(ここでtxtファイルを開くことでfgets関数を呼び出すごとに1行ずつ次の行を読み込める)
              読み込んだbmpファイル数を標準出力
              txtファイルを閉じる
  注意事項	：
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t Ckadai5(char *txtfilename, char *csvfilename)
{
    FILE *fp = NULL;         /* File構造体へのポインタ変数、初期値NULL */
    int32_t ret = RESULT_OK; /* return値、初期値OK */
    int32_t count_bmp = 0;   /* 読み込んだbmpファイル数、初期値0 */

    /* txtファイルを開く(ここでtxtファイルを開くことでfgets関数を呼び出すごとに1行ずつ次の行を読み込める) */
    fp = fopen(txtfilename, "rb");
    if (fp == NULL)
    {
        printf("ERR[%s][%d]: Failed to open file.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    do
    {
        count_bmp = Ckadai5_sub(fp, csvfilename);
        if (count_bmp == RESULT_ERROR)
        {
            ret = RESULT_ERROR;
            break;
        }

        /* 読み込んだbmpファイル数を標準出力 */
        printf("SUC[%s][%d]: %d bmp files were successfully loaded.\n", __FUNCTION__, __LINE__, count_bmp);

        ret = RESULT_OK;

    } while (0);
    /* txtファイルを閉じる */
    if (fp != NULL)
    {
        if (fclose(fp) == EOF)
        {
            printf("ERR[%s][%d]: Failed to close file.\n", __FUNCTION__, __LINE__);
            ret = RESULT_ERROR;
        }
    }
    else
    {
        printf("ERR[%s][%d]: File pointer has no value.\n", __FUNCTION__, __LINE__);
        ret = RESULT_ERROR;
    }

    return ret;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： Ckadai5_sub
  引数		： fp                   File型構造体へのポインタ変数
               csvfilename          char型配列へのポインタ変数
  戻り値	： count_bmp	読み込んだbmpファイル数
               RESULT_ERROR	        異常終了
  処理概要	： csvファイルの全データを消去(csvファイルを追記モードで開くと前回実行して書き込んだ文字が残るため)
              変数の初期化(前回のforループで使った文字を初期化)
              txtファイルを1行読み込み
              bmpファイルのヘッダーを読み込み
              bmpファイルの4項目(ファイル名、画像の幅、画像の高さ、ピクセルあたりのビット数)を抽出
              csvファイルに最後尾から書き込み
  注意事項	：
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t Ckadai5_sub(FILE *fp, char *csvfilename)
{
    char bmpfilename[MAX_STR_SIZ] = {0};         /* bmpファイル名を格納する配列、初期値{0} */
    BITMAP_HEADER bmpheader = {0};               /* bmpファイルのヘッダーを格納する構造体変数、初期値{0} */
    BITMAP_ITEM bmpitem = {0};                   /* bmpファイルのヘッダーの4項目を格納する構造体変数、初期値{0} */
    char bmpitemlist[MAX_BMPITEMLIST_SIZ] = {0}; /* bmpファイルのヘッダーの4項目を結合して格納する配列、初期値{0} */
    int32_t count_bmp = 0;                       /* 読み込んだbmpファイル数、初期値0 */

    /* csvファイルの全データを消去(csvファイルを追記モードで開くと前回実行して書き込んだ文字が残るため) */
    if (DeleteCsvfile(csvfilename) != RESULT_OK)
    {
        return RESULT_ERROR;
    }

    /* bmpファイルを一つずつ読み込んでcsvファイルに書き込む処理を繰り返す */
    while (1)
    {
        /* 変数の初期化(前回のforループで使った文字を初期化) */
        memset(bmpfilename, 0, MAX_STR_SIZ);
        memset(&bmpheader, 0, sizeof(BITMAP_HEADER));
        memset(&bmpitem, 0, sizeof(BITMAP_ITEM));
        memset(bmpitemlist, 0, MAX_BMPITEMLIST_SIZ);

        /* txtファイルを1行読み込み */
        if (ReadTxtfile_1line(fp, bmpfilename) != RESULT_OK)
        {
            break;
        }

        /* bmpファイルのヘッダーを読み込み */
        if (ReadBmpfile_header(bmpfilename, &bmpheader) != RESULT_OK)
        {
            break;
        }

        /* bmpファイルの4項目(ファイル名、画像の幅、画像の高さ、ピクセルあたりのビット数)を抽出 */
        if (ExtractBmpfile_4item(bmpfilename, &bmpheader, &bmpitem) != RESULT_OK)
        {
            break;
        }

        /* bmpファイルの4項目を結合 */
        if (CombineBmpfile_4item(&bmpitem, bmpitemlist) != RESULT_OK)
        {
            break;
        }

        /* csvファイルに最後尾から書き込み */
        if (WriteCsvfile_lasttail(csvfilename, bmpitemlist) != RESULT_OK)
        {
            break;
        }

        ++count_bmp;
    }

    /* 戻り値にcount_bmpを返す */
    return count_bmp;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： DeleteCsvfile
  引数		： csvfilename     　　char型配列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： csvファイルの全データを消去(csvファイルを追記モードで開くと前回実行して書き込んだ文字が残るため)
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t DeleteCsvfile(char *csvfilename)
{
    FILE *fp = NULL;         /* File構造体へのポインタ変数、初期値NULL */
    int32_t ret = RESULT_OK; /* return値、初期値OK */

    /* csvファイルを書き込みモードで開く(既に指定ファイルがある場合は長さ0に切り捨て) */
    fp = fopen(csvfilename, "wb");
    if (fp == NULL)
    {
        printf("ERR[%s][%d]: Failed to open outfile.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    /* csvファイルを閉じる */
    if (fp != NULL)
    {
        if (fclose(fp) == EOF)
        {
            printf("ERR[%s][%d]: Failed to close outputfile.\n", __FUNCTION__, __LINE__);
            return RESULT_ERROR;
        }
    }
    else
    {
        printf("ERR[%s][%d]: Outputfile pointer has no value.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： ReadTxtfile_1line
  引数		： fp	             File型構造体へのポインタ変数
               line				 char型配列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
              RESULT_ERROR	異常終了
  処理概要	： txtファイルを1行読み込み
               改行をNULL文字で消去
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t ReadTxtfile_1line(FILE *fp, char *line)
{
    /* txtファイルを読み込み */
    if ((fgets(line, MAX_STR_SIZ, fp)) == NULL)
    {
        printf("INFO[%s][%d]: The text does not exist.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    /* 最後の文字が改行か確認 */
    if (line[strnlen(line, MAX_STR_SIZ) - 1] == '\n')
    {
        /* 改行をNULL文字で消去 */
        line[strnlen(line, MAX_STR_SIZ) - 1] = 0;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： ReadBmpfile_header
  引数		： filename     char型配列へのポインタ変数
               bmpheader    BITMAP_HEADER型構造体へのポインタ変数
  戻り値	： RESULT_OK	 正常終了
               RESULT_ERROR	異常終了
  処理概要	： bmpファイルのヘッダーを読み込み
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t ReadBmpfile_header(char *filename, BITMAP_HEADER *bmpheader)
{
    FILE *fp = NULL;         /* File構造体へのポインタ変数、初期値NULL */
    int32_t ret = RESULT_OK; /* return値、初期値OK */

    /* bmpファイルを開ける */
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("ERR[%s][%d]: Failed to open bmpfile(%s).\n", __FUNCTION__, __LINE__, filename);
        return RESULT_ERROR;
    }

    do
    {
        /* bmpファイルのヘッダーを読み込み */
        if (ReadBmpfile_header_sub(filename, bmpheader, fp) != RESULT_OK)
        {
            ret = RESULT_ERROR;
            break;
        }

        ret = RESULT_OK;
    } while (0);
    /* bmpファイルを閉じる */
    if (fp != NULL)
    {
        if (fclose(fp) == EOF)
        {
            printf("ERR[%s][%d]: Failed to close inputfile.\n", __FUNCTION__, __LINE__);
            ret = RESULT_ERROR;
        }
    }
    else
    {
        printf("ERR[%s][%d]: Inputfile(%s) pointer has no value.\n", __FUNCTION__, __LINE__);
        ret = RESULT_ERROR;
    }

    return ret;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： ReadBmpfile_header_sub
  引数		： filename     char型配列へのポインタ変数
               pb           BITMAP_HEADER型構造体へのポインタ変数
               fp           File型構造体へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
               MISMATCH 不一致終了
  処理概要	： bmpファイルのヘッダーを読み込み
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t ReadBmpfile_header_sub(char *filename, BITMAP_HEADER *bmpheader, FILE *fp)
{
    char extension[EXTENSION_BMP_SIZ] = ""; /* ファイルの拡張子を格納する配列、初期値0 */

    /* 拡張子を取り出す */
    if (GetExtension_4char(filename, extension) == RESULT_ERROR)
    {
        return RESULT_ERROR;
    }

    /* 拡張子がbmpファイルか確認 */
    if (strncmp(extension, EXTENSION_BMP, EXTENSION_BMP_SIZ) != 0)
    {
        printf("ERR[%s][%d]: Inputile is not bmpfile.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    /* errnoの初期化 */
    errno = 0;

    /* bmpファイルのヘッダーを読み込み */
    fread(bmpheader, sizeof(BITMAP_HEADER), 1, fp);

    /* もしfwriteでエラーが発生すればerrnoの値が変化 */
    if (errno != 0)
    {
        printf("ERR[%s][%d]: Failed to write to the outputfile.(errno = %d)\n", __FUNCTION__, __LINE__, errno);
        return RESULT_ERROR;
    }

    /* ファイルタイプがBMか確認 */
    if (!IsBmpfile(bmpheader))
    {
        return RESULT_MISMATCH;
    }

    /* ファイルサイズが54byteより大きいか確認 */
    if (CheckFilesize_over54byte(filename) != RESULT_OK)
    {
        return RESULT_MISMATCH;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： ExtractBmpfile_4item
  引数		： bmpfilename         char型配列へのポインタ変数
               bmpheader           BITMAP_HEADER型構造体へのポインタ変数
               bmpitem             BITMAP_ITEM型構造体へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： bmpファイルの4項目(ファイル名、画像の幅、画像の高さ、ピクセルあたりのビット数)を抽出
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t ExtractBmpfile_4item(char *bmpfilename, BITMAP_HEADER *bmpheader, BITMAP_ITEM *bmpitem)
{
    /* bmpファイルの4項目(ファイル名、画像の幅、画像の高さ、ピクセルあたりのビット数)を抽出　*/
    strncpy(bmpitem->filename, bmpfilename, strnlen(bmpfilename, MAX_STR_SIZ));
    bmpitem->width = bmpheader->BITMAPINFOHEADER.biWidth;
    bmpitem->height = bmpheader->BITMAPINFOHEADER.biHeight;
    bmpitem->bitCount = bmpheader->BITMAPINFOHEADER.biBitCount;

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： CombineBmpfile_4item
  引数		： bmpitem             BITMAP_ITEM型構造体へのポインタ変数
               bmpitemlist         char型配列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： bmpファイルの4項目を結合
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t CombineBmpfile_4item(BITMAP_ITEM *bmpitem, char *bmpitemlist)
{
    /* bmpファイルの4項目を結合 */
    if (snprintf(bmpitemlist, MAX_BMPITEMLIST_SIZ, "%s,%d,%d,%d\n", bmpitem->filename, bmpitem->width, bmpitem->height, bmpitem->bitCount) < 0)
    {
        printf("ERR[%s][%d]: Failed to combine 4items of bmpfile.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： WriteCsvfile_lasttail
  引数		： csvfilename     　　char型配列へのポインタ変数
               bmpitemlist     　　char型配列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： csvファイルに最後尾から書き込み
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t WriteCsvfile_lasttail(char *csvfilename, char *bmpitemlist)
{
    FILE *fp = NULL;         /* File構造体へのポインタ変数、初期値NULL */
    int32_t ret = RESULT_OK; /* return値、初期値OK */

    /* csvファイルを追記モードで開く */
    fp = fopen(csvfilename, "ab");
    if (fp == NULL)
    {
        printf("ERR[%s][%d]: Failed to open outfile.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    do
    {
        /* csvファイルに最後尾から書き込み */
        if (WriteCsvfile_lasttail_sub(bmpitemlist, fp) != RESULT_OK)
        {
            ret = RESULT_ERROR;
            break;
        }

        ret = RESULT_OK;
    } while (0);
    /* csvファイルを閉じる */
    if (fp != NULL)
    {
        if (fclose(fp) == EOF)
        {
            printf("ERR[%s][%d]: Failed to close outputfile.\n", __FUNCTION__, __LINE__);
            return RESULT_ERROR;
        }
    }
    else
    {
        printf("ERR[%s][%d]: Outputfile pointer has no value.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(5)
  関数名	： WriteCsvfile_lasttail_sub
  引数		： csvfilename     　　char型配列へのポインタ変数
               bmpitemlist     　　char型配列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： csvファイルに最後尾から書き込み
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/12/15
  変更履歴	：
*******************************************************************************/
int32_t WriteCsvfile_lasttail_sub(char *bmpitemlist, FILE *fp)
{
    /* errnoの初期化 */
    errno = 0;

    /* csvファイルに最後尾から書き込み */
    fwrite(bmpitemlist, sizeof(char), strnlen(bmpitemlist, MAX_BMPITEMLIST_SIZ), fp);

    /* もしfwriteでエラーが発生すればerrnoの値が変化 */
    if (errno != 0)
    {
        printf("ERR[%s][%d]: Failed to write to the outputfile.(errno = %d)\n", __FUNCTION__, __LINE__, errno);
        return RESULT_ERROR;
    }

    return RESULT_OK;
}
