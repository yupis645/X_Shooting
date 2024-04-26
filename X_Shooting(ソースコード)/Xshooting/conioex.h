#pragma once
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
*			LoadBmpExA/WとLoadBmp()を廃止して、
*			LoadBmpA/W()に変更。
*			LoadBmpA/W()は、LoadBmpFromImgFileA/Wを
*			inlineでラップしている。
*
* @note
*  コンソールウィンドウのプロパティの設定を以下に変更すること
*  ・「従来のコンソールを使う」のチェックを外す
*  ・「簡易編集モード」のチェックを外す
*
* コンソール関数について
* https://learn.microsoft.com/ja-jp/windows/console/console-functions
*/

#include	<locale.h>
#include	<stdio.h>
#include	<conio.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	<mbstring.h>
#include	<process.h>
#include	<windows.h>
#include	<tchar.h>
#include	<mmsystem.h>
#include	<digitalv.h>
#include	<xinput.h>	//XInput
#include	<CRTDBG.H>	//OutputDebugString/_RPT*
//#define _USE_MATH_DEFINES
#include	<cmath>
//#include <math.h>
#define	M_PI	3.1415926535897940041763831686694175004959106445312500
#include	<d3d11_1.h>
#include	<d2d1_3.h>	//
#include	<dwrite.h>	//
#include	<wchar.h>	//
#include	<directxcolors.h>
#include	<wincodec.h>
#include	<wincodecsdk.h>
#include	<wrl/client.h>
//#include	<wincodec_proxy.h>
//for XAudio2.
#include	<xaudio2.h>
//for MediaFoundation.
#include	<mfapi.h>
#include	<mfidl.h>
#include	<mfreadwrite.h>
//=== Library ===
#pragma comment( lib, "d3d11.lib")
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "windowscodecs.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xinput.lib")
//for XAudio2.
#pragma comment(lib,"xaudio2.lib")	//XAudio2
//for MediaFoundation.
#pragma comment(lib, "Mf.lib")	//MediaFoundation
#pragma comment(lib, "mfplat.lib")	//MediaFoundation
#pragma comment(lib, "Mfreadwrite.lib")	//MediaFoundation
#pragma comment(lib, "mfuuid.lib")	//MediaFoundation
//
#pragma warning(disable:4996)

#define NOCURSOR		(0)
#define SOLIDCURSOR		(1)
#define NORMALCURSOR	(2)

#define DEF_FONTSIZE_X		(16)
#define DEF_FONTSIZE_Y		(16)

#define DEF_DOTSCALE_X		((double)1.0)
#define DEF_DOTSCALE_Y		((double)1.0)

// palette
#define	NUM_D2D_PAL (256)

// https://en.wikipedia.org/wiki/Color_Graphics_Adapter
enum COLORS {
	BLACK,			/* #000000	黒				*/
	BLUE,			/* #0000AA	青				*/
	GREEN,			/* #00AA00	緑				*/
	CYAN,			/* #00AAAA	シアン			*/
	RED,			/* #AA0000	赤				*/
	MAGENTA,		/* #AA00AA	マゼンタ		*/
	BROWN,			/* #AA5500	茶				*/
	LIGHTGRAY,		/* #AAAAAA	明るい灰色		*/
	DARKGRAY,		/* #555555	暗い灰色		*/
	LIGHTBLUE,		/* #5555FF	明るい青		*/
	LIGHTGREEN,		/* #55FF55	明るい緑		*/
	LIGHTCYAN,		/* #55FFFF	明るいシアン	*/
	LIGHTRED,		/* #FF5555	明るい赤		*/
	LIGHTMAGENTA,	/* #FF55FF	明るいマゼンタ	*/
	YELLOW,			/* #FFFF55	黄				*/
	WHITE			/* #FFFFFF	白				*/
};

/* キーコード */
#define PK_ENTER				VK_RETURN
#define PK_ESC					VK_ESCAPE
#define PK_F1					VK_F1
#define PK_F2					VK_F2
#define PK_F3					VK_F3
#define PK_F4					VK_F4
#define PK_F5					VK_F5
#define PK_F6					VK_F6
#define PK_F7					VK_F7
#define PK_F8					VK_F8
#define PK_F9					VK_F9
#define PK_F10					VK_F10
#define PK_F11					VK_F11
#define PK_F12					VK_F12
#define PK_SP					VK_SPACE
#define PK_UP					VK_UP
#define PK_DOWN					VK_DOWN
#define PK_RIGHT				VK_RIGHT
#define PK_LEFT					VK_LEFT
#define PK_SHIFT				VK_SHIFT
#define PK_CTRL					VK_CONTROL
#define PK_ALT					VK_MENU
#define PK_BS					VK_BACK
#define PK_PAUSE				VK_PAUSE
#define PK_INS					VK_INSERT
#define PK_DEL					VK_DELETE
#define PK_TAB					VK_TAB
#define PK_NFER					VK_KANA		/* [無変換]	*/
#define PK_XFER					VK_CONVERT	/* [変換]	*/
#define PK_0					0x30
#define PK_1					0x31
#define PK_2					0x32
#define PK_3					0x33
#define PK_4					0x34
#define PK_5					0x35
#define PK_6					0x36
#define PK_7					0x37
#define PK_8					0x38
#define PK_9					0x39
#define PK_NUMPAD0				VK_NUMPAD0	/* テンキーの[0]	*/
#define PK_NUMPAD1				VK_NUMPAD1	/* テンキーの[1]	*/
#define PK_NUMPAD2				VK_NUMPAD2	/* テンキーの[2]	*/
#define PK_NUMPAD3				VK_NUMPAD3	/* テンキーの[3]	*/
#define PK_NUMPAD4				VK_NUMPAD4	/* テンキーの[4]	*/
#define PK_NUMPAD5				VK_NUMPAD5	/* テンキーの[5]	*/
#define PK_NUMPAD6				VK_NUMPAD6	/* テンキーの[6]	*/
#define PK_NUMPAD7				VK_NUMPAD7	/* テンキーの[7]	*/
#define PK_NUMPAD8				VK_NUMPAD8	/* テンキーの[8]	*/
#define PK_NUMPAD9				VK_NUMPAD9	/* テンキーの[9]	*/
#define PK_A					0x41
#define PK_B					0x42
#define PK_C					0x43
#define PK_D					0x44
#define PK_E					0x45
#define PK_F					0x46
#define PK_G					0x47
#define PK_H					0x48
#define PK_I					0x49
#define PK_J					0x4A
#define PK_K					0x4B
#define PK_L					0x4C
#define PK_M					0x4D
#define PK_N					0x4E
#define PK_O					0x4F
#define PK_P					0x50
#define PK_Q					0x51
#define PK_R					0x52
#define PK_S					0x53
#define PK_T					0x54
#define PK_U					0x55
#define PK_V					0x56
#define PK_W					0x57
#define PK_X					0x58
#define PK_Y					0x59
#define PK_Z					0x5A

#define PK_LT					0x0BC		/* [,]	*/
#define PK_GT					0x0BE		/* [.]	*/
#define PK_SLUSH				0x0BF		/* [?]	*/
#define PK_DOT					VK_DECIMAL	/* テンキーの[.]	*/
#define PK_DIV					VK_DIVIDE	/* テンキーの[/]	*/
#define PK_BSLUSH				0x0E2		/* [_]	*/

#define PK_SEMICOLON			0x0BB		/* [;]	*/
#define PK_ADD					VK_ADD		/* テンキーの[+]	*/
#define PK_COLON				0x0BA		/* [:]	*/
#define PK_MUL					VK_MULTIPLY	/* テンキーの[*]	*/
#define PK_RBRACE				0x0DD		/* []]	*/

#define PK_ATMARK				0x0C0		/* [@]	*/
#define PK_LBRACE				0x0DB		/* [[]	*/

#define PK_MINUS				0x0BD		/* [-]	*/
#define PK_SUB					VK_SUBTRACT	/* テンキーの[-]	*/
#define PK_XOR					0x0DE		/* [^]	*/
#define PK_YEN					0x0DC		/* [\]	*/

#define PK_KANJI				0x0F3		/* [半角/全角]	*/
#define PK_CAPS					0x0F0		/* [英数][ひらがな]	*/

#define PM_LEFT					VK_LBUTTON
#define PM_MID					VK_MBUTTON
#define PM_RIGHT				VK_RBUTTON
#define PM_CURX					0x0101
#define PM_CURY					0x0102

#define PJ1_XPOS				0x0200
#define PJ1_YPOS				0x0201
#define PJ1_ZPOS				0x0202
#define PJ1_BTNS				0x0203
#define PJ2_XPOS				0x0210
#define PJ2_YPOS				0x0211
#define PJ2_ZPOS				0x0212
#define PJ2_BTNS				0x0213
#define PJ3_XPOS				0x0220
#define PJ3_YPOS				0x0221
#define PJ3_ZPOS				0x0222
#define PJ3_BTNS				0x0223
#define PJ4_XPOS				0x0230
#define PJ4_YPOS				0x0231
#define PJ4_ZPOS				0x0232
#define PJ4_BTNS				0x0233
#define PJ_XPOS					PJ1_XPOS
#define PJ_YPOS					PJ1_YPOS
#define PJ_ZPOS					PJ1_ZPOS
#define PJ_BTNS					PJ1_BTNS

#define PJX1_LXPOS				0x0200
#define PJX1_LYPOS				0x0201
#define PJX1_LTRG				0x0202
#define PJX1_RXPOS				0x0203
#define PJX1_RYPOS				0x0204
#define PJX1_RTRG				0x0205
#define PJX1_BTNS				0x0206
#define PJX2_LXPOS				0x0210
#define PJX2_LYPOS				0x0211
#define PJX2_LTRG				0x0212
#define PJX2_RXPOS				0x0213
#define PJX2_RYPOS				0x0214
#define PJX2_RTRG				0x0215
#define PJX2_BTNS				0x0216
#define PJX3_LXPOS				0x0220
#define PJX3_LYPOS				0x0221
#define PJX3_LTRG				0x0222
#define PJX3_RXPOS				0x0223
#define PJX3_RYPOS				0x0224
#define PJX3_RTRG				0x0225
#define PJX3_BTNS				0x0226
#define PJX4_LXPOS				0x0230
#define PJX4_LYPOS				0x0231
#define PJX4_LTRG				0x0232
#define PJX4_RXPOS				0x0233
#define PJX4_RYPOS				0x0234
#define PJX4_RTRG				0x0235
#define PJX4_BTNS				0x0236
#define PJX_LXPOS				PJX1_LXPOS
#define PJX_LYPOS				PJX1_LYPOS
#define PJX_LTRG				PJX1_LTRG
#define PJX_RXPOS				PJX1_RXPOS
#define PJX_RYPOS				PJX1_RYPOS
#define PJX_RTRG				PJX1_RTRG
#define PJX_BTNS				PJX1_BTNS

#define	NUM_KEYS		(256)	//キーバッファに読み取るキーの数
#define	NUM_PALETTE		(16)	//カラーパレット数（画面の色数：４ビットカラー）
#define	NUM_ANSI_PAL	(256)	//ANIS 256 colors.

extern const COLORREF ANSI_PAL256_COLOR[NUM_ANSI_PAL];
extern const RGBQUAD ANSI_PAL256_RGB[NUM_ANSI_PAL];

//inline UINT RGB(int _r,int _g,int _b){return 0; }
/*
* RGBQUAD{} => {+0:Blue,+1:Green,+2:Red,+3:Reserve} => 0x00RRGGBB
* COLORREF => {+0:R,+1:G,+2:B,+3:0} => 0x00BBGGRR
*/
inline RGBQUAD DWRGB(int _r, int _g, int _b) { return RGBQUAD{ (BYTE)_b,(BYTE)_g,(BYTE)_r }; }//((_r & 0xFF)<<8) | ((_g&0xFF)<<16) | ((_b&0xFF)<<24);}

//=============================
//▼▼▼▼ 各種図形データ構造体 ▼▼▼▼
#define	CONIOEX_DDA_SHAPE
#ifdef CONIOEX_DDA_SHAPE
/*
* 円の構造体
*/
struct CIRCLE {
	int cx, cy;	//中心点
	size_t r;		//半径
};
/*
* 線分の構造体
*/
struct LINE {
	int x1, y1;	//始点
	int x2, y2;	//終点
};
/*
* 三角形構造体
*/
struct TRIANGLE {
	double	x0, y0;
	double	x1, y1;
	double	x2, y2;
};
/*
* 頂点
*/
struct POINT_F {
	double x, y;
};
/*
* 線分
*/
struct LINE_F {
	double x1, y1;	//始点
	double x2, y2;	//終点
};
/*
* 右回り頂点の四角形
*（四頂点ポリゴン）
*/
struct POLY_4F {
	POINT_F p[4];
	POLY_4F() :p{} {}
	POLY_4F(RECT _r) {
		p[0].x = _r.left;	p[0].y = _r.top;
		p[1].x = _r.right;	p[1].y = _r.top;
		p[2].x = _r.left;	p[2].y = _r.bottom;
		p[3].x = _r.right;	p[3].y = _r.bottom;
	}
};
#endif	//CONIOEX_DDA_SHAPE
//▲▲▲▲ 各種図形データ構造体 ▲▲▲▲
//=============================

/*
* LoadBmpで読み込んだ画像データを格納する構造体。
*/
struct Bmp {
	int width;		//画像の横方向ピクセル数
	int height;		//画像の縦方向ピクセル数
	int colbit;		//各ピクセルの色ビット数
	int numpal;		//パレットの色数
	int numpix;		//画像データのバイト数
	bool swapRB;	//24ビット以上の画像で、RとBが入れ替えられている場合は'TRUE'になる
	//パレットデータ配列の型を{COLORREF|RGBQUAD}をLoadBmp()時に選択。デフォルトCOLORREF型（）
	COLORREF* pal;		//COLORREFパレット（２～６５色）データへのポインタ※無い場合はNULL
	RGBQUAD* pal_rgb;	//RGBQUADパレット（２～６５色）データへのポインタ※無い場合はNULL
	char* pixel;	//画像データへのポインタ
	//以下はBiitmap文字用
	int aaLv;		//諧調数（アンチエイリアスレベル）Win32APIのGetGlyphOutline関数参照
	//GGO_BITMAP：２色{0|1}
	//GGO_GRAY2_BITMAP：５色{0~4}
	//GGO_GRAY4_BITMAP：１７色{0~16}
	//GGO_GRAY8_BITMAP：６５色{0~64}
	wchar_t	wch;	//変換元の文字
	//Bmp* _next;	//リスト構造用＜工事中＞
};

//----------------------------------------------------------------
//コンソール I / O 初期化
/**
 * @brief	コンソール I/O 初期化
 *
 * @param	_width [入力] コンソールウィンドウの横サイズ(1～)
 * @param	_height [入力] コンソールウィンドウの縦サイズ(1～)
 */
 //コンソール I / O 初期化（拡張版）
void	InitConioEx(int _width, int _height, double _scale_x = 1.0, double _scale_y = 1.0, int _font_w = 16, int _font_h = 16, const wchar_t* _font_face_name = nullptr, const COLORREF* _pal16 = nullptr, bool _init_wbuf = true);
inline void InitConio(int _width, int _height) {
	InitConioEx(_width, _height, 1.0, 1.0, DEF_FONTSIZE_X / 2, DEF_FONTSIZE_Y, nullptr, nullptr, true);
}
//void	InitConioEx(COORD _screen_size, D2D1_SIZE_F _scale, CONSOLE_FONT_INFOEX _font_ex);
void	EndConioEx(void);		//コンソール I / O 終了処理（拡張版）
inline int InitDoubleBuffer(void) { return 1; }	//ダブルバッファ初期化※無効⇒将来この関数は無くなる。
#define	FlipScreen	RenderScreen	//ダブルバッファ時の描画面切替※無効⇒RenderScreen()が呼び出される⇒将来この関数は無くなる。
void	RenderScreen(void);		//描画リストを画面に表示する。
void	SetScreenFontSize(int _width, int _height);	//フォントサイズ変更
//----------------------------------------------------------------
//ウィンドウ
LONG_PTR	FixWin(void);			//ウィンドウサイズ固定(変更前の状態を返す)
LONG_PTR	FixWin(int _x, int _y);	//ウィンドウ位置を指定して、ウィンドウサイズ固定(変更前の状態を返す)
HANDLE	GetCurrentHandle(void);	//現在アクセス中のバッファのハンドル。
void	SetCaption(const char* title);	//コンソールウィンドウのタイトルバーにテキストを設定
void	SetCaptionF(const char* _format, ...);	//コンソールウィンドウのタイトルバーに書式指定してテキストを設定
int		GetCaption(char* title, int len);	//コンソールウィンドウのタイトルバーに表示されるテキストを取得
RECT& GetConWinSize(RECT& _r);	//クライアント領域のサイズとフォントサイズの取得
//----------------------------------------------------------------
//カーソル
int		GetCursorX(void);				//水平方向のカーソル位置を取得
int		GetCursorY(void);				//垂直方向のカーソル位置を取得
void	SetCursorPosition(int x, int y);	//カーソル位置の移動
void	SetCursorType(int type);		//カーソルタイプ設定
POINT	GetCursorMousePos(POINT* _mp);	//マウス座標(文字単位)の取得
//----------------------------------------------------------------
//文字列描画
void	PrintStringA(const char* _srcbuf, int _size);	//文字列の出力（マルチバイト文字用）
void	PrintStringW(const wchar_t* _srcbuf, int _size);	//文字列の出力（Unicode文字用）
#ifdef UNICODE
void	PrintString(const char* _srcbuf, int _size);	// 文字列の出力(マルチバイト⇒Unicode変換版)
#else
#define	PrintString	PrintStringA
#endif // UNICODE
//文字全体
void	SetHighVideoColor(void);		//文字色高輝度化
void	SetLowVideoColor(void);			//文字色低輝度化
void	SetNormalVideoColor(void);		//既定文字色設定
//文字属性指定
void	SetTextBackColor(int color);	//文字背景色設定
void	SetConsoleTextColor(int color);	//文字色設定
void	SetTextAttribute(int attribute);	//文字色背景色同時設定
//行操作
void	ClearLine(void);				//行末まで消去
void	InsertLine(void);	//現在行に挿入
void	DeleteLine(void);	//現在行の削除
//----------------------------------------------------------------
//半角文字＜＝＞全角文字の相互変換
char* HanToZenA(const char* _src);	//半角文字を全角文字に変換する（マルチバイト版）
wchar_t* HanToZenW(const wchar_t* _src);	//半角文字を全角文字に変換する（Unicode版）
//----------------------------------------------------------------
//拡張文字列描画
void	VPrintStringFA(bool _zenkaku, const char* _format, va_list _ap);	//書式指定付文字列描画（引数リスト版）（マルチバイト文字用）
void	VPrintStringFW(bool _zenkaku, const wchar_t* _format, va_list _ap);	//書式指定付文字列描画（引数リスト版）（Unicode文字用）
void	PrintStringFA(bool _zenkaku, const char* _format, ...);	//書式指定付文字列描画（マルチバイト文字用）
void	PrintStringFW(bool _zenkaku, const wchar_t* _format, ...);	//書式指定付文字列描画（Unicode文字用）
void	PosPrintStringFA(int _xp, int _yp, bool _zenkaku, const char* _format, ...);	//位置指定＆書式指定付文字列描画（マルチバイト文字用）
void	PosPrintStringFW(int _xp, int _yp, bool _zenkaku, const wchar_t* _format, ...);	//位置指定＆書式指定付文字列描画（Unicode文字用）
void	DrawStringFA(int _x, int _y, bool _zenkaku, const char* _format, ...);	//座標指定（画面左上隅が(0, 0)座標）＋書式指定付文字列描画（マルチバイト文字用）
void	DrawStringFW(int _x, int _y, bool _zenkaku, const wchar_t* _format, ...);	//座標指定（画面左上隅が(0, 0)座標）＋書式指定付文字列描画（Unicode文字用）
// 以下の「#define」はプロジェクトが(マルチバイト／Unicode)に関わらず、
// 常にマルチバイト版（末尾'A'）の関数名を使うようにする為の定義。
// ※Unicode版を使うときは明示的に末尾'W'の関数名を呼び出す。
#ifdef UNICODE
#define	HanToZen	HanToZenA
#define	PrintStringF	PrintStringFA
#define	PosPrintStringF	PosPrintStringFA
#define	DrawStringF		DrawStringFA
#else	// UNICODE
#define	HanToZen	HanToZenA
#define	PrintStringF	PrintStringFA
#define	PosPrintStringF	PosPrintStringFA
#define	DrawStringF		DrawStringFA
#endif // UNICODE
//----------------------------------------------------------------
// パレット関係
// パレット固定データ（ANSI 256色）
//	COLORREFはBMP画像のパレット形式
extern const COLORREF ANSI_PAL256_COLOR[NUM_ANSI_PAL];	//COLORREF
extern const RGBQUAD ANSI_PAL256_RGB[NUM_ANSI_PAL];	//RGBQUAD
COLORREF	RGBQtoCREF(RGBQUAD rgb);	//RGBQUAD: {Blue, Green, Red, 0}型をCOLORREF:0x00BBGGRR型に変換
RGBQUAD		CREFtoRGBQ(COLORREF ref);	//COLORREF:0x00BBGGRR型をRGBQUAD : {Blue, Green, Red, 0}型に変換
COLORREF* ConvRGBQtoCREF(const RGBQUAD* _rgb, COLORREF* _cref);	//パレット変換：RGBQ[16]->COLORREF[16]
RGBQUAD* ConvCREFtoRGBQ(const COLORREF* _cref, RGBQUAD* _rgb);	//パレット変換：RGBQ[16]->COLORREF[16]
void	SetPalette(const COLORREF* _pal256, int _p1, int _p2);	//１６色パレット設定
void	SetPalette(const RGBQUAD* _pal256, int _p1, int _p2);	//パレット設定
void	SetPalette(const Bmp* _pBmp);	//Bmpから１６色パレット設定
inline void ResetPalette(void) { SetPalette(ANSI_PAL256_COLOR, 0, 15); }
//RGB値の作成
inline RGBQUAD GetRGBQUAD(int _r, int _g, int _b, int _a = 0) { return RGBQUAD{ (BYTE)_b,(BYTE)_g,(BYTE)_r,(BYTE)_a }; }
inline DWORD GetRGB32(int _r, int _g, int _b, int _a = 0) { return (DWORD)(((_a & 0xFF) << 24) | ((_r & 0xFF) << 16) | ((_g & 0xFF) << 8) | (_b & 0xFF)); }

//----------------------------------------------------------------
//フレームバッファ画像描画
void	ClearScreen(void);				//画面（スクリーンバッファ）消去
void	ClearScreen(int _cc);			//消去するパレット番号を指定する
void	ClearScreen(int _red, int _green, int _blue);		//消去するＲＧＢ値を指定する
void	PrintFrameBuffer(void);	//フレームバッファをレンダー用リストに追加する
void	PrintFrameBuffer(const char* _frame_buffer, int _width, int _height);	//指定バッファ(パレット値)の内容をフレームバッファにコピーしてからフレームバッファをレンダーリストに追加する
void	PrintFrameBuffer(const RGBQUAD* _frame_buffer, int _width, int _height);	//指定バッファ(RGB値)の内容をフレームバッファにコピーしてからフレームバッファをレンダーリストに追加する
inline void	PrintFrameBuffer(const UINT* _frame_buffer, int _width, int _height) {
	PrintFrameBuffer((RGBQUAD*)_frame_buffer, _width, _height);
}
//----------------------------------------------------------------
//文字列描画：DirectWriteによる文字列描画
//フォントサイズ指定有り⇒次の文字描画で文字サイズ指定しない場合は、ここでの文字サイズが引き継がれる。
void	WriteTextA(int _xp, int _yp, const char* _text, double _font_size_h = 0, D2D1::ColorF _fgc = D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF _bgc = D2D1::ColorF(0, 0, 0, 0), bool _new_line = false);
void	WriteTextW(int _xp, int _yp, const wchar_t* _text, double _font_size_h = 0, D2D1::ColorF _fgc = D2D1::ColorF(1, 1, 1, 1), D2D1::ColorF _bgc = D2D1::ColorF(0, 0, 0, 0), bool _new_line = false);
//--------------------------------
//書式指定と可変引数の文字列描画
void SetFontName(const wchar_t* _name);
void SetTextHD(bool _f);
void SetTextAntialiasD2D(D2D1_TEXT_ANTIALIAS_MODE _mod);
D2D1_TEXT_ANTIALIAS_MODE GetTextAntialiasD2D(void);
void	SetFontSize(FLOAT _width, FLOAT _height);	//フォントサイズ設定
void	GetFontSize(FLOAT* _p_width, FLOAT* _p_height);	//フォントサイズ取得
void	SetZenkaku(bool _f);	//全角・半角切替えフラグのＯＮ／ＯＦＦ
void	WriteTextFA(int _x, int _y, double _font_size_h, const char* _format, ...);
void	WriteTextFW(int _x, int _y, double _font_size_h, const wchar_t* _format, ...);
//--------------------------------
#ifdef UNICODE
#define	WriteText	WriteTextW
#define	WriteTextF	WriteTextFW
#else
#define	WriteText	WriteTextA
#define	WriteTextF	WriteTextFA
#endif // UNICODE

//----------------------------------------------------------------
//Bmp画像の描画：
//	_tr_codeには、１～８ビット画像なら透明にしたいパレット番号をDWORDで、
//	２４～３２ビット画像なら透過したいRGB値をDWORDで指定する。
void	DrawBmp(int _xp, int _yp, const Bmp* _bmp, bool _tr = true, DWORD _tr_code = 0);	//Bmp画像の出力
#define	BMP_NO_INV	(0b000)
#define	BMP_HINV	(0b100)
#define	BMP_VINV	(0b010)
#define	BMP_HVINV	(BMP_HINV|BMP_VINV)
#define	BMP_ROT90	(0b001)
#define	BMP_ROT180	BMP_HVINV
#define	BMP_ROT270	(BMP_ROT90|BMP_HVINV)
void	DrawBmp(int _xp, int _yp, const Bmp* _bmp, int _hvinv, bool _tr = true, DWORD _tr_code = 0);	//Bmp画像の出力(反転機能)
//αブレンドモードのＯＮ／ＯＦＦ
bool AlphaBlendMode(bool _flag);
//透過レベル指定＋（αブレンドモードがＯＮ）
double SetTrLevel(double _tr_level);
//透過レベルリセット＋（αブレンドモードがＯＦＦ）
void ResetTrLevel(void);
//----------------------------------------------------------------
/*
* @brief	画像の回転拡大縮小描画
*/
void Rotate2d(double _cx, double _cy, double& _x, double& _y, double _angle);
void Rotate2d(double _cx, double _cy, POINT_F& _p, double _angle);
bool GetPixel(const Bmp* _bmp, double _x, double _y, DWORD& _pix);
bool GetPixel(const Bmp* _bmp, double _x, double _y, BYTE& _cc);
/*
* @brief	Bmp画像の回転拡大縮小
*/
void DrawBmpEx(const Bmp* _bmp, int _cx, int _cy, double _angle, double _scale, int _hvinv, bool _tr, DWORD _tr_code);
/*
* @brief	Bmp画像の回転拡大縮小
*/
inline void DrawBmpEx(const Bmp* _bmp, POINT_F _cp, double _angle, double _scale, int _hvinv, bool _tr = true, DWORD _tr_code = 0) {
	DrawBmpEx(_bmp, (int)_cp.x, (int)_cp.y, _angle, _scale, _hvinv, _tr, _tr_code);
}

//################################################################################################################################
// ビットマップ(bmp)
//################################################################################################################################
//ＷＩＣ(Windows イメージング コンポーネント)による画像ファイルの読込
Bmp* LoadBmpFromImgFileW(const wchar_t* _filename);
Bmp* LoadBmpFromImgFileA(const char* _filename);
//画像ファイルの読込：BMP形式以外も読み込める
inline Bmp* LoadBmpW(const wchar_t* _file_name) {
	return LoadBmpFromImgFileW(_file_name);
}
inline Bmp* LoadBmpA(const char* _file_name) {
	return LoadBmpFromImgFileA(_file_name);
}
//画像の分割読込：BMP以外の形式も読込可能
bool LoadDivBmpW(const wchar_t* _path, int _x0, int _y0, size_t _xpix, size_t _ypix, size_t _xcount, size_t _ycount, Bmp** _pp_bmp);
bool LoadDivBmpA(const char* _path, int _x0, int _y0, size_t _xpix, size_t _ypix, size_t _xcount, size_t _ycount, Bmp** _pp_bmp);

#ifdef UNICODE
#define	LoadBmp	LoadBmpW
#define	LoadDivBmp	LoadDivBmpW
#define	LoadBmpFromImgFile	LoadBmpFromImgFileW
#else
#define	LoadBmp	LoadBmpA
#define	LoadDivBmp	LoadDivBmpA
#define	LoadBmpFromImgFile	LoadBmpFromImgFileA
#endif // UNICODE

void	DeleteBmp(Bmp** _p_bmp);	//使い終わったBMP画像の削除。
void	Bmp24SwapRB(Bmp* _bmp);	//BRGのピクセルをRGBに変換する。
//空のBmpオブジェクト作成。
Bmp* CreateBmp(int _width, int _height, int _colbits, size_t _numpal, const COLORREF* const  _pal);
//指定したBmpと同じ設定で空のBmpを作る(パレットがあればコピーされる)
inline Bmp* CreateBmp(const Bmp* _src) {
	return CreateBmp(_src->width, _src->height, _src->colbit, _src->numpal, _src->pal);
}
//Bmpの全ピクセルを初期化する。パレットのある画像の場合はパレット番号を指定、フルカラーの場合は0x00RRGGBBを指定する。
void ClearBmp(Bmp* _p, int _color = 0);
//元のBmpの一部を別のBmpにコピーする。
Bmp* CopyBmp(Bmp* _dest, int _dx, int _dy, const Bmp* _src, int _sx, int _sy, int _width, int _height, bool _tr);
//元のBmpの一部を別のBmpにコピーする。
inline Bmp* CopyBmp(Bmp* _dest, const Bmp* _src, int _sx, int _sy, int _width, int _height, bool _tr) {
	return CopyBmp(_dest, 0, 0, _src, _sx, _sy, _width, _height, _tr);
}
//取り込むサイズを転送先のサイズに合わせる
inline Bmp* CopyBmp(Bmp* _dest, const Bmp* _src, int _xp, int _yp, bool _tr) {
	return CopyBmp(_dest, 0, 0, _src, _xp, _yp, _dest->width, _dest->height, _tr);
}
//取り込むサイズを転送先のサイズに合わせる
inline Bmp* CopyBmp(Bmp* _dest, const Bmp* _src, bool _tr = false) {
	return CopyBmp(_dest, 0, 0, _src, 0, 0, _dest->width, _dest->height, _tr);
}
//Bmpの一部を切り出し、新しいBmpを作る
Bmp* CreateFromBmp(const Bmp* _src, int _xp, int _yp, int _width, int _height);
//同じサイズのBmpを作る
inline Bmp* CreateFromBmp(const Bmp* _src) {
	return CreateFromBmp(_src, 0, 0, _src->width, _src->height);
}
//----------------------------------------------------------------
//文字列をBmp画像の配列として生成する。
//	ビットマップ文字（文字をBmp画像として生成）
//	_ggo へは {GGO_BITMAP,GGO_GRAY2_BITMAP,GGO_GRAY4_BITMAP,GGO_GRAY8_BITMAP}のどれかを指定
Bmp* CreateBmpChar(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, const TCHAR* _text);
Bmp* CreateBmpString(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, const TCHAR* _text);
inline Bmp* CreateBmpString(const TCHAR* _font_name, int _font_size, int _bold, const TCHAR* _text) {
	return CreateBmpString(_font_name, _font_size, _bold, GGO_BITMAP, _text);
}
Bmp* CreateBmpStringF(const TCHAR* _font_name, int _font_size, int _bold, int _ggo, bool _zenkaku, const TCHAR* _format, ...);
Bmp* CreateBmpStringF(const TCHAR* _font_name, int _font_size, int _bold, bool _zenkaku, const TCHAR* _format, ...);
//----------------------------------------------------------------
// キー入力関係
void	ResetKeyMap(void);	//キー情報リセット
int		InputKeyMouse(int port);	//キーボード・マウス入力
//----------------------------------------------------------------
// 拡張キー入力
SHORT	GetKey(int _vk);	//単一キーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
SHORT	WaitKey(int _vk);	//キー入力を待つ。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
int		GetKeyEx(int _vk, int _chktype);	//全てのキーの入力と判定
void	GetKeyAll(void);	//全てのキーの入力。※このコンソールウィンドウにフォーカスが当たっている時だけ入力する。
int		ChkKeyEdge(int _vk);	//Edgeキー入力判定：GetKeyAll()で入力したキー情報についてEdgeキー（トリガーキー）入力判定する
int		ChkKeyPress(int _vk);	//Pressキー入力判定：GetKeyAll()で入力したキー情報についてPressキー（押下キー）入力判定する
//----------------------------------------------------------------
// ジョイパッド入力
int		InputJoystick(int port);	//ジョイパッド入力
#if FALSE
int		InputJoystickX(int port);					// ゲームパッド入力(XInput対応)
#else	//FALSE
int		InputJoystickX(int id, int port);			// ゲームパッド入力(XInput対応)
#endif // FALSE
// ================================================================
// ----------------------------------------------------------------
// サウンド再生
int* MciOpenSound(const char* path);	//サウンド ファイルを開く
void	MciCloseSound(int* sound_id);	//サウンド ファイルを閉じる
void	MciPlaySound(int* sound_id, int repeat);	//サウンドを再生する
void	MciStopSound(int* sound_id);	//サウンド再生を停止する
int		MciCheckSound(int* sound_id);	//サウンド再生状態の取得
void	MciUpdateSound(int* sound_id);	//ループ再生の強制更新
void	MciSetVolume(int* sound_id, int percent);	//再生音量を設定する
#if true
// ----------------------------------------------------------------
// XAudio2
struct Snd {
	WAVEFORMATEX	wfmt;
	//WAVEFORMATEXTENSIBLE	wfmtex;
	XAUDIO2_BUFFER	xbuf;
	IXAudio2SourceVoice* psv;
	float	volume;
};

extern HRESULT InitSnd(void);
extern void EndSnd(void);

extern Snd* LoadSnd(const wchar_t* _filename);
extern void DeleteSnd(Snd** _xa2);

extern bool CheckSnd(const Snd* _xa2);
extern bool PlaySnd(Snd* _xa2, bool _loop);
extern bool StopSnd(Snd* _xa2);

extern void SetVolumeSnd(Snd* _xa2, float _volume);
extern void SetVolumeSnd(const Snd* _xa2);
extern float GetVolumeSnd(Snd* _xa2);
#endif

#ifdef CONIOEX_DDA_SHAPE
//================================================================
//	図形の描画（DDAで描画）
//================================================================

/*
* @brief	点の描画
*
* @param	int _x	Ｘ座標
* @param	int _y	Ｙ座標
* @param	RGBQUAD _rgb	ＲＧＢ値
*/
void	DrawPixel(int _x, int _y, RGBQUAD _rgb);	//点を打つ
inline void	DrawPixel(const POINT& p, RGBQUAD _rgb) { DrawPixel(p.x, p.y, _rgb); }
inline void DrawPixel(int _x, int _y, DWORD _rgbword) { DrawPixel(_x, _y, *((RGBQUAD*)&_rgbword)); }	//DrawPixel
inline void DrawPixel(int _x, int _y, UINT _rgbword) { DrawPixel(_x, _y, *((RGBQUAD*)&_rgbword)); }	//DrawPixel
inline void DrawPixel(int _x, int _y, int _rgbword) { DrawPixel(_x, _y, *((RGBQUAD*)&_rgbword)); }	//DrawPixel

/*
* @brief	点の描画
*
* @param	int _x	Ｘ座標
* @param	int _y	Ｙ座標
* @param	BYTE _c	カラーパレット番号（０～２５５）
*/
void	DrawPixel(int _x, int _y, BYTE _cc);	//点を打つ
inline void	DrawPixel(const POINT& p, BYTE _cc) { DrawPixel(p.x, p.y, _cc); }
/*
* @brief	線分の移動
* @param	LINE* _pl	線分データのポインタ
* @param	int _dx		Ｘ方向移動量
* @param	int _dy		Ｙ方向移動量
*/
inline void MoveLine(LINE& _pl, int _dx, int _dy) {
	_pl.x1 += _dx;
	_pl.y1 += _dy;
	_pl.x2 += _dx;
	_pl.y2 += _dy;
}
inline void MoveLine(LINE* _pl, int _dx, int _dy) {
	_pl->x1 += _dx;
	_pl->y1 += _dy;
	_pl->x2 += _dx;
	_pl->y2 += _dy;
}
/*
* @brief	線分の移動
* @param	LINE* _pl	線分データのポインタ
* @param	int _px		移動先Ｘ座標（left）
* @param	int _py		移動先Ｙ座標（top）
*/
inline void SetLine(LINE& _pl, int _px, int _py) {
	_pl.x2 -= (_pl.x1 - 1);	//幅
	_pl.y2 -= (_pl.y1 - 1);	//高さ
	_pl.x1 = _px;
	_pl.y1 = _py;
	_pl.x2 += _px;
	_pl.y2 += _py;
}
inline void SetLine(LINE* _pl, int _px, int _py) {
	_pl->x2 -= (_pl->x1 - 1);	//幅
	_pl->y2 -= (_pl->y1 - 1);	//高さ
	_pl->x1 = _px;
	_pl->y1 = _py;
	_pl->x2 += _px;
	_pl->y2 += _py;
}
/*
* @brief	矩形の相対移動
* @param	RECT* _pr	矩形データのポインタ
* @param	int _dx		Ｘ方向移動量
* @param	int _dy		Ｙ方向移動量
*/
inline void MoveRect(RECT& _pr, int _dx, int _dy) {
	_pr.left += _dx;
	_pr.top += _dy;
	_pr.right += _dx;
	_pr.bottom += _dy;
}
inline void MoveRect(RECT* _pr, int _dx, int _dy) {
	_pr->left += _dx;
	_pr->top += _dy;
	_pr->right += _dx;
	_pr->bottom += _dy;
}
/*
* @brief	矩形の移動
* @param	RECT* pr	矩形データのポインタ
* @param	int _px		移動先Ｘ座標（left）
* @param	int _py		移動先Ｙ座標（top）
*/
inline void SetRect(RECT& _pr, int _px, int _py) {
	_pr.right -= _pr.left;	//幅-1
	_pr.bottom -= _pr.top;	//高さ-1
	_pr.left = _px;
	_pr.top = _py;
	_pr.right += _px;
	_pr.bottom += _py;
}
inline void SetRect(RECT* _pr, int _px, int _py) {
	_pr->right -= _pr->left;	//幅-1
	_pr->bottom -= _pr->top;	//高さ-1
	_pr->left = _px;
	_pr->top = _py;
	_pr->right += _px;
	_pr->bottom += _py;
}
//==================== 直線 ====================
/*
* @brief	水平線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _x2		終了Ｘ座標（Ｙ座標は_y1と同じ）
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLineH(int _x1, int _y1, int _x2, int _cc);
/*
* @brief	水平線を描画する
* @param	LINE _ln	線分構造体（x1,y1,x2を使って描画）
* @param	int _cc		カラーコード（パレットの番号）
*/
inline void DrawLineH(const LINE& _ln, int _cc) {
	DrawLineH(_ln.x1, _ln.y1, _ln.x2, _cc);
}
inline void DrawLineH(const LINE* _ln, int _cc) {
	DrawLineH(_ln->x1, _ln->y1, _ln->x2, _cc);
}
/*
* @brief	水平線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _x2		終了Ｘ座標（Ｙ座標は_y1と同じ）
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLineH(int _x1, int _y1, int _x2, RGBQUAD _rgb);
/*
* @brief	水平線を描画する
* @param	LINE _ln	線分構造体（x1,y1,x2を使って描画）
* @param	RGBQUAD _rgb	描画色のRGB値
*/
inline void DrawLineH(const LINE& _ln, RGBQUAD _rgb) {
	DrawLineH(_ln.x1, _ln.y1, _ln.x2, _rgb);
}
inline void DrawLineH(const LINE* _ln, RGBQUAD _rgb) {
	DrawLineH(_ln->x1, _ln->y1, _ln->x2, _rgb);
}
//=== 垂直線 ===
/*
* @brief	垂直線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _y2		終了Ｙ座標（Ｘ座標は_x1と同じ）
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLineV(int _x1, int _y1, int _y2, int _cc);
/*
* @brief	垂直線を描画する
* @param	LINE _ln	線分構造体（x1,y1,y2を使って描画）
* @param	int _cc		カラーコード（パレットの番号）
*/
inline void DrawLineV(const LINE& _ln, int _cc) {
	DrawLineV(_ln.x1, _ln.y1, _ln.y2, _cc);
}
inline void DrawLineV(const LINE* _ln, int _cc) {
	DrawLineV(_ln->x1, _ln->y1, _ln->y2, _cc);
}
/*
* @brief	垂直線を描画する
* @param	int _x1,_y1	開始座標
* @param	int _y2		終了Ｙ座標（Ｘ座標は_x1と同じ）
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLineV(int _x1, int _y1, int _y2, RGBQUAD _rgb);
/*
* @brief	垂直線を描画する
* @param	LINE _ln	線分構造体（x1,y1,y2を使って描画）
* @param	RGBQUAD _rgb	描画色のRGB値
*/
inline void DrawLineV(const LINE& _ln, RGBQUAD _rgb) {
	DrawLineV(_ln.x1, _ln.y1, _ln.y2, _rgb);
}
inline void DrawLineV(const LINE* _ln, RGBQUAD _rgb) {
	DrawLineV(_ln->x1, _ln->y1, _ln->y2, _rgb);
}
//=== 線分 ===
/*
* @brief	線分の描画
* @param	int _x1,_y2	開始座標
* @param	int _x2,_y2	終了座標
* @param	int _cc		カラーコード（パレットの番号）
*/
void DrawLine(int _x1, int _y1, int _x2, int _y2, int _cc);
/*
* @brief	線分の描画
* @param	LINE _ln	線分構造体
* @param	int _cc		カラーコード（パレットの番号）
*/
inline void DrawLine(const LINE& _ln, int _cc) {
	DrawLine(_ln.x1, _ln.y1, _ln.x2, _ln.y2, _cc);
}
inline void DrawLine(const LINE* _ln, int _cc) {
	DrawLine(_ln->x1, _ln->y1, _ln->x2, _ln->y2, _cc);
}
/*
* @brief	線分の描画
* @param	int _x1,_y2	開始座標
* @param	int _x2,_y2	終了座標
* @param	RGBQUAD _rgb	描画色のRGB値
*/
void DrawLine(int _x1, int _y1, int _x2, int _y2, RGBQUAD _rgb);
/*
* @brief	線分の描画
* @param	LINE _ln	線分構造体
* @param	RGBQUAD _rgb	描画色のRGB値
*/
inline void DrawLine(const LINE& _ln, RGBQUAD _rgb) {
	DrawLine(_ln.x1, _ln.y1, _ln.x2, _ln.y2, _rgb);
}
inline void DrawLine(const LINE* _ln, RGBQUAD _rgb) {
	DrawLine(_ln->x1, _ln->y1, _ln->x2, _ln->y2, _rgb);
}
//==================== 矩形 ====================
/*
* @brief	矩形を描画する
* @param	int _x1,_y1	左上座標
* @param	int _x2,_y2	右下座標
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
void DrawRect(int _x1, int _y1, int _x2, int _y2, int _cc, bool _fill = false);
/*
* @brief	矩形を描画する
* @param	RECT _rc	矩形の座標
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
inline void DrawRect(const RECT& _rc, int _cc, bool _fill = false) {
	DrawRect(_rc.left, _rc.top, _rc.right, _rc.bottom, _cc, _fill);
}
inline void DrawRect(const RECT* _rc, int _cc, bool _fill = false) {
	DrawRect(_rc->left, _rc->top, _rc->right, _rc->bottom, _cc, _fill);
}
/*
* @brief	矩形を描画する
* @param	POINT _rc	矩形の左上座標
* @param	SIZE _sz	矩形の幅と高さ
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
inline void DrawRect(const POINT& _p, const SIZE& _sz, int _cc, bool _fill = false) {
	DrawRect(_p.x, _p.y, _p.x + (_sz.cx - 1), _p.y + (_sz.cy - 1), _cc, _fill);
}
inline void DrawRect(const POINT* _p, const SIZE* _sz, int _cc, bool _fill = false) {
	DrawRect(_p->x, _p->y, _p->x + (_sz->cx - 1), _p->y + (_sz->cy - 1), _cc, _fill);
}
/*
* @brief	矩形を描画する
* @param	int _x1,_y1	左上座標
* @param	int _x2,_y2	右下座標
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
void DrawRect(int _x1, int _y1, int _x2, int _y2, RGBQUAD _rgb, bool _fill = false);
/*
* @brief	矩形を描画する
* @param	RECT _rc	矩形の座標
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
inline void DrawRect(const RECT& _rc, RGBQUAD _rgb, bool _fill = false) {
	DrawRect(_rc.left, _rc.top, _rc.right, _rc.bottom, _rgb, _fill);
}
inline void DrawRect(const RECT* _rc, RGBQUAD _rgb, bool _fill = false) {
	DrawRect(_rc->left, _rc->top, _rc->right, _rc->bottom, _rgb, _fill);
}
/*
* @brief	矩形を描画する
* @param	POINT _rc	矩形の左上座標
* @param	SIZE _sz	矩形の幅と高さ
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
inline void DrawRect(const POINT& _p, const SIZE& _sz, RGBQUAD _rgb, bool _fill = false) {
	DrawRect(_p.x, _p.y, _p.x + (_sz.cx - 1), _p.y + (_sz.cy - 1), _rgb, _fill);
}
inline void DrawRect(const POINT* _p, const SIZE* _sz, RGBQUAD _rgb, bool _fill = false) {
	DrawRect(_p->x, _p->y, _p->x + (_sz->cx - 1), _p->y + (_sz->cy - 1), _rgb, _fill);
}
//==================== 円形 ====================
/*
* @brief	円を描画する
* @param	int _cx,_cy	中心座標
* @param	int _r		半径
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
void DrawCircle(int _cx, int _cy, int _r, int _cc, bool _fill = false);
/*
* @brief	円を描画する
* @param	CIRCLE _pc	円の座標と半径
* @param	int _cc		カラーコード（パレットの番号）
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
inline void DrawCircle(const CIRCLE& _cr, int _cc, bool _fill = false) {
	DrawCircle(_cr.cx, _cr.cy, (int)_cr.r, _cc, _fill);
}
inline void DrawCircle(const CIRCLE* _pr, int _cc, bool _fill = false) {
	DrawCircle(_pr->cx, _pr->cy, (int)_pr->r, _cc, _fill);
}
/*
* @brief	円を描画する
* @param	int _cx,_cy	中心座標
* @param	int _r		半径
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
void DrawCircle(int _cx, int _cy, int _r, RGBQUAD _rgb, bool _fill = false);
/*
* @brief	円を描画する
* @param	CIRCLE _pc	円の座標と半径
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし(true:する/false:しない)
*/
inline void DrawCircle(const CIRCLE& _cr, RGBQUAD _rgb, bool _fill = false) {
	DrawCircle(_cr.cx, _cr.cy, (int)_cr.r, _rgb, _fill);
}
inline void DrawCircle(const CIRCLE* _pr, RGBQUAD _rgb, bool _fill = false) {
	DrawCircle(_pr->cx, _pr->cy, (int)_pr->r, _rgb, _fill);
}
//==================== 三角形 ====================
/*
* @brief	三角形の描画
*
* @param	int _x0,_y0	頂点＃０
* @param	int _x1,_y1	頂点＃１
* @param	int _x2,_y2	頂点＃２
* @param	RGBQUAD _rgb	描画色のRGB値
* @param	bool _fill	塗りつぶし（true:有り／false:無し）
*/
void DrawTriangle(int _x0, int _y0, int _x1, int _y1, int _x2, int _y2, RGBQUAD _rgbq, bool _fill);
inline void DrawTriangle(const TRIANGLE p3, RGBQUAD _rgbq, bool _fill) {
	DrawTriangle((int)p3.x0, (int)p3.y0, (int)p3.x1, (int)p3.y1, (int)p3.x2, (int)p3.y2, _rgbq, _fill);
}
inline void DrawTriangle(const TRIANGLE* p3, RGBQUAD _rgbq, bool _fill) {
	DrawTriangle((int)p3->x0, (int)p3->y0, (int)p3->x1, (int)p3->y1, (int)p3->x2, (int)p3->y2, _rgbq, _fill);
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
void DrawTriangle(int _x0, int _y0, int _x1, int _y1, int _x2, int _y2, int _cc, bool _fill);
inline void DrawTriangle(const TRIANGLE p3, int _cc, bool _fill) {
	DrawTriangle((int)p3.x0, (int)p3.y0, (int)p3.x1, (int)p3.y1, (int)p3.x2, (int)p3.y2, _cc, _fill);
}
inline void DrawTriangle(const TRIANGLE* p3, int _cc, bool _fill) {
	DrawTriangle((int)p3->x0, (int)p3->y0, (int)p3->x1, (int)p3->y1, (int)p3->x2, (int)p3->y2, _cc, _fill);
}
#endif // !CONIOEX_DDA_SHAPE

//----------------------------------------------------------------
/*
* @brief	フレーム同期＆計測用関数
*
* @param	double _FPS	FPS(Frame per scond)の値を指定する
*						デフォルト値：60.0
*/
void InitFrameSync(double _FPS = 60.0);
/*
* @brief	フレーム同期
*
* @param	 bool _debug_out	デバッグ出力(true:する/false:しない)
*								※Releaseビルドはこのフラグに限らず出力しない
*/
struct DEBUG_FRAME_INFO {
	double	fps;	//frame per second.
	int	fcnt;	//frame counter.
	double	tps;	//count per sec.
	double	tpf;	//count per frame.
	double	wait;	//wait count per frame.
	double	itrvl;	//interval count per frame.
	double	total;	//total count per frame.
};
void FrameSync(bool _debug_out = false, void (*_p)(DEBUG_FRAME_INFO* _pdfi) = nullptr);
extern volatile double g_FrameCalcFPS;	//FPS
extern volatile int g_FrameCountPerSec;	//フレーム回数(g_FrameCalcFPS算出直後に０リセットされる)
extern volatile UINT64 g_FrameCounter;	//フレームカウンタ

/**
* @copyright (c) 2018-2019 HAL Osaka College of Technology & Design (Ihara, H.)
*/
