//---------------------------------------------------------------------------
#ifndef TGrupoH
#define TGrupoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTGrupo.h"

//---------------------------------------------------------------------------
class TGrupo : public VTGrupo
   {
   public:
             __fastcall TGrupo(void);
             __fastcall ~TGrupo(void);
      bool   __fastcall ExisteLigacao(VTLigacao *ligacao);
      bool   __fastcall ExisteMutua(VTMutua *mutua);
      int    __fastcall IndexOf(VTMutua *mutua);
      void   __fastcall InsereLigacao(VTLigacao *ligacao);
      void   __fastcall InsereMutua(VTMutua *mutua);
      TList* __fastcall LisLigacao(void);
      TList* __fastcall LisMutua(void);
      void   __fastcall RemoveMutua(VTMutua *mutua);
      void   __fastcall Reinicia(void);

   protected:  //métodos acessados via property
      VTArranjo* __fastcall PM_GetArranjo(void);
      void       __fastcall PM_SetArranjo(VTArranjo *arranjo);

   private: // dados acessados via property
      struct {
             VTArranjo *arranjo;
             }PD;

   private: // dados
      TList   *lisMUT;
      TList   *lisLIG;
   };

//---------------------------------------------------------------------------
#endif
//eof
