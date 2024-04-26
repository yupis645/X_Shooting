/**
* @file	conioex.h
* @brief	Win32 コンソール I/O 拡張 for Visual C++/Borland C++
*
* @date	2019/10/15 2019年度初版
* @version	1.0
*		2022/10/01
*			グローバル変数の名前の先頭には'g_'を付加するように変更した。
*			「拡張文字列処理(マルチバイト/Unicode対応)」追加
*				半角⇒全角変換関数（HanToZen）追加。
*				書式指定で全角文字出力する関数(PrintStringF)の追加。
* @version	1.2
*		2022/11/01
*			ジョイパッド入力関数のInputJoystickXについて以下の様に更新した。
*				廃止：int InputJoystickX(int port)
*				新規：int InputJoystickX(int id,int port)
*			InitConio()の中でカーソルＯＦＦにした。
*			全キー読み取り関数GetKeyAll()追加。
*				これに伴いChkKeyPress(),ChkKeyEdge()を追加
* @version	1.4
*		2022/11/30
*			"BmpUTL"および"Bmp"取り込み。
*
* @version	1.6
*		2023/01/11
*			DrawBmpでスクリーンバッファに書き込んでいたが、毎回スクリーンへの転送がかかると処理が遅いので、
*			DrawBmpではフレームバッファに書いておいて、PrintFrameBufferでまとめて転送するように変更。
*
* @version	1.6.1
*		2023/01/19
*			図形描画の実装：DDAを使った線分や円の描画
*			背景色の指定：フレームバッファを初期化する値を指定出来る様に修正＆追記
*		2032/02/05
*			反転と９０単位の回転の実装：DrawBmp()に水平・垂直・回転（90度単位）の機能を追加した
*		2032/02/06
*			図形描画関数の名前と仕様を全面的に変更
*			DDA_Line等の名前で実装していた関数をすべて削除し、ShapeUTLとして外部関数だったものを取り込んだ。
*
* @version	1.7.0
*		2023/02/06
*			マップチップ画像の取り込み関数を実装した⇒LoadDivBmp()
*			画像の矩形転送を実装した⇒CreateFromBmp()/CopyBmp()
*
* @version	2.0.a
*		2023/02/12
*			描画のベースをDirect2Dに置き換え
*
* @version	2.0.b
*		2023/02/15
*			SetPalette()のバグを修正
*				⇒RGB値のRed(赤)しか転送していなかったのを修正した。
*			ClearScreenの追加
*				⇒パレットに縛られなくなってので、クリアする色はデフォルトパレットの０番固定になってしまっていた。
*				⇒ClearScree()の引数で、RGB値またはデフォルトパレットの番号で指定できるようにした。
*					ClearScreen(int _cc)/ClearScreen(int _red, int _green, int _blue)
*			図形描画の色指定に"RGBQUAD _rgb"RGB値の引数バージョンを追加
*				例）DrawLine(0,0,RGBQUAD{255,255,255})
*			WriteText()にConsoleAPIの時の文字サイズ１：２の「互換モード」を追加した。
*				⇒PrintString()関数を呼び出すと必ず「互換モード」になり、FilpScreen()で自動的に解消される。
*				⇒これにより、ピクセルスケールが1x1の時、文字の最小サイズは1×２となる。
*				⇒但しWriteText()も影響受けるのでPrintString()とWriteText()は混在して使わない方が良い。
*
* @version	2.0.c
*		2023/02/20
*			WriteTextの最適化
*				⇒レンダーターゲットビットマップを描画毎に生成～削除していたので、
*				InitD2D()で生成～EndD2D()削除に変更した。
*			フォント指定の有効化
*				SetScreenFontSize()でフォントサイズを設定していなかった。
*				⇒WriteText()でフォント指定が無い場合に有効になるようにした。
*			Bmpの描画に透過色指定の追加
*				DrawBmp()で画像の透過色を有効にする時、透過色を指定できるようにした。
*				⇒１～８ビット画像の場合はパレット番号を指定。
*				⇒２４～３２ビット画像の場合はRGB値を指定。
*			FlipScreen()をRenderScreen()に改名
*				描画リストの画面への描画処理の関数名をFlipScreen()からRenderScreen()に変更した。
*				FlipScreen()はダブルバッファを切替える処理の名前なので互換性の為名前だけ残した。
*
* @version	2.1.0
*		2023/02/27
*			アルファブレンドモードの実装：
*				関数AlphaBlendMode(true)で呼び出すと、DrawBmp()で描画する画像に
*				アルファチャンネル（透明度レイヤー）があれば、半透明などの表示が出来る。
*				※表示内容にもよるが非常に処理が重いので、使用する状況を考えて使う方が良い。
*			アルファブレンドモードの透過率（不透明度）の実装：
*				SetTrLevel()で０～１００％で透明度合いを設定できる。０が透明１００％が不透明
*				※表示内容にもよるが非常に処理が重いので、使用する状況を考えて使う方が良い。
*			WIC(Windows Imaging Component)による画像ファイルの読込
*				WINを使った画像の読込の実装：
*				⇒LoadBmpFromImgFile()でpng/jpeg/bmp/等が読み込める。（WICでサポートされている画像形式）
*				⇒圧縮されている画像や、パレット付の画像も読み込める。
*				⇒読み込んだ画像は全て３２ビット画像になる。
*				⇒読み込んだ画像は全てBmp画像のポインタ（Bmp＊）で生成されるので、
*				　これまでのBmp画像を扱う関数で利用できる。
*			DrawTriangle()の実装：
*				図形描画に三角形の描画を追加した。
*			Releaseモードの時FrameSync()でFPSの計算
*				Releaseモードでビルドした時も、FPSの計算をするように修正した
*				⇒extern 宣言してあるので外部から参照可能
*				⇒g_FrameCalcFPS ------- １秒間のフレーム回数（FPS)(浮動小数点) ⇒ Debugビルドだと１秒間の平均値
*				⇒g_FrameCountPerSec --- １秒間のフレーム回数（整数）⇒１秒ごとに０リセットされる
*				⇒g_FrameCounter ------- フレームカウンタ（整数）
*			WriteString()の修正：
*				フォントのサイズ設定SetScreenFontSize()を呼び出してもフォントサイズが変化しない場合があったので修正した。
*			InputKeyMouse()の入力が効かなくなっていたので修正した
*				"g_OrgInputHandle"とすべきところが、間違って"g_OrgOutputHandle"になっていたのを修正した
*			void PrintFrameBuffer(const char* _frame_buffer)が無くなっていたので追加した
*				画面と同じ大きさのフレームバッファを指定して画面に転送する
*				転送するフレームバッファのフォーマットは、タテヨコともに画面と同じピクセル数で
*				画像はパレットパレットインデックスの並びになっているもの。
*
* @version	2.2.0
*		2023/03/12
*			Bmp画像描画時の回転・拡大・縮小の実装：
*			DrawBmpEx()を追加した。
*
* @version	2.2.1
*		2023/03/15
*			WriteText()のバージョンアップ：
*				⇒文字サイズの幅と高さの設定"SetScreenFontSize()"を呼び出しても、
*				　Ｘ（幅)・Ｙ（高さ)の比率が１：１のまま（幅も高さもＹに指定したサイズ）だったのを、
*				　Ｘ（幅)・Ｙ（高さ)別々にサイズが変わる様に修正した。
*				⇒フォントの描画サイズを指定している部分で、幅指定の部分に比率（Ｘ÷Ｙ）を掛ける様に修正した。
*			文字描画のドットと画像のドット表示が１：１になるように修正：
*				⇒これまで文字描画はウィンドウ上の１ピクセルに合わせて描画していたので
*				　画像の１ドットとはドットの大きさが違っていたので文字描画の時も画像描画のドットと同じスケールで描画するように修正した
*
* @version	2.3.0
*		2023/03/18
*			XAudio2を使ったサウンドAPIの追加：
*			⇒MCIサウンドでmp3再生時にタイムラグが大きくなる場合があったので、
*			　処理の軽そうなXAudio2のAPIを利用したSndサウンドAPIを新設した。
*			⇒サウンド用オブジェクト"Snd"を使って初期化や再生・停止を行う～Sndの名前のAPIを実装。
*
* @version	2.4.0
*		2023/06/22
*			InitConioEx()の多重定義の内、冗長なものを消した。
*			SettePalette()の引数名_pal16を_pal256に修正した（実際２５６色パレットなので）
*			WriteTextA/W()の冗長部分を見直した。多重定義を少なくした。
*			PrintFrameBuffer()に外部のバッファ（配列）を取り込む機能を追加した。
*
* @version	2.4.1
*		2023/06/26
*			DWRGB()を追加した
*
* @version	2.4.2
*		2023/07/10
*			LoadDivImgFileA()を追加した
*
* @version	2.4.5
*		2023/07/14
*			BMP形式以外の画像読み込み対応
*			LoadBmpExA/W
*
* @version	2.4.6
*		2023/07/19
*			BMP形式以外の画像読み込み対応
*			LoadBmp()
*
* @note
*  コンソールウィンドウのプロパティの設定を以下に変更すること
*  ・「従来のコンソールを使う」のチェックを外す
*  ・「簡易編集モード」のチェックを外す
*
* コンソール関数について
* https://learn.microsoft.com/ja-jp/windows/console/console-functions
*/
/**
<関数の説明>
@param <引数名> <引数の説明>
@return <返り値の説明>
@note <注意点>
@exception <例外クラス名> <例外が発生するときの説明>
*/
/**
* @fn
* ここに関数の説明を書く
* @brief 要約説明
* @param (引数名) 引数の説明
* @param (引数名) 引数の説明
* @return 戻り値の説明
* @sa 参照すべき関数を書けばリンクが貼れる
* @detail 詳細な説明
*/
#include "conioex.h"
#include <vector>
#include <list>

using namespace Microsoft::WRL;

#define CONSOLE_INPUT_MODE	(ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT)
//コンソール出力でANSI-256-colorやフルカラーを扱うために'ENABLE_VIRTUAL_TERMINAL_PROCESSING'追加
#define CONSOLE_OUTPUT_MODE	(ENABLE_PROCESSED_OUTPUT | ENABLE_LVB_GRID_WORLDWIDE | ENABLE_VIRTUAL_TERMINAL_PROCESSING)

//----------------------------------------------------------------
// ビットマップファイル操作用
#define	NUM_BMP1_PALETTE	(2)		//1ビットカラー画像のパレットは２色
#define	NUM_BMP4_PALETTE	(16)	//4ビットカラー画像のパレットは１６色
#define	NUM_BMP8_PALETTE	(256)	//8ビットカラー画像のパレットは２５６色

//MCIサウンド用構造体
typedef struct {
	int				device_type;
	MCIDEVICEID		device_id;
	char			path[MAX_PATH];
	int				repeat;
} MciSoundInfo;

//コンソールウィンドウのハンドル保存用変数
static HWND g_hConWnd = NULL;

//Original.
static HANDLE	g_OrgOutputHandle = NULL;	//InitConio直前のハンドル
static DWORD	g_OrgOutputHandleMode = 0;
static HANDLE	g_OrgInputHandle = NULL;
static DWORD	g_OrgInputHandleMode = 0;
static CONSOLE_CURSOR_INFO	g_OrgCursorInfo = {};	//オリジナルのカーソル情報
static LONG_PTR	g_OrgWindowStylePtr = 0;	//元のウィンドウスタイル
static CONSOLE_SCREEN_BUFFER_INFOEX g_OrgScreenBufferInfoEx = { sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };	//スクリーンバッファ
static CONSOLE_FONT_INFOEX g_OrgFontSizeEx = { sizeof(CONSOLE_FONT_INFOEX) };	//フォント設定
static COLORREF	g_OrgColorTableD2D[NUM_D2D_PAL] = {};	//Conioexが起動したときのパレット

//キー入力関係
static DWORD	g_ConioKeyMap[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static COORD	g_ConioMousePosition = { 0, 0 };
//キー入力バッファ
static int g_KeyPress[NUM_KEYS] = {};	//プレス
static int g_KeyEdge[NUM_KEYS] = {};	//エッジ
static int g_KeyLast[NUM_KEYS] = {};	//TEMP

/*
* コンソールのクライアント領域のサイズと、フォントサイズ。{left=w,top=h,right=fw,bottom=fh}
* left：クライアント領域の幅, top：クライアント領域の高さ, right：フォントの幅, bottom：フォントの高さ
*/
static RECT g_ConWinSize = {};

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
//================================================================
//	Direct2D/DirectWrite用の変数とプロトタイプ宣言
//================================================================
ID2D1Factory* g_pD2DFactory = NULL;		//D2Dファクトリー
IDWriteFactory* g_pDWFactory = NULL;	//テキスト出力用DirectWriteファクトリー
//▼描画ターゲット
ID2D1HwndRenderTarget* g_pRenderTarget = NULL;	//ウィンドウに描画する為のレンダーターゲット
//画像描画用ビットマップレンダーターゲット
//ID2D1Bitmap* g_ptrTargetBitmap = NULL;
//テキスト描画用ビットマップレンダーターゲット
ID2D1BitmapRenderTarget* g_ptrBRTforText = NULL;
//▼描画リスト
std::vector<ID2D1Bitmap*>	g_pBmpList;		//画像用（ビットマップ描画リスト）

D2D1_SIZE_F	g_Scale = { 0.0,0.0 };	//画面のスケール（ピクセル/１ドット）１ドットの倍率（画面の１ドットを何ピクセルで表示するかの値）

//フレームバッファ
static COORD	g_FrameBufferSizeD2D = { 0,0 };	//フレームバッファのサイズ
RGBQUAD* g_FrameBuffer32bitD2D = NULL;	//フルカラー用フレームバッファ

//パレットテーブル
RGBQUAD	g_PaletteD2D[NUM_ANSI_PAL] = {};	//256色パレット：COLORREF:0x00BBGGRR->{R8,G8,B8,X}/RGBQUAD:{B8,G8,R8,A8}->0xAARRGGBB

//フォント関係
//static CONSOLE_FONT_INFOEX g_FontSizeExD2D = { sizeof(CONSOLE_FONT_INFOEX) };
COORD	g_CursorPosD2D = { 0,0 };		//文字表示開始位置（カーソル位置）
bool g_PrintStringCompatibleMode = false;	//true=ConsoleAPIの時の１：２の文字サイズ

/*
* 文字列描画用の変数
*/
static bool g_TextHDMode = false;	//文字の高画質モード：{true:ON|false:OFF}
static D2D1_TEXT_ANTIALIAS_MODE g_TextAntiAlias = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;	//文字描画のアンチエイリアス（デフォルトはアンチエイリアス無し）
static bool g_ZenkakuFlag = false;	//全角表示のＯＮ／ＯＦＦ
static FLOAT g_FontWidth = 0.0;
static FLOAT g_FontHeight = 0.0;
static WCHAR g_FontFaceName[LF_FACESIZE * 2];

//prototype
bool InitD2D(int _w, int _h);
void EndD2D(void);

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
//================================================================
//　２５６色パレット
// パレット：
//	最初の１６色が通常の１６色に対応する。
//	１７色～２５６色が追加の色になる。
//================================================================
/**
* @brief	ANSI color 256.\n
* COLORREF : #00BBGGRR
*/
const COLORREF ANSI_PAL256_COLOR[NUM_ANSI_PAL] = {
0x000000,0x800000,0x008000,0x808000,0x000080,0x800080,0x008080,0xC0C0C0,0x808080,0xFF0000,0x00FF00,0xFFFF00,0x0000FF,0xFF00FF,0x00FFFF,0xFFFFFF,
0x000000,0x00005F,0x000087,0x0000AF,0x0000D7,0x0000FF,0x005F00,0x005F5F,0x005F87,0x005FAF,0x005FD7,0x005FFF,0x008700,0x00875F,0x008787,0x0087AF,
0x0087D7,0x0087FF,0x00AF00,0x00AF5F,0x00AF87,0x00AFAF,0x00AFD7,0x00AFFF,0x00D700,0x00D75F,0x00D787,0x00D7AF,0x00D7D7,0x00D7FF,0x00FF00,0x00FF5F,
0x00FF87,0x00FFAF,0x00FFD7,0x00FFFF,0x5F0000,0x5F005F,0x5F0087,0x5F00AF,0x5F00D7,0x5F00FF,0x5F5F00,0x5F5F5F,0x5F5F87,0x5F5FAF,0x5F5FD7,0x5F5FFF,
0x5F8700,0x5F875F,0x5F8787,0x5F87AF,0x5F87D7,0x5F87FF,0x5FAF00,0x5FAF5F,0x5FAF87,0x5FAFAF,0x5FAFD7,0x5FAFFF,0x5FD700,0x5FD75F,0x5FD787,0x5FD7AF,
0x5FD7D7,0x5FD7FF,0x5FFF00,0x5FFF5F,0x5FFF87,0x5FFFAF,0x5FFFD7,0x5FFFFF,0x870000,0x87005F,0x870087,0x8700AF,0x8700D7,0x8700FF,0x875F00,0x875F5F,
0x875F87,0x875FAF,0x875FD7,0x875FFF,0x878700,0x87875F,0x878787,0x8787AF,0x8787D7,0x8787FF,0x87AF00,0x87AF5F,0x87AF87,0x87AFAF,0x87AFD7,0x87AFFF,
0x87D700,0x87D75F,0x87D787,0x87D7AF,0x87D7D7,0x87D7FF,0x87FF00,0x87FF5F,0x87FF87,0x87FFAF,0x87FFD7,0x87FFFF,0xAF0000,0xAF005F,0xAF0087,0xAF00AF,
0xAF00D7,0xAF00FF,0xAF5F00,0xAF5F5F,0xAF5F87,0xAF5FAF,0xAF5FD7,0xAF5FFF,0xAF8700,0xAF875F,0xAF8787,0xAF87AF,0xAF87D7,0xAF87FF,0xAFAF00,0xAFAF5F,
0xAFAF87,0xAFAFAF,0xAFAFD7,0xAFAFFF,0xAFD700,0xAFD75F,0xAFD787,0xAFD7AF,0xAFD7D7,0xAFD7FF,0xAFFF00,0xAFFF5F,0xAFFF87,0xAFFFAF,0xAFFFD7,0xAFFFFF,
0xD70000,0xD7005F,0xD70087,0xD700AF,0xD700D7,0xD700FF,0xD75F00,0xD75F5F,0xD75F87,0xD75FAF,0xD75FD7,0xD75FFF,0xD78700,0xD7875F,0xD78787,0xD787AF,
0xD787D7,0xD787FF,0xD7AF00,0xD7AF5F,0xD7AF87,0xD7AFAF,0xD7AFD7,0xD7AFFF,0xD7D700,0xD7D75F,0xD7D787,0xD7D7AF,0xD7D7D7,0xD7D7FF,0xD7FF00,0xD7FF5F,
0xD7FF87,0xD7FFAF,0xD7FFD7,0xD7FFFF,0xFF0000,0xFF005F,0xFF0087,0xFF00AF,0xFF00D7,0xFF00FF,0xFF5F00,0xFF5F5F,0xFF5F87,0xFF5FAF,0xFF5FD7,0xFF5FFF,
0xFF8700,0xFF875F,0xFF8787,0xFF87AF,0xFF87D7,0xFF87FF,0xFFAF00,0xFFAF5F,0xFFAF87,0xFFAFAF,0xFFAFD7,0xFFAFFF,0xFFD700,0xFFD75F,0xFFD787,0xFFD7AF,
0xFFD7D7,0xFFD7FF,0xFFFF00,0xFFFF5F,0xFFFF87,0xFFFFAF,0xFFFFD7,0xFFFFFF,0x080808,0x121212,0x1C1C1C,0x262626,0x303030,0x3A3A3A,0x444444,0x4E4E4E,
0x585858,0x626262,0x6C6C6C,0x767676,0x808080,0x8A8A8A,0x949494,0x9E9E9E,0xA8A8A8,0xB2B2B2,0xBCBCBC,0xC6C6C6,0xD0D0D0,0xDADADA,0xE4E4E4,0xEEEEEE,
};	//ANSI_PAL256_COLOR
/**
* @brief	ANSI color 256.\n
* RGBQUAD : #00RRGGBB{bb,gg,rr,00}
*/
const RGBQUAD ANSI_PAL256_RGB[NUM_ANSI_PAL] = {
{0x00,0x00,0x00,0},{0x80,0x00,0x00,0},{0x00,0x80,0x00,0},{0x80,0x80,0x00,0},{0x00,0x00,0x80,0},{0x80,0x00,0x80,0},{0x00,0x80,0x80,0},{0xC0,0xC0,0xC0,0},{0x80,0x80,0x80,0},{0xFF,0x00,0x00,0},{0x00,0xFF,0x00,0},{0xFF,0xFF,0x00,0},{0x00,0x00,0xFF,0},{0xFF,0x00,0xFF,0},{0x00,0xFF,0xFF,0},{0xFF,0xFF,0xFF,0},
{0x00,0x00,0x00,0},{0x00,0x00,0x5F,0},{0x00,0x00,0x87,0},{0x00,0x00,0xAF,0},{0x00,0x00,0xD7,0},{0x00,0x00,0xFF,0},{0x00,0x5F,0x00,0},{0x00,0x5F,0x5F,0},{0x00,0x5F,0x87,0},{0x00,0x5F,0xAF,0},{0x00,0x5F,0xD7,0},{0x00,0x5F,0xFF,0},{0x00,0x87,0x00,0},{0x00,0x87,0x5F,0},{0x00,0x87,0x87,0},{0x00,0x87,0xAF,0},
{0x00,0x87,0xD7,0},{0x00,0x87,0xFF,0},{0x00,0xAF,0x00,0},{0x00,0xAF,0x5F,0},{0x00,0xAF,0x87,0},{0x00,0xAF,0xAF,0},{0x00,0xAF,0xD7,0},{0x00,0xAF,0xFF,0},{0x00,0xD7,0x00,0},{0x00,0xD7,0x5F,0},{0x00,0xD7,0x87,0},{0x00,0xD7,0xAF,0},{0x00,0xD7,0xD7,0},{0x00,0xD7,0xFF,0},{0x00,0xFF,0x00,0},{0x00,0xFF,0x5F,0},
{0x00,0xFF,0x87,0},{0x00,0xFF,0xAF,0},{0x00,0xFF,0xD7,0},{0x00,0xFF,0xFF,0},{0x5F,0x00,0x00,0},{0x5F,0x00,0x5F,0},{0x5F,0x00,0x87,0},{0x5F,0x00,0xAF,0},{0x5F,0x00,0xD7,0},{0x5F,0x00,0xFF,0},{0x5F,0x5F,0x00,0},{0x5F,0x5F,0x5F,0},{0x5F,0x5F,0x87,0},{0x5F,0x5F,0xAF,0},{0x5F,0x5F,0xD7,0},{0x5F,0x5F,0xFF,0},
{0x5F,0x87,0x00,0},{0x5F,0x87,0x5F,0},{0x5F,0x87,0x87,0},{0x5F,0x87,0xAF,0},{0x5F,0x87,0xD7,0},{0x5F,0x87,0xFF,0},{0x5F,0xAF,0x00,0},{0x5F,0xAF,0x5F,0},{0x5F,0xAF,0x87,0},{0x5F,0xAF,0xAF,0},{0x5F,0xAF,0xD7,0},{0x5F,0xAF,0xFF,0},{0x5F,0xD7,0x00,0},{0x5F,0xD7,0x5F,0},{0x5F,0xD7,0x87,0},{0x5F,0xD7,0xAF,0},
{0x5F,0xD7,0xD7,0},{0x5F,0xD7,0xFF,0},{0x5F,0xFF,0x00,0},{0x5F,0xFF,0x5F,0},{0x5F,0xFF,0x87,0},{0x5F,0xFF,0xAF,0},{0x5F,0xFF,0xD7,0},{0x5F,0xFF,0xFF,0},{0x87,0x00,0x00,0},{0x87,0x00,0x5F,0},{0x87,0x00,0x87,0},{0x87,0x00,0xAF,0},{0x87,0x00,0xD7,0},{0x87,0x00,0xFF,0},{0x87,0x5F,0x00,0},{0x87,0x5F,0x5F,0},
{0x87,0x5F,0x87,0},{0x87,0x5F,0xAF,0},{0x87,0x5F,0xD7,0},{0x87,0x5F,0xFF,0},{0x87,0x87,0x00,0},{0x87,0x87,0x5F,0},{0x87,0x87,0x87,0},{0x87,0x87,0xAF,0},{0x87,0x87,0xD7,0},{0x87,0x87,0xFF,0},{0x87,0xAF,0x00,0},{0x87,0xAF,0x5F,0},{0x87,0xAF,0x87,0},{0x87,0xAF,0xAF,0},{0x87,0xAF,0xD7,0},{0x87,0xAF,0xFF,0},
{0x87,0xD7,0x00,0},{0x87,0xD7,0x5F,0},{0x87,0xD7,0x87,0},{0x87,0xD7,0xAF,0},{0x87,0xD7,0xD7,0},{0x87,0xD7,0xFF,0},{0x87,0xFF,0x00,0},{0x87,0xFF,0x5F,0},{0x87,0xFF,0x87,0},{0x87,0xFF,0xAF,0},{0x87,0xFF,0xD7,0},{0x87,0xFF,0xFF,0},{0xAF,0x00,0x00,0},{0xAF,0x00,0x5F,0},{0xAF,0x00,0x87,0},{0xAF,0x00,0xAF,0},
{0xAF,0x00,0xD7,0},{0xAF,0x00,0xFF,0},{0xAF,0x5F,0x00,0},{0xAF,0x5F,0x5F,0},{0xAF,0x5F,0x87,0},{0xAF,0x5F,0xAF,0},{0xAF,0x5F,0xD7,0},{0xAF,0x5F,0xFF,0},{0xAF,0x87,0x00,0},{0xAF,0x87,0x5F,0},{0xAF,0x87,0x87,0},{0xAF,0x87,0xAF,0},{0xAF,0x87,0xD7,0},{0xAF,0x87,0xFF,0},{0xAF,0xAF,0x00,0},{0xAF,0xAF,0x5F,0},
{0xAF,0xAF,0x87,0},{0xAF,0xAF,0xAF,0},{0xAF,0xAF,0xD7,0},{0xAF,0xAF,0xFF,0},{0xAF,0xD7,0x00,0},{0xAF,0xD7,0x5F,0},{0xAF,0xD7,0x87,0},{0xAF,0xD7,0xAF,0},{0xAF,0xD7,0xD7,0},{0xAF,0xD7,0xFF,0},{0xAF,0xFF,0x00,0},{0xAF,0xFF,0x5F,0},{0xAF,0xFF,0x87,0},{0xAF,0xFF,0xAF,0},{0xAF,0xFF,0xD7,0},{0xAF,0xFF,0xFF,0},
{0xD7,0x00,0x00,0},{0xD7,0x00,0x5F,0},{0xD7,0x00,0x87,0},{0xD7,0x00,0xAF,0},{0xD7,0x00,0xD7,0},{0xD7,0x00,0xFF,0},{0xD7,0x5F,0x00,0},{0xD7,0x5F,0x5F,0},{0xD7,0x5F,0x87,0},{0xD7,0x5F,0xAF,0},{0xD7,0x5F,0xD7,0},{0xD7,0x5F,0xFF,0},{0xD7,0x87,0x00,0},{0xD7,0x87,0x5F,0},{0xD7,0x87,0x87,0},{0xD7,0x87,0xAF,0},
{0xD7,0x87,0xD7,0},{0xD7,0x87,0xFF,0},{0xD7,0xAF,0x00,0},{0xD7,0xAF,0x5F,0},{0xD7,0xAF,0x87,0},{0xD7,0xAF,0xAF,0},{0xD7,0xAF,0xD7,0},{0xD7,0xAF,0xFF,0},{0xD7,0xD7,0x00,0},{0xD7,0xD7,0x5F,0},{0xD7,0xD7,0x87,0},{0xD7,0xD7,0xAF,0},{0xD7,0xD7,0xD7,0},{0xD7,0xD7,0xFF,0},{0xD7,0xFF,0x00,0},{0xD7,0xFF,0x5F,0},
{0xD7,0xFF,0x87,0},{0xD7,0xFF,0xAF,0},{0xD7,0xFF,0xD7,0},{0xD7,0xFF,0xFF,0},{0xFF,0x00,0x00,0},{0xFF,0x00,0x5F,0},{0xFF,0x00,0x87,0},{0xFF,0x00,0xAF,0},{0xFF,0x00,0xD7,0},{0xFF,0x00,0xFF,0},{0xFF,0x5F,0x00,0},{0xFF,0x5F,0x5F,0},{0xFF,0x5F,0x87,0},{0xFF,0x5F,0xAF,0},{0xFF,0x5F,0xD7,0},{0xFF,0x5F,0xFF,0},
{0xFF,0x87,0x00,0},{0xFF,0x87,0x5F,0},{0xFF,0x87,0x87,0},{0xFF,0x87,0xAF,0},{0xFF,0x87,0xD7,0},{0xFF,0x87,0xFF,0},{0xFF,0xAF,0x00,0},{0xFF,0xAF,0x5F,0},{0xFF,0xAF,0x87,0},{0xFF,0xAF,0xAF,0},{0xFF,0xAF,0xD7,0},{0xFF,0xAF,0xFF,0},{0xFF,0xD7,0x00,0},{0xFF,0xD7,0x5F,0},{0xFF,0xD7,0x87,0},{0xFF,0xD7,0xAF,0},
{0xFF,0xD7,0xD7,0},{0xFF,0xD7,0xFF,0},{0xFF,0xFF,0x00,0},{0xFF,0xFF,0x5F,0},{0xFF,0xFF,0x87,0},{0xFF,0xFF,0xAF,0},{0xFF,0xFF,0xD7,0},{0xFF,0xFF,0xFF,0},{0x08,0x08,0x08,0},{0x12,0x12,0x12,0},{0x1C,0x1C,0x1C,0},{0x26,0x26,0x26,0},{0x30,0x30,0x30,0},{0x3A,0x3A,0x3A,0},{0x44,0x44,0x44,0},{0x4E,0x4E,0x4E,0},
{0x58,0x58,0x58,0},{0x62,0x62,0x62,0},{0x6C,0x6C,0x6C,0},{0x76,0x76,0x76,0},{0x80,0x80,0x80,0},{0x8A,0x8A,0x8A,0},{0x94,0x94,0x94,0},{0x9E,0x9E,0x9E,0},{0xA8,0xA8,0xA8,0},{0xB2,0xB2,0xB2,0},{0xBC,0xBC,0xBC,0},{0xC6,0xC6,0xC6,0},{0xD0,0xD0,0xD0,0},{0xDA,0xDA,0xDA,0},{0xE4,0xE4,0xE4,0},{0xEE,0xEE,0xEE,0},
};


//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
//================================================================
// 初期化
//================================================================
static void init_conio_d2d(COORD, D2D1_SIZE_F, CONSOLE_FONT_INFOEX);
/**
 * @brief	コンソール I/O 初期化（拡張版）
 *
 * @param	int _width ：画面（フレームバッファ）の幅（コンソールウィンドウの横サイズ）
 * @param	int _height：画面（フレームバッファ）の高さ（コンソールウィンドウの縦サイズ）
 * @param	int _font_w：フォントの横サイズ(1～)
 * @param	int _font_h：フォントの縦サイズ(1～)
 * @param	const wchar_t* _font_face_name：設定するフォントの名前(Unicode文字列)
 * @param	const COLORREF*：※実装が削除された
 * @param	bool：※実装が削除された
 *
 * @return	無し
 */
void InitConioEx(int _width, int _height, double _scale_x, double _scale_y, int _font_w, int _font_h, const wchar_t* _font_face_name, const COLORREF* _pal16, bool _init_wbuf)
{
	COORD srnsiz = { (SHORT)_width, (SHORT)_height };	//画面（フレームバッファ）のサイズ
	D2D1_SIZE_F scale = { (FLOAT)_scale_x, (FLOAT)_scale_y };	//画面の倍率
	//フォント用の引数を受け取る
	CONSOLE_FONT_INFOEX fex;
	fex.dwFontSize.X = _font_w;
	fex.dwFontSize.Y = _font_h;
	memset(fex.FaceName, 0, LF_FACESIZE);
	if (_font_face_name != NULL) {
		memcpy_s(fex.FaceName, LF_FACESIZE * sizeof(WCHAR), _font_face_name, LF_FACESIZE * sizeof(WCHAR));
	}
	//初期化実行
	init_conio_d2d(srnsiz, scale, fex);
}
/**
 * @brief	コンソール I/O 初期化（拡張版）
 *
 * @param	COORD _screen_size	画面（フレームバッファ）の幅と高さ
 * @param	D2D1_SIZE_F _scale	画面のヨコ・タテ倍率（１ドットのピクセル数）
 * @param	CONSOLE_FONT_INFOEX _font_ex	フォントの設定情報：横サイズ(1～)、縦サイズ(1～)、書体名など
 *
 * @return	無し
 */
static void init_conio_d2d(COORD _screen_size, D2D1_SIZE_F _scale, CONSOLE_FONT_INFOEX _font_ex)
{
	const char* str_locale = setlocale(LC_CTYPE, "Japanese_Japan");
	g_Scale = _scale;
	//キーバッファクリア
	memset(g_KeyPress, 0, NUM_KEYS);
	memset(g_KeyEdge, 0, NUM_KEYS);
	memset(g_KeyLast, 0, NUM_KEYS);

	//----------------------------------------------------------------
	//コンソールウィンドウのウィンドウハンドル(HWND)取得＆保存
	g_hConWnd = GetConsoleWindow();
	FixWin();
	//----------------------------------------------------------------
	// コマンド履歴を保存しない
	CONSOLE_HISTORY_INFO history_info;
	history_info.cbSize = sizeof(CONSOLE_HISTORY_INFO);
	history_info.HistoryBufferSize = 0;
	history_info.NumberOfHistoryBuffers = 0;
	history_info.dwFlags = 0;
	SetConsoleHistoryInfo(&history_info);

	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	//----------------------------------------------------------------
	// 直前のディスプレイ情報取得
	g_OrgOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);	//出力ハンドル
	GetConsoleMode(g_OrgOutputHandle, &g_OrgOutputHandleMode);	//出力コンソール情報
	g_OrgInputHandle = GetStdHandle(STD_INPUT_HANDLE);	//入力ハンドル
	GetConsoleMode(g_OrgInputHandle, &g_OrgInputHandleMode);	//入力コンソール情報
	////入力側は右クリックメニューを止める為に「簡易編集モード」をONにする。
	SetConsoleMode(g_OrgInputHandle, ENABLE_EXTENDED_FLAGS | ENABLE_QUICK_EDIT_MODE);	//入力コンソール情報
	//SetConsoleMode(g_OrgInputHandle, ENABLE_EXTENDED_FLAGS);	//入力コンソール情報
	//----------------------------------------------------------------
	//画面情報を保存しておく（16色パレット含む）
	g_OrgScreenBufferInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(g_OrgOutputHandle, &g_OrgScreenBufferInfoEx);
	// 現在のカラーパレットを保存
	for (int n = 0; n < NUM_PALETTE; n++) {
		g_OrgColorTableD2D[n] = g_OrgScreenBufferInfoEx.ColorTable[n];
		//デフォルトパレットの最初の１６色の位置に取り込む
		g_PaletteD2D[n] = CREFtoRGBQ(g_OrgColorTableD2D[n]);
	}
	//----------------------------------------------------------------
	//フォントサイズ保存：オリジナル保存
	g_OrgFontSizeEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(g_OrgOutputHandle, FALSE, &g_OrgFontSizeEx);	//第2パラメータがTRUEだと画面バッファと同じサイズが返るみたいだ・・・
	//----------------------------------------------------------------
	//現在のカーソル状態保存
	GetConsoleCursorInfo(g_OrgOutputHandle, &g_OrgCursorInfo);
	//カーソル表示OFF
	CONSOLE_CURSOR_INFO cci = { sizeof(CONSOLE_CURSOR_INFO) };
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_OrgOutputHandle, &cci);
	//----------------------------------------------------------------
	//ウィンドウの状態保存
	g_OrgWindowStylePtr = GetWindowLongPtr(g_hConWnd, GWL_STYLE);
	//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	////----------------------------------------------------------------
	//// GetSystemMetrics
	//// https://learn.microsoft.com/ja-JP/windows/win32/api/winuser/nf-winuser-getsystemmetrics
	//int cx_size_frame = GetSystemMetrics(SM_CXSIZEFRAME); // 境界線幅X方向
	//int cy_size_frame = GetSystemMetrics(SM_CYSIZEFRAME); // 境界線幅Y方向
	//int cy_caption = GetSystemMetrics(SM_CYCAPTION);	   // タイトルバーの高さ
	//FixWin();

	//----------------------------------------------------------------
	//フレームバッファのサイズをセット
	g_FrameBufferSizeD2D = _screen_size;
	//----------------------------------------------------------------
	//フォントの設定：先ず起動時のフォント設定のコピーを取る（フォントの書体やサイズは起動時のサイズになる）
	SetFontSize((FLOAT)_font_ex.dwFontSize.X, (FLOAT)_font_ex.dwFontSize.Y);
	if (_font_ex.FaceName[0] != 0) {
		memcpy_s(g_FontFaceName, LF_FACESIZE * sizeof(WCHAR), _font_ex.FaceName, LF_FACESIZE * sizeof(WCHAR));
	}
	SetTextAntialiasD2D(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);	//デフォルト値セット：アンチエイリアス無し

	//----------------------------------------------------------------
	// フォント情報の設定（指定があれば書体も設定する）
	// 後でウィンドウサイズを強制的に合わせるためにフォントは最小サイズにする。
	// フォントのサイズについて：
	//	縦は２未満には設定されない。１を設定出来るが実際には２になる。
	//	ウィンドウのプロパティの「フォント」タブのサイズに表示されるのはＹの値
	CONSOLE_FONT_INFOEX cfsx;
	//元の情報を元に新たな設定を行う。
	memcpy_s(&cfsx, sizeof(CONSOLE_FONT_INFOEX), &g_OrgFontSizeEx, sizeof(CONSOLE_FONT_INFOEX));
	cfsx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	cfsx.dwFontSize.X = 1;
	cfsx.dwFontSize.Y = 2;
	SetCurrentConsoleFontEx(g_OrgOutputHandle, FALSE, &cfsx);

	//----------------------------------------------------------------
	//クライアント領域が指定された大きさになるように計算して設定
	// ウィンドウサイズを変更するとウィンドウサイズと画面バッファのサイズも自動的に変更されるが、
	// 画面バッファのサイズ（画面に表示されているサイズでは無い）が
	// ウィンドウサイズより大きいと縦スクロールバーが強制的に表示される。
	// （画面外の部分が出来てしまう為にスクロールバーが出てしまう）
	// （SetWindowLongPtr()でVSCROLLの表示をOFFにしても消えない・・・）
	// ⇒大きいサイズを小さくした時にこの状態になる。
	// ⇒小さいサイズから大きくしたときはこの状態にならない。
	// ⇒一旦、十分小さいサイズにしておいてから、SetWindowPosで拡大する事で対応。
	//----------------------------------------------------------------
	//もしスクロースバーが出ている状態かもしれないのでウィンドウサイズを測る前に補正しておく
	//dwMaximumWindowSize	現在の設定（バッファーサイズ・フォントサイズ・ウィンドウサイズ）で表示できる最大サイズ
	//srWindow				右上と左下のコンソール画面バッファー座標（画面バッファを表示しているウィンドウのサイズ（文字単位））
	//dwSize				画面バッファのサイズ
	//⇒srWindowのＹサイズに合わせておく
	//【１】画面バッファをウィンドウサイズより小さくすることでスクロールバーが出ていても消える様にしている。
	CONSOLE_SCREEN_BUFFER_INFOEX	csbiex;
	memcpy_s(&csbiex, sizeof(CONSOLE_SCREEN_BUFFER_INFOEX), &g_OrgScreenBufferInfoEx, sizeof(CONSOLE_SCREEN_BUFFER_INFOEX));
	csbiex.dwSize.X = (csbiex.srWindow.Right - csbiex.srWindow.Left);	//★ウィンドウサイズより１小さいサイズを設定
	csbiex.dwSize.Y = (csbiex.srWindow.Bottom - csbiex.srWindow.Top);	//★ウィンドウサイズより１小さいサイズを設定
	csbiex.dwMaximumWindowSize = csbiex.dwSize;
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	SetConsoleScreenBufferInfoEx(g_OrgOutputHandle, &csbiex);
	Sleep(10);	//？？？タイミングが悪いと以下の計算が違う場合があるのでSleep()入れている？？？
	//csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	//GetConsoleScreenBufferInfoEx(g_OrgOutputHandle, &csbiex);
	//【２】現在のウィンドウ状態からクライアント領域を合わせるサイズを算出する
	//　　　★小さくする前の状態の計測値を使う
	RECT rct_1;
	GetClientRect(g_hConWnd, &rct_1);	//現在のクライアント領域
	int w1 = rct_1.right - rct_1.left + 1;
	int h1 = rct_1.bottom - rct_1.top + 1;
	Sleep(10);
	RECT rct_2;
	GetWindowRect(g_hConWnd, &rct_2);	//現在のウィンドウ領域
	int w2 = rct_2.right - rct_2.left + 1;
	int h2 = rct_2.bottom - rct_2.top + 1;
	//ウィンドウ領域とクライアント領域の差分を計算する
	int w3 = w2 - w1;
	int h3 = h2 - h1;
	//クライアント領域が指定のサイズ(_width,_height)になるウィンドウサイズを算出
	int w = (int)g_FrameBufferSizeD2D.X * (int)g_Scale.width + w3;
	int h = (int)g_FrameBufferSizeD2D.Y * (int)g_Scale.height + h3;
	//int w = g_FrameBufferSizeD2D.X * g_FontSizeExD2D.dwFontSize.X + w3;
	//int h = g_FrameBufferSizeD2D.Y * g_FontSizeExD2D.dwFontSize.Y + h3;
	//【３】念のため一旦ウィンドウをとても小さなサイズにする。
	memcpy_s(&csbiex, sizeof(CONSOLE_SCREEN_BUFFER_INFOEX), &g_OrgScreenBufferInfoEx, sizeof(CONSOLE_SCREEN_BUFFER_INFOEX));
	csbiex.dwMaximumWindowSize = { 1,1 };
	csbiex.srWindow = { 0,0,1,1 };
	csbiex.dwSize = { 1,1 };
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	SetConsoleScreenBufferInfoEx(g_OrgOutputHandle, &csbiex);
	Sleep(10);
	//【４】指定されたサイズを持つウィンドウ領域になるようにウィンドウサイズを設定する
	//　　　⇒小さなウィンドウが指定のサイズまで拡大される。
	SetWindowPos(g_hConWnd, HWND_TOP, 0, 0, w, h, SWP_SHOWWINDOW | SWP_NOMOVE);
	Sleep(10);

	//----------------------------------------------------------------
	//★★★　Direct2D 初期化 ★★★
	InitD2D(g_FrameBufferSizeD2D.X, g_FrameBufferSizeD2D.Y);

	//ウィンドウサイズの固定
	FixWin();

	GetClientRect(g_hConWnd, &rct_1);
	//ここまでに設定済みの画面とフォントのサイズを取得
	GetConWinSize(g_ConWinSize);

	//初期化の為にキー入力呼び出し
	GetKeyAll();
	//フレーム同期の初期化
	InitFrameSync(60.0);
	return;
}	//InitConioEx

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
/**
* @brief	conioexの終了処理
* @param	なし
* @return	なし
*/
void EndConioEx(void)
{
	//Direct2D終了
	EndD2D();
	//元に戻す
	SetConsoleActiveScreenBuffer(g_OrgOutputHandle);	//アクティブなコンソール画面バッファを切替え
	SetConsoleMode(g_OrgOutputHandle, g_OrgOutputHandleMode);	//コンソールモード復帰
	SetConsoleCursorInfo(g_OrgOutputHandle, &g_OrgCursorInfo);	//カーソル表示状態の復帰
	//フォントサイズは"SetCurrentConsoleFontEx()"が無くても戻るみたいだが・・・
	g_OrgFontSizeEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	SetCurrentConsoleFontEx(g_OrgOutputHandle, FALSE, &g_OrgFontSizeEx);	//フォントサイズを元に戻す
	//InitConioEx直前のスクリーンバッファ状態に戻す。
	g_OrgScreenBufferInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	SetConsoleScreenBufferInfoEx(g_OrgOutputHandle, &g_OrgScreenBufferInfoEx);	//これが無いとパレットが戻らない
#ifdef _DEBUG
	//TEST:設定値が格納されているか確認
	CONSOLE_SCREEN_BUFFER_INFOEX	csbiex{ sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };
	GetConsoleScreenBufferInfoEx(g_OrgOutputHandle, &csbiex);
#endif // _DEBUG
	//window style recover.
	SetWindowLongPtr(g_hConWnd, GWL_STYLE, g_OrgWindowStylePtr);
}	//EndConioEx

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################

/*
* @brief	コンソールのクライアント領域のサイズと現在のフォントサイズの取得
*
* @param	RECT& _r：結果を入れるRECT構造体の参照
*
* @return	RECT& : 結果を入れたRECT構造体の参照
*					left：クライアント領域の幅
*					top：クライアント領域の高さ
*					right：フォントの幅
*					bottom：フォントの高さ
*/
RECT& GetConWinSize(RECT& _r)
{
	CONSOLE_FONT_INFOEX fsx;
	fsx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(g_OrgOutputHandle, FALSE, &fsx);
	GetClientRect(g_hConWnd, &_r);
	_r.left = _r.right - _r.left;
	_r.top = _r.bottom - _r.top;
	_r.right = fsx.dwFontSize.X;
	_r.bottom = fsx.dwFontSize.Y;
	g_ConWinSize = _r;
	return _r;
}

//================================================================
// ウィンドウ
//================================================================
LONG_PTR FixWin(void)
{
	//ウィンドウサイズ変更禁止
	//HWND hCon = GetConsoleWindow();
	LONG_PTR lastStylePtr = GetWindowLongPtr(g_hConWnd, GWL_STYLE);
	LONG_PTR lStylePtr = lastStylePtr;
	lStylePtr &= ~(WS_THICKFRAME | WS_HSCROLL | WS_VSCROLL);	//'~'ビットごとの反転(１の補数)
	lStylePtr = SetWindowLongPtr(g_hConWnd, GWL_STYLE, lStylePtr);
	//SetWindowPos(hCon, NULL, 0, 0, frmb.width + 20, frmb.height, SWP_NOSIZE | SWP_NOZORDER);
	return lastStylePtr;
}	//FixWin
/**
* @brief	ウィンドウサイズを固定する
*
* @param	int _x,int _y	表示位置の指定
*
* @return	LONG	変更前の状態を返す
*/
LONG_PTR FixWin(int _x, int _y)
{
	//ウィンドウサイズ変更禁止
	//HWND hCon = GetConsoleWindow();
	LONG_PTR lastStylePtr = GetWindowLong(g_hConWnd, GWL_STYLE);
	LONG_PTR lStyle = lastStylePtr;
	lStyle &= ~(WS_THICKFRAME | WS_HSCROLL | WS_VSCROLL);	//'~'ビットごとの反転(１の補数)
	lStyle = SetWindowLongPtr(g_hConWnd, GWL_STYLE, lStyle);
	//SWP_NOSIZEを指定しているので、座標(_x,_y)のみがに変更される。
	SetWindowPos(g_hConWnd, NULL, _x, _y, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
	return lastStylePtr;
}	//FixWin
/**
* @brief	現在のディスプレイハンドルを取得する。
*
* @return	HANDLE	現在のディスプレイハンドル
*/
HANDLE GetCurrentHandle(void) {
	return g_OrgOutputHandle;
}

/**
* @brief	コンソールウィンドウのタイトルバーにテキストを設定
*
* @param	title [入力] ウィンドウタイトルに表示するテキスト
*/
void SetCaption(const char* title)
{
	SetConsoleTitleA(title);
}

/**
* @brief	コンソールウィンドウのタイトルバーに書式指定してテキストを設定
*
* @param	const char *_format：書式指定文字列
* @param	...：可変長引数
*/
void SetCaptionF(const char* _format, ...)
{
	va_list ap;
	va_start(ap, _format);
	size_t length = _vscprintf(_format, ap) + 1;	//'\0'含まないので＋１している
	char* buf = (char*)_malloca(length);
	_ASSERT(buf);
	vsprintf_s(buf, length, _format, ap);
	SetConsoleTitleA(buf);
	va_end(ap);
}	//SetCaptionFA

/**
* @brief	コンソールウィンドウのタイトルバーに表示されるテキストを取得
*
* @param	title [出力] 現在のウィンドウタイトルのテキスト
* @param	len [入力] ウィンドウタイトルの文字数
*
* @retval	非0	現在のウィンドウタイトルの文字数
* @retval	0	エラー
*/
int GetCaption(char* title, int len)
{
	return GetConsoleTitleA(title, len);
}	//GetCaption

//================================================================
//カーソル
//================================================================
/**
 * @brief	水平方向のカーソル位置を取得
 *
 * @return	現在のカーソル位置のX座標(1～)
 */
int GetCursorX(void)
{
	return	g_CursorPosD2D.X + 1;
}	//GetCursorX

/**
 * @brief	垂直方向のカーソル位置を取得
 *
 * @return	現在のカーソル位置のY座標(1～)
 */
int GetCursorY(void)
{
	return	g_CursorPosD2D.Y + 1;
}	//GetCursorY

/**
 * @brief	カーソル位置の移動
 *
 * @param	x [入力] X座標(1～)
 * @param	y [入力] Y座標(1～)
 */
void SetCursorPosition(int _csr_x, int _csr_y)
{
	g_CursorPosD2D.X = _csr_x - 1;
	g_CursorPosD2D.Y = _csr_y - 1;
}	//SetCursorPosition

/**
 * @brief	カーソルタイプ設定
 *
 * @param	type [入力]\n
 */
void SetCursorType(int type)
{
}	//SetCursorType

/**
* @brief	マウス座標の取得
*
* @param	POINT* _mp：座標を受け取るPOINT構造体へのポインタ
*
* @return	POINT：マウスの座標（文字単位）
*
* @note		_mpにはクライアント座標が返される
*			（ポインタがNULLならクライアント座標は格納しない）
*			戻り値は文字単位に換算した座標が返される
*/
POINT GetCursorMousePos(POINT* _mp)
{
	POINT mpos = { 0,0 };	//戻り値用
	GetCursorPos(&mpos);	//現在の位置
	ScreenToClient(GetConsoleWindow(), &mpos);	//クライアント座標へ変換
	if (_mp != NULL) {
		*_mp = mpos;	//クライアント座標を返す
	}
	//文字単位の座標に変換
	mpos.x /= (LONG)g_Scale.width;
	mpos.y /= (LONG)g_Scale.height;
	//mpos.x /= g_FontSizeExD2D.dwFontSize.X;
	//mpos.y /= g_FontSizeExD2D.dwFontSize.Y;
	return mpos;	//文字単位の座標として返す
}	//GetCursorMousePos

//================================================================
//文字列描画
//================================================================
/**
 * @brief	文字列の出力（マルチバイト文字用）
 *
 * @param	_srcbuf [入力] 出力文字列配列のポインタ
 * @param	_size [入力] 出力文字数
 */
void PrintStringA(const char* _srcbuf, int _size)
{
	//表示文字数が指定文字列のサイズをオーバーしていたら補正する
	int src_len = (int)strlen(_srcbuf);
	if (_size < 0) {
		_size = src_len;
	}
	else if (_size > src_len) {
		_size = (int)strlen(_srcbuf);
	}
	g_PrintStringCompatibleMode = true;
	FLOAT orgSizeW = 0, orgSizeH = 0;
	GetFontSize(&orgSizeW, &orgSizeH);
	SetFontSize(1, 1);
	WriteTextA(g_CursorPosD2D.X, g_CursorPosD2D.Y, _srcbuf, g_FontHeight, D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF(0, 0, 0, 0), false);
	SetFontSize(orgSizeW, orgSizeH);
}	//PrintStringA

/**
 * @brief	文字列の出力（Unicode文字用）
 *
 * @param	_srcbuf [入力] 出力文字列配列のポインタ
 * @param	_size [入力] 出力文字数
 */
void PrintStringW(const wchar_t* _srcbuf, int _size)
{
	//表示文字数が指定文字列のサイズをオーバーしていたら補正する
	int src_len = (int)wcslen(_srcbuf);
	if (_size < 0) {
		_size = src_len;
	}
	else if (_size > src_len) {
		_size = (int)wcslen(_srcbuf);
	}
	g_PrintStringCompatibleMode = true;
	FLOAT orgSizeW = 0, orgSizeH = 0;
	GetFontSize(&orgSizeW, &orgSizeH);
	SetFontSize(1, 1);
	WriteTextW(g_CursorPosD2D.X, g_CursorPosD2D.Y, _srcbuf, g_FontHeight, D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF(0, 0, 0, 0), false);
	SetFontSize(orgSizeW, orgSizeH);
}	//PrintStringW

#ifdef UNICODE
	/**
	 * @brief	文字列の出力(マルチバイト⇒Unicode変換版)
	 *
	 * @param	_src [入力] 出力文字列配列のポインタ
	 * @param	_size [入力] 出力文字数
	 */
void PrintString(const char* _src, int _size)
{
	//指定されたマルチバイト文字全てをワイド文字(Unicode文字)変換した場合の
	//必要なバッファーサイズ (終端の null 文字を含む) を文字単位で算出し、
	//その文字数分のバッファーを確保する。
	int wc_src_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, NULL, 0);
	size_t wc_src_bytes = (wc_src_siz * sizeof(wchar_t));	//Unicode文字換算のバイト数。
	wchar_t* wc_src = (wchar_t*)_malloca(wc_src_bytes);	//スタック上に確保：free不要
	_ASSERT(wc_src);
	memset(wc_src, 0, wc_src_bytes);	//０クリア：書き込みが途中まででも'\0'終端文字列になる。
	//指定サイズが元の文字数をオーバーしている場合の補正
	if ((int)strlen(_src) < _size) {
		_size = (-1);	//(-1)指定で'\0'まで変換。
	}
	//指定サイズ分変換する（_size == (-1))なら'\0'まで全て変換する）
	//戻り値は変換した(バッファに書き込まれた)文字数が返る。
	//【注】(-1)指定で変換した場合、戻り値は'\0'を含む文字数になる。
	int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, _size, wc_src, wc_src_siz);
	//disp_sizは'\0'を含む文字数かもしれないので、変換した文字数になる様に再計算する。
	disp_siz = (int)wcslen(wc_src);
	g_PrintStringCompatibleMode = true;
	FLOAT orgSizeW = 0, orgSizeH = 0;
	GetFontSize(&orgSizeW, &orgSizeH);
	SetFontSize(1, 1);
	WriteTextW(g_CursorPosD2D.X, g_CursorPosD2D.Y, wc_src, g_FontHeight, D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF(0, 0, 0, 0), false);
	SetFontSize(orgSizeW, orgSizeH);
}	//PrintString
#endif	//UNICODE

//----------------
//文字全体
//----------------
void SetHighVideoColor(void) {}
void SetLowVideoColor(void) {}
void SetNormalVideoColor(void) {}
void SetTextBackColor(int color) {}
void SetConsoleTextColor(int color) {}
void SetTextAttribute(int attribute) {}
//----------------
//行操作
//----------------
void ClearLine(void) {}
void InsertLine(void) {}
void DeleteLine(void) {}

//================================================================
// 拡張文字列処理
//================================================================
/**
* @brief
* 半角文字を全角文字に変換する（マルチバイト版）
*
* @param	const char* _src	変換元になる文字列（マルチバイト文字）
*
* @return	char*\n
* 変換後の文字列が入っているバッファへのポインタ。\n
* 【注】戻り値にmalloc()したポインタを返すので、呼び出した側で必ずfree()する事。
*/
char* HanToZenA(const char* _src)
{
	//const char* _src = "変換するabcxyz;*@文字列1234567890";
	/*
	* 一旦Unicode文字列に変換したものを全角文字に変換してマルチバイト文字列に戻している。
	*	MultiByteToWideChar()：マルチバイト文字列からUnicode文字列へ変換
	*	 LCMapStringEx()：半角から全角へ変換
	*	 WideCharToMultiByte()：マルチバイト文字列からUbicode文字列へ変換
	*/
	//---- マルチバイト文字列をUnicode文字列に変換する
	int wc_count = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, NULL, 0);	//'\0'を含む文字数が返る
	size_t wc_src_bytes = (wc_count * sizeof(wchar_t));
	wchar_t* src_txt = (wchar_t*)_malloca(wc_src_bytes);
	_ASSERT(src_txt);
	memset(src_txt, 0, wc_src_bytes);
	int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, src_txt, wc_count);
	//---- Unicode文字列の半角文字を全角文字に変換する ----
	DWORD flags = LCMAP_FULLWIDTH;		//全角文字に変換
	//	DWORD flags = LCMAP_HALFWIDTH;		//半角文字に変換
	//	DWORD flags = LCMAP_HIRAGANA;		//ひらがなに変換
	//	DWORD flags = LCMAP_KATAKANA;		//カタカナに変換
	int dest_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, src_txt, -1, NULL, 0, NULL, NULL, 0);
	wchar_t* dest_buf = (wchar_t*)_malloca(dest_size * sizeof(wchar_t));
	_ASSERT(dest_buf);
	memset(dest_buf, 0, dest_size * sizeof(wchar_t));
	int output_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, src_txt, -1, dest_buf, dest_size, NULL, NULL, 0);
	//---- Unicode文字列をマルチバイト文字列に変換する ----
	//文字数を計測
	int mb_bytes = WideCharToMultiByte(CP_ACP, 0, dest_buf, -1, NULL, 0, NULL, NULL);	//'\0'含むサイズが戻る
	//変換先バッファを確保。
	char* mb_dest_buff = (char*)calloc(mb_bytes, sizeof(char));
	_ASSERT(mb_dest_buff);
	memset(mb_dest_buff, 0, mb_bytes);	//変換先バッファを０で初期化
	//変換
	int res = WideCharToMultiByte(CP_ACP, 0, dest_buf, -1, mb_dest_buff, mb_bytes, NULL, NULL);
	return mb_dest_buff;	//変換済み文字列バッファ(【注】動的メモリ確保したポインタ)を返す。
}	//HanToZenA

/**
* @brief
* 半角文字を全角文字に変換する（Unicode版）
*
* @param	const wchar_t* _src	変換元になる文字列（Unicode文字）
*
* @return	wchar_t*\n
* 変換後の文字列が入っているバッファへのポインタ。\n
* 【注】戻り値にmalloc()したポインタを返すので、呼び出した側で必ずfree()する事。
*/
wchar_t* HanToZenW(const wchar_t* _src)
{
	//---- Unicode文字列の半角文字を全角文字に変換する ----
	DWORD flags = LCMAP_FULLWIDTH;		//全角文字に変換
	//	DWORD flags = LCMAP_HALFWIDTH;		//半角文字に変換
	//	DWORD flags = LCMAP_HIRAGANA;		//ひらがなに変換
	//	DWORD flags = LCMAP_KATAKANA;		//カタカナに変換
	//文字数を計測
	int dest_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, _src, -1, NULL, 0, NULL, NULL, 0);
	//変換先バッファを確保。
	wchar_t* dest_buf = (wchar_t*)calloc(dest_size, sizeof(wchar_t));
	_ASSERT(dest_buf);
	memset(dest_buf, 0, dest_size * sizeof(wchar_t));	//変換先バッファを０で初期化
	//変換
	int output_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, _src, -1, dest_buf, dest_size, NULL, NULL, 0);
	return dest_buf;	//変換済み文字列バッファ(【注】動的メモリ確保したポインタ)を返す。
}	//HanToZenW

/**
* @brief
* 書式指定付文字列描画（引数リスト版）（マルチバイト文字用）
*
* @param	bool _zenkaku	trueなら全ての文字を全角で出力
* 例）
*	false:"全角%d",99 -> "全角99"
*	true:"全角%d",99 -> "全角９９"
* @param	const char* _format	書式指定文字列（マルチバイト文字）
* @param	va_list _ap	任意の可変長引数
*/
void VPrintStringFA(bool _zenkaku, const char* _format, va_list _ap)
{
	size_t length = _vscprintf(_format, _ap) + 1;	//'\0'含まないので＋１している
	char* buf = (char*)_malloca(length);
	_ASSERT(buf);
	vsprintf_s(buf, length, _format, _ap);
	if (_zenkaku == true) {
		char* p = HanToZen(buf);
		PrintString(p, -1);
		free(p);
	}
	else {
		PrintString(buf, -1);
	}
}	//VPrintStringFA

/**
* @brief
* 書式指定付文字列描画（引数リスト版）（Unicode文字用）
*
* @param	bool _zenkaku	trueなら全ての文字を全角で出力
* 例）
*	false:"全角%d",99 -> "全角99"
*	true:"全角%d",99 -> "全角９９"
* @param	const wchar_t* _format	書式指定文字列（Unicode文字）
* @param	va_list _ap	任意の可変長引数
*/
void VPrintStringFW(bool _zenkaku, const wchar_t* _format, va_list _ap)
{
	size_t length = _vscwprintf(_format, _ap) + 1;	//'\0'含まないので＋１している
	wchar_t* buf = (wchar_t*)_malloca(length * sizeof(wchar_t));
	_ASSERT(buf);
	vswprintf_s(buf, length, _format, _ap);
	if (_zenkaku == true) {
		wchar_t* p = HanToZenW(buf);
		PrintStringW(p, -1);
		free(p);
	}
	else {
		PrintStringW(buf, -1);
	}
}	//VPrintStringFW

/**
* @brief
* 書式指定付文字列描画（マルチバイト文字用）
*
* @param	bool _zenkaku	trueを指定すると、全ての文字(空白' 'も含む)を全角に変換して表示する。\n
* 例）\n
*	false:"全角%d",99 -> "全角99"\n
*	 true:"全角%d",99 -> "全角９９"\n
*	false:"全角%s","あ1い2う"-> "全角あ1い2う"\n
*	 true:"全角%s","あ1い2う"-> "全角あ１い２う"\n
*	false:"全角%-4s","９"-> "全角９  "\n
*	 true:"全角%-4s","９"-> "全角９　　"\n
* 【注】文字列指定に幅指定が入っている時、全角半角交じりの文字列だと位置を合わせにくい。\n
*		変換元が数値の場合、全角/半角の区別は無いが、\n
*		変換元が文字列の場合、全角/半角が混じる場合があるので、位置合わせが難しくなる。\n
* @param	const char* _format	書式指定文字列（マルチバイト文字）
* @param	...	任意の可変長引数
*
* @note
* ※_zenkaku以降はprintf()と同じ仕様。
*/
void PrintStringFA(bool _zenkaku, const char* _format, ...)
{
	va_list ap;
	va_start(ap, _format);
	VPrintStringFA(_zenkaku, _format, ap);
	va_end(ap);
}	//PrintStringFA

/**
* @brief
* 書式指定付文字列描画（Unicode文字用）
*
* @param	bool _zenkaku	trueを指定すると、全ての文字(空白' 'も含む)を全角に変換して表示する。\n
* 例）\n
*	false:"全角%d",99 -> "全角99"\n
*	 true:"全角%d",99 -> "全角９９"\n
*	false:"全角%s","あ1い2う"-> "全角あ1い2う"\n
*	 true:"全角%s","あ1い2う"-> "全角あ１い２う"\n
*	false:"全角%-4s","９"-> "全角９  "\n
*	 true:"全角%-4s","９"-> "全角９　　"\n
* 【注】文字列指定に幅指定が入っている時、全角半角交じりの文字列だと位置を合わせにくい。\n
*		変換元が数値の場合、全角/半角の区別は無いが、\n
*		変換元が文字列の場合、全角/半角が混じる場合があるので、位置合わせが難しくなる。\n
* @param	const wchar_t* _format	書式指定文字列（Unicode文字）
* @param	...	任意の可変長引数
*
* @note
* ※_zenkaku以降はprintf()と同じ仕様。
*/
void PrintStringFW(bool _zenkaku, const wchar_t* _format, ...)
{
	va_list ap;
	va_start(ap, _format);
	VPrintStringFW(_zenkaku, _format, ap);
	va_end(ap);
}	//PrintStringFW

/**
* @brief
* 位置指定＆書式指定付文字列描画（マルチバイト文字用）
*
* @param	int _xp	座標Ｘ指定（１オリジン）
* @param	int _yp	座標Ｙ指定（１オリジン）
* @param	bool _zenkaku	trueを指定すると、全ての文字(空白' 'も含む)を全角に変換して表示する。
* @param	const wchar_t* _format	書式指定文字列（マルチバイト文字）
* @param	...	任意の可変長引数\n
*
* @note
* ※_zenkaku以降はprintf()と同じ仕様。
*/
void PosPrintStringFA(int _xp, int _yp, bool _zenkaku, const char* _format, ...)
{
	SetCursorPosition(_xp, _yp);
	va_list ap;
	va_start(ap, _format);
	VPrintStringFA(_zenkaku, _format, ap);
	va_end(ap);
}	//PosPrintStringFA

/**
* @brief
* 位置指定＆書式指定付文字列描画（Unicode文字用）
*
* @param	int _xp	座標Ｘ指定（１オリジン）
* @param	int _yp	座標Ｙ指定（１オリジン）
* @param	bool _zenkaku	trueを指定すると、全ての文字(空白' 'も含む)を全角に変換して表示する。
* @param	const wchar_t* _format	書式指定文字列（Unicode文字）
* @param	...	任意の可変長引数\n
*
* @note
* ※_zenkaku以降はprintf()と同じ仕様。
*/
void PosPrintStringFW(int _xp, int _yp, bool _zenkaku, const wchar_t* _format, ...)
{
	SetCursorPosition(_xp, _yp);
	va_list ap;
	va_start(ap, _format);
	VPrintStringFW(_zenkaku, _format, ap);
	va_end(ap);
}	//PosPrintStringFW

/**
* @brief	座標指定（画面左上隅が(0,0)座標）＋書式指定付文字列描画（マルチバイト文字用）
*
* @param	int _x	表示Ｘ座標（０オリジン）
* @param	int _y	表示Ｙ座標（０オリジン）
* @param	bool _zenkaku	trueを指定すると、全ての文字(空白' 'も含む)を全角に変換して表示する。
* @param	const char* _format	書式指定文字列（マルチバイト文字）
* @param	...	可変長引数
*/
void DrawStringFA(int _x, int _y, bool _zenkaku, const char* _format, ...)
{
	g_CursorPosD2D.X = _x;
	g_CursorPosD2D.Y = _y;
	va_list ap;
	va_start(ap, _format);
	VPrintStringFA(_zenkaku, _format, ap);
	va_end(ap);
}	//DrawStringFA

/**
* @brief	座標指定（画面左上隅が(0,0)座標）＋書式指定付文字列描画（Unicode文字用）
*
* @param	int _x	表示Ｘ座標（０オリジン）
* @param	int _y	表示Ｙ座標（０オリジン）
* @param	bool _zenkaku	trueを指定すると、全ての文字(空白' 'も含む)を全角に変換して表示する。
* @param	const wchar_t* _format	書式指定文字列（Unicode文字）
* @param	...	可変長引き数
*/
void DrawStringFW(int _x, int _y, bool _zenkaku, const wchar_t* _format, ...)
{
	g_CursorPosD2D.X = _x;
	g_CursorPosD2D.Y = _y;
	va_list ap;
	va_start(ap, _format);
	VPrintStringFW(_zenkaku, _format, ap);
	va_end(ap);
}	//DrawStringFW

//================================================================
// パレット関係
//================================================================
/**
* @brief	RGBQUAD:{R,G,B,0}型をCOLORREF:0x00BBGGRR型に変換
*
* @param	RGBQUAD rgb	RGBQUAD色
*/
COLORREF RGBQtoCREF(RGBQUAD rgb)
{
	return (rgb.rgbRed & 0x0000FF) | ((rgb.rgbGreen << 8) & 0x00FF00) | ((rgb.rgbBlue << 16) & 0xFF0000);
}	//RGBQtoCREF

/**
* @brief	COLORREF:0x00BBGGRR型をRGBQUAD:{R,G,B,0}型に変換
*
* @param	COLORREF ref	COLORREF色
*/
RGBQUAD CREFtoRGBQ(COLORREF ref)
{
	RGBQUAD rgb = { (BYTE)((ref & 0x00FF0000) >> 16)/*Blue*/,(BYTE)((ref & 0x0000FF00) >> 8)/*Green*/,(BYTE)(ref & 0x000000FF)/*Red*/,0x00/*Reserved*/ };
	return rgb;
}	//CREFtoRGBQ

/**
* @brief	パレット変換：RGBQ[16] -> COLORREF[16]
*
* @param	const RGBQUAD* _rgb	変換元１６色
* @param	COLORREF* _cref	変換先１６色
*/
COLORREF* ConvRGBQtoCREF(const RGBQUAD* _rgb, COLORREF* _cref)
{
	for (int n = 0; n < NUM_PALETTE; n++) {
		_cref[n] = RGBQtoCREF(_rgb[n]);
	}
	return _cref;
}

/**
* @brief	パレット変換：RGBQ[16] -> COLORREF[16]
*
* @param	const COLORREF* _cref	変換元１６色
* @param	RGBQUAD* _rgb	変換先１６色
*/
RGBQUAD* ConvCREFtoRGBQ(const COLORREF* _cref, RGBQUAD* _rgb)
{
	for (int n = 0; n < NUM_PALETTE; n++) {
		_rgb[n] = CREFtoRGBQ(_cref[n]);
	}
	return _rgb;
}

/**
* @brief	パレットの設定
*
* @param	COLORREF* _pal256：パレットデータへのポインタ
* @param	int  _p1：セットしたいパレットの開始番号
* @param	int  _p2：セットしたいパレットの終了番号
*/
void SetPalette(const COLORREF* _pal256, int _p1, int _p2)
{
	if (_pal256 == NULL) {
		//パレット無し
		return;
	}
	//値の補正
	if (_p1 < 0) { _p1 = 0; }
	if (_p2 < 0) { _p2 = 0; }
	if (_p1 >= NUM_D2D_PAL) { _p1 = NUM_D2D_PAL - 1; }
	if (_p2 >= NUM_D2D_PAL) { _p2 = NUM_D2D_PAL - 1; }
	//_p1 <= _p2にする
	if (_p1 > _p2) {
		int t = _p1;
		_p1 = _p2;
		_p2 = t;
	}
	//COLORREFパレットのコピー
	for (int n = _p1; n <= _p2; n++) {
		g_PaletteD2D[n].rgbBlue = (BYTE)((_pal256[n] & 0x00FF0000) >> 16);	//00BBGGRR
		g_PaletteD2D[n].rgbGreen = (BYTE)((_pal256[n] & 0x0000FF00) >> 8);	//00BBGGRR
		g_PaletteD2D[n].rgbRed = (BYTE)((_pal256[n] & 0x000000FF));	//00BBGGRR
		g_PaletteD2D[n].rgbReserved = 0;	//00BBGGRR
	}
}	//SetPalette

/**
* @brief	パレットの設定
*
* @param	COLORREF* _pal16：パレットデータへのポインタ
* @param	int  _p1：セットしたいパレットの開始番号
* @param	int  _p2：セットしたいパレットの終了番号
*/
void SetPalette(const RGBQUAD* _pal16, int _p1, int _p2)
{
	if (_pal16 == NULL) {
		//パレット無し
		return;
	}

	//値の補正
	if (_p1 < 0) { _p1 = 0; }
	if (_p2 < 0) { _p2 = 0; }
	if (_p1 >= NUM_D2D_PAL) { _p1 = NUM_D2D_PAL - 1; }
	if (_p2 >= NUM_D2D_PAL) { _p2 = NUM_D2D_PAL - 1; }
	//_p1 <= _p2にする
	if (_p1 > _p2) {
		int t = _p1;
		_p1 = _p2;
		_p2 = t;
	}
	//RGBQUADパレットのコピー
	for (int n = _p1; n <= _p2; n++) {
		g_PaletteD2D[n] = _pal16[n];
	}
}	//SetPalette

/**
* @brief	Bmpからパレット１６色の設定
*
* @param	Bmp* _pBmp：パレットデータへのポインタ
*/
void SetPalette(const Bmp* _pBmp)
{
	if ((_pBmp->pal == NULL) && (_pBmp->pal_rgb == NULL)) {
		return;
	}
	if (_pBmp->numpal <= 0) {
		return;
	}
	if (_pBmp->pal != NULL) {
		SetPalette(_pBmp->pal, 0, _pBmp->numpal - 1);
	}
	else if (_pBmp->pal_rgb != NULL) {
		SetPalette(_pBmp->pal_rgb, 0, _pBmp->numpal - 1);
	}
}

//================================================================
// フレームバッファ画像描画
//================================================================
/*
* @brief	文字描画用ビットマップレンダーターゲットの描画内容を消去する
*/
inline	void clear_text_screen(void) {
	_ASSERT(g_ptrBRTforText != NULL);
	g_ptrBRTforText->BeginDraw();
	g_ptrBRTforText->Clear();
	g_ptrBRTforText->EndDraw();
}
/**
 * @brief	画面（スクリーンバッファ）消去
 *
 * @param	int _cc	塗りつぶすパレット番号
 */
void ClearScreen(int _cc)
{
	//画像用バッファ消去
	//memset(g_FrameBuffer32bitD2D, *((DWORD*)(&g_PaletteD2D[_cc % NUM_D2D_PAL])), sizeof(RGBQUAD) * g_FrameBufferSizeD2D.X * g_FrameBufferSizeD2D.Y);
	for (int i = 0; i < NUM_D2D_PAL; i++) {
		g_FrameBuffer32bitD2D[i] = g_PaletteD2D[_cc % NUM_D2D_PAL];
	}
	clear_text_screen();	//文字表示用ターゲットを消去
}	//ClearScreen
/**
 * @brief	画面（スクリーンバッファ）消去
 *
 * @param	int _red, _green, _blue	塗りつぶすRGB値
 */
void ClearScreen(int _red, int _green, int _blue)
{
	RGBQUAD rgb{ (BYTE)_blue,(BYTE)_green,(BYTE)_red,0 };
	//rgb.rgbBlue = _blue;
	//rgb.rgbGreen = _green;
	//rgb.rgbRed = _red;
	//rgb.rgbReserved = _alpha;

	//画像用バッファ消去
	//memset(g_FrameBuffer32bitD2D, ((DWORD*)&_rgb)[0], sizeof(RGBQUAD) * g_FrameBufferSizeD2D.X * g_FrameBufferSizeD2D.Y);
	for (int i = 0; i < (g_FrameBufferSizeD2D.X * g_FrameBufferSizeD2D.Y); i++) {
		g_FrameBuffer32bitD2D[i] = rgb;
	}
	clear_text_screen();	//文字表示用ターゲットを消去
}	//ClearScreen
/**
 * @brief	画面（スクリーンバッファ）消去
 *	画像用バッファを「０」で消去
 */
void ClearScreen(void)
{
	//画像用バッファ消去
	ZeroMemory(g_FrameBuffer32bitD2D, sizeof(RGBQUAD) * g_FrameBufferSizeD2D.X * g_FrameBufferSizeD2D.Y);
	clear_text_screen();	//文字表示用ターゲットを消去
}	//ClearScreen

//----------------------------------------------------------------
// BMP画像の描画(LoadBmp/CreateBmpChar/CreateBmpStringで生成したBmp画像)
//----------------------------------------------------------------
static RGBQUAD tmpRGBQ[NUM_ANSI_PAL] = { 0 };	//一時的COLORREFパレットをRGBQパレットに変換する為のパレット
static const RGBQUAD* g_ptrRGBQ = g_PaletteD2D;	//NULL;	//パレットポインタ
static int g_TrCC = 0;	//透明色のパレット番号
static DWORD g_TrRGB = 0;	//透明色のRGB値

const double DEFAULT_TR_LEVEL = 1.0;
static double g_TrLevel = DEFAULT_TR_LEVEL;	//透明度の強さ
static bool g_AlphaBlendMode = false;
/*
* @brief	αブレンドモードのＯＮ／ＯＦＦ
*
* @return	直前の状態を返す
*/
bool AlphaBlendMode(bool _flag)
{
	bool old = g_AlphaBlendMode;
	g_AlphaBlendMode = _flag;
	return old;
}
/*
* @brief	透過レベルの設定
*
* @param	double _tr_level	透過レベル０～１００％
*
* @return	直前のレベルを返す
*/
double SetTrLevel(double _tr_level)
{
	if (_tr_level > 100.0) { _tr_level = 100.0; }
	if (_tr_level < 0.0) { _tr_level = 0.0; }
	double	old_lebel = g_TrLevel * 100.0;
	g_TrLevel = (_tr_level / 100.0);	//透明度設定
	AlphaBlendMode(true);
	return	old_lebel;
}
/*
* @brief	透過レベルのリセット
*			１００．０％になる
*/
void ResetTrLevel(void)
{
	g_TrLevel = DEFAULT_TR_LEVEL;	//透明度元に戻す
	AlphaBlendMode(false);
}

/*
* @brief	[result = (Dest * (1 - Alpha)) + (Source * Alpha)]のアルファブレンディングによる透過処理
*/
inline DWORD calc_alpha_blend(register DWORD _dest, register DWORD _src)
{
	//source:Alpha,R,G,B
	register double	alpha = (double)(((BYTE*)&_src)[3]) / 255.0;	//値を0.0～1.0にする
	alpha *= g_TrLevel;	//透明度を掛ける
	register double	alpha2 = (1.0 - alpha);
	register double red = (double)(((BYTE*)&_src)[2]) / 255.0;	//値を0.0～1.0にする
	register double green = (double)(((BYTE*)&_src)[1]) / 255.0;	//値を0.0～1.0にする
	register double blue = (double)(((BYTE*)&_src)[0]) / 255.0;	//値を0.0～1.0にする
	//destination:R,G,B
	register double dest_r = (double)(((BYTE*)&_dest)[2]) / 255.0;	//値を0.0～1.0にする
	register double dest_g = (double)(((BYTE*)&_dest)[1]) / 255.0;	//値を0.0～1.0にする
	register double dest_b = (double)(((BYTE*)&_dest)[0]) / 255.0;	//値を0.0～1.0にする

	register DWORD bR = (DWORD)(255.0 * ((dest_r * alpha2) + (red * alpha)));
	register DWORD bG = (DWORD)(255.0 * ((dest_g * alpha2) + (green * alpha)));
	register DWORD bB = (DWORD)(255.0 * ((dest_b * alpha2) + (blue * alpha)));
	//make DWORD format
	register DWORD result_rgb = ((bR << 16) | (bG << 8) | bB);
	return result_rgb;
}	//calc_alpha_blend

/**
* @brief	点を打つ
*
* @param	int _x,_y：座標
* @param	RGBQUAD _rgb：色（RGBQUAD:0x00RRGGBB）
*/
void DrawPixel(int _xx, int _yy, RGBQUAD _rgb)
{
	if ((_xx < 0) || (_xx >= g_FrameBufferSizeD2D.X) || (_yy < 0) || (_yy >= g_FrameBufferSizeD2D.Y)) {
		return;
	}
	if (g_AlphaBlendMode) {
		register DWORD src = (*((DWORD*)&_rgb)) | 0xFF000000;
		register DWORD dest = ((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx];
		((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx] = calc_alpha_blend(dest, src);
		return;
	}
	g_FrameBuffer32bitD2D[_yy * g_FrameBufferSizeD2D.X + _xx] = _rgb;
}	//DrawPixel

/**
* @brief	点を打つ
*
* @param	int _x,_y：座標
* @param	int _c：色（パレット番号０～１５）
*/
void DrawPixel(int _x, int _y, BYTE _c)
{
	DrawPixel(_x, _y, g_PaletteD2D[_c & 0xFF]);
}

/**
* @brief	８ビット/Pixel画像の出力
*
* @param	int _cc		ピクセルのパレット番号
* @param	int _xp		Ｘ座標（０～）
* @param	int _yp		Ｙ座標（０～）
*
*/
inline void pixel_copy08_rgbq(register int _cc, register int _xx, register int _yy) {
	if ((_xx >= 0) && (_yy >= 0) && (_yy < g_FrameBufferSizeD2D.Y) && (_xx < g_FrameBufferSizeD2D.X)) {
		//画面内である
		//RGBQUAD <= RGBQUAD
		g_FrameBuffer32bitD2D[_yy * g_FrameBufferSizeD2D.X + _xx] = g_ptrRGBQ[_cc % NUM_ANSI_PAL];
	}
}	//pixel_copy04_rgbq
inline void pixel_copy08_rgbq_tr(register int _cc, register int _xx, register int _yy) {
	if (_cc == g_TrCC)	return;
	pixel_copy08_rgbq(_cc, _xx, _yy);
}	//pixel_copy04_rgbq

/**
* @brief	８ビット/Pixel画像の出力
*
* @param	int _cc		ピクセルのパレット番号
* @param	int _xp		Ｘ座標（０～）
* @param	int _yp		Ｙ座標（０～）
*
*/
inline void pixel_copy08_rgbq_alpha(register int _cc, register int _xx, register int _yy) {
	//RGBQUAD[]のパレットである
	if ((_xx >= 0) && (_yy >= 0) && (_yy < g_FrameBufferSizeD2D.Y) && (_xx < g_FrameBufferSizeD2D.X)) {
		register DWORD src = ((DWORD*)g_ptrRGBQ)[_cc % NUM_ANSI_PAL] | 0xFF000000;
		register DWORD dest = ((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx];
		((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx] = calc_alpha_blend(dest, src);
	}
}	//pixel_copy04_rgbq_alpha
inline void pixel_copy08_rgbq_alpha_tr(register int _cc, register int _xx, register int _yy) {
	if (_cc == g_TrCC)	return;
	pixel_copy08_rgbq_alpha(_cc, _xx, _yy);
}	//pixel_copy04_rgbq_alpha

/*
* １～８ビット画像：転送モード別関数のためのポインタ
*/
void (*fp_pcp08)(register int _cc, register int _xx, register int _yy);

//----------------------------------------------------------------
/*
* @brief	Bmpのパレットを調べてRBQUADパレットのポインタを返す
*			COLORREFパレットだったらRGBQUADパレットに変換する
*
* @param	Bmp*	Bmp画像へのポインタ
*/
static RGBQUAD* palette_ptr_set(const Bmp* _bmp)
{
	/*
	* パレット用ポインタ設定
	*	パレットがCOLORREFだったら変換する
	*	 COLORREF	[0]alpha[1]Blue[2]Green[3]Red -> DWORD:0xAABBGGRR
	*	 RGBQUAD	[0]alpha[1]Red[2]Green[3]Blue -> DWORD:0xAARRGGBB
	*/
	if (_bmp->pal_rgb != NULL) {
		//RGBQUAD型のパレットだったらそのままセットする
		//g_ptrRGBQ = _bmp->pal_rgb;
		return _bmp->pal_rgb;
	}
	else if (_bmp->pal != NULL) {
		//COLORREF型パレットだったら変換する
		BYTE* dest = (BYTE*)tmpRGBQ;
		BYTE* src = (BYTE*)_bmp->pal;
		//for (int n = 0; n < NUM_ANSI_PAL; n++)
		for (int n = 0; n < _bmp->numpal; n++)
		{
			dest[0] = src[2];	//Blue
			dest[1] = src[1];	//Green
			dest[2] = src[0];	//Red
			dest[3] = src[3];	//alpha
			dest += sizeof(RGBQUAD);
			src += sizeof(COLORREF);
		}
		//g_ptrRGBQ = tmpRGBQ;
		return tmpRGBQ;
	}
	//どちらでもなかったらデフォルトパレットをセットする
	//g_ptrRGBQ = g_PaletteD2D;
	return g_PaletteD2D;
}	//palette_ptr_set

//================================================================
//	インデックスカラー画像を現在設定されている２５６色パレットで32ビットバッファに描画する
/*
* ★★★
*	描画時に１ピクセル単位で全て３２ビットバッファに描きこんでいるのでこの処理は不要。
*	というか、今のところピクセル単位で処理しても重くは無いので、
* 	４ビットバッファが不要かも知れない・・・
*	それに、PrintImag()と併用すると、32ビットバッファを上書きしてしまうのでもややこしくなるし・・・
*	まあ、4ビット画像からの変換用に専用の32ビットバッファを持つといいけど、それではパレットから解放されないので、それもややこしい・・・
*	しかし、パレットインデックス画像の仕組みを勉強する為には良いのかもしれないので検討の余地は大きいか・・・
* ★★★
*	int src_pix_s = (g_FrameBufferSizeD2D.X * g_FrameBufferSizeD2D.Y * sizeof(char));
*	RGBQUAD* tmp = g_FrameBuffer32bitD2D;
*	for (int n = 0; n < src_pix_s; n++) {
*		//8ビットのパレットインデックスバッファでパレットバッファ参照して32ビットRGBQUADバッファへ書き込み
*		tmp[n] = g_PaletteD2D[_buf_8bit[n] % NUM_ANSI_PAL];
*	}
* ★★★
*/
static void draw_bmp_bpp08(int _xp, int _yp, const Bmp* _bmp, int _inv, bool _tr, int _tr_cc)
{
	g_TrCC = _tr_cc;	//透明色のパレット番号セット⇒pixel_copy08_rgbq_alpha_tr,pixel_copy08_rgbq_trで使用

	g_ptrRGBQ = palette_ptr_set(_bmp);


	//モードを見て転送関数を選択する
	if (g_AlphaBlendMode) {
		if (_tr) {
			fp_pcp08 = pixel_copy08_rgbq_alpha_tr;	//αブレンドモード＋透明処理あり
		}
		else {
			fp_pcp08 = pixel_copy08_rgbq_alpha;	//αブレンドモード
		}
	}
	else if (_tr) {
		fp_pcp08 = pixel_copy08_rgbq_tr;	//透過処理有り
	}
	else {
		fp_pcp08 = pixel_copy08_rgbq;	//透過処理なし
	}
	/*
	* 各回転反転方向別に処理
	*/
	register const BYTE* buf = (BYTE*)_bmp->pixel;	//転送先フレームバッファのポインタ設定
	if (_inv == 0) {
		//反転回転無し（頭が上・鼻が左）
		for (int y = 0; y < _bmp->height; y++) {
			for (int x = 0; x < _bmp->width; x++) {
				fp_pcp08(*buf, x + _xp, y + _yp);
				buf++;
			}
		}
	}
	else if (_inv == BMP_HINV) {
		//水平反転のみ（頭が上・鼻が右）
		for (int y = 0; y < _bmp->height; y++) {
			for (int x = _bmp->width - 1; x >= 0; x--) {
				fp_pcp08(*buf, x + _xp, y + _yp);
				buf++;
			}
		}
	}
	else if (_inv == BMP_VINV) {
		//垂直反転のみ（頭が下・鼻が右）
		for (int y = _bmp->height - 1; y >= 0; y--) {
			for (int x = 0; x < _bmp->width; x++) {
				fp_pcp08(*buf, x + _xp, y + _yp);
				buf++;
			}
		}
	}
	else if (_inv == BMP_HVINV) {
		//水平＋垂直反転（頭が下・鼻が左）（１８０度回転）
		for (int y = _bmp->height - 1; y >= 0; y--) {
			for (int x = _bmp->width - 1; x >= 0; x--) {
				fp_pcp08(*buf, x + _xp, y + _yp);
				buf++;
			}
		}
	}
	else if (_inv & BMP_ROT90) {
		//横倒しになっている場合
		_inv &= (~BMP_ROT90);	//横倒しフラグは消す
		if (_inv == 0) {
			//横倒しのみ（頭が右・鼻が下）■▼
			for (int x = _bmp->height - 1; x >= 0; x--) {
				for (int y = 0; y < _bmp->width; y++) {
					fp_pcp08(*buf, x + _xp, y + _yp);
					buf++;
				}
			}
		}
		else if (_inv == BMP_HINV) {
			//横倒し＋水平反転（頭が左・鼻が下）▼■
			for (int x = 0; x < _bmp->height; x++) {
				for (int y = 0; y < _bmp->width; y++) {
					fp_pcp08(*buf, x + _xp, y + _yp);
					buf++;
				}
			}
		}
		else if (_inv == BMP_VINV) {
			//横倒し＋垂直反転（頭が右・鼻が上）■▲
			for (int x = _bmp->height - 1; x >= 0; x--) {
				for (int y = _bmp->width - 1; y >= 0; y--) {
					fp_pcp08(*buf, x + _xp, y + _yp);
					buf++;
				}
			}
		}
		else if (_inv == BMP_HVINV) {
			//横倒し＋水平＋垂直反転（頭が左・鼻が上）▲■
			for (int x = 0; x < _bmp->height; x++) {
				for (int y = _bmp->width - 1; y >= 0; y--) {
					fp_pcp08(*buf, x + _xp, y + _yp);
					buf++;
				}
			}
		}
	}
}	//draw_bmp_bpp08

/**
* @brief	24ビット/Pixel画像の出力
*
* @param	int _rgb	ピクセルのＲＧＢ値
* @param	int _xp		Ｘ座標（０～）
* @param	int _yp		Ｙ座標（０～）
*
*/
inline void pixel_copy24(register DWORD _rgb, register int _xx, register int _yy) {
	if ((_xx >= 0) && (_yy >= 0) && (_yy < g_FrameBufferSizeD2D.Y) && (_xx < g_FrameBufferSizeD2D.X)) {
		//画面内である
		((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx] = _rgb;
	}
}
inline void pixel_copy24_tr(register DWORD _rgb, register int _xx, register int _yy) {
	if (_rgb == g_TrRGB)	return;
	pixel_copy24(_rgb, _xx, _yy);
}

/**
* @brief	32ビット/Pixel画像の出力(透明処理有り)
*
* @param	int _rgb	ピクセルのＲＧＢ値
* @param	int _xp		Ｘ座標（０～）
* @param	int _yp		Ｙ座標（０～）
*
*/
inline void pixel_copy24_alpha(register DWORD _rgb, register int _xx, register int _yy) {
	if ((_xx >= 0) && (_yy >= 0) && (_yy < g_FrameBufferSizeD2D.Y) && (_xx < g_FrameBufferSizeD2D.X)) {
		register DWORD dest = ((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx];
		((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx] = calc_alpha_blend(dest, _rgb);
	}
}
inline void pixel_copy24_alpha_tr(register DWORD _rgb, register int _xx, register int _yy) {
	if (_rgb == g_TrRGB)	return;
	pixel_copy24_alpha(_rgb, _xx, _yy);
}

/*
* １～８ビット画像：転送モード別関数のためのポインタ
*/
void (*fp_pcp24)(register DWORD _rgb, register int _xx, register int _yy);

//------------------------------------------------
static void draw_bmp_bpp24(int _xp, int _yp, const Bmp* _bmp, int _inv, bool _tr, DWORD _tr_rgb)
{
	register const BYTE* in_buf = (const BYTE*)_bmp->pixel;	//CharRGBconvTBL[][]のindexとして扱うので、符号無しにしている。
	register DWORD _rgb = 0;
	g_TrRGB = _tr_rgb;	//透過色指定：pixel_copy24_alpha_tr,pixel_copy24_trで使用
	if ((in_buf == NULL) || (g_FrameBuffer32bitD2D == NULL)) {
		return;
	}
	//モードを見て転送関数を選択する
	if (g_AlphaBlendMode) {
		if (_tr) {
			fp_pcp24 = pixel_copy24_alpha_tr;	//αブレンドモード＋透明処理あり
		}
		else {
			fp_pcp24 = pixel_copy24_alpha;	//αブレンドモード
		}
	}
	else if (_tr) {
		fp_pcp24 = pixel_copy24_tr;	//透過処理有り
	}
	else {
		fp_pcp24 = pixel_copy24;	//透過処理なし
	}
	/*
	* 各回転反転方向別に処理
	*/
	if (_inv == 0) {
		//反転回転無し（頭が上・鼻が左）
		for (int y = 0; y < _bmp->height; y++) {
			for (int x = 0; x < _bmp->width; x++) {
				//DWORDでreadすると4byte目は隣のピクセルなのでカット⇒24bitはalpha無し
				fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
				in_buf += 3;
			}
		}
	}
	else if (_inv == BMP_HINV) {
		//水平反転のみ（頭が上・鼻が右）
		for (int y = 0; y < _bmp->height; y++) {
			for (int x = _bmp->width - 1; x >= 0; x--) {
				fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
				in_buf += 3;
			}
		}
	}
	else if (_inv == BMP_VINV) {
		//垂直反転のみ（頭が下・鼻が右）
		for (int y = _bmp->height - 1; y >= 0; y--) {
			for (int x = 0; x < _bmp->width; x++) {
				fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
				in_buf += 3;
			}
		}
	}
	else if (_inv == BMP_HVINV) {
		//水平＋垂直反転（頭が下・鼻が左）（１８０度回転）
		for (int y = _bmp->height - 1; y >= 0; y--) {
			for (int x = _bmp->width - 1; x >= 0; x--) {
				fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
				in_buf += 3;
			}
		}
	}
	else if (_inv & BMP_ROT90) {
		//横倒しになっている場合
		_inv &= (~BMP_ROT90);	//横倒しフラグは消す
		if (_inv == 0) {
			for (int x = _bmp->height - 1; x >= 0; x--) {
				for (int y = 0; y < _bmp->width; y++) {
					fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
					in_buf += 3;
				}
			}
		}
		else if (_inv == BMP_HINV) {
			//横倒し＋水平反転（頭が左・鼻が下）▼■
			for (int x = 0; x < _bmp->height; x++) {
				for (int y = 0; y < _bmp->width; y++) {
					fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
					in_buf += 3;
				}
			}
		}
		else if (_inv == BMP_VINV) {
			//横倒し＋垂直反転（頭が右・鼻が上）■▲
			for (int x = _bmp->height - 1; x >= 0; x--) {
				for (int y = _bmp->width - 1; y >= 0; y--) {
					fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
					in_buf += 3;
				}
			}
		}
		else if (_inv == BMP_HVINV) {
			//横倒し＋水平＋垂直反転（頭が左・鼻が上）▲■
			for (int x = 0; x < _bmp->height; x++) {
				for (int y = _bmp->width - 1; y >= 0; y--) {
					fp_pcp24(*((DWORD*)in_buf) & 0x00FFFFFF, x + _xp, y + _yp);
					in_buf += 3;
				}
			}
		}
	}
}	//draw_bmp_bpp24
/**
* @brief	32ビット/Pixel画像の出力
*
* @param	int _rgb	ピクセルのＲＧＢ値
* @param	int _xp		Ｘ座標（０～）
* @param	int _yp		Ｙ座標（０～）
*
*/
inline void pixel_copy32(register DWORD _rgb, register int _xx, register int _yy) {
	if ((_xx >= 0) && (_yy >= 0) && (_yy < g_FrameBufferSizeD2D.Y) && (_xx < g_FrameBufferSizeD2D.X)) {
		//画面内である：α値込みで書き込む
		((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx] = _rgb;
	}
}
inline void pixel_copy32_tr(register DWORD _rgb, register int _xx, register int _yy) {
	//α値に関係なく透明に指定されている色は描きこまない
	if ((_rgb & 0x00FFFFFF) == g_TrRGB)	return;
	pixel_copy32(_rgb, _xx, _yy);
}

/**
* @brief	32ビット/Pixel画像の出力(透明処理有り)
*
* @param	int _rgb	ピクセルのＲＧＢ値
* @param	int _xp		Ｘ座標（０～）
* @param	int _yp		Ｙ座標（０～）
*
*/
inline void pixel_copy32_alpha(register DWORD _rgb, register int _xx, register int _yy) {
	if ((_xx >= 0) && (_yy >= 0) && (_yy < g_FrameBufferSizeD2D.Y) && (_xx < g_FrameBufferSizeD2D.X)) {
		register DWORD dest = ((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx];
		((DWORD*)g_FrameBuffer32bitD2D)[_yy * g_FrameBufferSizeD2D.X + _xx] = calc_alpha_blend(dest, _rgb);
	}
}
inline void pixel_copy32_alpha_tr(register DWORD _rgb, register int _xx, register int _yy) {
	//α値に関係なく透明に指定されている色は描きこまない
	if ((_rgb & 0x00FFFFFF) == g_TrRGB)	return;
	pixel_copy32_alpha(_rgb, _xx, _yy);
}

/*
* １～８ビット画像：転送モード別関数のためのポインタ
*/
void (*fp_pcp32)(register DWORD _rgb, register int _xx, register int _yy);

//------------------------------------------------
static void draw_bmp_bpp32(int _xp, int _yp, const Bmp* _bmp, int _inv, bool _tr, DWORD _tr_rgb)
{
	register const DWORD* in_buf = (const DWORD*)_bmp->pixel;
	g_TrRGB = _tr_rgb;
	if ((in_buf == NULL) || (g_FrameBuffer32bitD2D == NULL)) {
		return;
	}

	//モードを見て転送関数を選択する
	if (g_AlphaBlendMode) {
		if (_tr) {
			fp_pcp32 = pixel_copy32_alpha_tr;	//αブレンドモード＋透明処理あり
		}
		else {
			fp_pcp32 = pixel_copy32_alpha;	//αブレンドモード
		}
	}
	else if (_tr) {
		fp_pcp32 = pixel_copy32_tr;	//透過処理有り
	}
	else {
		fp_pcp32 = pixel_copy32;	//透過処理なし
	}

	if (_inv == 0) {
		//反転回転無し（頭が上・鼻が左）
		for (int y = 0; y < _bmp->height; y++) {
			for (int x = 0; x < _bmp->width; x++) {
				fp_pcp32(*in_buf, x + _xp, y + _yp);
				in_buf++;
			}
		}
	}
	else if (_inv == BMP_HINV) {
		//水平反転のみ（頭が上・鼻が右）
		for (int y = 0; y < _bmp->height; y++) {
			for (int x = _bmp->width - 1; x >= 0; x--) {
				fp_pcp32(*in_buf, x + _xp, y + _yp);
				in_buf++;
			}
		}
	}
	else if (_inv == BMP_VINV) {
		//垂直反転のみ（頭が下・鼻が右）
		for (int y = _bmp->height - 1; y >= 0; y--) {
			for (int x = 0; x < _bmp->width; x++) {
				fp_pcp32(*in_buf, x + _xp, y + _yp);
				in_buf++;
			}
		}
	}
	else if (_inv == BMP_HVINV) {
		//水平＋垂直反転（頭が下・鼻が左）（１８０度回転）
		for (int y = _bmp->height - 1; y >= 0; y--) {
			for (int x = _bmp->width - 1; x >= 0; x--) {
				fp_pcp32(*in_buf, x + _xp, y + _yp);
				in_buf++;
			}
		}
	}
	else if (_inv & BMP_ROT90) {
		//横倒しになっている場合
		_inv &= (~BMP_ROT90);	//横倒しフラグは消す
		if (_inv == 0) {
			//横倒しのみ（頭が右・鼻が下）■▼
			for (int x = _bmp->height - 1; x >= 0; x--) {
				for (int y = 0; y < _bmp->width; y++) {
					fp_pcp32(*in_buf, x + _xp, y + _yp);
					in_buf++;
				}
			}
		}
		else if (_inv == BMP_HINV) {
			//横倒し＋水平反転（頭が左・鼻が下）▼■
			for (int x = 0; x < _bmp->height; x++) {
				for (int y = 0; y < _bmp->width; y++) {
					fp_pcp32(*in_buf, x + _xp, y + _yp);
					in_buf++;
				}
			}
		}
		else if (_inv == BMP_VINV) {
			//横倒し＋垂直反転（頭が右・鼻が上）■▲
			for (int x = _bmp->height - 1; x >= 0; x--) {
				for (int y = _bmp->width - 1; y >= 0; y--) {
					fp_pcp32(*in_buf, x + _xp, y + _yp);
					in_buf++;
				}
			}
		}
		else if (_inv == BMP_HVINV) {
			//横倒し＋水平＋垂直反転（頭が左・鼻が上）▲■
			for (int x = 0; x < _bmp->height; x++) {
				for (int y = _bmp->width - 1; y >= 0; y--) {
					fp_pcp32(*in_buf, x + _xp, y + _yp);
					in_buf++;
				}
			}
		}
	}
}	//draw_bmp_bpp32

/**
* @brief	画像の出力
*
* @param	int _xp：表示座標（０～）
* @param	int _yp：表示座標（０～）
* @param	Bmp _bmp：Bmp構造体へのポインタ
* @param	int _inv：反転フラグ：0=反転無し/BMP_HINV=水平反転/BMP_VINV=垂直反転/BMP_HVINV=水平垂直反転(１８０°回転)
* @param	bool _tr：透明フラグ：true=透明/false=不透明　--- '０'の部分を描きこまないことで透明処理をする。
* @param	DWORD _tr_code：透明色の指定：１～４ビット画像の場合はパレット番号を指定する。２４・３２ビット画像の場合は透過色のＲＧＢ値をDWORD（RGBQUAD）で指定する
*
* @note		Bmp画像の色数を自動判定して描画している
*/
void DrawBmp(int _xp, int _yp, const Bmp* _bmp, int _inv, bool _tr, DWORD _tr_code)
{
	_ASSERT(_bmp);
	if (_bmp == NULL) { return; }
	if (((_xp + _bmp->width - 1) < 0) || (_xp >= g_FrameBufferSizeD2D.X) ||
		((_yp + _bmp->height - 1) < 0) || (_yp >= g_FrameBufferSizeD2D.Y)) {
		return;	//画面外は処理しない
	}
	if (_bmp->colbit == 4) {
		draw_bmp_bpp08(_xp, _yp, _bmp, _inv, _tr, (int)_tr_code);
	}
	else if (_bmp->colbit == 8) {
		draw_bmp_bpp08(_xp, _yp, _bmp, _inv, _tr, (int)_tr_code);
	}
	else if (_bmp->colbit == 24) {
		draw_bmp_bpp24(_xp, _yp, _bmp, _inv, _tr, _tr_code);
	}
	else if (_bmp->colbit == 32) {
		draw_bmp_bpp32(_xp, _yp, _bmp, _inv, _tr, _tr_code);
	}
	//透明用変数のリセット（透明処理は１回の描画毎に指定する）
	g_ptrRGBQ = g_PaletteD2D;	//パレット用ポインタデフォルトに戻す
	g_TrCC = 0;	//透明パレットを０番に戻しておく
	g_TrRGB = 0;	//透過色を０に戻しておく
}	//DrawBmp
/**
* @brief	画像の出力
*
* @param	int _xp：表示座標（０～）
* @param	int _yp：表示座標（０～）
* @param	Bmp _bmp：Bmp構造体へのポインタ
* @param	bool _tr：透明フラグ：true=透明/false=不透明　--- '０'の部分を描きこまないことで透明処理をする。
*
* @note		Bmp画像の色数を自動判定して描画している
*/
void DrawBmp(int _xp, int _yp, const Bmp* _bmp, bool _tr, DWORD _tr_code)
{
	DrawBmp(_xp, _yp, _bmp, 0, _tr, _tr_code);
}
//################################################################################################################################
//################################################################################################################################
//################################################################################################################################

/*
* @brief	座標の回転（右回り）
*			_x,_yに指定した座標を回転させて返す
*
* @param	int _cx,_cy		回転の中心座標
* @param	int& _x,_y		回転させる座標（参照型）
* @param	double _angle	回転させる角度
*
* @return	無し
*/
void Rotate2d(double _cx, double _cy, double& _x, double& _y, double _angle)
{
	_x -= _cx;
	_y -= _cy;
	//回転
	double theta = (double)(M_PI * _angle / 180.0);
	double x = (_x * std::cos(theta)) - (_y * std::sin(theta));
	double y = (_x * std::sin(theta)) + (_y * std::cos(theta));
	_x = x + _cx;
	_y = y + _cy;
}
void Rotate2d(double _cx, double _cy, POINT_F& _p, double _angle)
{
	Rotate2d(_cx, _cy, _p.x, _p.y, _angle);
}
/*
* @brief	Bmp画像の中の指定位置のピクセルを取得
*
* @param	Bmp* _p			Bmp画像へのポインタ
* @param	double _x,_y	画像内の点の位置
* @param	DWORD& _pix		取得した点の情報が入る３２ビット変数の参照
*							指定位置の32ビットピクセル値がこのDWORD変数に入る
*							（エラーの場合は内容の変化なし）
*
* @return	bool			取得結果：true=取得出来た／false=範囲外で取得不可能
*/
bool GetPixel(const Bmp* _p, double _x, double _y, DWORD& _pix)
{
	if (_x < 0)	return false;
	if (_y < 0)	return false;
	if (_x >= _p->width)	return false;
	if (_y >= _p->height)	return false;
	int ix = (int)(_x + 0);
	int iy = (int)(_y + 0);
	_pix = ((DWORD*)_p->pixel)[iy * _p->width + ix];
	return true;
}
bool GetPixel24(const Bmp* _p, double _x, double _y, DWORD& _pix)
{
	if (_x < 0)	return false;
	if (_y < 0)	return false;
	if (_x >= _p->width)	return false;
	if (_y >= _p->height)	return false;
	int ix = (int)(_x + 0);
	int iy = (int)(_y + 0);
	//_pix = ((DWORD*)_p->pixel)[iy * _p->width + ix];
	_pix = (*((DWORD*)(&_p->pixel[(iy * _p->width + ix) * 3]))) & 0x00FFFFFF;
	return true;
}
/*
* @brief	Bmp画像の中の指定位置のピクセルを取得
*
* @param	Bmp* _p			Bmp画像へのポインタ
* @param	double _x,_y	画像内の点の位置
* @param	BYTE& _cc		取得した点の情報が入るパレット番号変数の参照
*							指定位置のパレット番号（８ビット値）がこのBYTE変数に入る
*							（エラーの場合は内容の変化なし）
*
* @return	bool			取得結果：true=取得出来た／false=範囲外で取得不可能
*/
bool GetPixel(const Bmp* _p, double _x, double _y, BYTE& _cc)
{
	if (_x < 0)	return false;
	if (_y < 0)	return false;
	if (_x >= _p->width)	return false;
	if (_y >= _p->height)	return false;
	int ix = (int)(_x + 0);
	int iy = (int)(_y + 0);
	_cc = ((BYTE*)_p->pixel)[iy * _p->width + ix];
	return true;
}

/*
* @brief	画像の回転・拡大・縮小
*			画像の回転拡大縮小を行って、指定の座標を中心に描画する。
*			※回転拡大縮小の中心は常に画像の中心点となる
*
* @param	Bmp* _bmp	Bmp画像のポインタ
* @param	int _ncx, _ncy	回転・拡大・縮小した画像の表示位置：個の指定位置を中心に表示される
* @param	double _angle	回転角度（ー３６０～０～３６０）（右回り）
* @param	double _scale	倍率：浮動小数点最小値～浮動小数点最大値
* @param	int _hvinv		水平垂直反転フラグ（９０回転フラグは無視される）
* @param	bool _tr		透過処理フラグ（true:有り／false:無し）
* @param	DWORD _tr_code	透過処理がtrueの時、
*							１・４・８ビットのパレット色画像の場合は透過したいパレットの番号
*							２４ビット32ビット画像の場合は透過したいＲＧＢ値（DWORD:0x00RRGGBB）
*/
#define	SCALE_MIN	(1.0/1000.0)
#define	SCALE_MAX	(1.0*1000.0)
#define	ANGLE_MIN	(1.0/100.0)
#define	ANGLE_MAX	(360.0)
void DrawBmpEx(const Bmp* _bmp, int _cx, int _cy, double _angle, double _scale, int _hvinv, bool _tr, DWORD _tr_code)
{
	//倍率は制限値の範囲を超えない様に補正する
	if (_scale < SCALE_MIN) {
		_scale = SCALE_MIN;
	}
	else if (_scale > SCALE_MAX) {
		_scale = SCALE_MAX;
	}
	//正の数が右回りになるように反転しておく。
	_angle = (0 - _angle);
	//角度は倍精度浮動小数点の限界値を超えないようにする
	//※但し０は０のままにしておく
	if (_angle > 0) {
		//０より大きい正の数
		if (_angle >= 360.0) {
			//360.0未満にする
			_angle -= (double)((int)(_angle / 360.0) * (int)360);
		}
		if (_angle != 0) {
			if (_angle < ANGLE_MIN) {
				_angle = ANGLE_MIN;
			}
		}
	}
	else if (_angle < 0) {
		//０より小さい負の数
		_angle = abs(_angle);	//一旦正の数にする
		//正の数と同じ補正を行う
		if (_angle >= 360.0) {
			//360.0未満にする
			_angle -= (double)((int)(_angle / 360.0) * (int)360);
		}
		if (_angle != 0) {
			if (_angle < ANGLE_MIN) {
				_angle = ANGLE_MIN;
			}
		}
		_angle = (0 - _angle);	//負の数に戻す
	}

	/*
	* 反転フラグによる回転角の補正
	*/
	_hvinv &= (~BMP_ROT90);	//90度回転フラグは無効（消す）
	if ((_hvinv & BMP_HINV) && (_hvinv & BMP_VINV)) {
		//水平垂直反転は１８０回転にする
		_angle += 180.0;	//
		_hvinv = 0;	//反転無し
	}
	else if ((_hvinv & BMP_HINV) || (_hvinv & BMP_VINV)) {
		//垂直か水平どちらか単独で反転の場合は、
		//回転結果も反転するので反転していない時と同じなるように回転方向を反転させておく。
		_angle = 0 - _angle;	//回転方向反転
	}

	/*
	* 対角線の長さは現在の画像を３６０度回転させた場合に全ての頂点を含む円の直径である。
	*/
	//double 対角線の長さ = sqrt(pow(_bmp->width, 2) + pow(_bmp->height, 2));
	//double 画像の幅 = _bmp->width;
	//double 画像の高さ = _bmp->height;
	//double 回転させたい点X = (0-((対角線の長さ - 画像の幅) / 2));
	//double 回転させたい点Y = (0-((対角線の長さ - 画像の高さ) / 2));
	//double 回転の中心X = 画像の幅 /2;
	//double 回転の中心Y = 画像の高さ /2;
	//２次元の回転(回転の中心X, 回転の中心Y, 回転させたい点X, 回転させたい点Y,回転角度);
	//double スキャン開始する画像左上からの相対位置X = 四捨五入(回転させたい点X);
	//double スキャン開始する画像左上からの相対位置Y = 四捨五入(回転させたい点Y);
	double DiagonalLength = sqrt(pow(_bmp->width, 2) + pow(_bmp->height, 2));
	double ImageWidth = _bmp->width;
	double ImageHeight = _bmp->height;
	double RotatePointX = (0 - ((DiagonalLength - ImageWidth) / 2));
	double RotatePointY = (0 - ((DiagonalLength - ImageHeight) / 2));
	double CenterOfRotationX = ImageWidth / 2;
	double CenterOfRotationY = ImageHeight / 2;
	Rotate2d(CenterOfRotationX, CenterOfRotationY, RotatePointX, RotatePointY, _angle);
	double rotate_org_X = std::round(RotatePointX);
	double rotate_org_Y = std::round(RotatePointY);
	//描画領域の幅(WW)高さ(HH)＝対角線の長さ×倍率
	int WW = (int)(DiagonalLength * _scale);
	int HH = (int)(DiagonalLength * _scale);
#ifdef DEBUG_BOX_BMPEX
	//描画範囲となっている矩形の枠線描画
	DrawRect(_ncx, _ncy, _ncx + WW - 1, _ncy + HH - 1, RGBQUAD{ 255,255,0,0 }, false);
	DrawLine(_ncx, _ncy, _ncx + WW - 1, _ncy + HH - 1, RGBQUAD{ 255,255,0,0 });
	DrawLine(_ncx, _ncy + HH - 1, _ncx + WW - 1, _ncy, RGBQUAD{ 255,255,0,0 });
	DrawLineH(_ncx, _ncy + (HH / 2) - 1, _ncx + WW - 1, RGBQUAD{ 255,255,0,0 });
	DrawLineV(_ncx + (WW / 2) - 1, _ncy, _ncy + HH - 1, RGBQUAD{ 255,255,0,0 });
#endif // DEBUG_BOX_BMPEX
	/*
	* 反転フラグによるオフセット座標の補正
	*/
	int off_x = (WW / 2);
	int off_y = (HH / 2);
	if (_hvinv & BMP_HINV) {
		off_x = 0 - off_x + 1;	//NORMAL
	}
	if (_hvinv & BMP_VINV) {
		off_y = 0 - off_y + 1;	//NORMAL
	}
	off_x = (_cx - off_x);	//
	off_y = (_cy - off_y);	//
	//X方向に1px進んだ時の元画像を求める為のsinΘとcosΘ計算しておく
	double theta = (double)(M_PI * _angle / 180.0);
	double cosTheta = std::cos(theta);
	double sinTheta = std::sin(theta);
	int o_idx = 0;	//x,y{0,0}描画範囲の左上を{0,0}としたピクセル単位の表示位置
	double pixel_scale = (1.0 / _scale);
	if (_bmp->colbit < 24) {
		//パレットのある画像の時
		g_ptrRGBQ = palette_ptr_set(_bmp);
		if (_tr) {
			//透明在り
			g_TrCC = (int)_tr_code;	//透過パレット番号指定
			for (int y = 0; y < HH; y++) {
				for (int x = 0; x < WW; x++) {
					double sx = (cosTheta * x - sinTheta * y);
					double sy = (sinTheta * x + cosTheta * y);
					sx *= pixel_scale;
					sy *= pixel_scale;
					//double sx = (cosTheta * x - sinTheta * y) * (1.0/_scale);
					//double sy = (sinTheta * x + cosTheta * y) * (1.0/_scale);
					BYTE cc = 0;
					bool f = GetPixel(_bmp, sx + rotate_org_X, sy + rotate_org_Y, cc);
					if (f && (cc != g_TrCC)) {
						RGBQUAD rgbq = g_ptrRGBQ[cc];
						int ix = o_idx % WW;
						int iy = o_idx / WW;
						if (_hvinv & BMP_HINV) { ix = 0 - ix; }	//HINV
						if (_hvinv & BMP_VINV) { iy = 0 - iy; }	//VINV
						DrawPixel(off_x + ix, off_y + iy, rgbq);
					}
					o_idx++;
				}
			}
		}
		else {
			//透過無し
			for (int y = 0; y < HH; y++) {
				for (int x = 0; x < WW; x++) {
					double sx = (cosTheta * x - sinTheta * y);
					double sy = (sinTheta * x + cosTheta * y);
					sx *= pixel_scale;
					sy *= pixel_scale;
					//double sx = (cosTheta * x - sinTheta * y) * (1.0/_scale);
					//double sy = (sinTheta * x + cosTheta * y) * (1.0/_scale);
					BYTE cc = 0;
					bool f = GetPixel(_bmp, sx + rotate_org_X, sy + rotate_org_Y, cc);
					if (f) {
						RGBQUAD rgbq = g_ptrRGBQ[cc];
						int ix = o_idx % WW;
						int iy = o_idx / WW;
						if (_hvinv & BMP_HINV) { ix = 0 - ix; }	//HINV
						if (_hvinv & BMP_VINV) { iy = 0 - iy; }	//VINV
						DrawPixel(off_x + ix, off_y + iy, rgbq);
					}
					o_idx++;
				}
			}
		}
	}
	else if (_bmp->colbit == 24) {
		//24ビットの画像
		if (_tr) {
			//透過処理有り
			g_TrRGB = _tr_code;
			for (int y = 0; y < HH; y++) {
				for (int x = 0; x < WW; x++) {
					double sx = (cosTheta * x - sinTheta * y);
					double sy = (sinTheta * x + cosTheta * y);
					sx *= pixel_scale;
					sy *= pixel_scale;
					//double sx = (cosTheta * x - sinTheta * y) * (1.0/_scale);
					//double sy = (sinTheta * x + cosTheta * y) * (1.0/_scale);
					DWORD pix = 0;
					bool f = GetPixel24(_bmp, sx + rotate_org_X, sy + rotate_org_Y, pix);
					if (f && (pix != g_TrRGB)) {
						RGBQUAD rgbq = *((RGBQUAD*)&pix);
						int ix = o_idx % WW;
						int iy = o_idx / WW;
						if (_hvinv & BMP_HINV) { ix = 0 - ix; }	//HINV
						if (_hvinv & BMP_VINV) { iy = 0 - iy; }	//VINV
						DrawPixel(off_x + ix, off_y + iy, rgbq);
					}
					o_idx++;
				}
			}
		}
		else {
			for (int y = 0; y < HH; y++) {
				for (int x = 0; x < WW; x++) {
					double sx = (cosTheta * x - sinTheta * y);
					double sy = (sinTheta * x + cosTheta * y);
					sx *= pixel_scale;
					sy *= pixel_scale;
					//double sx = (cosTheta * x - sinTheta * y) * (1.0/_scale);
					//double sy = (sinTheta * x + cosTheta * y) * (1.0/_scale);
					DWORD pix = 0;
					bool f = GetPixel24(_bmp, sx + rotate_org_X, sy + rotate_org_Y, pix);
					if (f) {
						RGBQUAD rgbq = *((RGBQUAD*)&pix);
						int ix = o_idx % WW;
						int iy = o_idx / WW;
						if (_hvinv & BMP_HINV) { ix = 0 - ix; }	//HINV
						if (_hvinv & BMP_VINV) { iy = 0 - iy; }	//VINV
						DrawPixel(off_x + ix, off_y + iy, rgbq);
					}
					o_idx++;
				}
			}
		}
	}
	else if (_bmp->colbit == 32) {
		//32ビットの画像
		if (_tr) {
			//透過処理有り
			g_TrRGB = _tr_code;
			for (int y = 0; y < HH; y++) {
				for (int x = 0; x < WW; x++) {
					double sx = (cosTheta * x - sinTheta * y);
					double sy = (sinTheta * x + cosTheta * y);
					sx *= pixel_scale;
					sy *= pixel_scale;
					//double sx = (cosTheta * x - sinTheta * y) * (1.0/_scale);
					//double sy = (sinTheta * x + cosTheta * y) * (1.0/_scale);
					DWORD pix = 0;
					bool f = GetPixel(_bmp, sx + rotate_org_X, sy + rotate_org_Y, pix);
					if (f && ((pix & 0x00FFFFFF) != g_TrRGB)) {
						RGBQUAD rgbq = *((RGBQUAD*)&pix);
						int ix = o_idx % WW;
						int iy = o_idx / WW;
						if (_hvinv & BMP_HINV) { ix = 0 - ix; }	//HINV
						if (_hvinv & BMP_VINV) { iy = 0 - iy; }	//VINV
						DrawPixel(off_x + ix, off_y + iy, rgbq);
					}
					o_idx++;
				}
			}
		}
		else {
			for (int y = 0; y < HH; y++) {
				for (int x = 0; x < WW; x++) {
					double sx = (cosTheta * x - sinTheta * y);
					double sy = (sinTheta * x + cosTheta * y);
					sx *= pixel_scale;
					sy *= pixel_scale;
					//double sx = (cosTheta * x - sinTheta * y) * (1.0/_scale);
					//double sy = (sinTheta * x + cosTheta * y) * (1.0/_scale);
					DWORD pix = 0;
					bool f = GetPixel(_bmp, sx + rotate_org_X, sy + rotate_org_Y, pix);
					if (f) {
						RGBQUAD rgbq = *((RGBQUAD*)&pix);
						int ix = o_idx % WW;
						int iy = o_idx / WW;
						if (_hvinv & BMP_HINV) { ix = 0 - ix; }	//HINV
						if (_hvinv & BMP_VINV) { iy = 0 - iy; }	//VINV
						DrawPixel(off_x + ix, off_y + iy, rgbq);
					}
					o_idx++;
				}
			}
		}
	}
#ifdef DEBUG_BOX_BMPEX
	//描画範囲となっている矩形の枠線描画
	RECT r{ _ncx,_ncy,_ncx + WW - 1, _ncy + HH - 1 };
	MoveRect(&r, -(WW / 2), -(HH / 2));
	LINE line0{ r.left,r.top ,r.right,r.bottom };
	LINE line1{ r.left,r.bottom ,r.right,r.top };
	LINE lineH{ r.left, r.top + (HH / 2) - 1, r.right, r.top + (HH / 2) - 1 };
	LINE lineV{ r.left + (WW / 2) - 1, r.top, r.left + (WW / 2) - 1,r.bottom };
	DrawRect(r, RGBQUAD{ 255,255,0,0 }, false);
	//DrawLine(_ncx, _ncy, _ncx + WW - 1, _ncy + HH - 1, RGBQUAD{ 255,255,0,0 });
	DrawLine(line0, RGBQUAD{ 255,255,0,0 });
	DrawLine(line1, RGBQUAD{ 255,255,0,0 });
	DrawLineH(lineH.x1, lineH.y1, lineH.x2, RGBQUAD{ 255,255,0,0 });
	DrawLineV(lineV.x1, lineV.y1, lineV.y2, RGBQUAD{ 255,255,0,0 });
#endif // DEBUG_BOX_BMPEX
	g_TrRGB = 0;	//透過色０にリセット
	g_TrCC = 0;		//透過パレット番号０にリセット
	g_ptrRGBQ = g_PaletteD2D;	//パレット用ポインタデフォルトに戻す
}	//RotateScaleBmp

//################################################################################################################################
//################################################################################################################################
// ビットマップファイル操作ユーティリティー
//################################################################################################################################
//################################################################################################################################
/*
* @note	パレットの型について：
*	COLORREF型をメモリ上の並びを
*		BYTE[]で読み取ると{[0]Red,[1]Green,[2]Blue,[3]0}となり
*		DWORD型で読み取ると{0x00BBGGRR}
*	となる。
*	RGBQUAD型をメモリ上の並びを
*		BYTE[]で読み取ると{[0]Blue,[1]Green,[2]Red,[3]0}となり
*		DWORD型で読み取ると{0x00RRGGBB}
*	となる。
*
* @note	24/32ピクセルのデータの並びについて：
*	BITMAPのピクセルデータ{24bit/pixel|32bit/pixel}は,
*		24bit/pixelの場合BYTE[3]{Blue,Green,Red}/DWORD{0x**RRGGBB}
*		32bit/pixelの場合BYTE[4]{Blue,Green,Blue,alpha?}/DWORD{0xaaRRGGBB}
*	となっている。
*
*/

//################################################################################################################################
//################################################################################################################################
/*
* @brief	ＷＩＣイメージからＤＩＢビットマップを生成し、そのハンドルを返す
*
* @param	IWICBitmapSource* _p_bmpsrc	ＷＩＣイメージへのポインタ
*/
static HBITMAP WIC_to_DIB(IWICBitmapSource* _p_bmpsrc)
{
	//フォーマット（32bppBGRA）を確認
	WICPixelFormatGUID pixel_format;
	HRESULT hr = _p_bmpsrc->GetPixelFormat(&pixel_format);
	_ASSERT(hr == S_OK);
	_ASSERT_EXPR(pixel_format == GUID_WICPixelFormat32bppBGRA, _T("このファイルは３２ビットＢＧＲＡ画像ではない"));
	// ビットマップサイズを取得
	UINT width, height;
	hr = _p_bmpsrc->GetSize(&width, &height);
	_ASSERT(hr == S_OK);
	//ＤＩＢ画像（デバイス依存しない独立したビットマップ）を作って、そのハンドルHBITMAPを取得する。
	_ASSERT(hr == S_OK);
	//生成するDIBの属性を設定する
	BITMAPINFO bmi; 		  // ビットマップヘッダ
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);	//ヘッダサイズ
	bmi.bmiHeader.biBitCount = 32;	//色深度（ビット数／ピクセル）
	bmi.bmiHeader.biCompression = BI_RGB;	//非圧縮のRGB値で出来た画像
	bmi.bmiHeader.biWidth = width;	//画像の幅ピクセル数
	bmi.bmiHeader.biHeight = -static_cast<LONG>(height);	//画像の高さ（負数にすると画像データの並びがY方向上から下に向かう並びになる）
	bmi.bmiHeader.biPlanes = 1;	//プレーンは１枚
	//ＤＩＢ画像生成
	HDC hdc = GetDC(HWND_DESKTOP);	//ウィンドウハンドルはデスクトップ全体（NULLでもよい）
	LPVOID ptr_pixel;		//ピクセルバッファ
	HBITMAP hBmp = NULL;	//ハンドル
	hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &ptr_pixel, NULL, 0);
	_ASSERT(hBmp != NULL);
	ReleaseDC(HWND_DESKTOP, hdc);	//DC解放
	//IWICBitmapSource　から　hBmpに確保したptr_pixel（VOID[]）にイメージをコピー
	hr = _p_bmpsrc->CopyPixels(NULL, width * sizeof(DWORD), width * height * sizeof(DWORD), reinterpret_cast<BYTE*>(ptr_pixel));
	_ASSERT(hr == S_OK);
	// ビットマップハンドルを返す
	return hBmp;
}	//WIC_to_DIB

/*
* @brief	画像イメージからＷＩＣ(Windows イメージング コンポーネント)のビットマップを生成する
*
* @parama	const BYTE* _ptr_img
* @param	const int _img_size		画像イメージのバイト数
*
* @return	IWICBitmapSource*		ＷＩＣのビットマップソースへのポインタ
*									※呼び出した側で"->Release()"する事
*/
static IWICBitmapSource* create_bmp_src(const BYTE* _ptr_img, const int _img_size)
{
	// COMの初期化
	CoInitialize(NULL);	//CoCreateInstance()前に必要⇒無いとCoCreateInstance()がエラーになる。
	// ＷＩＣファクトリ生成
	IWICImagingFactory* pFactory = NULL;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (void**)(&pFactory));

	//ファクトリからＷＩＣストリームを生成
	IWICStream* pStream = NULL;
	hr = pFactory->CreateStream(&pStream);
	_ASSERT(hr == S_OK);
	//画像イメージストリームを生成：イメージデータをストリームに設定
	hr = pStream->InitializeFromMemory(reinterpret_cast<WICInProcPointer>(const_cast<BYTE*>(_ptr_img)), _img_size);
	_ASSERT(hr == S_OK);
	//ストリームからデコーダ生成
	IWICBitmapDecoder* pDecoder = NULL;
	hr = pFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &pDecoder);
	_ASSERT(hr == S_OK);
	//デコーダからフレーム取得（＃０：第１フレーム）
	IWICBitmapFrameDecode* pFrame = NULL;
	hr = pDecoder->GetFrame(0, &pFrame);	//＃０：第１フレーム
	_ASSERT(hr == S_OK);
	//ファクトリからコンバータ取得：変換処理用オブジェクト
	IWICFormatConverter* pConverter = NULL;
	hr = pFactory->CreateFormatConverter(&pConverter);
	_ASSERT(hr == S_OK);
	//コンバータ初期化：取得したフレームを元に「３２ビットのＢＧＲＡ画像」用コンバータとして初期化
	hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
	_ASSERT(hr == S_OK);
	//コンバータでビットマップソースイメージに変換して取得
	IWICBitmapSource* ptr_bmp_src = NULL;
	//hr = pConverter->QueryInterface(IID_PPV_ARGS(&pImage));
	hr = pConverter->QueryInterface(IID_IWICBitmapSource, (void**)(&ptr_bmp_src));
	_ASSERT(hr == S_OK);

	//後始末：使ったオブジェクトの解放
	if (pConverter != NULL) { pConverter->Release(); }	//コンバーター
	if (pFrame != NULL) { pFrame->Release(); }		//フレーム
	if (pDecoder != NULL) { pDecoder->Release(); }	//デコーダー
	if (pStream != NULL) { pStream->Release(); }	//ストリーム
	if (pFactory != NULL) { pFactory->Release(); }	//ファクトリー
	//イメージデータからビットマップに変換⇒ビットマップハンドルを返す
	return ptr_bmp_src;
}	//create_bmp_src

/*
* @brief	HBITMAPからBmpオブジェクトを作成する
*
* @param	const HBITMAP _h_bmp	ビットマップのハンドル
*
* @return	Bmp*	Bmp画像へのポインタ
*/
static Bmp* create_bmp_from_HBITMAP(const HBITMAP _h_bmp)
{
	// BITMAP情報を取得する
	BITMAP bmp = {};
	int byte_size = GetObject(_h_bmp, sizeof(bmp), &bmp);
	_ASSERT(byte_size != 0);
	HDC hdc = GetDC(NULL);	//NULL⇒ウィンドウを指定せず、画面全体の DC を取得。
	HDC hdc_mem = CreateCompatibleDC(hdc);	//メモリDC作成
	SelectObject(hdc_mem, _h_bmp);	//メモリDC選択中にする
	//Bmp画像のパラメータ設定
	Bmp* ptrBmp = (Bmp*)calloc(1, sizeof(Bmp));
	ptrBmp->colbit = bmp.bmBitsPixel;	//32のハズ
	ptrBmp->numpal = 0;			//パレット無し
	ptrBmp->pal = nullptr;		//パレット無し
	ptrBmp->pal_rgb = nullptr;	//パレット無し
	ptrBmp->width = (int)bmp.bmWidth;	//幅（ピクセル数）
	ptrBmp->height = (int)bmp.bmHeight;	//高さ（ピクセル数）
	ptrBmp->numpix = (int)(bmp.bmWidthBytes * bmp.bmHeight);	//画像のバイト数
	ptrBmp->swapRB = false;		//Blue<->Redの交換無し
	ptrBmp->aaLv = 0;			//文字画像データ用：アンチエイリアスレベル０
	ptrBmp->wch = (wchar_t)0;	//文字画像データ用：文字コード（Unicode）
	ptrBmp->pixel = (char*)calloc(ptrBmp->numpix, sizeof(char));	//画像の格納場所（バッファ）確保
	memcpy_s(ptrBmp->pixel, ptrBmp->numpix, bmp.bmBits, ptrBmp->numpix);	//bmpのピクセルデータをコピーして取り込む
	DeleteDC(hdc_mem);		//メモリDC削除
	ReleaseDC(NULL, hdc);	//デバイスコンテキスト(NULL⇒画面全体の DC)を解放。
	//
	return ptrBmp;
}	//create_bmp_from_HBITMAP

/*
* @brief	画像ファイルを読み込んでBmp画像を作成して返す。
*
* @param	const HANDLE _fh	画像ファイルのファイルハンドル。
*
* @return	Bmp*	読み込んだ画像のBmp画像へのポインタ
*					※受け取った側でDeletBmp(Bmp**)による削除が必要
*/
static Bmp* load_bmp_from_image_file(const HANDLE _fh)
{
	if (_fh == INVALID_HANDLE_VALUE) {
		return (Bmp*)NULL;
	}
	LONG data_size = GetFileSize(_fh, NULL);	//ファイル全体のバイト数を取得
	BYTE* ptr_data = (BYTE*)calloc(data_size, sizeof(BYTE));
	DWORD rsiz = 0;
	BOOL rc = ReadFile(_fh, ptr_data, data_size, &rsiz, NULL);	//ファイル全体をBYTE[]に読み込む
	_ASSERT(rc);
	//読み込んだデータ(BYTE配列)をＷＩＣのビットマップイメージとして生成する
	IWICBitmapSource* pImage = create_bmp_src(ptr_data, data_size);
	_ASSERT(pImage != NULL);
	//ＷＩＣのソースイメージからDIBビットマップに変換してそのハンドルを取得
	HBITMAP hBitmap = WIC_to_DIB(pImage);
	_ASSERT(hBitmap != NULL);
	//pImageは不要になったので解放しておく
	pImage->Release();
	pImage = NULL;
	//元データも変換が終わったので解放しておく
	free(ptr_data);
	ptr_data = NULL;
	//ビットマップ(HBITMAP)からBmp画像を取得
	Bmp* pBmp = create_bmp_from_HBITMAP(hBitmap);
	//Bmpに取り込んだので元になったビットマップ(HBITMAP)を削除する
	DeleteObject(hBitmap);
	return pBmp;
}	//load_bmp_from_image_file
/*
* @brief	画像ファイルを読み込んでBmp画像を作成して返す。
*			※返されたポインタ(Bmp*)は呼び出し側で解放する事
*
* @param	const wchar_t* _filename	読み込みたいファイル名（Unicode文字）
*
* @return	Bmp*	読み込んだ画像のBmp画像へのポインタ
*					※受け取った側でDeletBmp(Bmp**)による削除が必要
*/
Bmp* LoadBmpFromImgFileW(const wchar_t* _filename)
{
	HANDLE hFile = CreateFileW(_filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	Bmp* ptr = load_bmp_from_image_file(hFile);
	CloseHandle(hFile);
	return ptr;
}	//LoadBmpFromImgFileW
/*
* @brief	画像ファイルを読み込んでBmp画像を作成して返す。
*			※返されたポインタ(Bmp*)は呼び出し側で解放する事
*
* @param	const char* _filename	読み込みたいファイル名（マルチバイト文字）
*
* @return	Bmp*	読み込んだ画像のBmp画像へのポインタ
*					※受け取った側でDeletBmp(Bmp**)による削除が必要
*/
Bmp* LoadBmpFromImgFileA(const char* _filename)
{
	HANDLE hFile = CreateFileA(_filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	Bmp* ptr = load_bmp_from_image_file(hFile);
	CloseHandle(hFile);
	return ptr;
}	//LoadBmpFromImgFileA

/**
* @brief	画像ファイルの読込：BMP以外の形式も読込可能
*
* @param	const char* _path	画像ファイル名
* 
* @return	Bmp*	正常に読み込めた場合はBmp構造体のポインタを返す。\n
*	【注】正常に返されたポインタは、使い終わったら必ずDeleteBmp(Bmp*)を呼び出して削除する事。
*/
Bmp* LoadBmpExW(const wchar_t* _filename)
{
	return LoadBmpFromImgFileW(_filename);
}
Bmp* LoadBmpExA(const char* _filename)
{
	return LoadBmpFromImgFileA(_filename);
}

/*
* @brief	画像の分割読込：BMP以外の形式も読込可能
*
* @param	const char* _path	分割元になる画像ファイル名
* @param	int _x0,_y0		分割開始する座標
* @param	int _xpix,_ypix	分割するセル画像１個の幅と高さ
* @param	int _xcount		横方向の個数
* @param	int _ycount		縦方向の個数
* @param	Bmp** _pp_bmp	分割したBmp*を入れる配列のアドレス
*
* @return	bool：読込の成否（true:成功/false:失敗）
*/
bool LoadDivBmpA(const char* _path, int _x0, int _y0, size_t _xpix, size_t _ypix, size_t _xcount, size_t _ycount, Bmp** _pp_bmp)
{
	if ((_path == NULL) || (_pp_bmp == NULL)) {
		return false;
	}
	if ((_xpix * _ypix * _xcount * _ycount) == 0) {
		return false;
	}
	Bmp* ptr = LoadBmpFromImgFileA(_path);
	if (!ptr) {
		return false;
	}
	//Bmp** pp = new Bmp * [_xcount * _ycount];
	for (int y = 0; y < _ycount; y++) {
		for (int x = 0; x < _xcount; x++) {
			_pp_bmp[y * _xcount + x] = CreateFromBmp(ptr, x * _xpix, y * _ypix, _xpix, _ypix);
			if (_pp_bmp[y * _xcount + x] == nullptr) {
				return false;
			}
		}
	}
	DeleteBmp(&ptr);
	return true;
}	//LoadDivBmp
bool LoadDivBmpW(const wchar_t* _path, int _x0, int _y0, size_t _xpix, size_t _ypix, size_t _xcount, size_t _ycount, Bmp** _pp_bmp)
{
	if ((_path == NULL) || (_pp_bmp == NULL)) {
		return false;
	}
	if ((_xpix * _ypix * _xcount * _ycount) == 0) {
		return false;
	}
	Bmp* ptr = LoadBmpFromImgFileW(_path);
	if (!ptr) {
		return false;
	}
	//Bmp** pp = new Bmp * [_xcount * _ycount];
	for (int y = 0; y < _ycount; y++) {
		for (int x = 0; x < _xcount; x++) {
			_pp_bmp[y * _xcount + x] = CreateFromBmp(ptr, x * _xpix, y * _ypix, _xpix, _ypix);
			if (_pp_bmp[y * _xcount + x] == nullptr) {
				return false;
			}
		}
	}
	DeleteBmp(&ptr);
	return true;
}	//LoadDivBmp

/**
* @brief	使い終わったBMP画像の削除
*
* @param	Bmp** _pp_bmp	Bmp構造体ポインタ変数のアドレス。
*
* @note		渡されたBmp構造体のメンバついて：\n
*			pixelとpal（ポインタ）は確保されているメモリが解放される。\n
*			渡されたポインタ変数が指し示すBmp構造体も解放されNULLが代入される。\n
*/
void DeleteBmp(Bmp** _pp_bmp)
{
	if (_pp_bmp == NULL) {
		//NULLポインタなら何もしない
		return;
	}
	if ((*_pp_bmp) == NULL) {
		//ポインタの中身（アドレス）がNULLなら何もしない（Bmp構造体が確保されていない）
		return;
	}
	//確保したメモリ（ポインタ）が入っていれば削除する。
	if ((*_pp_bmp)->pixel != NULL) {
		//ピクセルデータ削除
		free((*_pp_bmp)->pixel);	//削除
		//Bmpの内容も消去
		//(*_pp_bmp)->pixel = NULL;
	}
	if ((*_pp_bmp)->pal != NULL) {
		//パレットデータ削除
		free((*_pp_bmp)->pal);	//削除
		//Bmpの内容も消去
		//(*_pp_bmp)->pal = NULL;
	}
	//(*_pp_bmp)->width = 0;
	//(*_pp_bmp)->height = 0;
	//(*_pp_bmp)->colbit = 0;
	//
	free(*_pp_bmp);	//Bmp本体を解放
	(*_pp_bmp) = NULL;	//NULLにしておく
	return;
}	//DeleteBmp

/**
* @brief	２４ビット画像の'Red'と'Blue'を入れ替える
*
* @note		BMPファイルの２４ビット画像の１ピクセルのRGB値の並び[B][R][G]を、<br/>
*			"conioex"の"PrintImage()"で出力する時の並び[R][G][B]に変換する。
*
* @param	Bmp構造体へのポインタ
*/
void Bmp24SwapRB(Bmp* _bmp)
{
	if (_bmp == NULL) {
		return;
	}
	if (_bmp->colbit != 24) {
		return;
	}
	if (_bmp->swapRB) {
		return;	//既に入替済み
	}
	char* pix = _bmp->pixel;
	int width = _bmp->width;
	int height = _bmp->height;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			char tmp = pix[2];	//2
			pix[2] = pix[0];	//2<-0
			pix[0] = tmp;		//0<-2
			pix += 3;
		}
	}
	_bmp->swapRB = TRUE;
}	//Bmp24SwapRB

/*
* @brief	Bmpオブジェクトの生成
*
* @param	int _width,_height：画像の幅と高さ（ピクセル数）
* @param	int _colbits：色数：１ピクセル当たりのビット数
* @param	int _numpal：パレット数：０なら、パレット用バッファ確保しない（COLORREF*palがNULLになる）
* @param	const COLORREF* const  _pal：転送元パレット：_numpalが１以上ある場合、転送元になるパレット。
*				このポインタがNULLで_numpalが１以上ある場合はパレット確保してデフォルト色をセットする。
*
* @return	Bmp*：作成したBmp構造体へのポインタ／エラーの場合はNULLを返す
*/
Bmp* CreateBmp(int _width, int _height, int _colbits, size_t _numpal, const COLORREF* const  _pal)
{
	//サイズが不正な場合は生成しない。
	if ((_width <= 0) || (_height <= 0)) {
		return (Bmp*)NULL;
	}
	Bmp* p_bmp = (Bmp*)calloc(1, sizeof(Bmp));
	_ASSERT(p_bmp);

	//=== パレット設定 ===
	//パレット数の補正（元のパレット数が間違っていたら正しい値を算出）
	switch (_colbits) {
	case	1:
		_numpal = 2;
		break;
	case	4:
		//パレット数がオーバーしていたら補正
		if (_numpal > 16) {
			_numpal = 16;
		}
		break;
	case	8:
		//パレット数がオーバーしていたら補正
		if (_numpal > 256) {
			_numpal = 256;
		}
		break;
	case	16:
	case	24:
	case	32:
		_numpal = 0;
		break;
	default:
		//パレットが必要な色ビット数じゃなければパレットは確保しないのでパレット数を０にする。
		_colbits = 4;	//範囲外は4bitColor
		_numpal = 0;	//パレットは無し
		break;
	}
	p_bmp->colbit = _colbits;		//補正済み色ビット数
	p_bmp->numpal = (int)_numpal;	//正しい値を新しく作るBmpのパレット数にセット
	//
	//パレット用バッファの確保
	p_bmp->pal = (COLORREF*)NULL;		//COLORREF*パレット（16色）データへのポインタ※無い場合はNULL
	if (p_bmp->numpal > 0) {
		//パレット数指定が１以上あるのでメモリを確保
		p_bmp->pal = (COLORREF*)calloc(p_bmp->numpal, sizeof(COLORREF));
		_ASSERT(p_bmp->pal);
		if (_pal != NULL) {
			//転送元パレット指定があればコピーする。
			memcpy(p_bmp->pal, _pal, p_bmp->numpal * sizeof(COLORREF));
		}
		else {
			//指定が無い場合はデフォルトカラーを入れておく。
			memcpy(p_bmp->pal, ANSI_PAL256_COLOR, p_bmp->numpal * sizeof(COLORREF));
		}
	}

	//=== 画像ピクセル ===
	p_bmp->width = _width;		//幅
	p_bmp->height = _height;	//高さ
	int bypp = 1;
	switch (p_bmp->colbit) {
	case	1:
	case	2:
	case	4:
	case	8:
		//１ピクセル=1バイトなのでそのまま
		break;
	case	16:
		//１ピクセル=２バイト
		bypp = 2;
		break;
	case	24:
		bypp = 3;
		break;
	case	32:
		bypp = 4;
		break;
	}
	p_bmp->numpix = (p_bmp->width * p_bmp->height) * bypp;	//画像データのバイト数
	//メモリを確保
	p_bmp->pixel = (char*)calloc(1, p_bmp->numpix);	//char*画像データへのポインタ
	_ASSERT(p_bmp->pixel);
	//
	p_bmp->swapRB = false;	//24ビット以上の画像で、RとBが入れ替わっている場合は'TRUE'になる
	//
	p_bmp->wch = 0;		//変換元の文字 wchar_t
	p_bmp->aaLv = 0;	//文字表示の時のアンチエイリアスレベル
	return p_bmp;
}	//CreateBmp

/*
* @brief	Bmpの全ピクセルを初期化する
*
* @param	int	_color	パレット番号又はRGB値を指定
*				パレットのある画像の場合はパレット番号を指定する
*				フルカラーの場合は0x00RRGGBBを指定する
*/
void ClearBmp(Bmp* _p, int _color)
{
	if (_p->colbit == 4) {
		memset(_p->pixel, _color & 0xF, _p->numpix);
	}
	else if (_p->colbit == 8) {
		memset(_p->pixel, _color & 0xFF, _p->numpix);
	}
	else if (_p->colbit == 24) {
		int rr = (_color & 0xFF0000) >> 16;
		int gg = (_color & 0x00FF00) >> 8;
		int bb = (_color & 0x0000FF);
		for (int i = 0; i < _p->numpix; i += 3) {
			_p->pixel[i + 0] = bb;
			_p->pixel[i + 1] = gg;
			_p->pixel[i + 2] = rr;
		}
	}
}	//ClearBmp

/*
* @brief	Bmp画像からBmp画像への矩形転送
*			・転送先と転送元の画像の大きさは違っていても良い
*			・転送先と転送元の画像のカラービット数は同じでなければならない
*			・転送先のパレットは転送元のパレットで上書きされる（パレットのサイズが違う場合は小さい方に合わせる）
*
* @param	Bmp* _dest			転送先Bmp画像（元に画像は上書きされる）
* @param	int _dx,_dy			転送先の座標
* @param	const Bmp* _src		転送元Bmp画像
* @param	int	_sx,_sy			転送元の座標
* @param	int	_width,_height	転送元のサイズ
*/
Bmp* CopyBmp(Bmp* _dest, int _dx, int _dy, const Bmp* _src, int _sx, int _sy, int _width, int _height, bool _tr)
{
	if ((_dest == NULL) || (_src == NULL) || (_width <= 0) || (_height <= 0)) {
		return NULL;
	}
	if (_src->colbit != _dest->colbit) {
		return NULL;
	}
	//パレットのコピー
	int pal_size = _src->numpal;	//転送元のサイズ
	if (_dest->numpal < _src->numpal) {
		//転送先のサイズが小さい場合は、転送先のサイズに合わせる。
		pal_size = _dest->numpal;
	}
	memcpy(_dest->pal, _src->pal, pal_size);	//パレット転送実行
	//////memset(_dest->pixel, 0, _dest->numpix);	//ピクセル全部を０で初期化しておく（転送しない部分は０になる）
	//矩形転送 ---
	if ((_src->colbit == 4) || (_src->colbit == 8)) {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				int dx = _dx + x;
				int dy = _dy + y;
				if ((dx < 0) || (dx >= _dest->width) || (dy < 0) || (dy >= _dest->height)) {
					continue;
				}
				int xx = _sx + x;
				int yy = _sy + y;
				if ((xx >= 0) && (xx < _src->width) && (yy >= 0) && (yy < _src->height)) {
					//元画像の範囲内のみ転送する⇒範囲外は元の画像のまま
					char c = _src->pixel[yy * _src->width + xx];
					if ((!_tr) || (c != 0)) {
						//透明指定が無しor透明ピクセルでは無い
						//*dest_p = c;
						_dest->pixel[dy * _dest->width + dx] = c;
					}
				}
				//dest_p++;
			}
		}
	}
	else if (_src->colbit == 16) {
		//矩形ブロック転送
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				int dx = _dx + x;
				int dy = _dy + y;
				if ((dx < 0) || (dx >= _dest->width) || (dy < 0) || (dy >= _dest->height)) {
					continue;
				}
				int xx = _sx + x;
				int yy = _sy + y;
				if ((xx >= 0) && (xx < _src->width) && (yy >= 0) && (yy < _src->height)) {
					//元画像の範囲内のみ転送する⇒範囲外は元の画像のまま
					UINT16 ui16 = ((UINT16*)_src->pixel)[yy * _src->width + xx];
					if ((!_tr) || (ui16 != 0)) {
						//透明指定が無しor透明ピクセルでは無い
						((UINT16*)_dest->pixel)[dy * _dest->width + dx] = ui16;
					}
				}
			}
		}
	}
	else if (_src->colbit == 24) {
		//矩形ブロック転送
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				int dx = _dx + x;
				int dy = _dy + y;
				if ((dx < 0) || (dx >= _dest->width) || (dy < 0) || (dy >= _dest->height)) {
					continue;
				}
				int xx = _sx + x;
				int yy = _sy + y;
				if ((xx >= 0) && (xx < _src->width) && (yy >= 0) && (yy < _src->height)) {
					char* pd = &_dest->pixel[dy * (_dest->width * 3) + (dx * 3)];
					char* ps = &_src->pixel[yy * (_src->width * 3) + (xx * 3)];
					int c = (ps[0] | ps[1] | ps[2]);
					if ((!_tr) || (c != 0)) {
						pd[0] = ps[0];
						pd[1] = ps[1];
						pd[2] = ps[2];
					}
				}
				//dest_p += 3;
			}
		}
	}
	else if (_src->colbit == 32) {
		//矩形ブロック転送
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				int dx = _dx + x;
				int dy = _dy + y;
				if ((dx < 0) || (dx >= _dest->width) || (dy < 0) || (dy >= _dest->height)) {
					continue;
				}
				int xx = _sx + x;
				int yy = _sy + y;
				if ((xx >= 0) && (xx < _src->width) && (yy >= 0) && (yy < _src->height)) {
					//元画像の範囲内のみ転送する⇒範囲外は元の画像のまま
					UINT32 ui32 = ((UINT32*)_src->pixel)[yy * _src->width + xx];
					if ((!_tr) || (ui32 != 0)) {
						//透明指定が無しor透明ピクセルでは無い
						((UINT32*)_dest->pixel)[dy * _dest->width + dx] = ui32;
					}
				}
			}
		}
	}
	return _dest;
}	//CopyBmp

/*
* @brief	Bmp画像の指定範囲を元に、新しいBmpを生成する。
*
* @param	const Bmp* _src		元の画像
* @param	int _xp,_yp			指定範囲の左上座標
* @param	int _width,_height	指定範囲の幅と高さ
*
* @return	Bmp*	新しく作ったBmp画像のポインタ
*/
Bmp* CreateFromBmp(const Bmp* _src, int _xp, int _yp, int _width, int _height)
{
	if ((_src == NULL) || (_width <= 0) || (_height <= 0)) {
		return NULL;
	}
	Bmp* p_dest = CreateBmp(_width, _height, _src->colbit, _src->numpal, _src->pal);
	_ASSERT(p_dest);
	CopyBmp(p_dest, _src, _xp, _yp, _width, _height, false);
	return p_dest;
}	//CreateFromBmp

//================================================================
// Bitmapフォント生成関数
//
// 指定のフォントを使ってそのフォントで生成された文字列イメージをビットマップ画像データに変換する。
// 【注】コンソールプログラム専用⇒内部でコンソールウィンドウのハンドルを使っている。
//
// ToDo:
// パレットの何番の色を使うかを外部から出来る様にする。
// ex)２値色の時、'0'->pal[13], '1'->pal[14]とか。
//================================================================
const DWORD Gray2[2] = {
	//======== START ========
	0x00000000,
	0xFFffffff,//[ 1]
	//======== END ========
};
const DWORD Gray5[5] = {
	//======== START ========
	0x00000000,
	0x3FFFFFFF,0x7FFFFFFF,0xBFFFFFFF,0xFFffffff,//[ 4]
	//======== END ========
};
const DWORD Gray17[17] = {
	//======== START ========
	0x00000000,
	0x0FFFFFFF,0x1FFFFFFF,0x2FFFFFF,0x3FFFFFFF,0x4FFFFFFF,0x5FFFFFFF,0x6FFFFFFF,0x7FFFFFFF,
	0x8FFFFFFF,0x9FFFFFFF,0xAFFFFFF,0xBFFFFFFF,0xCFFFFFFF,0xDFFFFFFF,0xEFFFFFFF,0xFFFFFFFF,//[16]
	//======== END ========
};
const DWORD Gray65[65] = {
	//======== START ========
	0x00000000,
	0x03FFFFFF,0x07FFFFFF,0x0BFFFFFF,0x0FFFFFFF,0x13FFFFFF,0x17FFFFFF,0x1BFFFFFF,0x1FFFFFFF,0x23FFFFFF,0x27FFFFFF,0x2BFFFFFF,0x2FFFFFFF,0x33FFFFFF,0x37FFFFFF,0x3BFFFFFF,0x3FFFFFFF,//[16]
	0x43FFFFFF,0x47FFFFFF,0x4BFFFFFF,0x4FFFFFFF,0x53FFFFFF,0x57FFFFFF,0x5BFFFFFF,0x5FFFFFFF,0x63FFFFFF,0x67FFFFFF,0x6BFFFFFF,0x6FFFFFFF,0x73FFFFFF,0x77FFFFFF,0x7BFFFFFF,0x7FFFFFFF,//[32]
	0x83FFFFFF,0x87FFFFFF,0x8BFFFFFF,0x8FFFFFFF,0x93FFFFFF,0x97FFFFFF,0x9BFFFFFF,0x9FFFFFFF,0xA3FFFFFF,0xA7FFFFFF,0xABFFFFFF,0xAFFFFFFF,0xB3FFFFFF,0xB7FFFFFF,0xBBFFFFFF,0xBFFFFFFF,//[48]
	0xC3FFFFFF,0xC7FFFFFF,0xCBFFFFFF,0xCFFFFFFF,0xD3FFFFFF,0xD7FFFFFF,0xDBFFFFFF,0xDFFFFFFF,0xE3FFFFFF,0xE7FFFFFF,0xEBFFFFFF,0xEFFFFFFF,0xF3FFFFFF,0xF7FFFFFF,0xFBFFFFFF,0xFFFFFFFF,//[64]
	//======== END ========
};

#if false
//=== ビットマップフォント用グレイスケール出力コード ===
void GrayDump(void)
{
	double rgb = 0;
	const int resolution = 65;	//{2|5|17|65}
	const double step = (1.0 / (double)(resolution - 1));
	_RPTN(_CRT_WARN, "const DWORD Gray%d[%d]={\n", resolution, resolution);
	_RPT0(_CRT_WARN, "//======== START ========\n");
	_RPT0(_CRT_WARN, "0x00000000,\n");
	for (int n = 1; n < resolution; n++) {
		int rgb = (int)((step * (double)n) * 255.0);
		_RPTN(_CRT_WARN, "0x00%02x%02x%02x,", rgb, rgb, rgb);
		if ((n % 16) == 0) {
			_RPTN(_CRT_WARN, "//[%2d]\n", n);
		}
		else if (n == (resolution - 1)) {
			_RPTN(_CRT_WARN, "//[%2d]\n", n);
		}
	}
	_RPT0(_CRT_WARN, "//======== END ========\n");
	_RPT0(_CRT_WARN, "};\n");
}
#endif // false

/**
* @brief	1bppの画像を8bppの画像に変換する。
*
* @param	Bmp* _pbc	ビットマップ文字データへのポインタ
* @param	GLYPHMETRICS* _pgm	変換元文字のグリフ情報
*
* @return
* 	なし
*/
static void convert_bpp1_to_bpp8(Bmp* _pbc, const GLYPHMETRICS* _pgm)
{
	int w_pix = _pgm->gmBlackBoxX;
	int h_pix = _pgm->gmBlackBoxY;
	int stride = (_pbc->numpix / _pgm->gmBlackBoxY);
	int stride4 = (w_pix + 0b0011) & (~0b0011);			//８bpp画像の４バイト境界のバイト数
	int bits_size = stride4 * h_pix;
	char* pFontBitmap = (char*)calloc(bits_size, sizeof(char));
	_ASSERT(pFontBitmap != NULL);
	//ZeroMemory(pFontBitmap, bits_size);
	for (int y = 0; y < h_pix; y++) {
		for (int x = 0; x < stride; x++) {
			int idxSrc = (y * stride + x);
			UINT bit8 = _pbc->pixel[idxSrc];
			int idxDest = (y * stride4) + (x * 8);
			for (int bitN = 0; bitN < 8; bitN++) {
				if ((idxDest + bitN) < bits_size) {
					//pFontBitmap[idxDest + bitN] = (bit8 & (0b10000000 >> bitN)) ? 1 : 0;	//0xFF : 0x00;
					if ((bit8 & (0b10000000 >> bitN)) != 0) {
						pFontBitmap[idxDest + bitN] = 1;
					}
				}
			}
		}
	}
	//古い1bppのバッファは削除して新しく作った8bppバッファに入れ替える。
	free(_pbc->pixel);
	_pbc->pixel = pFontBitmap;
	_pbc->numpix = bits_size;
}	//convert_bpp1_to_bpp8

/**
*  @brief
* 	ビットマップ文字１文字の表示位置を調整してビットマップを作り直す。
*
* @param	BitmapChar* _pbc	ビットマップ文字のポインタ。このポインタが指すビットマップ文字データの表示位置を調整してバッファが作り直される。
* @param	GLYPHMETRICS* _pgm	変換元文字のグリフ情報
* @param	TEXTMETRIC* _ptxm	変換元フォントの計測（文字の寸法）情報
*
* @return
* 	無し
*/
static void build_bmp_char(Bmp* _pbc, const GLYPHMETRICS* _pgm, const TEXTMETRICW* _ptxm)
{
	//転送先バッファを作る
	int	dest_width = _pgm->gmCellIncX;
	int dest_height = _ptxm->tmHeight;
	int dest_buf_size = dest_width * dest_height;
	char* pDest = (char*)calloc(dest_buf_size, sizeof(char));	//転送先を全て０で初期化（ドットの無い場所（処理しない場所）は０になる）
	_ASSERT(pDest != NULL);
	//転送元サイズを計算（横幅は４の倍数）
	int width = _pgm->gmBlackBoxX;
	//int widthBytes = (width + 0b0011) & (~0b0011);	//横幅のバイト数は４の倍数に合わせる
	int height = _pgm->gmBlackBoxY;
	int stride = _pbc->numpix / _pbc->height;	//転送元バッファの横幅
	//
	for (int y = 0; y < height; y++)
	{
		int yp = (_ptxm->tmAscent - _pgm->gmptGlyphOrigin.y) + y;
		if ((yp < 0) || (yp >= dest_height))
		{
			//上下方向に範囲外なら処理しない
			continue;
		}
		for (int x = 0; x < width; x++)
		{
			int xp = _pgm->gmptGlyphOrigin.x + x;
			if ((xp < 0) || (xp >= dest_width))
			{
				//左右方向に範囲外なら処理しない
				continue;
			}
			//X,Y位置から１次元配列の読み出し位置を算出
			//int read_idx = (y * widthBytes + x);
			int read_idx = (y * stride + x);
			if ((read_idx < 0) || (read_idx >= (int)_pbc->numpix))
			{
				//転送元の範囲を超えていたら処理しない
				continue;
			}
			//BYTE dot = _pbc->pPix[y * stride + x];
			BYTE dot = _pbc->pixel[read_idx];	//転送元から１ピクセル読み出す
			if (dot == 0x00)
			{
				//色コード０は透明扱いなので処理しない
				continue;
			}
			pDest[yp * dest_width + xp] = dot;
		}
	}
	free(_pbc->pixel);
	_pbc->pixel = pDest;
	_pbc->numpix = dest_buf_size;
	_pbc->width = dest_width;
	_pbc->height = dest_height;
	return;
}	//build_bmp_char

#ifndef UNICODE
/**
* @brief	マルチバイト文字列から次の１文字を取り出しUINTに変換して返す。\n
*			渡されたポインタをマルチバイト文字換算で１文字進める。
*
* @param	BYTE** p：マルチバイト文字列のポインタのポインタ
*
* @return	UINT：マルチバイト文字コード
*/
static UINT get_MBC(BYTE** p) {
	UINT mbc = **p;	//１バイト取り込む
	if (IsDBCSLeadByte(mbc)) {
		mbc <<= 8;	//１バイト目は上位バイトへ入れる
		(*p)++;	//全角文字は２バイト目を取り込む
		mbc |= **p;	//２バイト目は下位バイトに入れる
	}
	(*p)++;	//ポインタ進める
	return mbc;
}	//get_MBC
#endif // UNICODE

/**
* @brief	文字列（ワイド文字｜Unocode文字）をBmp画像の配列として生成する。\n
*
* @param	const TCHAR* _font_name : フォント名（ワイド文字｜Unocode文字）
* @param	int _font_size : フォント・サイズ
* @param	int _bold : 太字指定：trueで太字
* @param	int _ggo : アンチ・エイリアスの諧調指定\n
*			{GGO_BITMAP,GGO_GRAY2_BITMAP,GGO_GRAY4_BITMAP,GGO_GRAY8_BITMAP}\n
*			※WIN32PAI::GetGlyphOutline()関数参照
* @param	const TCHAR* _text : 変換したい文字列（ワイド文字｜Unocode文字）
*
* @return	Bmp*	変換後のビットマップ文字の配列へのポインタ。(ターミネーターとして全メンバがNULLのBmpが入る)
*					※返されたBmpは必ずDeleteBmp()で削除する事
*
* @note
*		指定のフォントで出来たビットマップ文字Bmpの配列を作成し先頭のポインタを返す。\n
*		※ワイド文字(char)/Unocode文字(wchar_t)両対応。\n
*		※コンパイル時にTCHARがワイド文字(char)かUnocode文字(wchar_t)に切替わる。\n
*
*/
Bmp* CreateBmpChar(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, const TCHAR* _text)
{
	//回転行列
	MAT2	mat2{ {0,1},{0,0},{0,0},{0,1} };
	//フォントの設定～作成
	LOGFONT	lf;
	lf.lfHeight = _font_size;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;	//文字単位の回転角度左回り
	lf.lfOrientation = 0;
	if (_bold) {
		lf.lfWeight = FW_BOLD;	//太字設定
	}
	else {
		lf.lfWeight = FW_NORMAL;
	}
	lf.lfItalic = FALSE;	//斜体
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
#ifdef UNICODE
	lf.lfCharSet = ANSI_CHARSET;
#else
	lf.lfCharSet = SHIFTJIS_CHARSET;
#endif // UNICODE
	lf.lfOutPrecision = OUT_TT_PRECIS;	//OUT_DEFAULT_PRECIS
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = (DEFAULT_PITCH | FF_MODERN);
	lf.lfFaceName[0] = '\0';
	//書体名をコピー（文字数制限あり）
	if (_font_name != nullptr) {
		CopyMemory(lf.lfFaceName, _font_name, LF_FACESIZE * sizeof(TCHAR));
	}
	else {
		//指定が無い時は「ＭＳ明朝」とする
		CopyMemory(lf.lfFaceName, _T("ＭＳ 明朝"), LF_FACESIZE * sizeof(TCHAR));
		//CopyMemory(lf.lfFaceName, _T(""), LF_FACESIZE * sizeof(TCHAR));
	}
	//フォント生成
	HFONT hFont = CreateFontIndirect(&lf);
	_ASSERT(hFont);
	if (hFont == NULL) {
		return	NULL;
	}
	// デバイスにフォントを選択する
	HWND hWnd = GetConsoleWindow();	//★★★このコンソールのウィンドウハンドル
	HDC hdc = GetDC(hWnd);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
	//生成したフォントの計測データを取得する
	TEXTMETRICW	txm;		//変換したフォントの情報を入れる構造体
	GetTextMetricsW(hdc, &txm);	//計測データを取得
	int aa_level = 2;
	switch (_ggo)
	{
	default:	//２値
	case GGO_BITMAP:		aa_level = 2;		break;	//２値
	case GGO_GRAY2_BITMAP:	aa_level = 5;		break;	//５階調
	case GGO_GRAY4_BITMAP:	aa_level = 17;	break;	//１７階調
	case GGO_GRAY8_BITMAP:	aa_level = 65;	break;	//６５階調
	}
	//指定のフォントで出来たビットマップ文字で文字列を作成する。
	GLYPHMETRICS	gm{ 0,0,0,0,0,0 };	//グリフ設定データ
	UINT code = 0;
	//文字列の文字数を求める。
#ifdef UNICODE
	size_t length = (int)wcslen(_text);	//ワイド文字(Unicode)の文字数を数える
	size_t buff_len = length + 1;
	const TCHAR* code_ary = (TCHAR*)_text;
#else
	//マルチバイト文字の場合は全角だけ２文字分をUINTに変換する
	size_t length = _mbstrlen(_text);	//全角も半角も１文字として数える
	size_t buff_len = length + 1;
	UINT* code_ary = (UINT*)_alloca(buff_len * sizeof(UINT));	//'\0'含む文字数のBmpを確保する
	memset(code_ary, 0, buff_len * sizeof(UINT));
	const BYTE* p = (BYTE*)_text;
	for (int i = 0; (*p != '\0') && (i < length); i++) {
		code_ary[i] = get_MBC((BYTE**)&p);
	}
#endif // UNICODE
	//Bmp用バッファを文字数分確保（最後の'\0'用のBmpも含める）（全て０で初期化）
	Bmp* pBmpChr = (Bmp*)calloc(buff_len, sizeof(Bmp));
	_ASSERT(pBmpChr != NULL);
	//１文字に付き１つのBmpオブジェクトを生成してBmpの配列に格納して行く
	for (size_t txn = 0; txn < length; txn++) {
		code = (UINT)code_ary[txn];
		//これから生成する文字ビットマップデータのバイト数を取得する。
		int buff_size = GetGlyphOutline(hdc, code, _ggo, &gm, 0, NULL, &mat2);
		//if (buff_size > 0)
		if (code != 0)
		{
			//取得したサイズ分のバッファを確保する。’ ’空白の場合は０(zero)が返るが、そのままmallocする。
			pBmpChr[txn].pixel = (char*)calloc(buff_size, sizeof(char));
			//’ ’空白の場合buff_size＝０でもgmには正しい値がセットされている様だ。
			GetGlyphOutline(hdc, code, _ggo, &gm, buff_size, pBmpChr[txn].pixel, &mat2);
			if (_ggo == GGO_BITMAP)
			{
				//１bppのビットマップは表示しにくいので８bppに変換する。
				pBmpChr[txn].numpix = buff_size;		//バッファサイズ
				convert_bpp1_to_bpp8(&pBmpChr[txn], &gm);	//１ビット/ピクセル画像を８ビット/ピクセル画像に変換
				buff_size = pBmpChr[txn].numpix;
			}
			pBmpChr[txn].width = gm.gmBlackBoxX;	//横ピクセル数
			pBmpChr[txn].height = gm.gmBlackBoxY;	//縦ピクセル数
			//全ての文字画像を８ビット/ピクセルの画像として扱う
			pBmpChr[txn].colbit = 8;				//８ビット/pixel画像
			pBmpChr[txn].numpix = buff_size;		//バッファサイズ
			pBmpChr[txn].aaLv = aa_level;			//アンチエイリアスの諧調レベル
			pBmpChr[txn].wch = code;				//変換元の文字コード
			//文字位置を調整してバッファを作り直す。
			build_bmp_char(&pBmpChr[txn], &gm, &txm);

			//位置調整が済んだ画像にパレット(RGBQUAD型)を確保する
			pBmpChr[txn].pal = 0;	//COLORREF型は確保しない
			pBmpChr[txn].pal_rgb = (RGBQUAD*)calloc(NUM_D2D_PAL, sizeof(RGBQUAD));
			pBmpChr[txn].numpal = NUM_D2D_PAL;	//パレット数は256色固定とする
			switch (_ggo)
			{
			default:	//２値
			case GGO_BITMAP:
				memcpy_s(pBmpChr[txn].pal_rgb, NUM_D2D_PAL * sizeof(RGBQUAD), Gray2, sizeof(Gray2));
				break;	//２値
			case GGO_GRAY2_BITMAP:
				memcpy_s(pBmpChr[txn].pal_rgb, NUM_D2D_PAL * sizeof(RGBQUAD), Gray5, sizeof(Gray5));
				break;	//５階調
			case GGO_GRAY4_BITMAP:
				memcpy_s(pBmpChr[txn].pal_rgb, NUM_D2D_PAL * sizeof(RGBQUAD), Gray17, sizeof(Gray17));
				break;	//１７階調
			case GGO_GRAY8_BITMAP:
				memcpy_s(pBmpChr[txn].pal_rgb, NUM_D2D_PAL * sizeof(RGBQUAD), Gray65, sizeof(Gray65));
				break;	//６５階調
			}
		}
	}
	//デバイスのフォント選択を解除する（元に戻す）
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(hWnd, hdc);
	_ASSERT(pBmpChr);
	return	pBmpChr;
}	//CreateBmpChar

/**
* @brief	文字列（ワイド文字｜Unocode文字）を１枚の画像として生成する。
*
* @param	const TCHAR* _font_name	フォント名（ワイド文字｜Unocode文字）
* @param	int _font_size : フォント・サイズ
* @param	int _bold : 太字指定：trueで太字
* @param	int _ggo : アンチ・エイリアスの諧調指定\n
*			{GGO_BITMAP,GGO_GRAY2_BITMAP,GGO_GRAY4_BITMAP,GGO_GRAY8_BITMAP}\n
*			※WIN32PAI::GetGlyphOutline()関数参照
* @param	const TCHAR* _text	変換したい文字列（ワイド文字｜Unocode文字）
*
* @return	Bmp* : 変換後のビットマップ文字の配列へのポインタ。
*
* @note		指定のフォントで出来たビットマップ文字列のBmp画像を作成し、そのポインタを返す。\n
* 			CreateBmpChar()で得られたBmpの配列（１文字ごとの画像の配列）を連結して１枚の画像にし、
* 			そのバッファ（BmpString）のポインタを返す。
*			※ワイド文字(char)/Unocode文字(wchar_t)両対応。\n
*			※コンパイル時にTCHARがワイド文字(char)かUnocode文字(wchar_t)に切替わる。\n
*/
Bmp* CreateBmpString(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, const TCHAR* _text)
{
	//先ず、１文字ごとの画像の配列を作成する。
	Bmp* pBc = CreateBmpChar(_font_name, _font_size, _bold, _ggo, _text);
	_ASSERT(pBc);
	int n = 0;
	int xpos = 0;
	if (pBc != NULL) {
		while (pBc[n].pixel != NULL) {
			//全ての文字を繋ぎ合わせた時の幅（ピクセル数）を計算する
			xpos += pBc[n].width;	//次の文字の横方向位置をセット
			n++;	//次の文字
		}
	}
	int width = xpos; //文字列画像全体の幅（ピクセル数）
	int height = _font_size;	//この文字列画像の高さ（ピクセル数）
	Bmp* bm_str = (Bmp*)calloc(1, sizeof(Bmp));	//Bmpオブジェクト１個生成：全ての文字Bmpを１つのBmpに集約する。
	//ZeroMemory(bm_str, sizeof(Bmp));
	bm_str->numpix = (width * height * sizeof(char));	//文字列画像のピクセルサイズ
	bm_str->pixel = (char*)calloc(bm_str->numpix, sizeof(char));	//画像バッファ確保
	bm_str->aaLv = pBc[0].aaLv;	//先頭文字[0]の諧調コードを使う
	bm_str->width = width;		//画像の幅（ピクセル）
	bm_str->height = height;	//画像高さ（ピクセル）
	//パレット作成とコピー
	bm_str->colbit = pBc[0].colbit;	//先頭文字[0]のビット数/ピクセルを使う
	bm_str->numpal = pBc[0].numpal;	//先頭文字[0]のパレット数
	//
	bm_str->pal_rgb = (RGBQUAD*)calloc(pBc[0].numpal, sizeof(RGBQUAD));	//[0]と同じパレットを確保する
	memcpy_s(bm_str->pal_rgb, NUM_D2D_PAL * sizeof(RGBQUAD), pBc[0].pal_rgb, NUM_D2D_PAL * sizeof(RGBQUAD));
	//
	n = 0;
	xpos = 0;
	while (pBc[n].pixel != NULL) {
		int pn = 0;
		for (int y = 0; y < pBc[n].height; y++) {
			for (int x = 0; x < pBc[n].width; x++) {
				int xp = (xpos + x);
				if ((xp >= 0) && (xp < bm_str->width) && (y >= 0) && (y < bm_str->height)) {
					//フレームバッファの範囲を超えてなければ書き込む
					bm_str->pixel[y * bm_str->width + xp] = pBc[n].pixel[pn];	//１ピクセル書き込む
				}
				pn++;	//次のピクセル読み出し位置
			}
		}
		xpos += pBc[n].width;	//次の横方向位置
		n++;
	}
	//確保したメモリの開放
	n = 0;
	while (pBc[n].pixel != NULL) {
		free(pBc[n].pixel);
		pBc[n].pixel = NULL;

		if (pBc[n].pal_rgb != NULL) {
			free(pBc[n].pal_rgb);
			pBc[n].pal_rgb = NULL;
		}
		if (pBc[n].pal != NULL) {
			free(pBc[n].pal);
			pBc[n].pal = NULL;
		}

		n++;
	}
	free(pBc);
	return bm_str;
}	//CreateBmpString

/**
* @brief	文字列（ワイド文字｜Unocode文字）を書式指定して１枚の画像として生成する。
*
* @param	const TCHAR* _font_name	フォント名（ワイド文字｜Unocode文字）
* @param	int _font_size : フォント・サイズ
* @param	int _bold : 太字指定：trueで太字
* @param	int _ggo : アンチ・エイリアスの諧調指定\n
*			{GGO_BITMAP,GGO_GRAY2_BITMAP,GGO_GRAY4_BITMAP,GGO_GRAY8_BITMAP}\n
*			※WIN32PAI::GetGlyphOutline()関数参照
* @param	const char* _format：書式指定文字列
* @param	...：可変長引数
*
* @return	Bmp* : 変換後のビットマップ文字の配列へのポインタ。
*
* @note		指定のフォントで出来たビットマップ文字列を作成し、そのポインタを返す。\n
* 			CreateBmpStringで得られたBmpのポインタを返す。\n
*			※ワイド文字(char)/Unocode文字(wchar_t)両対応。\n
*			※コンパイル時にTCHARがワイド文字(char)かUnocode文字(wchar_t)に切替わる。\n
*/
Bmp* CreateBmpStringF(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, bool _zenkaku, const TCHAR* _format, ...)
{
	Bmp* p_bmp = nullptr;
	va_list ap;
	va_start(ap, _format);
	//VPrintStringFA(_zenkaku, _format, ap);
#ifdef UNICODE
	size_t length = _vscwprintf(_format, ap) + 1;	//'\0'含まないので＋１している
	wchar_t* buf = (wchar_t*)_malloca(length * sizeof(wchar_t));
	_ASSERT(buf);
	vswprintf_s(buf, length, _format, ap);
	if (_zenkaku == true) {
		//全て全角に変換してから生成
		wchar_t* p = HanToZenW(buf);
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, _ggo, p);
		free(p);
	}
	else {
		//半角のまま生成
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, _ggo, buf);
	}
#else
	//VPrintStringFA(_zenkaku, _format, ap);
	size_t length = _vscprintf(_format, ap) + 1;	//'\0'含まないので＋１している
	char* buf = (char*)_malloca(length);
	vsprintf_s(buf, length, _format, ap);
	if (_zenkaku == true) {
		//全て全角に変換してから生成
		char* p = HanToZen(buf);
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, _ggo, p);
		free(p);
	}
	else {
		//半角のまま生成
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, _ggo, buf);
	}
#endif // UNICODE
	va_end(ap);
	return p_bmp;
}	//CreateBmpStringF

Bmp* CreateBmpStringF(const TCHAR* _font_name, int _font_size, int _bold, bool _zenkaku, const TCHAR* _format, ...)
{
	Bmp* p_bmp = nullptr;
	va_list ap;
	va_start(ap, _format);
	//VPrintStringFA(_zenkaku, _format, ap);
#ifdef UNICODE
	size_t length = _vscwprintf(_format, ap) + 1;	//'\0'含まないので＋１している
	wchar_t* buf = (wchar_t*)_malloca(length * sizeof(wchar_t));
	_ASSERT(buf);
	vswprintf_s(buf, length, _format, ap);
	if (_zenkaku == true) {
		//全て全角に変換してから生成
		wchar_t* p = HanToZenW(buf);
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, GGO_BITMAP, p);
		free(p);
	}
	else {
		//半角のまま生成
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, GGO_BITMAP, buf);
	}
#else
	//VPrintStringFA(_zenkaku, _format, ap);
	size_t length = _vscprintf(_format, ap) + 1;	//'\0'含まないので＋１している
	char* buf = (char*)_malloca(length);
	vsprintf_s(buf, length, _format, ap);
	if (_zenkaku == true) {
		//全て全角に変換してから生成
		char* p = HanToZen(buf);
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, GGO_BITMAP, p);
		free(p);
	}
	else {
		//半角のまま生成
		p_bmp = CreateBmpString(_font_name, _font_size, _bold, GGO_BITMAP, buf);
	}
#endif // UNICODE
	va_end(ap);
	return p_bmp;
}	//CreateBmpStringF

//================================================================
// キー入力関係
//================================================================
/**
 * @brief	キー情報リセット
 */
void ResetKeyMap(void)
{
	for (int count = 0; count < 8; count++) {
		g_ConioKeyMap[count] = 0;
	}
}

/**
 * @brief	キーボード・マウス入力
 *
 * @param	port [入力] ポート番号(P*_*)
 * @return	入力値
 */
int InputKeyMouse(int port)
{
	DWORD event = 0;
	DWORD read = 0;
	volatile PINPUT_RECORD input_record;
	KEY_EVENT_RECORD* key_event;
	MOUSE_EVENT_RECORD* mouse_event;

	// キーボードイベントチェック
	if (GetNumberOfConsoleInputEvents(g_OrgInputHandle, &event) && event)
	{
		read = 0;
		input_record = (PINPUT_RECORD)_malloca(event * sizeof(INPUT_RECORD));
		_ASSERT(input_record);
		if (ReadConsoleInput(g_OrgInputHandle, input_record, event, &read) && read)
		{
			//input_record = input_record;
			for (unsigned int count = 0; count < read; count++, input_record++) {
				switch (input_record->EventType) {
				case KEY_EVENT: {
					key_event = &input_record->Event.KeyEvent;
					if (key_event->wVirtualKeyCode > 0x0FF) {
						break;
					}
					if (key_event->bKeyDown) {
						g_ConioKeyMap[key_event->wVirtualKeyCode >> 5] |= (0x01 << (key_event->wVirtualKeyCode & 31));
					}
					else {
						g_ConioKeyMap[key_event->wVirtualKeyCode >> 5] &= ~(0x01 << (key_event->wVirtualKeyCode & 31));
					}
					if (key_event->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) {
						g_ConioKeyMap[VK_MENU >> 5] |= (0x01 << (VK_MENU & 31));
					}
					else {
						g_ConioKeyMap[VK_MENU >> 5] &= ~(0x01 << (VK_MENU & 31));
					}
					if (key_event->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) {
						g_ConioKeyMap[VK_CONTROL >> 5] |= (0x01 << (VK_CONTROL & 31));
					}
					else {
						g_ConioKeyMap[VK_CONTROL >> 5] &= ~(0x01 << (VK_CONTROL & 31));
					}
					if (key_event->dwControlKeyState & SHIFT_PRESSED) {
						g_ConioKeyMap[VK_SHIFT >> 5] |= (0x01 << (VK_SHIFT & 31));
					}
					else {
						g_ConioKeyMap[VK_SHIFT >> 5] &= ~(0x01 << (VK_SHIFT & 31));
					}
					break;
				}
				case MOUSE_EVENT: {
					mouse_event = &input_record->Event.MouseEvent;
					g_ConioMousePosition = mouse_event->dwMousePosition;
					if (mouse_event->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
						g_ConioKeyMap[VK_LBUTTON >> 5] |= (0x01 << (VK_LBUTTON & 31));
					}
					else {
						g_ConioKeyMap[VK_LBUTTON >> 5] &= ~(0x01 << (VK_LBUTTON & 31));
					}
					if (mouse_event->dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {
						g_ConioKeyMap[VK_MBUTTON >> 5] |= (0x01 << (VK_MBUTTON & 31));
					}
					else {
						g_ConioKeyMap[VK_MBUTTON >> 5] &= ~(0x01 << (VK_MBUTTON & 31));
					}
					if (mouse_event->dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
						g_ConioKeyMap[VK_RBUTTON >> 5] |= (0x01 << (VK_RBUTTON & 31));
					}
					else {
						g_ConioKeyMap[VK_RBUTTON >> 5] &= ~(0x01 << (VK_RBUTTON & 31));
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}

	// マウス座標を返す
	switch (port) {
	case PM_CURX:
		return g_ConioMousePosition.X + 1;
	case PM_CURY:
		return g_ConioMousePosition.Y + 1;
	default:
		break;
	}
	// キー状態を返す
	return (g_ConioKeyMap[(port & 0x0FF) >> 5] & (0x01 << (port & 31))) != 0;
}

//================================================================
// 拡張キー入力
//================================================================
/**
* @brief	単一キーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
*
* @return	SHORT\tWin32apiの"GetAsyncKeyState()"と同じ
*/
SHORT GetKey(int _vk)
{
	if (GetForegroundWindow() != g_hConWnd) {
		//フォーカスが外れている。
		return 0;
	}
	//フォーカスが当たっている時だけ入力する。
	return GetAsyncKeyState(_vk);
}	//GetKey

/**
* @brief	キー入力を待つ。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
*
* @return	SHORT	Win32apiの"GetAsyncKeyState()"と同じ
*/
SHORT WaitKey(int _vk)
{
	SHORT k = 0;
	//フォーカスが当たっている時だけ入力する。
	do {
		k = GetKey(_vk);
	} while (!k);
	return k;
}	//GetKey

/**
* @brief	全てのキーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。\n
* この関数を呼び出した後、全てのキー入力がChkKeyEdge()/ChkKeyPress()で判定できる。
*
* @param	_vk	仮想キーコード：Pressキー（押下キー）入力判定する
* @param	_chktype	キー入力判定方法の選択：1=Edge判定/0=Press判定
*
* @return	int\n
*	キーＯＮなら１\n
*	キーＯＦＦなら０
*/
int GetKeyEx(int _vk, int _chktype)
{
	GetKeyAll();
	if (_chktype == 1) {
		//Edgeキー入力判定
		return ChkKeyEdge(_vk);
	}
	//Pressキー入力判定
	return ChkKeyPress(_vk);
}
/**
* @brief	全てのキーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。\n
* この関数を呼び出した後、全てのキー入力がChkKeyEdge()/ChkKeyPress()で判定できる。
*
* @return
*	なし
*/
void GetKeyAll(void)
{
	if (GetForegroundWindow() != g_hConWnd) {
		//フォーカスが外れている。
		return;
	}
	//フォーカスが当たっている時だけ入力する。
	//全キーを入力しEdgeとPressを作る。
	for (int vk = 0; vk < NUM_KEYS; vk++) {
		//現在の押し下げ状態をセットする。
		g_KeyPress[vk] = (int)((GetAsyncKeyState(vk) & (~0x1)) != 0);
		//前回ＯＦＦ⇒今回ＯＮの時だけＯＮにする。
		g_KeyEdge[vk] = (int)((g_KeyPress[vk] != 0) && (g_KeyLast[vk] == 0));
		//前回の状態を更新する
		g_KeyLast[vk] = g_KeyPress[vk];
	}
}	//GetKeyAll

/**
* @brief	Edgeキー入力判定：GetKeyAll()で入力したキー情報についてEdgeキー（トリガーキー）入力判定する
*
* @param	vk	仮想キーコード
*
* @return	int\n
*	キーＯＮなら１\n
*	キーＯＦＦなら０
*/
int ChkKeyEdge(int _vk) {
	return g_KeyEdge[_vk & 0xFF];
}	//ChkKeyEdge

/**
* @brief	Pressキー入力判定：GetKeyAll()で入力したキー情報についてPressキー（押下キー）入力判定する
*
* @param	vk	仮想キーコード
*
* @return	int\n
*	キーＯＮなら１\n
*	キーＯＦＦなら０
*/
int ChkKeyPress(int _vk) {
	return g_KeyPress[_vk & 0xFF];
}	//ChkKeyPress

//================================================================
// ジョイパッド入力関係
//================================================================
/**
 * @brief	ジョイパッド入力
 *
 * @param	port [入力] ポート番号(P*_*)
 * @return	入力値
 */
int InputJoystick(int port)
{
	JOYINFO	joy_info;
	int id;
	int func;

	// ゲームパッド入力
	if ((port & 0xfe00) == 0x0200) {
		id = (port & 0x01f0) >> 4;
		func = port & 0x0f;

		switch (func) {
		case 0:
		case 1:
		case 2:
		case 3:
			if (joyGetPos(id, &joy_info) != JOYERR_NOERROR) {
				return -1;
			}
			switch (func) {
			case 0:
				return joy_info.wXpos;
			case 1:
				return joy_info.wYpos;
			case 2:
				return joy_info.wZpos;
			case 3:
				return joy_info.wButtons;
			}
			break;
		default:
			break;
		}
		return 0;
	}
	return -1;
}

#if FALSE
/**
 * @brief	ジョイパッド入力(XInput対応)
 *
 * @param	port [入力] ポート番号(P*_*)
 * @return	入力値
 */
int InputJoystickX(int port)
{
	XINPUT_STATE controller_state[4];	// XInputコントローラ情報
	int id;
	int func;
	unsigned int  result;

	// ゲームパッド入力
	if ((port & 0xfe00) == 0x0200) {	//0x200～0x236
		id = (port & 0x01f0) >> 4;		//bit4～8(5bits)がコントローラ番号
		func = port & 0x0f;				//bit0～3(4bits)がボタン番号

		// Simply get the state of the controller from XInput.
		result = XInputGetState(id, &controller_state[id]);
		if (result == ERROR_SUCCESS) {
			switch (func) {
			case 0:
				return controller_state[id].Gamepad.sThumbLX;
			case 1:
				return controller_state[id].Gamepad.sThumbLY;
			case 2:
				return controller_state[id].Gamepad.bLeftTrigger;
			case 3:
				return controller_state[id].Gamepad.sThumbRX;
			case 4:
				return controller_state[id].Gamepad.sThumbRY;
			case 5:
				return controller_state[id].Gamepad.bRightTrigger;
			case 6:
				return controller_state[id].Gamepad.wButtons;
			}
		}
	}
	return -1;
}
#else	//FALSE
/**
 * @brief	ジョイパッド入力(XInput対応)
 *
 * @param	id		コントローラ番号(ID)：０～
 * @param	port [入力] ポート番号(P*_*)
 *
 * @retval	0		正常終了
 * @retval	1以上	Joystickの入力値
 * @retval	-1		エラー
 *
 * @note
 *	LRのスティックは中心から左右に移動する際のデッドゾーンが用意されている
 *	（デッドゾーンは左右に移動したとみなさないエリアのこと）
 *	 #define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
 *	 #define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
 */
int InputJoystickX(int id, int port)
{
	XINPUT_STATE controller_state;	// XInputコントローラ情報

	if (XInputGetState(id, &controller_state) != ERROR_SUCCESS) {
		return -1;
	}

	switch (port) {
	case PJX_LXPOS:
		return controller_state.Gamepad.sThumbLX;
	case PJX_LYPOS:
		return controller_state.Gamepad.sThumbLY;
	case PJX_LTRG:
		return controller_state.Gamepad.bLeftTrigger;
	case PJX_RXPOS:
		return controller_state.Gamepad.sThumbRX;
	case PJX_RYPOS:
		return controller_state.Gamepad.sThumbRY;
	case PJX_RTRG:
		return controller_state.Gamepad.bRightTrigger;
	case PJX_BTNS:
		if (controller_state.dwPacketNumber) {
			return controller_state.Gamepad.wButtons;
		}
	}
	return 0;
}	//InputJoystickX
#endif // FALSE

//================================================================
// サウンド関係
//================================================================
/**
* @brief	サウンド ファイルを開く
*
* @param	path [入力] ファイル名
* @retval	非0	サウンド ハンドル
* @retval	0	エラー
*/
int* MciOpenSound(const char* path)
{
	union {
		MCI_WAVE_OPEN_PARMSA	wave_param;
		MCI_OPEN_PARMSA			open_param;
	} MciParam;
	TCHAR error_str[256];
	const char midi_ext[] = ".mid|.midi|.rmi";
	const char wave_ext[] = ".wav|.wave";
	const char mp3_ext[] = ".mp3";
	char input_ext[_MAX_EXT];
	DWORD_PTR mci_command;
	MCIERROR mci_error;
	MciSoundInfo* sound_info;

	sound_info = (MciSoundInfo*)calloc(1, sizeof(MciSoundInfo));
	if (sound_info == NULL) {
		return 0;
	}
	ZeroMemory(sound_info, sizeof(*sound_info));
	ZeroMemory(&MciParam, sizeof(MciParam));
	_splitpath_s(path, NULL, 0, NULL, 0, NULL, 0, input_ext, sizeof(input_ext));
	_strlwr_s(input_ext, strlen(input_ext) + 1);
	mci_command = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
	if (strstr(midi_ext, input_ext)) {
		sound_info->device_type = MCI_DEVTYPE_SEQUENCER;
		lstrcpynA(sound_info->path, path, MAX_PATH);
		MciParam.open_param.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_SEQUENCER;
		MciParam.open_param.lpstrElementName = sound_info->path;
		mci_command |= MCI_OPEN_TYPE_ID;
	}
	else if (strstr(wave_ext, input_ext)) {
		sound_info->device_type = MCI_DEVTYPE_WAVEFORM_AUDIO;
		lstrcpynA(sound_info->path, path, MAX_PATH);
		MciParam.wave_param.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
		MciParam.wave_param.lpstrElementName = sound_info->path;
		mci_command |= MCI_OPEN_TYPE_ID;
		// MciParam.wave_param.dwBufferSeconds	= 60;
		// mci_command |= MCI_WAVE_OPEN_BUFFER;
	}
	else if (strstr(mp3_ext, input_ext)) {
		sound_info->device_type = MCI_DEVTYPE_DIGITAL_VIDEO;
		lstrcpynA(sound_info->path, path, MAX_PATH);
		MciParam.open_param.lpstrDeviceType = "MPEGVideo";
		MciParam.open_param.lpstrElementName = sound_info->path;
	}
	else {
		free(sound_info);
		return 0;
	}
	mci_error = mciSendCommandA(0, MCI_OPEN, mci_command, (DWORD_PTR)&MciParam);
	if (mci_error != 0) {
		free(sound_info);
		mciGetErrorString(mci_error, error_str, sizeof(error_str) / sizeof(TCHAR));
		MessageBox(NULL, error_str, NULL, MB_ICONWARNING);
		return 0;
	}
	sound_info->device_id = MciParam.open_param.wDeviceID;
	return (int*)sound_info;
}	//MciOpenSound

/**
 * @brief	サウンド ファイルを閉じる
 *
 * @param	sound_id [入力] サウンド ハンドル
 */
void MciCloseSound(int* sound_id)
{
	MciSoundInfo* sound_info;

	if (!sound_id) {
		return;
	}
	sound_info = (MciSoundInfo*)sound_id;
	if (sound_info->device_id) {
		mciSendCommand(sound_info->device_id, MCI_CLOSE, 0, 0);
		sound_info->device_id = 0;
	}
	free(sound_info);
}	//MciCloseSound

/**
 * @brief	サウンドを再生する
 *
 * @param	sound_id [入力] サウンド ハンドル
 * @param	repeat [入力] ループ有無
 */
void MciPlaySound(int* sound_id, int repeat)
{
	MciSoundInfo* sound_info;
	DWORD_PTR mci_command;
	MCI_PLAY_PARMS play_param;

	if (!sound_id) {
		return;
	}
	sound_info = (MciSoundInfo*)sound_id;
	if (!sound_info->device_id) {
		return;
	}
	sound_info->repeat = repeat;
	ZeroMemory(&play_param, sizeof(play_param));
	mci_command = 0;
	if (repeat) {
		switch (sound_info->device_type) {
		case MCI_DEVTYPE_DIGITAL_VIDEO:
			mci_command |= (MCI_FROM | MCI_DGV_PLAY_REPEAT);
			play_param.dwFrom = 0;
			break;
		case MCI_DEVTYPE_SEQUENCER:
		case MCI_DEVTYPE_WAVEFORM_AUDIO:
			break;
		default:
			break;
		}
	}
	mciSendCommand(sound_info->device_id, MCI_SEEK, MCI_SEEK_TO_START, 0);
	mciSendCommand(sound_info->device_id, MCI_PLAY, mci_command, (DWORD_PTR)&play_param);
}	//MciPlaySound

/**
 * @brief	サウンド再生を停止する
 *
 * @param	sound_id [入力] サウンド ハンドル
 */
void MciStopSound(int* sound_id)
{
	MciSoundInfo* sound_info;

	if (!sound_id) {
		return;
	}
	sound_info = (MciSoundInfo*)sound_id;
	if (!sound_info->device_id) {
		return;
	}
	sound_info->repeat = 0;
	mciSendCommand(sound_info->device_id, MCI_STOP, MCI_WAIT, 0);
	mciSendCommand(sound_info->device_id, MCI_SEEK, MCI_SEEK_TO_START, 0);
}	//MciStopSound

/**
 * @brief	サウンド再生状態の取得
 *
 * @param	sound_id [入力] サウンド ハンドル
 * @return	再生中ならば 0 以外を返す
 */
int MciCheckSound(int* sound_id)
{
	MciSoundInfo* sound_info;
	MCI_STATUS_PARMS status_param;

	if (!sound_id) {
		return 0;
	}
	sound_info = (MciSoundInfo*)sound_id;
	if (!sound_info->device_id) {
		return 0;
	}
	ZeroMemory(&status_param, sizeof(status_param));
	status_param.dwItem = MCI_STATUS_MODE;
	if (mciSendCommand(sound_info->device_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&status_param)) {
		return 0;
	}
	return status_param.dwReturn == MCI_MODE_PLAY;
}	//MciCheckSound

/**
 * @brief	ループ再生の強制更新
 *
 * @param	sound_id [入力] サウンド ハンドル
 * @note
 *	サウンドが停止したら同じサウンドを再生する
 *	更新時は音量設定が標準値に戻るので再設定を行う必要がある
 */
void MciUpdateSound(int* sound_id)
{
	MciSoundInfo* sound_info;
	MCI_STATUS_PARMS status_param;

	if (!sound_id) {
		return;
	}
	sound_info = (MciSoundInfo*)sound_id;
	if (!sound_info->device_id || !sound_info->repeat) {
		return;
	}

	switch (sound_info->device_type) {
	case MCI_DEVTYPE_DIGITAL_VIDEO:
		break;
	case MCI_DEVTYPE_SEQUENCER:
	case MCI_DEVTYPE_WAVEFORM_AUDIO:
		ZeroMemory(&status_param, sizeof(status_param));
		status_param.dwItem = MCI_STATUS_MODE;
		if (!mciSendCommand(sound_info->device_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&status_param)) {
			if (status_param.dwReturn == MCI_MODE_NOT_READY) {
			}
			else if (status_param.dwReturn == MCI_MODE_STOP) {
				if (sound_info->repeat > 0) {
					mciSendCommand(sound_info->device_id, MCI_SEEK, MCI_SEEK_TO_START, 0);
					mciSendCommand(sound_info->device_id, MCI_PLAY, 0, 0);
				}
			}
		}
		break;
	default:
		break;
	}
}	//MciUpdateSound

/**
 * @brief	再生音量を設定する
 *
 * @param	sound_id [入力] サウンド ハンドル
 * @param	percent [入力] 音量 (0 ～ 100)
 */
void MciSetVolume(int* sound_id, int percent)
{
	MciSoundInfo* sound_info;
	MCI_DGV_SETAUDIO_PARMS	audio_param;
	DWORD volume;

	if (!sound_id) {
		return;
	}
	sound_info = (MciSoundInfo*)sound_id;
	if (!sound_info->device_id) {
		return;
	}
	switch (sound_info->device_type) {
	case MCI_DEVTYPE_DIGITAL_VIDEO:
		ZeroMemory(&audio_param, sizeof(audio_param));
		audio_param.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		audio_param.dwValue = percent * 10;
		mciSendCommand(sound_info->device_id, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD_PTR)&audio_param);
		break;
	case MCI_DEVTYPE_SEQUENCER:
		volume = 0x0ffff * percent / 100;
		midiOutSetVolume(0, (DWORD)MAKELONG(volume, volume));
		break;
	case MCI_DEVTYPE_WAVEFORM_AUDIO:
		volume = 0x0ffff * percent / 100;
		waveOutSetVolume(0, (DWORD)MAKELONG(volume, volume));
		break;
	default:
		break;
	}
}	//MciSetVolume

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
#ifdef CONIOEX_DDA_SHAPE
//================================================================
//	図形の描画（DDAで描画）
//================================================================
template<typename T>
inline void swap(T& _a, T& _b) { T tmp = _a; _a = _b; _b = tmp; }
#ifdef _DEBUG
int g_dbg_draw_count = 0;	//描画ループの回数等を測るカウンタ
#endif // _DEBUG
//================================================================
// 線分
//================================================================
//=== 水平線 ===
/*
* @brief	水平線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _x2		終了Ｘ座標（Ｙ座標は_y1と同じ）
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLineH(int _x1, int _y1, int _x2, RGBQUAD _rgb)
{
	if (_x2 < _x1) {
		//left<Rightにする
		swap(_x1, _x2);
	}
	for (; _x1 <= _x2; _x1++) {
		DrawPixel(_x1, _y1, _rgb);
	}
}
/*
* @brief	水平線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _x2		終了Ｘ座標（Ｙ座標は_y1と同じ）
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLineH(int _x1, int _y1, int _x2, int _cc)
{
	DrawLineH(_x1, _y1, _x2, g_PaletteD2D[_cc % NUM_D2D_PAL]);
}	//DrawLineH
//=== 垂直線 ===
/*
* @brief	垂直線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _y2		終了Ｙ座標（Ｘ座標は_x1と同じ）
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLineV(int _x1, int _y1, int _y2, RGBQUAD _rgb)
{
	if (_y2 < _y1) {
		//Top<Bottomにする
		swap(_y1, _y2);
	}
	for (; _y1 <= _y2; _y1++) {
		DrawPixel(_x1, _y1, _rgb);
	}
}	//DrawLineV
/*
* @brief	垂直線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _y2		終了Ｙ座標（Ｘ座標は_x1と同じ）
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLineV(int _x1, int _y1, int _y2, int _cc)
{
	DrawLineV(_x1, _y1, _y2, g_PaletteD2D[_cc % NUM_D2D_PAL]);
}	//DrawLineV
//=== 45度傾斜線 ===
/*
* @brief	45度の直線の描画
* @param	int _x1,_y2	開始座標
* @param	int _len	長さ
* @param	int _dir	方向
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLine45(int _x1, int _y1, int _len, int _dir, RGBQUAD _rgb)
{
	//45度
	switch (_dir) {
	case 0:	//右下45度＼
		_len = _x1 + _len;
		for (; _x1 < _len; _x1++, _y1++) {
			DrawPixel(_x1, _y1, _rgb);
		}
		break;
	case 1:	//左下45度／
		_len = _y1 + _len;
		for (; _y1 < _len; _x1--, _y1++) {
			DrawPixel(_x1, _y1, _rgb);
		}
		break;
	case 2:	//右上45度／
		_len = _x1 + _len;
		for (; _x1 < _len; _x1++, _y1--) {
			DrawPixel(_x1, _y1, _rgb);
		}
		break;
	case 3:	//左上45度＼
		_len = _x1 - _len;
		for (; _x1 > _len; _x1--, _y1--) {
			DrawPixel(_x1, _y1, _rgb);
		}
		break;
	}
}
/*
* @brief	45度の直線の描画
* @param	int _x1,_y2	開始座標
* @param	int _len	長さ
* @param	int _dir	方向
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLine45(int _x1, int _y1, int _len, int _dir, int _cc)
{
	DrawLine45(_x1, _y1, _len, _dir, g_PaletteD2D[_cc % NUM_D2D_PAL]);
}
//=== 直線(線分) ===
/*
* @brief	直線の描画
* @param	int _x1,_y2	開始座標
* @param	int _x2,_y2	終了座標
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLine(int _x1, int _y1, int _x2, int _y2, RGBQUAD _rgb)
{
	if (_x1 == _x2 && _y1 == _y2) {
		DrawPixel(_x1, _y1, _rgb);	//点
		return;
	}
	else if (_y1 == _y2) {
		DrawLineH(_x1, _y1, _x2, _rgb);	//水平線
		return;
	}
	else if (_x1 == _x2) {
		DrawLineV(_x1, _y1, _y2, _rgb);	//垂直線
		return;
	}
	//DDA-line
	int dx = abs(_x2 - _x1);	//幅
	int dy = abs(_y2 - _y1);	//高
	int err = dx - dy;	//幅と高の差分(+)なら横長(-)なら縦長
	if (err == 0) {
		int area = 0;
		if (_x2 < _x1)area |= 1;
		if (_y2 < _y1)area |= 2;
		DrawLine45(_x1, _y1, dx, area, _rgb);	//45度
		return;
	}
	int sx = (_x1 < _x2) ? (1) : (-1);	//X方向の符号
	int sy = (_y1 < _y2) ? (1) : (-1);	//Y方向の符号
	do {
		DrawPixel(_x1, _y1, _rgb);
		int e2 = (err << 1);	//
		if (e2 > -dy) {
			err -= dy;
			_x1 += sx;
		}
		if (e2 < dx) {
			err += dx;
			_y1 += sy;
		}
	} while ((_x1 != _x2) || (_y1 != _y2));
	return;
}	//DrawLine
/*
* @brief	直線の描画
* @param	int _x1,_y2	開始座標
* @param	int _x2,_y2	終了座標
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLine(int _x1, int _y1, int _x2, int _y2, int _cc)
{
	DrawLine(_x1, _y1, _x2, _y2, g_PaletteD2D[_cc % NUM_D2D_PAL]);
}	//DrawLine
//================================================================
// 矩形
//================================================================
/*
* @brief	矩形を描画する
* @param	int _x1,_y1	左上座標
* @param	int _x2,_y2	右下座標
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
void DrawRect(int _x1, int _y1, int _x2, int _y2, RGBQUAD _rgb, bool _fill)
{
	if (_fill) {
		if (_y2 < _y1)swap(_y1, _y2);
		for (; _y1 <= _y2; _y1++) {
			DrawLineH(_x1, _y1, _x2, _rgb);	//Top-line
		}
	}
	else {
		DrawLineH(_x1, _y1, _x2, _rgb);	//Top-line
		DrawLineH(_x1, _y2, _x2, _rgb);	//Bottom-line
		DrawLineV(_x1, _y1, _y2, _rgb);	//Left-line
		DrawLineV(_x2, _y1, _y2, _rgb);	//Right-line
	}
}	//DrawRect
/*
* @brief	矩形を描画する
* @param	int _x1,_y1	左上座標
* @param	int _x2,_y2	右下座標
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
void DrawRect(int _x1, int _y1, int _x2, int _y2, int _cc, bool _fill) {
	DrawRect(_x1, _y1, _x2, _y2, g_PaletteD2D[_cc % NUM_D2D_PAL], _fill);
}
//================================================================
// 円形
//================================================================
/*
* @brief	円を描画する
* @param	int _cx,_cy	中心座標
* @param	int _r		半径
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
void DrawCircle(register int _cx, register int _cy, register int _r, RGBQUAD _rgb, register bool _fill)
{
	register int D = _r;
	register int x = (D - 1);
	register int y = 0;
	register int cxL = _cx - x;
	register int cxR = _cx + x;
	register int cyU = _cy - x;
	register int cyD = _cy + x;
	register int cxmy = _cx;// - y;
	register int cxpy = _cx;// + y;
	register int cypy = _cy;// + y;
	register int cymy = _cy;// - y;

	if (_fill) {
		while (x >= y) {
			//右回りで水平が０度：１回で８ドット（８象限分）を描く
			//塗りつぶすので水平方向中方からからそれぞれの点へ線を引く
			//DrawLineH(cxL, cypy, cxR, _rgb);	//第４象限：１３５°～｜第１象限：　　０°～
			//DrawLineH(cxL, cymy, cxR, _rgb);	//第５象限：１８０°～｜第８象限：３１５°～３６０°
			for (int ix = cxL; ix < cxR; ix++) {
				DrawPixel(ix, cypy, _rgb);
				DrawPixel(ix, cymy, _rgb);
			}
			//DrawLineH(cxmy, cyU, cxpy, _rgb);	//第６象限：２２５°～｜第７象限：２７０°～
			//DrawLineH(cxmy, cyD, cxpy, _rgb);	//第３象限：　９０°～｜第２象限：　４５°～
			for (int ix = cxmy; ix < cxpy; ix++) {
				DrawPixel(ix, cyU, _rgb);
				DrawPixel(ix, cyD, _rgb);
			}
			//---
			//DrawLineH(_cx, _cy + y, _cx + x,/* _cy + y,*/ _rgb);	//第１象限：　　０°～
			//DrawLineH(_cx, _cy + x, _cx + y,/* _cy + x,*/ _rgb);	//第２象限：　４５°～
			//DrawLineH(_cx, _cy + x, _cx - y,/* _cy + x,*/ _rgb);	//第３象限：　９０°～
			//DrawLineH(_cx, _cy + y, _cx - x,/* _cy + y,*/ _rgb);	//第４象限：１３５°～
			//DrawLineH(_cx, _cy - y, _cx - x,/* _cy - y,*/ _rgb);	//第５象限：１８０°～
			//DrawLineH(_cx, _cy - x, _cx - y,/* _cy - x,*/ _rgb);	//第６象限：２２５°～
			//DrawLineH(_cx, _cy - x, _cx + y,/* _cy - x,*/ _rgb);	//第７象限：２７０°～
			//DrawLineH(_cx, _cy - y, _cx + x,/* _cy - y,*/ _rgb);	//第８象限：３１５°～３６０°
			D -= (y << 1);
			if (D <= 0) {
				x--;
				cxL = _cx - x;
				cxR = _cx + x;
				cyU = _cy - x;
				cyD = _cy + x;
				D += (x << 1);
			}
			y++;
			cxmy--;
			cxpy++;
			cymy--;
			cypy++;
		}
	}
	else {
		while (x >= y) {
			//右回りで水平が０度：１回で８ドット（８象限分）を描く
			DrawPixel(_cx + x, _cy + y, _rgb);	//第１象限：　　０°～
			DrawPixel(_cx + y, _cy + x, _rgb);	//第２象限：　４５°～
			DrawPixel(_cx - y, _cy + x, _rgb);	//第３象限：　９０°～
			DrawPixel(_cx - x, _cy + y, _rgb);	//第４象限：１３５°～
			DrawPixel(_cx - x, _cy - y, _rgb);	//第５象限：１８０°～
			DrawPixel(_cx - y, _cy - x, _rgb);	//第６象限：２２５°～
			DrawPixel(_cx + y, _cy - x, _rgb);	//第７象限：２７０°～
			DrawPixel(_cx + x, _cy - y, _rgb);	//第８象限：３１５°～３６０°
			D -= (y << 1);
			if (D <= 0) {
				x--;
				D += (x << 1);
			}
			y++;
		}
	}
}	//DDA_Circle
/*
* @brief	円を描画する
* @param	int _cx,_cy	中心座標
* @param	int _r		半径
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
void DrawCircle(int _cx, int _cy, int _r, int _cc, bool _fill) {
	DrawCircle(_cx, _cy, _r, g_PaletteD2D[_cc % NUM_D2D_PAL], _fill);
}
//================================================================
// 三角形
//================================================================
/*
* @brief	三角形の描画
*
* @param	int _x0,_y0	頂点＃０
* @param	int _x1,_y1	頂点＃１
* @param	int _x2,_y2	頂点＃２
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
struct LY {
	int x;
	bool f;
};
static inline int min3(int a, int b, int c) { return min(c, min(a, b)); }
static inline int max3(int a, int b, int c) { return max(c, max(a, b)); }
static inline void swap(int& _a, int& _b) {
	int tmp = _a;
	_a = _b;
	_b = tmp;
}
static inline void point_swap(int& _x1, int& _y1, int& _x2, int& _y2) {
	swap(_x1, _x2);
	swap(_y1, _y2);
}
//三角形の左側の辺
static void dda_lineL(int _x1, int _x2, LY* _vL, size_t _size)
{
	//DDA-line
	const int Xsz = abs(_x2 - _x1) + 1;	//幅
	const int Ysz = (int)_size;	//高
	int err = (Xsz - Ysz);	//幅と高の差分(+)なら横長(-)なら縦長
	const int XS = (_x1 < _x2) ? (1) : (-1);	//X方向の符号
	int iy = 0;
	do {
		//DrawPixel(_x1, _y1, _rgb)
		if (!_vL->f) {
			//未だ書かれていない
			_vL->x = _x1;
			_vL->f = true;	//書いた
		}
		else if (_vL->x > _x1) {
			//すでに書かれているが右側である
			_vL->x = _x1;	//より左側の座標をセットする
		}
		const int e2 = (err << 1);	//0.5
		if (e2 > -Ysz) {
			err -= Ysz;
			_x1 += XS;
		}
		if (e2 < Xsz) {
			err += Xsz;
			_vL++;
			iy++;
		}
	} while (iy < (int)_size);
}	//dda_lineL
//三角形の右側の辺
static void dda_lineR(int _x1, int _x2, LY* _vR, size_t _size)
{
	//DDA-line
	const int Xsz = abs(_x2 - _x1) + 1;	//幅
	const int Ysz = (int)_size;	//高
	int err = (Xsz - Ysz);	//幅と高の差分(+)なら横長(-)なら縦長
	const int XS = (_x1 < _x2) ? (1) : (-1);	//X方向の符号
	int iy = 0;
	do {
		//DrawPixel(_x1, _y1, _rgb)
		if (!_vR->f) {
			//未だ書かれていない
			_vR->x = _x1;
			_vR->f = true;
		}
		else if (_vR->x < _x1) {
			//すでに書かれているが右側である
			_vR->x = _x1;	//より左側の座標をセットする
		}
		int e2 = (err << 1);	//0.5
		if (e2 > -Ysz) {
			err -= Ysz;
			_x1 += XS;
		}
		if (e2 < Xsz) {
			err += Xsz;
			_vR++;
			iy++;
		}
	} while (iy < (int)_size);
}	//dda_lineR

/*
* @brief	３頂点をＹ座標の小さい順に並べ替える
*
* @param	int x0,y0, x1,y1, x2,y2	点０～点２
*
*/
static void TriSort(int& _x0, int& _y0, int& _x1, int& _y1, int& _x2, int& _y2)
{
	//Y座標順にP0～P2を並べ変える
	int minY = min3(_y0, _y1, _y2);
	if (minY == _y0) {
		//P0が一番小さい
		int yy = min(_y1, _y2);
		if (yy == _y2) {
			//P2が二番目に小さいのでP1と入れ替える
			point_swap(_x1, _y1, _x2, _y2);	//P1 <-> P2
		}
	}
	else if (minY == _y1) {
		//P1が一番小さい場合
		//P0とP1を入れ替える
		point_swap(_x0, _y0, _x1, _y1);	//P0 <-> P1
		int yy = min(_y1, _y2);
		if (yy == _y2) {
			//P2が二番目に小さいのでP1と入替
			point_swap(_x1, _y1, _x2, _y2);	//P1 <-> P2
		}
	}
	else if (minY == _y2) {
		//P2が一番小さい場合
		//P0とP2を入れ替える
		point_swap(_x0, _y0, _x2, _y2);
		int yy = min(_y1, _y2);
		if (yy == _y2) {
			//P2が二番目に小さいのでP1と入替
			point_swap(_x1, _y1, _x2, _y2);	//P1 <-> P2
		}
	}
}	//TriSort
/*
*
*/
void DrawTriangle(int _x0, int _y0, int _x1, int _y1, int _x2, int _y2, RGBQUAD _rgb, bool _fill)
{
	//最初に「点」と「直線」になる座標は省く（点・直線の描画にする）
	if ((_x0 == _x1 && _x1 == _x2) && (_y0 == _y1 && _y1 == _y2)) {
		//全て同じ座標なので「点」である
		DrawPixel(_x0, _y0, _rgb);	//点
		return;
	}
	else if ((_x1 == _x2) && (_y1 == _y2)) {
		//点０～点１｜２への「直線」である
		DrawLine(_x0, _y0, _x2, _y2, _rgb);	//線
		return;
	}
	else if ((_x0 == _x2) && (_y0 == _y2)) {
		//点１～点０・２への直線である
		DrawLine(_x0, _y0, _x1, _y1, _rgb);	//線
		return;
	}
	else if ((_x0 == _x1) && (_y0 == _y1)) {
		//点２～点０｜１への直線である
		DrawLine(_x0, _y0, _x2, _y2, _rgb);	//線
		return;
	}
	if (!_fill) {
		//線だけの三角形
		DrawLine(_x0, _y0, _x1, _y1, _rgb);
		DrawLine(_x1, _y1, _x2, _y2, _rgb);
		DrawLine(_x2, _y2, _x0, _y0, _rgb);
	}
	else {
		//塗りつぶし三角形
		TriSort(_x0, _y0, _x1, _y1, _x2, _y2);
		int Ysize = (_y2 - _y0 + 1);	//高さ＝タテドット数
		if (_y1 == _y2) {
			//上向きの底辺水平三角形
			LY* const vL = (LY*)calloc(Ysize, sizeof(LY));
			LY* const vR = (LY*)calloc(Ysize, sizeof(LY));
			if (_x1 > _x2) {
				//変数xy1がＰ１の座標、変数xy2がＰ２の座標にする。
				point_swap(_x1, _y1, _x2, _y2);
			}
			//P0から左P1への直線
			dda_lineL(_x0, _x1, vL, Ysize);	//0-1 line
			vL[Ysize - 1].x = _x1;
			//P0から右P2への直線
			dda_lineR(_x0, _x2, vR, Ysize);	//0-2 line
			vR[Ysize - 1].x = _x2;
			//動かすポインタ生成
			LY* pL = vL;
			LY* pR = vR;
			int yy = _y0;
			for (; yy <= _y1; yy++) {
				DrawLineH(pL->x, yy, pR->x, _rgb);
				pL++;
				pR++;
			}
			free(vR);
			free(vL);
		}
		else if (_y0 == _y1) {
			//下向きの底辺水平三角形
			LY* const vL = (LY*)calloc(Ysize, sizeof(LY));
			LY* const vR = (LY*)calloc(Ysize, sizeof(LY));
			if (_x0 > _x1) {
				//変数xy0がＰ０の座標、変数xy1がＰ１の座標にする。
				point_swap(_x0, _y0, _x1, _y1);
			}
			//左P0からP2への直線
			dda_lineL(_x0, _x2, vL, Ysize);	//0-2 line
			vL[Ysize - 1].x = _x2;
			//右P1からP2への直線
			dda_lineR(_x1, _x2, vR, Ysize);	//1-2 line
			vR[Ysize - 1].x = _x2;
			//動かすポインタ生成
			LY* pL = vL;
			LY* pR = vR;
			int yy = _y0;
			for (; yy < _y2; yy++) {
				DrawLineH(pL->x, yy, pR->x, _rgb);
				pL++;
				pR++;
			}
			free(vR);
			free(vL);
		}
		else {
			//y1の位置で上下２つに分けて描画
			//先ずＹ座標が一番下にある辺のＹサイズを求めるＰ０～Ｐ２
			int P02_Ysize = (_y2 - _y0 + 1);
			//中間点のＸと比較して、右側か左側かを決める
			if (_x1 < _x2) {
				//右側
				int P01_Ysize = (_y1 - _y0 + 1);	//上から中間点までの高さ＝タテドット数
				int P12_Ysize = (_y2 - _y1 + 1);	//中間点から下までの高さ＝タテドット数
				LY* const vR02 = (LY*)calloc(P02_Ysize, sizeof(LY));
				LY* const vL01 = (LY*)calloc(P01_Ysize, sizeof(LY));
				LY* const vL12 = (LY*)calloc(P12_Ysize, sizeof(LY));
				//P0から左P2への直線
				dda_lineR(_x0, _x2, vR02, P02_Ysize);	//0-2 line
				vR02[P02_Ysize - 1].x = _x2;
				//P0から右P1への直線
				dda_lineL(_x0, _x1, vL01, P01_Ysize);	//0-2 line
				vL01[P01_Ysize - 1].x = _x1;
				//右P1からP2への直線
				dda_lineL(_x1, _x2, vL12, P12_Ysize);	//1-2 line
				vL12[P12_Ysize - 1].x = _x2;
				//動かすポインタ生成
				LY* p02 = vR02;
				LY* p01 = vL01;
				LY* p12 = vL12;
				int yy = _y0;
				for (; yy <= _y1; yy++) {
					DrawLineH(p01->x, yy, p02->x, _rgb);
					p01++;
					p02++;
				}
				yy--;
				p02--;
				for (; yy <= _y2; yy++) {
					DrawLineH(p12->x, yy, p02->x, _rgb);
					p12++;
					p02++;
				}
				//解放
				free(vR02);
				free(vL01);
				free(vL12);
			}
			else {
				//左側
				int P01_Ysize = (_y1 - _y0 + 1);	//上から中間点までの高さ＝タテドット数
				int P12_Ysize = (_y2 - _y1 + 1);	//中間点から下までの高さ＝タテドット数
				LY* const vL02 = (LY*)calloc(P02_Ysize, sizeof(LY));
				LY* const vR01 = (LY*)calloc(P01_Ysize, sizeof(LY));
				LY* const vR12 = (LY*)calloc(P12_Ysize, sizeof(LY));
				//P0から左P2への直線
				dda_lineL(_x0, _x2, vL02, P02_Ysize);	//0-2 line
				vL02[P02_Ysize - 1].x = _x2;
				//P0から右P1への直線
				dda_lineR(_x0, _x1, vR01, P01_Ysize);	//0-2 line
				vR01[P01_Ysize - 1].x = _x1;
				//右P1からP2への直線
				dda_lineR(_x1, _x2, vR12, P12_Ysize);	//1-2 line
				vR12[P12_Ysize - 1].x = _x2;
				//動かすポインタ生成
				LY* p02 = vL02;
				LY* p01 = vR01;
				LY* p12 = vR12;
				int yy = _y0;
				for (; yy <= _y1; yy++) {
					DrawLineH(p01->x, yy, p02->x, _rgb);
					p01++;
					p02++;
				}
				p02--;
				yy--;
				for (; yy <= _y2; yy++) {
					DrawLineH(p12->x, yy, p02->x, _rgb);
					p12++;
					p02++;
				}
				//解放
				free(vL02);
				free(vR01);
				free(vR12);
			}
		}
		//_rgb = RGBQUAD{ 0,0,255 };
		//DrawLine(_x0, _y0, _x1, _y1, _rgb);
		//_rgb = RGBQUAD{ 0,255,0 };
		//DrawLine(_x1, _y1, _x2, _y2, _rgb);
		//_rgb = RGBQUAD{ 255,0,0 };
		//DrawLine(_x2, _y2, _x0, _y0, _rgb);
	}
}
/*
* @brief	三角形の描画
*
* @param	int _x0,_y0	頂点＃０
* @param	int _x1,_y1	頂点＃１
* @param	int _x2,_y2	頂点＃２
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
void DrawTriangle(int _x0, int _y0, int _x1, int _y1, int _x2, int _y2, int _cc, bool _fill)
{
	DrawTriangle(_x0, _y0, _x1, _y1, _x2, _y2, g_PaletteD2D[_cc % NUM_D2D_PAL], _fill);
}

#endif // !CONIOEX_DDA_SHAPE

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################

//================================================================
//	フレーム同期＆計測用関数
#define	USE_NONE	0
#define	USE_MMSEC	1	//mm(ミリ)秒精度"timeGetTime()"を使う
#define	USE_QPC		2	//μ(マイクロ)秒精度"QueryPerformanceCounter"を使う
#define	USE_RDTSC	3	//"ReaD Time Stamp Counter"を使う（CPUのクロックカウンタ）
#define	FRAME_SYNC	USE_MMSEC	//USE_NONE

#if (FRAME_SYNC==USE_RDTSC)
#include	"intrin.h"
#endif // USE_RDTSC

//フレームスピード計測用
volatile static double FPS = 60.0;
static char g_FrameSyncStr[4096] = {};	//デバッグ用文字列
volatile int g_FrameCountPerSec = 0;	//フレーム回数
volatile double g_FrameCalcFPS = 60.0;	//FPS
volatile UINT64 g_FrameCounter = 0;	//フレームカウンタ
#ifdef _DEBUG
static DEBUG_FRAME_INFO g_pDFI;
void FrameSyncFunc(DEBUG_FRAME_INFO* _p)
{
	SetCaptionF("[fps:%7.2f][count:%3d][1sec:%7.2f][tpf:%7.2f][wait:%7.2f][interval:%7.2f][total:%7.2f]\n",
		(double)_p->fps,	//１秒間のフレーム数（平均値）
		(int)_p->fcnt,		//１秒間のフレーム数（カウンタ）
		(double)_p->tps,	//
		(double)_p->tpf,
		(double)_p->wait,
		(double)_p->itrvl,
		(double)_p->total
	);
	GetCaption(g_FrameSyncStr, sizeof(g_FrameSyncStr));
	OutputDebugStringA(g_FrameSyncStr);
}
#endif // _DEBUG

#if (FRAME_SYNC==USE_RDTSC)
// 高詳細計測用
volatile static __int64	i64_frequency = 0;
volatile static __int64	i64_t1 = 0;
volatile static __int64	i64_t2 = 0;
volatile static __int64	i64_t3 = 0;
volatile static double f_1sec = 0.0;		//１秒のカウント数
volatile static double f_tpf = 0.0;		//１フレームのカウント数（分解能）
#ifdef _DEBUG
static volatile double f_total = 0.0;
#endif // _DEBUG
/*
* フレーム同期
* 高解像度タイム スタンプを使って計測
*/
//初期化
void InitFrameSync(double _FPS)
{
	if (_FPS <= 0) {
		//範囲外は１FPSとする
		_FPS = 60.0;
	}
	FPS = _FPS;
	timeBeginPeriod(1);	//計測精度を1ミリ秒に設定
	i64_t1 = __rdtsc();
	Sleep(1000 / 10);	//計測基準として一定時間待つ（1/10秒）
	i64_t2 = __rdtsc();
	//1/10秒間のt1～t2間のカウントから１秒間のカウントを算出
	i64_frequency = (i64_t2 - i64_t1) * (__int64)10;	//1513233427/1681604920/1813127620
	f_1sec = (double)i64_frequency;	//１秒間のカウント数（分解能）
	f_tpf = (f_1sec / FPS);	//１フレームのカウント数（分解能）
	g_FrameCounter = 0;
}
//同期
#ifdef _DEBUG
void FrameSync(bool _debug_out, void (*_pFrameFunc)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	i64_t2 = __rdtsc();	//現在時間取得
	volatile double f_frame_interval = (double)(i64_t2 - i64_t1);	//フレーム間隔(時間)算出
	volatile double f_wait = (f_tpf - f_frame_interval);	//待ち時間を算出（待ち時間＝１フレームに必要な時間－実際に掛かった時間）
	if (f_wait > 0) {
		volatile __int64 t2 = 0;
		do {
			//Sleep(1);
			t2 = __rdtsc();	//現在時間取得
			//このループの経過時間を測り、待ち時間以下ならループ継続する。
		} while ((double)(t2 - i64_t2) < f_wait);
		//１フレーム時間を積算（１フレーム時間＝前のフレームから今のフレームまでの経過時間＋足りなかった分の待ち時間）
		f_total += (f_frame_interval + f_wait);
	}
	else {
		//待ち時間が無かったので１フレーム時間だけを積算
		f_total += f_frame_interval;
	}
	g_FrameCounter++;	//フレームカウンタ
	g_FrameCountPerSec++;	//１秒間のフレーム数カウント(デバッグ用)
	if ((f_total >= f_1sec) && (g_FrameCountPerSec > 0)) {
		volatile double fps = f_1sec / (f_total / (double)g_FrameCountPerSec);
		g_FrameCalcFPS = fps;
		if (_debug_out) {
			g_pDFI.fps = (double)g_FrameCalcFPS;
			g_pDFI.fcnt = (int)g_FrameCountPerSec;
			g_pDFI.tps = (double)f_1sec;
			g_pDFI.tpf = (double)f_tpf;
			g_pDFI.wait = (double)f_wait;
			g_pDFI.itrvl = (double)f_frame_interval;
			g_pDFI.total = (double)f_total;
			if (_pFrameFunc != nullptr) {
				_pFrameFunc(&g_pDFI);
			}
			else {
				FrameSyncFunc(&g_pDFI);
			}
		}
		g_FrameCountPerSec = 0;
		f_total = 0.0;
	}
	i64_t1 = __rdtsc();	//計測開始時間取得
}
#else	//_DEBUG
void FrameSync(bool, void (*)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	i64_t2 = __rdtsc();	//t1～t2計測用の開始時間(t2)取得
	volatile double f_frame_interval = (double)(i64_t2 - i64_t1);	//フレーム間隔(時間)算出
	volatile double f_wait = (f_tpf - f_frame_interval);	//待ち時間を算出（待ち時間＝１フレームに必要な時間－実際に掛かった時間）
	if (f_wait > 0) {
		volatile __int64 t2 = 0;
		do {
			//Sleep(1);
			t2 = __rdtsc();	//現在時間取得
			//このループの経過時間を測り、待ち時間以下ならループ継続する。
		} while ((double)(t2 - i64_t2) < f_wait);
	}
	g_FrameCounter++;	//フレームカウンタ
	g_FrameCountPerSec++;	//１秒間のフレーム数カウント
	volatile __int64 t3 = (__rdtsc() - i64_t3);	//前回のFPS計測からの経過時間（rdtscカウント数）を求める
	if (t3 > f_1sec) {
		//１秒経った⇒１秒間に数えたフレームカウントをFPSとする
		g_FrameCalcFPS = (double)g_FrameCountPerSec;	//１秒のフレーム数
		g_FrameCountPerSec = 0;	//新たなカウントの為にリセット
		i64_t3 = __rdtsc();	//FPS用計測開始時間の更新
	}
	i64_t1 = __rdtsc();	//t1～t2計測用の開始時間(t1)取得
}
#endif // _DEBUG
#elif (FRAME_SYNC==USE_QPC)
// 高詳細計測用
volatile LARGE_INTEGER	li_frequency = {};
volatile LARGE_INTEGER	li_t1 = {};
volatile LARGE_INTEGER	li_t2 = {};
volatile LARGE_INTEGER	li_t3 = {};
volatile double f_1sec = 0.0;	//１秒のカウント数
volatile double f_tpf = 0.0;		//１フレームのカウント数（分解能）
#ifdef _DEBUG
volatile double f_total = 0.0;
#endif // _DEBUG
/*
* フレーム同期
* 高解像度タイム スタンプを使って計測
*/
//初期化
void InitFrameSync(double _FPS)
{
	if (_FPS <= 0) {
		//範囲外は１FPSとする
		_FPS = 60.0;
	}
	FPS = _FPS;
	timeBeginPeriod(1);	//計測精度を1ミリ秒に設定
	//分解能：カウンタの周波数（１秒間に何カウント行うか）を取得
	if (!QueryPerformanceFrequency(const_cast<LARGE_INTEGER*>(&li_frequency))) {
		//失敗した場合は、10,000,000Hz(10MHz)にする。
		li_frequency.QuadPart = (1000 * 1000 * 10);	//分解能＝(1/10,000,000)
	}
	f_1sec = (double)li_frequency.QuadPart;	//１秒間のカウント数（分解能）
	f_tpf = (f_1sec / FPS);	//１フレームのカウント数（分解能）
	g_FrameCounter = 0;
}	//InitFrameSync
//同期
#ifdef _DEBUG
void FrameSync(bool _debug_out, void (*_pFrameFunc)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&li_t2));	//t1～t2計測用の開始時間(t2)取得
	volatile double f_frame_interval = (double)(li_t2.QuadPart - li_t1.QuadPart);	//フレーム間隔(時間)算出
	volatile double f_wait = (f_tpf - f_frame_interval);	//待ち時間を算出（待ち時間＝１フレームに必要な時間－実際に掛かった時間）
	if (f_wait > 0) {
		volatile LARGE_INTEGER t2 = {};
		do {
			//Sleep(1);
			//std::this_thread::yield();
			QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&t2));	//現在時間取得
			//このループの経過時間を測り、待ち時間以下ならループ継続する。
		} while ((double)(t2.QuadPart - li_t2.QuadPart) < f_wait);
		//１フレーム時間を積算（１フレーム時間＝前のフレームから今のフレームまでの経過時間＋足りなかった分の待ち時間）
		f_total += (f_frame_interval + f_wait);
	}
	else {
		//待ち時間が無かったので１フレーム時間だけを積算
		f_total += f_frame_interval;
	}
	g_FrameCounter++;	//フレームカウンタ
	g_FrameCountPerSec++;	//１秒間のフレーム数カウント(デバッグ用)
	if ((f_total >= f_1sec) && (g_FrameCountPerSec > 0)) {
		volatile double fps = f_1sec / (f_total / (double)g_FrameCountPerSec);
		g_FrameCalcFPS = fps;
		if (_debug_out) {
			g_pDFI.fps = (double)g_FrameCalcFPS;
			g_pDFI.fcnt = (int)g_FrameCountPerSec;
			g_pDFI.tps = (double)f_1sec;
			g_pDFI.tpf = (double)f_tpf;
			g_pDFI.wait = (double)f_wait;
			g_pDFI.itrvl = (double)f_frame_interval;
			g_pDFI.total = (double)f_total;
			if (_pFrameFunc != nullptr) {
				_pFrameFunc(&g_pDFI);
			}
			else {
				FrameSyncFunc(&g_pDFI);
			}
		}
		g_FrameCountPerSec = 0;
		f_total = 0.0;
	}
	QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&li_t1));	//t1～t2計測用の開始時間(t1)取得
}
#else	// _DEBUG
void FrameSync(bool, void (*)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&li_t2));	//t1～t2計測用の開始時間(t2)取得
	volatile double f_frame_interval = (double)(li_t2.QuadPart - li_t1.QuadPart);	//フレーム間隔(時間)算出
	volatile double f_wait = (f_tpf - f_frame_interval);	//待ち時間を算出（待ち時間＝１フレームに必要な時間－実際に掛かった時間）
	if (f_wait > 0) {
		volatile LARGE_INTEGER t2 = {};
		do {
			//Sleep(1);
			//std::this_thread::yield();
			QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&t2));	//現在時間取得
			//このループの経過時間を測り、待ち時間以下ならループ継続する。
		} while ((double)(t2.QuadPart - li_t2.QuadPart) < f_wait);
	}
	g_FrameCounter++;	//フレームカウンタ
	g_FrameCountPerSec++;	//１秒間のフレーム数カウント
	volatile LARGE_INTEGER t3 = {};
	QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&t3));	//前回のFPS計測からの経過時間（rdtscカウント数）を求める
	if ((t3.QuadPart - li_t3.QuadPart) > f_1sec) {
		//１秒経った⇒１秒間に数えたフレームカウントをFPSとする
		g_FrameCalcFPS = (double)g_FrameCountPerSec;	//１秒のフレーム数
		g_FrameCountPerSec = 0;	//新たなカウントの為にリセット
		QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&li_t3));//FPS用計測開始時間の更新
	}
	QueryPerformanceCounter(const_cast<LARGE_INTEGER*>(&li_t1));	//t1～t2計測用の開始時間(t1)取得
}
#endif // _DEBUG
#elif (FRAME_SYNC==USE_MMSEC)
volatile DWORD	dd_frequency = 0;
volatile DWORD	dd_t1 = 0;
volatile DWORD	dd_t2 = 0;
volatile DWORD	dd_t3 = 0;
volatile double f_1sec = 0.0;	//１秒のカウント数
volatile double f_tpf = 0.0;		//１フレームの時間（ミリ秒）
#ifdef _DEBUG
volatile double f_total = 0.0;
#endif // _DEBUG
/*
* フレーム同期
* 高解像度タイム スタンプを使って計測
*/
//初期化
void InitFrameSync(double _FPS)
{
	if (_FPS <= 0) {
		//範囲外は１FPSとする
		_FPS = 60.0;
	}
	FPS = _FPS;
	timeBeginPeriod(1);				//分解能を１ミリ秒に設定
	dd_frequency = 1000;			//１秒のカウント数（１０００ミリ秒）
	f_1sec = (double)dd_frequency;	//１秒のカウント数（１０００ミリ秒）
	f_tpf = (f_1sec / FPS);	//１フレームの時間（ミリ秒）
	g_FrameCounter = 0;
}	//InitFrameSync
//同期
#ifdef _DEBUG
void FrameSync(bool _debug_out, void (*_pFrameFunc)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	dd_t2 = timeGetTime();	//現在時間取得
	volatile double f_frame_interval = (double)(dd_t2 - dd_t1);	//t1～t2計測用の開始時間(t2)取得
	volatile double f_wait = (f_tpf - f_frame_interval);	//待ち時間を算出（待ち時間＝１フレームに必要な時間－実際に掛かった時間）
	if (f_wait > 0) {
		volatile DWORD t2 = 0;
		do {
			Sleep(1);
			//std::this_thread::yield();
			t2 = timeGetTime();	//現在時間取得
			//このループの経過時間を測り、待ち時間以下ならループ継続する。
		} while ((double)(t2 - dd_t2) < f_wait);
		//１フレーム時間を積算（１フレーム時間＝前のフレームから今のフレームまでの経過時間＋足りなかった分の待ち時間）
		f_total += (f_frame_interval + f_wait);
	}
	else {
		//待ち時間が無かったので１フレーム時間だけを積算
		f_total += f_frame_interval;
	}
	g_FrameCounter++;	//フレームカウンタ
	g_FrameCountPerSec++;	//１秒間のフレーム数カウント(デバッグ用)
	if ((f_total >= f_1sec) && (g_FrameCountPerSec > 0)) {
		volatile double fps = f_1sec / (f_total / (double)g_FrameCountPerSec);
		g_FrameCalcFPS = fps;
		if (_debug_out) {
			g_pDFI.fps = (double)g_FrameCalcFPS;
			g_pDFI.fcnt = (int)g_FrameCountPerSec;
			g_pDFI.tps = (double)f_1sec;
			g_pDFI.tpf = (double)f_tpf;
			g_pDFI.wait = (double)f_wait;
			g_pDFI.itrvl = (double)f_frame_interval;
			g_pDFI.total = (double)f_total;
			if (_pFrameFunc != nullptr) {
				_pFrameFunc(&g_pDFI);
			}
			else {
				FrameSyncFunc(&g_pDFI);
			}
		}
		g_FrameCountPerSec = 0;
		f_total = 0.0;
	}
	dd_t1 = timeGetTime();	//t1～t2計測用の開始時間(t1)取得
}
#else	// _DEBUG
void FrameSync(bool, void (*)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	dd_t2 = timeGetTime();	//t1～t2計測用の開始時間(t2)取得
	volatile double f_frame_interval = (double)(dd_t2 - dd_t1);	//フレーム間隔(時間)算出
	volatile double f_wait = (f_tpf - f_frame_interval);	//待ち時間を算出（待ち時間＝１フレームに必要な時間－実際に掛かった時間）
	if (f_wait > 0) {
		volatile DWORD t2 = 0;
		do {
			Sleep(1);
			//std::this_thread::yield();
			t2 = timeGetTime();	//現在時間取得
			//このループの経過時間を測り、待ち時間以下ならループ継続する。
		} while ((double)(t2 - dd_t2) < f_wait);
	}
	g_FrameCounter++;	//フレームカウンタ
	g_FrameCountPerSec++;	//１秒間のフレーム数カウント
	DWORD t3 = (timeGetTime() - dd_t3);	//前回のFPS計測からの経過時間（rdtscカウント数）を求める
	if (t3 > f_1sec) {
		//１秒経った⇒１秒間に数えたフレームカウントをFPSとする
		g_FrameCalcFPS = (double)g_FrameCountPerSec;	//１秒のフレーム数
		g_FrameCountPerSec = 0;	//新たなカウントの為にリセット
		dd_t3 = timeGetTime();	//FPS用計測開始時間の更新
	}
	dd_t1 = timeGetTime();	//t1～t2計測用の開始時間(t1)取得
}
#endif // _DEBUG
#else	// USE_RDTSC
//DWORD	dd_frequency = 0;
static volatile int	dd_t1 = 0;
static volatile int	dd_t2 = 0;
static volatile int mmpf = 0;
/*
* @brief	フレーム同期の初期化
*/
void InitFrameSync(double _fps)
{
	if (_fps <= 0) {
		_fps = 1;
	}
	FPS = _fps;
	timeBeginPeriod(1);		//分解能を１ミリ秒に設定
	dd_t1 = timeGetTime();	//t1～t2計測用の開始時間(t1)取得
	mmpf = (int)(1000.0 / FPS);
	g_FrameCounter = 0;
}	//InitFrameSync
/*
* @brief	フレーム同期
*/
void FrameSync(bool, void (*)(DEBUG_FRAME_INFO*))
{
	//フレーム待ち時間計測
	dd_t2 = timeGetTime();	//t1～t2計測用の開始時間(t2)取得
	volatile int t_wait = (mmpf - (dd_t2 - dd_t1));
	if (t_wait > 0) {
		Sleep(t_wait);
	}
	g_FrameCounter++;	//フレームカウンタ
	dd_t1 = timeGetTime();	//t1～t2計測用の開始時間(t1)取得
}
#endif // USE_RDTSC

//################################################################################################################################
//################################################################################################################################
//################################################################################################################################
//================================================================
//	D2D初期化
//================================================================
bool InitD2D(int _window_width, int _window_height)
{
	/*
	* https://learn.microsoft.com/ja-jp/windows/win32/direct2d/improving-direct2d-performance
	* マルチスレッド デバイス コンテキストを使用する：
	*	https://learn.microsoft.com/ja-jp/windows/win32/direct2d/improving-direct2d-performance#use-a-multithreaded-device-context
	*	大量の複雑な幾何学的コンテンツをレンダリングすることを想定しているアプリケーションでは、
	*	Direct2D デバイス コンテキストを作成するときに、
	*	D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTI_THREADED_OPTIMIZATIONS フラグを指定することを
	*	検討する必要があります。
	*	このフラグを指定すると、Direct2D はシステムに存在するすべての論理コアにレンダリングを分散します。
	*	これにより、全体的なレンダリング時間が大幅に短縮されます。
	* Direct2D を使用したテキストの描画：
	*/
	HRESULT hr;	//
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
	_ASSERT(hr == S_OK);
#if false
	RECT cr;
	GetClientRect(g_hConWnd, &cr);	//クライアント領域の{0,0,幅,高さ}取得
	_window_width = cr.right;
	_window_height = cr.bottom;
	D2D1_RENDER_TARGET_PROPERTIES rtp = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hw_rtp = D2D1::HwndRenderTargetProperties(g_hConWnd, D2D1_SIZE_U{ (UINT32)_window_width, (UINT32)_window_height });
	hw_rtp.presentOptions = D2D1_PRESENT_OPTIONS_IMMEDIATELY;//★★★★★非同期（フレーム時間まで待たない）★★★★★
	hr = g_pD2DFactory->CreateHwndRenderTarget(rtp, hw_rtp, &g_pRenderTarget);
	_ASSERT(hr == S_OK);
#else
	/*
	* レンダーターゲットの種類
	* ID2D1BitmapRenderTarget ------> CreateCompatibleRenderTarget メソッドによって作成された中間テクスチャにレンダリングします。
	* ID2D1DCRenderTarget ----------> GDI デバイス コンテキストに対して描画コマンドを発行します。
	* ID2D1GdiInteropRenderTarget --> GDI 描画コマンドを受け入れることができるデバイスコンテキストへのアクセスを提供します。
	* ID2D1HwndRenderTarget --------> 描画命令をウィンドウにレンダリングします。
	*/
	//ウィンドウへの描画なので ID2D1HwndRenderTarget を使用する。
	//サイズはウィンドウと同じサイズにすること。サイズが違うと、ウィンドウに合わせて拡大・縮小がかかる。
	D2D1_SIZE_U renderTargetSize = { (UINT32)_window_width, (UINT32)_window_height };
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties;
	D2D1_PIXEL_FORMAT pixelFormat;
	pixelFormat.format = DXGI_FORMAT_UNKNOWN;	//既定の形式
	pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;
	//pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	//pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;	//ハードウェアによる描画とソフトウェアによる描画のどちらを利用するか
	renderTargetProperties.pixelFormat = pixelFormat;				//ピクセル形式とアルファモード
	renderTargetProperties.dpiX = 0;								//それぞれ水平方向と垂直方向の DPI （Donts per Inch、ピクセル密度）を指定します。
	renderTargetProperties.dpiY = 0;								//既定の DPI を使用するには 0 を指定します。
	renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;	//レンダーターゲットのリモート処理と GDI との互換性
	renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;	//ハードウェアによる描画に必要な Direct3D の最小限の機能レベル

	RECT cr;
	GetClientRect(g_hConWnd, &cr);	//クライアント領域の{0,0,幅,高さ}取得
	renderTargetSize.width = cr.right;
	renderTargetSize.height = cr.bottom;
	hwndRenderTargetProperties.hwnd = g_hConWnd;	//ターゲットとなるウィンドウのハンドル
	hwndRenderTargetProperties.pixelSize = renderTargetSize;	//ウィンドウのクライアント領域のサイズ
	hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_IMMEDIATELY;	//★★★★★非同期（フレーム時間まで待たない）★★★★★
	//hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;	//フレーム同期
	//レンダーターゲット取得
	hr = g_pD2DFactory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties, &g_pRenderTarget);
	_ASSERT(hr == S_OK);
#endif // false

	//テキスト描画用ビットマップレンダーターゲット生成
	hr = g_pRenderTarget->CreateCompatibleRenderTarget(&g_ptrBRTforText);
	_ASSERT(hr == S_OK);

	//DirectWrite用ファクトリー生成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&g_pDWFactory));
	_ASSERT(hr == S_OK);

	g_FrameBuffer32bitD2D = (RGBQUAD*)calloc(g_FrameBufferSizeD2D.X * g_FrameBufferSizeD2D.Y, sizeof(RGBQUAD));
	//デフォルト２５６パレットをANSI256色で初期化
	//最初の１６色は起動時のパレットを取り込んでいるので、１７色以降の色を取り込む。
	for (int n = 16; n < NUM_ANSI_PAL; n++) {
		g_PaletteD2D[n] = ANSI_PAL256_RGB[n];
	}
	return true;
}	//InitD2D

//==================================================================
// D2D終了
//==================================================================
void EndD2D(void)
{
	//free(g_pMask);
	if (g_FrameBuffer32bitD2D != NULL) {
		free(g_FrameBuffer32bitD2D);
		g_FrameBuffer32bitD2D = NULL;
	}

	//if (g_FrameBuffer4bitD2D != NULL) {
	//	free(g_FrameBuffer4bitD2D);
	//	g_FrameBuffer4bitD2D = NULL;
	//}

	for (ID2D1Bitmap* pbmp : g_pBmpList) {
		pbmp->Release();
	}
	g_pBmpList.clear();

	// IDWriteFactoryの破棄
	if (NULL != g_pDWFactory) {
		g_pDWFactory->Release();
		g_pDWFactory = NULL;
	}

	//テキスト描画用ビットマップレンダーターゲット破棄
	if (g_ptrBRTforText != NULL) {
		g_ptrBRTforText->Release();
		g_ptrBRTforText = NULL;
	}

	// ID2D1HwndRenderTargetの破棄
	if (NULL != g_pRenderTarget) {
		g_pRenderTarget->Release();
		g_pRenderTarget = NULL;
	}

	// ID2D1Factoryの破棄
	if (NULL != g_pD2DFactory) {
		g_pD2DFactory->Release();
		g_pD2DFactory = NULL;
	}
}	//EndD2D

//================================================================
/*
* 現在の32ビットフレームバッファ(g_FrameBuffer32bitD2D)をビットマップ(ID2D1Bitmap)に転送して、ビットマップリストに g_BmpList 追加する。
* DXGI_FORMAT_B8G8R8A8_UNORM
* DXGI_FORMAT_R8G8B8A8_UNORM
* DXGI_FORMAT_A8_UNORM
*/
static void push_screen_buffer(void)
{
	HRESULT hr = ~S_OK;
	/*
	* 描画先のビットマップを作る。サイズはフレームバッファと同じサイズ。
	*/
	D2D1_SIZE_U siz = { (UINT32)g_FrameBufferSizeD2D.X, (UINT32)g_FrameBufferSizeD2D.Y };	//フレームバッファのサイズ
	ID2D1Bitmap* pD2D1_Bmp;
	//ビットマップ作成：ピクセル＝RGBA各8ビット、不透明
	//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),	//不透明
	//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),	//不透明
	//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_STRAIGHT)),	//不透明
	//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_STRAIGHT)),	//不透明
	//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),	//不透明
	//D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)),	//NG
	hr = g_pRenderTarget->CreateBitmap(siz, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &pD2D1_Bmp);
	_ASSERT(hr == S_OK);
	if (pD2D1_Bmp == NULL) {
		return;	//ビットマップ作成失敗
	}
	/*
	* 作成したビットマップにフレームバッファを転送する。転送開始位置は左上隅（０，０）、転送サイズはフレームバッファと同じサイズ（全体）
	*/
	D2D1_RECT_U	ru = { 0,0,(UINT32)g_FrameBufferSizeD2D.X, (UINT32)g_FrameBufferSizeD2D.Y };
	hr = pD2D1_Bmp->CopyFromMemory(&ru, reinterpret_cast<const void*>(g_FrameBuffer32bitD2D), g_FrameBufferSizeD2D.X * 4);
	_ASSERT(hr == S_OK);
	//■■■■ 描画したビットマップをリストに追加 ■■■
	g_pBmpList.push_back(pD2D1_Bmp);
	//★★★★ pD2D1_Bmp の Release 不要！★★★★　＝＞　描画時に Release している ★★★★
	return;
}	//push_screen_buffer

/*
* @brief	フレームバッファをレンダーリストに追加する
*/
void PrintFrameBuffer(void)
{
	if (g_pRenderTarget == NULL) {
		return;
	}
	push_screen_buffer();
}	//PrintFrameBuffer

/*
* @brief	指定バッファ(パレット値)の内容を
*			フレームバッファにコピーしてから
*			フレームバッファをレンダーリストに追加する
*
* @param	const char* _frame_buffer	パレット値の画像バッファ
* @param	int _width, _height			パレット値の画像バッファの幅と高さ
*/
void PrintFrameBuffer(const char* _frame_buffer, int _width, int _height)
{
	const char* sp = _frame_buffer;
	RGBQUAD* dp = g_FrameBuffer32bitD2D;
	for (int iy = 0; (iy < _height) && (iy < g_FrameBufferSizeD2D.Y); iy++) {
		for (int ix = 0; (ix < _width) && (ix < g_FrameBufferSizeD2D.X); ix++) {
			dp[ix] = g_PaletteD2D[sp[ix]];
		}
		sp += _width;
		dp += g_FrameBufferSizeD2D.X;
	}
	if (g_pRenderTarget == NULL) {
		return;
	}
	push_screen_buffer();
}	//PrintFrameBuffer

/*
* @brief	指定バッファ(RGB値)の内容を
*			フレームバッファにコピーしてから
*			フレームバッファをレンダーリストに追加する
*
* @param	const RGBQUAD* _frame_buffer	RGB値の画像バッファ
* @param	int _width, _height				RGB値の画像バッファの幅と高さ
*/
void PrintFrameBuffer(const RGBQUAD* _frame_buffer, int _width, int _height)
{
	const RGBQUAD* sp = _frame_buffer;
	RGBQUAD* dp = g_FrameBuffer32bitD2D;
	for (int iy = 0; (iy < _height) && (iy < g_FrameBufferSizeD2D.Y); iy++) {
		for (int ix = 0; (ix < _width) && (ix < g_FrameBufferSizeD2D.X); ix++) {
			dp[ix] = sp[ix];
		}
		sp += _width;
		dp += g_FrameBufferSizeD2D.X;
	}
	if (g_pRenderTarget == NULL) {
		return;
	}
	push_screen_buffer();
}	//PrintFrameBuffer

//================================================================
/*
* @brief	フォント名（フォントの書体）を設定する
*
* @param	const wchar_t* _name	フォント名（書体）の設定
*/
void SetFontName(const wchar_t* _name)
{
	memcpy_s(g_FontFaceName, LF_FACESIZE, _name, LF_FACESIZE);
}

/*
* @brief	文字描画の高画質モード設定
*			※文字表示全体に影響する（フレーム単位）
*
* @param	bool _f	高画質：{true=ON|false=OFF}
*/
void SetTextHD(bool _f)
{
	g_TextHDMode = _f;
}

/*
* @brief	全角／半角フラグの設定
*
* @param	bool _f	全角文字列処理のＯＮ／ＯＦＦ：true=全角｜false=半角
*/
void SetZenkaku(bool _f)
{
	g_ZenkakuFlag = _f;
}
/*
* @brief	文字列描画のアンチエイリアスモードの設定
*
* @param	D2D1_TEXT_ANTIALIAS_MODE _mod	アンチエイリアスモード
*/
void SetTextAntialiasD2D(D2D1_TEXT_ANTIALIAS_MODE _mod)
{
	g_TextAntiAlias = _mod;
}	//SetTextAntialiasD2D

/*
* @brief	文字列描画のアンチエイリアスモードの取得
*
* @return	D2D1_TEXT_ANTIALIAS_MODE	アンチエイリアスモード
*/
D2D1_TEXT_ANTIALIAS_MODE GetTextAntialiasD2D(void)
{
	return g_TextAntiAlias;
}	//SetTextAntialiasD2D

/*
* @brief	フォントサイズの設定
*
* @param	int _width	フォントの幅
* @param	int _height	フォントの高さ
*/
void SetFontSize(FLOAT _width, FLOAT _height)
{
	if (_width > 0) {
		g_FontWidth = _width;
	}
	if (_height > 0) {
		g_FontHeight = _height;
	}
}	//SetFontSize
/*
* @brief	フォントサイズの取得
*
* @param	int* _p_width	フォントの幅
* @param	int* _p_height	フォントの高さ
*/
void GetFontSize(FLOAT* _p_width, FLOAT* _p_height)
{
	(*_p_width) = g_FontWidth;
	(*_p_height) = g_FontHeight;
}	//GetFontSize
/*
* @brief	書式指定文字列リスト
*
* @param	int _x,_y	表示座標（ドット単位）
* @param	const wchar_t* _format	書式指定文字列（マルチバイト）
* @param	va_list _ap	引数リスト
*
*/
static void _wtx_format_va_W(int _x, int _y, const wchar_t* _format, va_list _ap)
{
	size_t length = _vscwprintf(_format, _ap) + 1;	//'\0'含まないので＋１している
	wchar_t* buf = (wchar_t*)_malloca(length * sizeof(wchar_t));
	_ASSERT(buf);
	vswprintf_s(buf, length, _format, _ap);
	WriteTextW(_x, _y, buf, 0, D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF(0, 0, 0, 0), false);
}	//_wtx_format_va_W
/*
* @brief	書式指定文字列リスト
*
* @param	int _x,_y	表示座標（ドット単位）
* @param	const wchar_t* _format	書式指定文字列（マルチバイト）
* @param	...	可変長引数リスト
*
*/
void WriteTextFW(int _x, int _y, double _font_size_h, const wchar_t* _format, ...)
{
	//_font_size_hが0以下だったら外部でSetFontSize()されたサイズになる。
	if (_font_size_h > 0) {
		SetFontSize((FLOAT)_font_size_h, (FLOAT)_font_size_h);
	}
	va_list ap;
	va_start(ap, _format);
	_wtx_format_va_W(_x, _y, _format, ap);
	va_end(ap);
}	//WriteTextFW

/*
* @brief	書式指定文字列リスト
*
* @param	int _x,_y	表示座標（ドット単位）
* @param	const wchar_t* _format	書式指定文字列（Unicode）
* @param	va_list _ap	引数リスト
*
*/
void _wtx_format_va_A(int _x, int _y, const char* _format, va_list _ap)
{
	size_t length = _vscprintf(_format, _ap) + 1;	//'\0'含まないので＋１している
	char* buf = (char*)_malloca(length * sizeof(char));
	_ASSERT(buf);
	vsprintf_s(buf, length, _format, _ap);
	WriteTextA(_x, _y, buf, 0, D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF(0, 0, 0, 0), false);
}	//_wtx_format_va_A
/*
* @brief	書式指定文字列リスト
*
* @param	int _x,_y	表示座標（ドット単位）
* @param	const wchar_t* _format	書式指定文字列（Unicode）
* @param	...	可変長引数リスト
*
*/
void WriteTextFA(int _x, int _y, double _font_size_h, const char* _format, ...)
{
	//_font_size_hが0以下だったら外部でSetFontSize()されたサイズになる。
	if (_font_size_h > 0) {
		SetFontSize((FLOAT)_font_size_h, (FLOAT)_font_size_h);
	}
	va_list ap;
	va_start(ap, _format);
	_wtx_format_va_A(_x, _y, _format, ap);
	va_end(ap);
}	//WriteTextFA

//================================================================
//	文字列をID2D1Bitmapに描画する（フォントサイズ指定在り）
void WriteTextA(int _xp, int _yp, const char* _text, double _font_size_h, D2D1::ColorF _fgc, D2D1::ColorF _bgc, bool _new_line)
{
	//_font_size_hが0以下だったら外部でSetFontSize()されたサイズになる。
	if (_font_size_h > 0) {
		SetFontSize((FLOAT)_font_size_h, (FLOAT)_font_size_h);
	}
	//---- マルチバイト文字列をUnicode文字列に変換する
	//先ずUnicode文字列に必要なバッファサイズを求める
	int wc_count = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _text, -1, NULL, 0);	//'\0'を含む文字数が返る
	size_t wc_src_bytes = (wc_count * sizeof(wchar_t));	//Unicode文字列に必要なバイト数
	wchar_t* src_txt = (wchar_t*)_malloca(wc_src_bytes);	//スタック上に確保（free不要）
	_ASSERT(src_txt);
	memset(src_txt, 0, wc_src_bytes);	//０クリア
	//バッファを確保したので、そのバッファに変換して格納する。
	int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _text, -1, src_txt, wc_count);
	//変換したUnicode文字列を描画する
	WriteTextW(_xp, _yp, src_txt, 0, _fgc, _bgc, _new_line);
}	//WriteTextA
//================================================================
//	文字列をID2D1Bitmapに描画する（フォントサイズ指定在り）
void WriteTextW(int _xp, int _yp, const wchar_t* _text, double _font_size_h, D2D1::ColorF _fgc, D2D1::ColorF _bgc, bool _new_line)
{
	//_font_size_hが0以下だったら外部でSetFontSize()されたサイズになる。
	if (_font_size_h > 0) {
		SetFontSize((FLOAT)_font_size_h, (FLOAT)_font_size_h);
	}
	wchar_t* p_src_text = nullptr;
	if (g_ZenkakuFlag) {
		//全角フラグがＯＮなら全ての文字を全角に変換する
		p_src_text = HanToZenW(_text);
		g_ZenkakuFlag = false;	//フラグは１回だけ有効
	}
	//Windows関数の戻り値
	HRESULT hr;
	//現状でフォントサイズが有効なのは高さだけで、幅の値は影響しない。
	//TextFormat/TextLayoutのStretchが効かない。⇒調査中
	FLOAT font_size = g_FontHeight;// * g_Scale.height;
	if (g_TextHDMode) {
		font_size *= g_Scale.height;
	}

	//テキストフォーマットの生成
	//CreateTextFormat( L"フォント名", コレクション, 太さ, スタイル, 拡縮, サイズ, ローカルネーム？, 受け取るポインタ );
	IDWriteTextFormat* pTextFormat = NULL;
	//g_pDWFactory->CreateTextFormat(g_FontFaceName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, font_size, L"", &pTextFormat);
	if (g_FontFaceName[0] != 0) {
		//フォント名の指定がある場合
		g_pDWFactory->CreateTextFormat(g_FontFaceName, NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			font_size, L"", &pTextFormat);
	}
	else {
		//g_pDWFactory->CreateTextFormat(L"ＭＳ 明朝", NULL,
		g_pDWFactory->CreateTextFormat(L"ＭＳ ゴシック", NULL,
			//g_pDWFactory->CreateTextFormat(L"メイリオ", NULL,
			//g_pDWFactory->CreateTextFormat(L"Migu 1M", NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,//DWRITE_FONT_STYLE_OBLIQUE,
			DWRITE_FONT_STRETCH_NORMAL,
			font_size, L"", &pTextFormat);
	}
	//画面端で改行させるかどうかの設定
	if (_new_line) {
		//レイアウト ボックスからテキストがオーバーフローしないように、単語が複数行に分割されることを示します。
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
	}
	else {
		//レイアウト ボックスから行がオーバーフローしても、同じ行内に単語を保持することを示します。
		//このオプションでは、多くの場合、オーバーフロー テキストを表示するためにスクロールを併用します。
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}

	// ブラシの作成
	ID2D1SolidColorBrush* pBrushFG = NULL;
	g_pRenderTarget->CreateSolidColorBrush(_fgc, &pBrushFG);	//フォントの色
	ID2D1SolidColorBrush* pBrushBG = NULL;
	g_pRenderTarget->CreateSolidColorBrush(_bgc, &pBrushBG);	//背景の色

	// IDWriteTextLayout 取得
	IDWriteTextLayout* pTextLayout = NULL;
	D2D1_RECT_F trf;
	{
		//	CreateTextLayout(文字列, 文字列の長さ, DWriteTextFormat*, 枠の幅, 枠の高さ, IDWriteTextLayout** );
		//	※枠のサイズはレンダーターゲット（実質画面サイズ）と同じにしている。
		D2D1_SIZE_F szf = g_pRenderTarget->GetSize();	//レンダーターゲットのサイズ取得
		if (p_src_text != nullptr) {
			hr = g_pDWFactory->CreateTextLayout(p_src_text, (UINT32)wcslen(p_src_text), pTextFormat, szf.width, szf.height, &pTextLayout);
		}
		else {
			hr = g_pDWFactory->CreateTextLayout(_text, (UINT32)wcslen(_text), pTextFormat, szf.width, szf.height, &pTextLayout);
		}
		_ASSERT(hr == S_OK);

		//pTextLayout->SetFontStretch(
		//DWRITE_FONT_STRETCH_ULTRA_EXPANDED,
		//	DWRITE_TEXT_RANGE{0,128}
		//);
		//pTextLayout->SetFontSize();

		DWRITE_TEXT_METRICS mtx;	//テキストを囲む矩形の計測値
		hr = pTextLayout->GetMetrics(&mtx);	//テキストを囲む矩形の計測値を取得
		_ASSERT(hr == S_OK);
		//テキストを表示する座標（囲む枠）を作る
		trf.left = (mtx.left + _xp);// * g_Scale.width;	//左端（この関数の引数_xp）
		trf.top = (mtx.top + _yp);// * g_Scale.height;		//上端（この関数の引数_xp）
		if (g_TextHDMode) {
			trf.left *= g_Scale.width;
			trf.top *= g_Scale.height;
		}
		trf.right = trf.left + mtx.width;				//右端（枠幅）
		trf.bottom = trf.top + mtx.height;				//下端（枠高さ）
	}
	//★ターゲットのサイズはウィンドウのサイズなのでスケーリングが入っていないので注意。
	g_ptrBRTforText->SetTextAntialiasMode(g_TextAntiAlias);	//アンチエイリアスモード設定
	g_ptrBRTforText->BeginDraw();
	g_ptrBRTforText->FillRectangle(&trf, pBrushBG);	// 文字列背景の四角形の描画
	g_ptrBRTforText->DrawTextLayout(D2D1_POINT_2F{ trf.left,trf.top }, pTextLayout, pBrushFG);	//文字列
	hr = g_ptrBRTforText->EndDraw();
	_ASSERT(hr == S_OK);

	//テキストレイアウトの破棄
	if (pTextLayout != NULL) {
		pTextLayout->Release();
	}
	// テキストフォーマットの破棄
	if (pTextFormat != NULL) {
		pTextFormat->Release();
	}
	// ブラシの破棄
	if (pBrushBG != NULL) {
		pBrushBG->Release();
	}
	if (pBrushFG != NULL) {
		pBrushFG->Release();
	}

	if (p_src_text != nullptr) {
		free(p_src_text);
	}
	return;
}	//WriteTextW

//================================================================
// ダブルバッファ関連※ダブルバッファは無くなった
//================================================================
/*
* @brief	描画オブジェクトを実画面へ描画
*/
void RenderScreen(void)
{
	//Windows関数の戻り値
	HRESULT hr;
	/*
	* レンダーターゲットへの描画
	* ビットマップの準備が出来たらレンダーターゲットに渡して描画を行う。
	* デフォルトの補間モードは D2D1_BITMAP_INTERPOLATION_MODE_LINEAR になっているので、
	* 何も指定しないと拡大縮小時にぼやけてしまう。
	*/
	//left,top,right,bottom --- x,y,x,y
	const FLOAT	scale_x = g_Scale.width;	//描画ヨコ倍率
	const FLOAT	scale_y = g_Scale.height;	//描画タテ倍率
	const FLOAT	opacity = 1.0f;	//透明度：不透明(1.0f)～(0.0f)透明
	//描画する矩形を作成
	//D2D1_RECT_F rf = { 0,0,((FLOAT)g_FrameBufferSizeD2D.X - 1) * scale_x,((FLOAT)g_FrameBufferSizeD2D.Y - 1) * scale_y };
	D2D1_RECT_F rf = { 0,0,(FLOAT)g_FrameBufferSizeD2D.X * scale_x,(FLOAT)g_FrameBufferSizeD2D.Y * scale_y };
	//D2D1_RECT_F drf = { 0,0,(FLOAT)g_FrameBufferSizeD2D.X - 1,(FLOAT)g_FrameBufferSizeD2D.Y - 1 };
	//D2D1_RECT_F srf = { 0,0,(FLOAT)g_FrameBufferSizeD2D.X - 1,(FLOAT)g_FrameBufferSizeD2D.Y - 1 };

	//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
	g_pRenderTarget->BeginDraw();	//▼▼▼描画開始▼▼▼
	g_pRenderTarget->Clear();		//画面消去

	//DrawBitmap(bitmap,描画される領域のサイズと位置,不透明度,補間モード)
	for (ID2D1Bitmap* pbmp : g_pBmpList) {
		//リストに溜まっているビットマップオブジェクトを全て表示する。
		//ドット拡大してもぼやけない様に"_NEAREST_NEIGHBOR"を指定している。
		g_pRenderTarget->DrawBitmap(pbmp, &rf, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);	//補完無し
		pbmp->Release();
	}
	g_pBmpList.clear();	//リストを消去

	//文字の表示サイズ（倍率）を決める
	FLOAT bairitu = (g_FontWidth / g_FontHeight);
	if (g_PrintStringCompatibleMode) {
		//文字の表示比率を１：２にする⇒互換モード：文字のピクセルは１ｘ２＝＞１ｘ１のドットにするとヨコに拡大される
		bairitu = (scale_x * 2) / scale_y;
	}
	if (g_TextHDMode) {
		rf = { 0,0,(FLOAT)g_FrameBufferSizeD2D.X * scale_x * bairitu,(FLOAT)g_FrameBufferSizeD2D.Y * scale_y };
	}
	else {
		FLOAT txt_buf_width = (FLOAT)g_ptrBRTforText->GetPixelSize().width;
		FLOAT txt_buf_height = (FLOAT)g_ptrBRTforText->GetPixelSize().height;
		rf = { 0,0,txt_buf_width * scale_x * bairitu,txt_buf_height * scale_y };
	}

	//■■■■ 描画したビットマップをリストに追加 ■■■
	ID2D1Bitmap* pD2D1_Bmp;
	g_ptrBRTforText->GetBitmap(&pD2D1_Bmp);	//描画したビットマップを取得
	//ドット拡大してもぼやけない様に"_NEAREST_NEIGHBOR"を指定している。
	g_pRenderTarget->DrawBitmap(pD2D1_Bmp, &rf, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);	//補完無し
	pD2D1_Bmp->Release();

	hr = g_pRenderTarget->EndDraw();	//▲▲▲描画終了▲▲▲
	//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
	_ASSERT(hr == S_OK);
	g_PrintStringCompatibleMode = false;
	return;
}	//RenderScreen

/*
* @brief	フォントサイズ変更
*
* @param	width	フォントの横サイズ(1～)
* @param	height	フォントの縦サイズ(1～)
*/
void SetScreenFontSize(int _width, int _height)
{
	SetFontSize((FLOAT)_width, (FLOAT)_height);
}	//SetScreenFontSize
/*
* @brief	フォント情報の設定【工事中】
*
* @param	const CONSOLE_FONT_INFOEX* _sfix	フォント情報構造体へのポインタ
*/
void SetFontInfoEx(const CONSOLE_FONT_INFOEX* _p_sfix)
{
	SetFontSize((FLOAT)_p_sfix->dwFontSize.X, (FLOAT)_p_sfix->dwFontSize.Y);
	if (_p_sfix->FaceName[0] != 0) {
		memcpy_s(g_FontFaceName, LF_FACESIZE * sizeof(WCHAR), _p_sfix->FaceName, LF_FACESIZE * sizeof(WCHAR));
	}
}	//SetScreenFontSize

//################################################################################
//################################################################################
// XAudio2を使ったサウンド処理
//################################################################################
//################################################################################
//=== XAudio2処理の基礎となるオブジェクト ===
static ComPtr<IXAudio2> g_cpSnd;	//XAudio2 インターフェイス
static IXAudio2MasteringVoice* g_pMasterVoice = nullptr;	//マスターボイス

/*
* @brief	音量の設定
*
* @param	float _vloume	音量
*			※同時に、引数Snd*のvolumeに音量が書き込まれる
*/
void SetVolumeSnd(Snd* _xa2, float _volume)
{
	_xa2->volume = _volume;
	_xa2->psv->SetVolume(_xa2->volume, XAUDIO2_COMMIT_NOW);
}
/*
* @brief	音量の設定
*			※引数Snd*のvolumeに音量を設定して呼び出す
*/
void SetVolumeSnd(const Snd* _xa2)
{
	_xa2->psv->SetVolume(_xa2->volume, XAUDIO2_COMMIT_NOW);
}
/*
* @brief	音量の取得
*
* @return	float	現在の音量
*/
float GetVolumeSnd(Snd* _xa2)
{
	_xa2->psv->GetVolume(&_xa2->volume);
	return _xa2->volume;
}

/*
* @brief	再生状態の確認
*
* @param	Snd* _xa2	Sndオブジェクトへのポインタ
*
* @return	bool	true:再生中／false:停止中
*/
bool CheckSnd(const Snd* _xa2)
{
	if (_xa2 == nullptr)return false;
	XAUDIO2_VOICE_STATE xvst;
	//xvst.pCurrentBufferContext = 0;//&buffer;
	//xvst.BuffersQueued = 0;	//音声で現在キューに登録されているオーディオ バッファーの数 (現在処理されているバッファーを含む)。
	//xvst.SamplesPlayed = 0;	//この音声が最後に開始されてから、この音声によって処理されたサンプルの合計数。
	_xa2->psv->GetState(&xvst, XAUDIO2_VOICE_NOSAMPLESPLAYED);
	return (xvst.BuffersQueued != 0);
}	//CheckXA2

/*
* @brief	サウンドの停止
*
* @param	Snd* _xa2	Sndオブジェクトへのポインタ
*
* @return	bool	true:成功／false:失敗
*/
bool StopSnd(Snd* _xa2)
{
	if (_xa2 == nullptr)return false;
	HRESULT hr = S_OK;
	if (CheckSnd(_xa2) != 0) {
		hr = _xa2->psv->Stop(0, XAUDIO2_COMMIT_NOW);
		if (FAILED(hr))return false;
		hr = _xa2->psv->FlushSourceBuffers();
		if (FAILED(hr))return false;
		do {
			;//_xa2->ptr_src_voice->GetState(&xvst);
		} while (CheckSnd(_xa2) != 0);
	}
	return true;
}	//StopSnd

/*
* @brief	サウンドの再生
*
* @param	Snd* _xa2	Sndオブジェクトへのポインタ
* @param	bool _loop	ループ再生フラグ⇒true:ループ再生／false:単発再生
*
* @return	bool	true:成功／false:失敗
*/
bool PlaySnd(Snd* _xa2, bool _loop)
{
	if (_xa2 == nullptr)return false;
	//一旦止める
	if (!StopSnd(_xa2)) { return false; }
	//ループ再生を選択して再生バッファの状態を変更
	if (_loop) {
		//無限ループ
		_xa2->xbuf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else {
		//１回だけ再生
		_xa2->xbuf.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}
	HRESULT hr;
	//バッファを登録
	hr = _xa2->psv->SubmitSourceBuffer(&_xa2->xbuf);
	if (FAILED(hr))return false;
	//再生開始
	hr = _xa2->psv->Start(0, XAUDIO2_COMMIT_NOW);
	if (FAILED(hr))return false;
	return true;
}	//PlayXA2

/*
* Snd：XAudio2用構造体：の削除（メモリの解放など）
*/
void DeleteSnd(Snd** _xa2)
{
	if (_xa2 == nullptr)return;
	if (*_xa2 == nullptr)return;
	//delete[](*_xa2)->xbuf.pAudioData;	//free BYTE*
	free(const_cast<BYTE*>((*_xa2)->xbuf.pAudioData));	//free BYTE*
	(*_xa2)->xbuf.pAudioData = nullptr;	//clear
	(*_xa2)->psv->DestroyVoice();	//source-voice delete.
	(*_xa2)->psv = nullptr;			//clear
	//delete (*_xa2);	//Snd構造体の削除
	free(*_xa2);	//Snd構造体の削除
	(*_xa2) = nullptr;
	//終わり
}

/*
* XAudio2の初期化
*/
HRESULT InitSnd(void)
{
	HRESULT hr;
	/*
	* Com初期化
	* 呼び出し元スレッドが使用する COM ライブラリを初期化し、
	* スレッドのコンカレンシー モデルを設定し、
	* 必要に応じてスレッドの新しいアパートメントを作成します。
	*/
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr))return hr;
	/*
	* XAudio2生成・初期化
	* 新しい XAudio2 オブジェクトを作成し、
	* その IXAudio2 インターフェイスへのポインターを返します。
	*/
	hr = XAudio2Create(&g_cpSnd);
	if (FAILED(hr))return hr;
	/*
	* マスターボイス生成
	* マスタリング音声を作成して構成します。
	*/
	hr = g_cpSnd->CreateMasteringVoice(&g_pMasterVoice);
	if (FAILED(hr))return hr;
	//
	return S_OK;
}	//InitSnd

/*
* XAudio2終了処理
*/
void EndSnd(void)
{
	/*
	* マスターボイス削除
	* ボイスを破棄します。
	* 必要に応じて、音声を停止し、XAudio2 グラフから削除します。
	*/
	if (g_pMasterVoice != nullptr) {
		g_pMasterVoice->DestroyVoice();	//破棄
		g_pMasterVoice = nullptr;
	}
	/*
	* Com終了
	* 現在のスレッドの COM ライブラリを閉じ、
	* スレッドによって読み込まれるすべての DLL をアンロードし、
	* スレッドが保持するその他のリソースを解放し、
	* スレッド上のすべての RPC 接続を強制的に閉じます。
	*/
	CoUninitialize();
}	//EndSnd

/*
* @brief	音源ファイルの読込～Sndオブジェクト作成
*
* @param	const wchar_t* _filename	音源ファイル名（Unicode）
*
* @return	Sndオブジェクトへのポインタ
*			※呼び出した側で破棄する事。
*/
Snd* LoadSnd(const wchar_t* _filename)
{
	/*
	* ここまでにXAudio2の初期化が必要
	*/
	if (g_cpSnd == nullptr)return nullptr;

	//Sndオブジェクトのメモリ確保
	Snd* xa2 = (Snd*)calloc(1, sizeof(Snd));
	//memset(xa2, 0, sizeof(Snd));

	//
	HRESULT hr = S_OK;
	//CoInitializeEx
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr))return nullptr;
	hr = MFStartup(MF_VERSION);
	if (hr != S_OK) { return nullptr; }
	//
	ComPtr<IMFSourceReader> p_src_reader;

	//通常、メディア ファイルの場合は MFCreateSourceReaderFromURL を使用します。
	hr = MFCreateSourceReaderFromURL(_filename, NULL, &p_src_reader);
	if (hr != S_OK) { return nullptr; }

	//ソースリーダーにmp3からPCMへ変換する為のデコーダを挿入する。
	/*
	* 1.MFCreateMediaType を呼び出して、新しいメディアの種類を作成します。
	* 2.オーディオまたはビデオを指定するには 、MF_MT_MAJOR_TYPE 属性を設定します。
	* 3.デコード形式のサブタイプを指定するには、 MF_MT_SUBTYPE 属性を設定します。
	*	( オーディオ サブタイプ GUID と ビデオ サブタイプ GUID を参照)。
	* 4.IMFSourceReader::SetCurrentMediaType を呼び出します。
	*/
	ComPtr<IMFMediaType> pt = NULL;
	// メディアタイプ作成：
	hr = MFCreateMediaType(&pt);
	if (hr != S_OK) { return nullptr; }
	// メジャータイプ設定：
	hr = pt->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	if (hr != S_OK) { return nullptr; }
	// サブタイプ決定：
	hr = pt->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	if (hr != S_OK) { return nullptr; }
	// メディアタイプを設定：ソース リーダーによってデコーダーが自動的に読み込まれる。
	hr = p_src_reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pt.Get());
	if (hr != S_OK) { return nullptr; }
	// メディアタイプを取得：設定されたデコード形式の完全な詳細を取得する。
	ComPtr<IMFMediaType> p_type = NULL;
	p_src_reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &p_type);
	if (hr != S_OK) { return nullptr; }

	// メディアタイプからWAVEFORMATEX を取得
	WAVEFORMATEX* p_media_wave_format;
	unsigned int un_wave_format_length;
	hr = MFCreateWaveFormatExFromMFMediaType(p_type.Get(), &p_media_wave_format, &un_wave_format_length);
	if (hr != S_OK) { return nullptr; }
	// Snd構造体のメンバにコピーを取る
	memcpy(&xa2->wfmt, p_media_wave_format, sizeof(WAVEFORMATEX));
	CoTaskMemFree(p_media_wave_format);	//コピーしたので元のメモリは解放
	if (hr != S_OK) { return nullptr; }

	int total_size = 0;
	//BYTE* ptr_dest = nullptr;
	//size_t size_dest = 0;
	BYTE* last_ptr = nullptr;
	size_t last_size = 0;
	//変換データをサンプル単位で最後まで取得するループ
	for (;;) {
		/*
		* １サンプル分取得する
		*/
		//ComPtr<IMFSample> cp_sample;
		IMFSample* p_sample = nullptr;
		DWORD res_flg = 0;	//結果状態フラグ
		hr = p_src_reader->ReadSample(
			MF_SOURCE_READER_FIRST_AUDIO_STREAM,	//[in]
			NULL,		// [in]Flags.現在の保留中サンプルだけ取得するときはMF_SOURCE_READER_CONTROLF_DRAIN、全て取得するときはNULL。
			NULL,		// [out]実際にデータを生成するストリームのインデックスを受け取る。最初のパラメーターを MF_SOURCE_READER_ANY_STREAM に設定した場合に必要。
			&res_flg,	// [out]MF_SOURCE_READER_FLAG：ストリームの形式変更など、データの読み取り中に発生する可能性のあるさまざまなイベントを示す状態フラグを受け取り
			NULL,		// [out]サンプルのタイム スタンプ、または pdwStreamFlags で示されるストリーム イベントの時刻を受け取ります。 時間は 100 ナノ秒単位で指定されます。
			&p_sample	// [out]取得したサンプルデータ、終端ならNULL。
		);
		if (res_flg & MF_SOURCE_READERF_ENDOFSTREAM) {
			break;
		}
		if (p_sample == NULL) {
			//ストリームの終わりに達した。
			break;
		}
		if (hr != S_OK) { break; }
		//ComPtr<IMFMediaBuffer> p_mb;
		IMFMediaBuffer* p_mb = nullptr;
		hr = p_sample->ConvertToContiguousBuffer(&p_mb);
		BYTE* cur_ptr = nullptr;	//[out]１回分のバッファポインタを受け取る
		DWORD cur_size = 0;	//[out]１回分のバッファの長さを受け取る
		hr = p_mb->Lock(&cur_ptr, nullptr, &cur_size);
		{
			total_size = (int)cur_size + (int)last_size;
			//BYTE* new_ptr = new BYTE[total_size];	//バッファサイズ拡張
			BYTE* new_ptr = (BYTE*)calloc(sizeof(BYTE), total_size);	//バッファサイズ拡張
			if (last_ptr != nullptr) {
				//前回のバッファを今回のバッファに移動する
				memcpy_s(new_ptr, total_size, last_ptr, last_size);
				//delete[] last_ptr;
				free(last_ptr);
			}
			memcpy_s(&new_ptr[last_size], cur_size, cur_ptr, cur_size);
			//
			last_ptr = new_ptr;
			last_size = total_size;	//サイズ積算
			//memcpy_s(p_dst, src_size, p_src, src_size);
			//p_dst += src_size;
		}
		hr = p_mb->Unlock();
		p_mb->Release();
		p_sample->Release();
	}
	if (hr != S_OK) { return nullptr; }	//エラーでループ抜けてきたらエラー終了

	hr = MFShutdown();	//MediaFoundation終了（MFStartupを1回呼び出す度に１回呼び出す）
	if (FAILED(hr))return nullptr;
	CoUninitialize();

	/*
	* メディアソースの１秒あたりのバイト数と全体の秒数からバッファサイズをを求め、
	* そのサイズのバッファを確保する。
	*/
	// 全体サイズのバッファ（実際のデータより大き目）を確保する
	xa2->xbuf.AudioBytes = total_size;
	//xa2->xbuf.pAudioData = new BYTE[xa2->xbuf.AudioBytes];
	xa2->xbuf.pAudioData = (BYTE*)calloc(sizeof(BYTE), xa2->xbuf.AudioBytes);
	memcpy_s(const_cast<BYTE*>(xa2->xbuf.pAudioData), xa2->xbuf.AudioBytes, last_ptr, last_size);
	//delete[] last_ptr;
	free(last_ptr);
	//BYTE* p_dst = const_cast<BYTE*>(xa2->xbuf.pAudioData);

	/*
	* XAudio2:メディアデータ情報のXAUDIO2_BUFFERを作成
	* xbuf.AudioBytes、xbuf.pAudioData、xbuf.Flags、xbuf.LoopCount
	* 以外は全て０。※ゼロクリアしたままの状態
	*/
	xa2->xbuf.Flags = XAUDIO2_END_OF_STREAM;	//ストリーム内の最後のバッファー
	xa2->xbuf.LoopCount = XAUDIO2_NO_LOOP_REGION;
	/*
	* XAudio2:ソースボイス(IXAudio2SourceVoice*)へのポインタの作成⇒Sndオブジェクトのpsvメンバに保存。
	*/
	hr = g_cpSnd->CreateSourceVoice(&xa2->psv, (WAVEFORMATEX*)&xa2->wfmt);
	if (FAILED(hr))return nullptr;

	xa2->volume = 1.0;
	SetVolumeSnd(xa2);

	return xa2;
}	//LoadSnd




//################################################################################################################################
//################################################################################################################################
//################################################################################################################################

/**
* @copyright (c) 2018-2019 HAL Osaka College of Technology & Design (Ihara, H.)
*/
