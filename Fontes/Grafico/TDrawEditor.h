//---------------------------------------------------------------------------
#ifndef TDrawEditorH
#define TDrawEditorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TDraw.h"

//---------------------------------------------------------------------------
class VTLigacao;
class VTMutua;

//---------------------------------------------------------------------------
class TDrawEditor : public TDraw
   {
   public:
           __fastcall TDrawEditor(VTApl *apl);
           __fastcall ~TDrawEditor(void);
      void __fastcall EqptoNovo(VTEqpto *eqpto);
	  void __fastcall FigNova(VTFigura *figura);
      void __fastcall Marca(VTMarca *marca);

   private:        
      void __fastcall EqptoNovo(VTBarra *pbar);
      void __fastcall EqptoNovo(VTChave *pchave);
      void __fastcall EqptoNovo(VTEqbar *eqbar);
      void __fastcall EqptoNovo(VTLigacao *pliga);
      void __fastcall EqptoNovo(VTMutua *mutua);
      void __fastcall EqptoNovo(VTRede *rede);
      void __fastcall EqptoNovo(VTReducao *reducao);
      void __fastcall EqptoNovo(VTTrecho *ptrecho);
      void __fastcall FigComentarioNovo(VTFiguraComentario *comentario);
      void __fastcall FigRetanguloNovo(VTRetangulo *retangulo);

   };

//---------------------------------------------------------------------------
#endif
//eof
