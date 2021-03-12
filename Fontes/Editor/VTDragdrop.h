//---------------------------------------------------------------------------
#ifndef VTDragdropH
#define VTDragdropH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEdt;
class VTArea;

//---------------------------------------------------------------------------
class VTDragdrop : public TObject
   {
   public:
              __fastcall VTDragdrop(void) {};
      virtual __fastcall ~VTDragdrop(void) {};
		virtual void __fastcall InsEdtCopia(VTEdt *edt) = 0;
      virtual void __fastcall InsEdtCola(VTEdt *edt) = 0;
      virtual void __fastcall RetEdtCola(VTEdt *edt) = 0;
      virtual void __fastcall RetEdtCopia(VTEdt *edt) = 0;

   protected:  //métodos acessados via property
   };

//---------------------------------------------------------------------------
//funções globais p/ criar objetos da classe VTDragdrop
//---------------------------------------------------------------------------
VTDragdrop* __fastcall NewObjDragdrop(void);

#endif
//---------------------------------------------------------------------------
//eof
