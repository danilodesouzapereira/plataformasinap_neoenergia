//---------------------------------------------------------------------------
#ifndef TUnifFlowH
#define TUnifFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTTrecho;

//---------------------------------------------------------------------------
class TUnifFlow : public TUnifEditor
   {
   public:
           __fastcall TUnifFlow(VTApl *apl, int tipo_graf, AnsiString nome);
           __fastcall ~TUnifFlow(void);
      void __fastcall Redraw(VTEdt *editor);
      void __fastcall Redraw(int op, VTEdt *editor);
      
   protected: //métodos que podem ser redefinidos
      virtual void __fastcall RedrawAnimacao(void);
      virtual void __fastcall RedrawAnimacao(VTTrecho *ptrecho);
      virtual void __fastcall RedrawAnimacaoB1B2(VTLigacao *pliga);
      virtual void __fastcall RedrawAnimacaoB1B2B3(VTLigacao *pliga);
      virtual void __fastcall RedrawAnimacaoX1Y1X2Y2(int x1, int y1, int x2, int y2, int sentido);

   private: //métodos locais
      //bool __fastcall ExisteFlow(void);
      void __fastcall RedrawAllBarra(void);
      void __fastcall RedrawAllCapserie(void);
      void __fastcall RedrawAllChave(void);
      void __fastcall RedrawAllGerador(void);
      void __fastcall RedrawAllRegulador(void);
      void __fastcall RedrawAllSuprimento(void);
      void __fastcall RedrawAllTrafo(void);
      void __fastcall RedrawAllTrafo3E(void);
      void __fastcall RedrawAllTrecho(void);
      void __fastcall RedrawPie(VTEqbar *eqbar, double cargto, TColor cor);
      void __fastcall RedrawPie(VTLigacao *pliga, TColor cor);
      void __fastcall RedrawReducaoEqptos(VTReducao *reducao, TColor cor);
      void __fastcall RedrawReducaoEsquematico(VTReducao *reducao, TColor cor);
   };

#endif
//---------------------------------------------------------------------------
//eof
