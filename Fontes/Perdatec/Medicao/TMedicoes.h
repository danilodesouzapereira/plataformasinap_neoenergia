//---------------------------------------------------------------------------
#ifndef TMedicoesH
#define TMedicoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMedicoes.h"

//---------------------------------------------------------------------------
class TMedicoes : public VTMedicoes
   {
   public:
                __fastcall  TMedicoes(VTApl *apl_owner);
                __fastcall ~TMedicoes(void);
      void      __fastcall  Clear(void);
      TMedicao* __fastcall  ExisteMedicao(VTRede *rede);
      bool      __fastcall  ImportaMedicao(TStrings *files);
      TMedicao* __fastcall  InsereMedicao(VTRede *rede);
      TList*    __fastcall  LisMedicao(void);
      TList*    __fastcall  OrdenaLisMedicao(void);
      void      __fastcall  RetiraMedicao(TMedicao *medicao);
   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList    *lisMEDICAO;
      TStrings *lines;
    };

//---------------------------------------------------------------------------
#endif
//eof

