//---------------------------------------------------------------------------
#ifndef TUnifEditorH
#define TUnifEditorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnif.h"

//---------------------------------------------------------------------------
class TUnifEditor : public TUnif
   {
   public:
                   __fastcall TUnifEditor(VTApl *apl, int tipo_graf, AnsiString nome);
      virtual      __fastcall ~TUnifEditor(void);
      virtual void __fastcall Redraw(VTEdt *editor);
      virtual void __fastcall Redraw(int op, VTEdt *editor);

   protected: //métodos
      //TColor __fastcall CorLigacao(TBarra *barra);
      void __fastcall RedrawAllEqptoNovo(void);
      void __fastcall RedrawAllEqptoSelCorOriginal(void);
      void __fastcall RedrawAllEqptoSelCorUnica(void);
      void __fastcall RedrawAllMarca(void);
	  void __fastcall RedrawBarraCorOriginal(VTBarra *barra);
	  void __fastcall RedrawFiguraSelCorOriginal(void);
	  void __fastcall RedrawFiguraCorUnica(void);
      void __fastcall RedrawFiguraNova(void);
      void __fastcall RedrawLigacaoCorOriginal(VTLigacao *pliga);
      void __fastcall RedrawEditor(void);

   private: //dados
      struct   {
               TColor eqpto_novo;
               TColor eqpto_sel;
               TColor marca;
               } cor;
   };

#endif
//---------------------------------------------------------------------------
//eof
