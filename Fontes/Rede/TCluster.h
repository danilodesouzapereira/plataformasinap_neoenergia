//---------------------------------------------------------------------------
#ifndef TClusterH
#define TClusterH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCluster.h"

//---------------------------------------------------------------------------
class TCluster : public VTCluster
   {
   public:
                 __fastcall TCluster(void);
                 __fastcall ~TCluster(void);
      VTCluster* __fastcall Clone(void);
      void       __fastcall CopiaAtributosDe(VTEqpto &ref);
      bool       __fastcall ExisteRede(VTRede *rede);
      void       __fastcall InsereRede(VTRede *rede);
      TList*     __fastcall LisRede(void);
      void       __fastcall LisRede(TList *lisEQP);
      void       __fastcall Reinicia(void);
      void       __fastcall RemoveRede(VTRede *rede);
      int        __fastcall NumeroRedes(void);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected:  //métodos acessados via property
      bool       __fastcall PM_GetCarregado(void);

   private: //locais
      TList *lisREDE;   //lista de objetos VTRede
   };

//---------------------------------------------------------------------------
#endif
//eof

