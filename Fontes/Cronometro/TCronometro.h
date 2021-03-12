//---------------------------------------------------------------------------
#ifndef TCronometroH
#define TCronometroH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
#include "VTCronometro.h"

//---------------------------------------------------------------------------
class TRelogio : public TObject
   {
   public:
                  __fastcall  TRelogio(AnsiString codigo, AnsiString descricao);
                  __fastcall ~TRelogio(void);
      int         __fastcall  NumAcionamentos(AnsiString codigo);
      void        __fastcall  Reset(void);
      void        __fastcall  Start(void);
      void        __fastcall  Stop(void);
      int         __fastcall  TempoMSeg(void);
      AnsiString  __fastcall  TempoStringMSeg(void);

   public:  //dados
      enum       eSTATUS {stRESET, stSTART, stSTOP};
      AnsiString codigo, descricao;
      int        acionamentos;
      int        status;
      int        mseg;
      TDateTime  t1, t2;
   };

//---------------------------------------------------------------------------
class TCronometro : public VTCronometro
   {
   public:
           __fastcall  TCronometro(VTApl *apl);
           __fastcall ~TCronometro(void);
      int  __fastcall  NumAcionamentos(AnsiString codigo);
      void __fastcall  Reinicia(void);
      void __fastcall  Reset(void);
      void __fastcall  Reset(AnsiString codigo);
      void __fastcall  SaveToFile(AnsiString filename);
      void __fastcall  Show(void);
      void __fastcall  Start(void);
      void __fastcall  Start(AnsiString codigo, AnsiString descricao);
      void __fastcall  Stop(void);
      void __fastcall  Stop(AnsiString codigo);
      int  __fastcall  TempoMSeg(AnsiString codigo);

   private: //métodos
      TRelogio* __fastcall ExisteRelogio(AnsiString codigo);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisREL;
   };

#endif
//---------------------------------------------------------------------------
//eof
