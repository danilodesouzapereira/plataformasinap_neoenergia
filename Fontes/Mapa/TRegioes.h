//---------------------------------------------------------------------------
#ifndef TRegioesH
#define TRegioesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTRegioes.h"

//---------------------------------------------------------------------------
class TRegioes : public VTRegioes
   {
   public:
                __fastcall TRegioes(VTApl *apl);
                __fastcall ~TRegioes(void);
      void      __fastcall  Clear(void);
      VTRegiao* __fastcall  CriaRegiao(void);
      VTRegiao* __fastcall  ExisteRegiao(int regiao_id, bool somenteVisible = false);
      VTRegiao* __fastcall  ExisteRegiao(AnsiString codigo, bool somenteVisible = false);
      VTRegiao* __fastcall  ExisteRegiao(TPoint *point_cm, bool somenteVisible = false);
      void      __fastcall  ExisteRegioes(TList *lisREGIAO_vazia, TPoint *point_cm, bool somenteVisible = false);
      bool      __fastcall  ImportaRegiaoLatLon(AnsiString arq_latlon);
      bool      __fastcall  ImportaRegiaoUtm(AnsiString arq_utm);
      void      __fastcall  InsereRegiao(VTRegiao *regiao);
      TList*    __fastcall  LisRegiao(void);
      void      __fastcall  OrdenaArvore(TList *lisRegiao);
      //TList*    __fastcall  OrdenaArvoreRegiao(TList *lisRegiao);

      bool      __fastcall  ReiniciaLisEqptoRegiao(void);
      bool      __fastcall  ReiniciaLisEqptoRegiao(VTRede *rede);
      void      __fastcall  RegioesVisible(TList *lisRegiaoParcial, bool visible);
      void      __fastcall  RegioesVisibleComplementar(TList *lisRegiaoParcial,
                                                                  bool visible);
      void      __fastcall  RetiraRegiao(VTRegiao *regiao);

   private: //métodos
      bool   __fastcall AreaRedeContemAreaRegiao(VTRegiao *regiao);
      void   __fastcall DefineRegiaoVisivel(void);
      void   __fastcall IniciaEqptosRegiao(void);
      void   __fastcall InsereRegiaoEFilhas(VTRegiao *regiao);
      bool   __fastcall IsRegiao1InternaRegiao2(VTRegiao *regiao1, VTRegiao *regiao2);
      void   __fastcall OrdenaArvoreRegiao(VTRegiao *regiaoA, TList *lisRegiao);
      void   __fastcall OrdenaLisRegiao(void);
      //void   __fastcall ListaRegioesLevelZero(TList *lisVAZIA, TList *lisTODAS);
      //int    __fastcall ListaRegiaoContem(VTRegiao *regiaoA, TList *lisTODAS, TList *lisCONTEM_A);
      int    __fastcall ListaRegiaoContidas(VTRegiao *regiaoA, TList *lisTODAS, TList *lisCONTIDAS);
      //void   __fastcall OrganizaRegiaoInterna(TList *lisRegiao);
      TColor __fastcall SelecionaCor(void);

   private: //dados externos
      VTApl *apl;

   private: //dados locais
      TList *lisREGIAO;
      TList *lisREGIAO_ORD;
      TList *lisAUX;
   };

//---------------------------------------------------------------------------
#endif
//eof
