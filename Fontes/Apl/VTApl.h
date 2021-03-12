//---------------------------------------------------------------------------
#ifndef VTAplH
#define VTAplH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
class VTApl : public TObject
   {
   public:  //property
      __property VTApl* Owner  = {read=apl_owner};
      __property TForm* Form   = {read=form_owner};

   public:
                       __fastcall  VTApl(void) {};
      virtual          __fastcall ~VTApl(void) {};
      virtual void     __fastcall  Add(TObject *obj) = 0;
      //virtual TForm*   __fastcall  GetForm(void) = 0;
      virtual TObject* __fastcall  GetObject(AnsiString class_name) = 0;
      virtual TObject* __fastcall  GetObject(TMetaClass* meta_class) = 0;
      virtual void     __fastcall  Remove(TObject *obj) = 0;

   protected:  //dados acessados via property
      TForm *form_owner;
      VTApl *apl_owner;
   };

//---------------------------------------------------------------------------
//método global p/ criar objetos da classe VTApl
//---------------------------------------------------------------------------
VTApl* __fastcall NewObjApl(TForm *form_owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
