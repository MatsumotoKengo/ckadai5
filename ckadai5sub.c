#include "ckadai5sub.h"

/*******************************************************************************
  モジュール名 ： C言語課題(4)
  関数名	： GetExtension_4char
  引数		： filename		ファイル名の文字列へのポインタ変数
               extension	ファイルの拡張子へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
  処理概要	： 拡張子を取り出す
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/11/29
  変更履歴	：
*******************************************************************************/
int32_t GetExtension_4char(char *filename, char *extension)
{
    /* ファイル名が4文字以上か確認 */
    if (strnlen(filename, MAX_STR_SIZ - 1) < strnlen(EXTENSION_BMP, EXTENSION_BMP_SIZ - 1))
    {
        printf("ERR[%s][%d]: The name of the file does not have enough characters.\n", __FUNCTION__, __LINE__);
        return RESULT_ERROR;
    }

    /* ポインタを末尾まで移動 */
    while (*filename != '\0')
    {
        filename++;
    }

    /* 拡張子を格納 */
    for (int8_t i = (EXTENSION_BMP_SIZ - 1); i >= 0; i--)
    {
        extension[i] = *filename;
        filename--;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(4)
  関数名	： IsBmpfile
  引数		： pb           BITMAP_HEADER型へのポインタ変数
  戻り値	： true	真
               false 偽
  処理概要	： ファイルタイプがBMか確認
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/11/24
  変更履歴	：
*******************************************************************************/
int32_t IsBmpfile(BITMAP_HEADER *pb)
{
    /* ファイルタイプがBMか確認 */
    if ((pb->BITMAPFILEHEADER.bfType != BM))
    {
        printf("ERR[%s][%d]: Inputile is not bmpfile.\n", __FUNCTION__, __LINE__);
        return false;
    }

    // printf("true = %d\n",true);
    return true;
}

/*******************************************************************************
  モジュール名 ： C言語課題(4)
  関数名	： CheckFilesize_over54byte
  引数		： filename     ファイル名の文字列へのポインタ変数
  戻り値	： RESULT_OK	正常終了
               RESULT_ERROR	異常終了
               RESULT_MISMATCH 不一致終了
  処理概要	： ファイルサイズが54byteより大きいか確認
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/11/24
  変更履歴	：
*******************************************************************************/
int32_t CheckFilesize_over54byte(char *filename)
{
    int32_t filesize = 0; /* ファイルのサイズを格納する変数、初期値0 */
    int32_t minsize = 0;  /* bmp構造体のサイズを格納する変数、初期値0 */

    /* ファイルサイズを取り出す */
    filesize = GetFilesize(filename);
    if (filesize == FILESIZE_INVALID)
    {
        return RESULT_ERROR;
    }

    /* ファイルサイズが54byteより大きいか確認 */
    minsize = sizeof(BITMAP_HEADER);
    if (filesize < minsize)
    {
        printf("ERR[%s][%d]: Filesize is under 54byte.\n", __FUNCTION__, __LINE__);
        return RESULT_MISMATCH;
    }

    return RESULT_OK;
}

/*******************************************************************************
  モジュール名 ： C言語課題(2)
  関数名	： GetFilesize
  引数		： filename	ファイル名の文字列へのポインタ変数
  戻り値	： filesize	ファイルの文字数
               FILESIZE_INVALID	異常終了
  処理概要	： ファイルサイズを取り出す
  注意事項	： なし
  作成者	： 松本健吾
  作成日	： 2022/11/02
  変更履歴	：
*******************************************************************************/
int32_t GetFilesize(char *filename)
{
    FILE *fp = NULL;      /* File構造体へのポインタ変数、初期値NULL */
    int32_t filesize = 0; /* ファイルの文字数を格納する変数、初期値0 */
    int32_t ret = 0;      /* return値、初期値OK */

    /* ファイルを開く */
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("ERR[%s][%d]: Failed to open inputfile.\n", __FUNCTION__, __LINE__);
        return FILESIZE_INVALID;
    }

    do
    {
        /* ファイルポインタを末尾まで移動 */
        if (fseek(fp, 0, SEEK_END) != 0)
        {
            printf("ERR[%s][%d]: Failed to move file pointer to end.\n", __FUNCTION__, __LINE__);
            ret = FILESIZE_INVALID;
            break;
        }

        /* ファイルサイズを取り出す */
        filesize = ftell(fp);
        if (filesize == -1)
        {
            printf("ERR[%s][%d]: Failed to extract input filesize.\n", __FUNCTION__, __LINE__);
            ret = FILESIZE_INVALID;
            break;
        }

        /* ポインタを先頭まで移動 */
        if (fseek(fp, 0, SEEK_SET) != 0)
        {
            printf("ERR[%s][%d]: Failed to move file pointer to beginning.\n", __FUNCTION__, __LINE__);
            ret = FILESIZE_INVALID;
            break;
        }

        ret = filesize;
    } while (0);
    /* ファイルを閉じる */
    if (fp != NULL)
    {
        if (fclose(fp) == EOF)
        {
            printf("ERR[%s][%d]: Failed to close inputfile.\n", __FUNCTION__, __LINE__);
            ret = FILESIZE_INVALID;
        }
    }
    else
    {
        printf("ERR[%s][%d]: File pointer has no value.\n", __FUNCTION__, __LINE__);
        ret = FILESIZE_INVALID;
    }

    return ret;
}
