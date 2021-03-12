//-----------------------------------------------------------------------------
#ifndef VTLogH
#define VTLogH

//-----------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTLog  : public TObject
   {
   public:  //property
      __property bool ExibirDataHora = {read=PD.exibir_dh, write=PD.exibir_dh};

   public:
                         __fastcall  VTLog(void) {};
      virtual            __fastcall ~VTLog(void) {};
      virtual bool       __fastcall  Add(AnsiString msg) = 0;
      virtual AnsiString __fastcall  ArqLog(void) = 0;
      virtual TStrings*  __fastcall  Campos(void) = 0;
      virtual void       __fastcall  Close(void) = 0;
      virtual void       __fastcall  DefineValor(AnsiString campo, AnsiString valor) = 0;
      virtual void       __fastcall  InsereCampo(AnsiString campo, AnsiString valor=" ") = 0;
      virtual bool       __fastcall  Open(AnsiString arq_log="", bool append=false) = 0;
      virtual void       __fastcall  ShowModal(AnsiString Caption) = 0;

   protected:  //dados acessados via property
      struct   {
               bool exibir_dh;
               }PD;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTLog
//-----------------------------------------------------------------------------
VTLog* NewObjLog(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
