//-----------------------------------------------------------------------------
#ifndef VTAlertaH
#define VTAlertaH

//-----------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;
//-----------------------------------------------------------------------------
class VTAlerta  : public TObject
   {
   public:  //property
      __property int Length = {read=PD.length, write=PD.length};

   public:
                   __fastcall  VTAlerta(void) {};
      virtual      __fastcall ~VTAlerta(void) {};
	  virtual int  __fastcall  Add(AnsiString msg) = 0;
	  virtual int  __fastcall  AddMsgMalha(VTRede *rede, AnsiString msg) = 0;
		virtual void __fastcall  Clear(bool completo = false) = 0;
	  virtual AnsiString __fastcall ExisteAvisoMalha(VTRede *rede)=0;
		virtual int  __fastcall  NumeroDeMensagens(void) = 0;
      virtual void __fastcall  ReexibeAlertas(bool somenteMalhas = false) = 0;
      virtual void __fastcall  SaveToFile(AnsiString filename) = 0;
      virtual void __fastcall  ShowModal(void) = 0;

   protected:
      struct{
            int length;
            }PD;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTAlerta
//-----------------------------------------------------------------------------
VTAlerta* NewObjAlerta(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
