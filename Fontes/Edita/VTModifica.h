//---------------------------------------------------------------------------
#ifndef VTModificaH
#define VTModificaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTObra;

//---------------------------------------------------------------------------
class VTModifica : public TObject
   {
   public:
                   __fastcall VTModifica(void) {};
                   __fastcall ~VTModifica(void) {};
      virtual void __fastcall DefineActionExterna(TList *lisACTION) = 0;
      virtual bool __fastcall Do(VTObra *obra, VTAcao *acao) = 0;
      virtual bool __fastcall Redo(VTObra *obra, VTAcao *acao) = 0;
	  virtual bool __fastcall Redo(TList *lisREDE, TList *lisACAO) = 0;
      virtual bool __fastcall Undo(VTObra *obra, VTAcao *acao) = 0;
      virtual bool __fastcall Undo(TList *lisREDE, TList *lisACAO) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTModifica
//---------------------------------------------------------------------------
VTModifica* __fastcall NewObjModifica(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 