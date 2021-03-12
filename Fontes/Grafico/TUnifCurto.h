//---------------------------------------------------------------------------
#ifndef TUnifCurtoH
#define TUnifCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTTrecho;
class VTLigCC;
class VTRedeCC;

//---------------------------------------------------------------------------
class TUnifCurto : public TUnifEditor
   {
   public:
           __fastcall TUnifCurto(VTApl *apl, int tipo_graf, AnsiString nome);
           __fastcall ~TUnifCurto(void);
      void __fastcall Redraw(VTEdt *editor);
      void __fastcall Redraw(int op, VTEdt *editor);

   protected: //métodos que podem ser redefinidos
      virtual void __fastcall RedrawAnimacao(void);
      virtual void __fastcall RedrawAnimacao(VTLigCC *ligCC);
      virtual void __fastcall RedrawAnimacaoTrecho(VTLigCC *ligCC);
      virtual void __fastcall RedrawAnimacaoB1B2(VTLigCC *ligCC);
      virtual void __fastcall RedrawAnimacaoB1B2B3(VTLigCC *ligCC);
      virtual void __fastcall RedrawAnimacaoX1Y1X2Y2(int x1, int y1, int x2, int y2, int sentido);
      virtual void __fastcall RedrawPontoDefeito(void);

   private: //métodos que podem ser redefinidos
      //bool __fastcall ExisteFlow(void);
      //void __fastcall RedrawAllBarra(void);
      //void __fastcall RedrawAllChave(void);
      //void __fastcall RedrawAllGerador(void);
      //void __fastcall RedrawAllRedeReduzida(void);
      //void __fastcall RedrawAllSuprimento(void);
      //void __fastcall RedrawAllTrafo(void);
      //void __fastcall RedrawAllTrafo3E(void);
      //void __fastcall RedrawAllTrecho(void);
      //void __fastcall RedrawPie(VTEqbar *eqbar, double cargto, TColor cor);
      //void __fastcall RedrawPie(VTLigacao *pliga, TColor cor);
      //void __fastcall RedrawReducaoEqptos(VTReducao *reducao);
      //void __fastcall RedrawReducaoEsquematico(VTReducao *reducao);

   private: //objetos externos
      VTRedeCC *redeCC;
   };

#endif
//---------------------------------------------------------------------------
//eof
