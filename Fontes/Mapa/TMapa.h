//---------------------------------------------------------------------------
#ifndef TMapaH
#define TMapaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMapa.h"

//---------------------------------------------------------------------------
class TMapa : public VTMapa
   {
   public:
                __fastcall  TMapa(VTApl *apl);
                __fastcall ~TMapa(void);
      VTRegiao* __fastcall  CriaRegiao(void);
      VTRegiao* __fastcall  ExisteRegiao(int regiao_id);
      VTRegiao* __fastcall  ExisteRegiao(AnsiString codigo);
      bool      __fastcall  ImportaRegiaoLatLon(AnsiString arq_latlon);
      bool      __fastcall  ImportaRegiaoUtm(AnsiString arq_utm);
      void      __fastcall  InsereRegiao(VTRegiao *regiao);
      TList*    __fastcall  LisRegiao(void);
      bool      __fastcall  ReiniciaLisEqptoRegiao(void);
      bool      __fastcall  ReiniciaLisEqptoRegiao(VTRede *rede);
      void      __fastcall  RetiraRegiao(VTRegiao *regiao);

   private: //métodos
      bool   __fastcall AreaRedeContemAreaRegiao(VTRegiao *regiao);
      void   __fastcall DefineRegiaoVisivel(void);
      void   __fastcall IniciaEqptosRegiao(void);
      TColor __fastcall SelecionaCor(void);

   private: //dados externos
      VTApl *apl;

   private: //dados locais
      TList *lisREGIAO;
   };

//---------------------------------------------------------------------------
#endif
//eof

