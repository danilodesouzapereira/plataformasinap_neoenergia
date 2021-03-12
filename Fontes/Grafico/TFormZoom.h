//---------------------------------------------------------------------------
#ifndef TFormZoomH
#define TFormZoomH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTGrafico;
class VTRedes;

//---------------------------------------------------------------------------
class TFormZoom : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction     *ActionZoomChanged;
   TImage      *Image;
   void __fastcall ActionZoomChangedExecute(TObject *Sender);
   void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ImageDblClick(TObject *Sender);

public:		// User declarations
   __fastcall TFormZoom(TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormZoom(void);

private:	//métodos
   void __fastcall CalculaEscala(void);
   void __fastcall CalculaOffSet(TRect &r);
   void __fastcall ConverteAreasEsqToPixel(void);
   void __fastcall DeterminaAreas(void);
   void __fastcall DrawAreaRede(void);
   void __fastcall DrawAreaSubRede(void);
   void __fastcall DrawAreaZoom(void);
   void __fastcall DrawMoldura(TRect r, TColor color, int width);
   void __fastcall DrawRetangulo(TRect r, TColor color);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall RedfineAreaZoom(int x, int y);
   int  __fastcall Xesq_to_Xpixel(int x);
   int  __fastcall Xpixel_to_Xesq(int x);
   int  __fastcall Yesq_to_Ypixel(int y);
   int  __fastcall Ypixel_to_Yesq(int y);

private:	//objetos externos
    VTApl     *apl;
    VTGrafico *grafico;
    VTRedes   *redes;

private:	//dados locais
   #define DELTA 2
   double fat_escala;
   struct   {int x, y;} offset;
   struct   {
            VTArea *area;
            TRect  r;
            } rede, zoom, tela;
   //memória alocada p/ TImage
   Graphics::TBitmap *graph_bmp;
};

//---------------------------------------------------------------------------
#endif
//eof
