//-----------------------------------------------------------------------------
#ifndef VTCronometroH
#define VTCronometroH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTCronometro  : public TObject
   {
   public:
                   __fastcall  VTCronometro(void) {};
      virtual      __fastcall ~VTCronometro(void) {};
      virtual int  __fastcall  NumAcionamentos(AnsiString codigo) = 0;
      virtual void __fastcall  Reinicia(void) = 0;
      virtual void __fastcall  Reset(void) = 0;
      virtual void __fastcall  Reset(AnsiString codigo) = 0;
      virtual void __fastcall  SaveToFile(AnsiString filename) = 0;
      virtual void __fastcall  Show(void) = 0;
      virtual void __fastcall  Start(void) = 0;
      virtual void __fastcall  Start(AnsiString codigo, AnsiString descricao="") = 0;
      virtual void __fastcall  Stop(void) = 0;
      virtual void __fastcall  Stop(AnsiString codigo) = 0;
      virtual int  __fastcall  TempoMSeg(AnsiString codigo) = 0;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTCronometro
//-----------------------------------------------------------------------------
VTCronometro* NewObjCronometro(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
