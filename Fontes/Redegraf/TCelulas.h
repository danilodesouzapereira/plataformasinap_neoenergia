//---------------------------------------------------------------------------
#ifndef TCelulasH
#define TCelulasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCelulas.h"

//---------------------------------------------------------------------------
//class VTRedegraf;

//---------------------------------------------------------------------------
class TCelulas : public VTCelulas
   {
   public:
                __fastcall  TCelulas(VTApl *apl);
                __fastcall ~TCelulas(void);
      void      __fastcall  DefineAreaTotal(void);
      void      __fastcall  DefineAreaAtiva(VTArea *area);
      void      __fastcall  DefineGrid(int num_lin, int num_col);
      void      __fastcall  DefineGridQuadrado(int dim_cel_m);
      int       __fastcall  Distancia(VTCelula *celula1, VTCelula *celula2);
      VTCelula* __fastcall  ExisteCelula(int x, int y);
      bool      __fastcall  IndicesDaCelula(VTCelula *celula, int &linha, int &coluna);
      TList*    __fastcall  LisCelula(void);
      TList*    __fastcall  LisCelulaVisivel(void);
      void      __fastcall  LisCelulaVizinha(TList *lisEXT, VTCelula *celula, int delta);
      void      __fastcall  LisEqptoArea(TList *lisEXT, VTArea *area, int tipo);
      void      __fastcall  LisEqptoRedeAreaAtiva(TList *lisEXT, VTRede *rede, int tipo);
      void      __fastcall  LisEqptoReducaoAreaAtiva(TList *lisEXT, VTReducao *reducao, int tipo);
      void      __fastcall  LisEqptoVisivelArea(TList *lisEXT, VTArea *area, int tipo);
      void      __fastcall  LisRedeArea(TList *lisEXT, VTArea *area);
      void      __fastcall  LisReducaoArea(TList *lisEXT, VTArea *area);
      int       __fastcall  QtdeEqptoRedeAreaAtiva(VTRede *rede, int tipo);

   protected:  //métodos acessados via property
      int     __fastcall PM_GetNumColuna(void);
      int     __fastcall PM_GetNumLinha(void);
      int     __fastcall PM_GetNumCelula(void);
      void    __fastcall PM_SetNumCelula(int num_celula);

   private: //métodos
      void __fastcall InsereBarrasRede(void);
      void __fastcall InsereBarrasReducao(void);
      void __fastcall InsereEqbarRede(void);
      void __fastcall InsereEqbarReducao(void);
      void __fastcall InsereLigacoesRede(void);
      void __fastcall InsereLigacoesReducao(void);
      void __fastcall InsereLigacaoFicticiaReducao(void);
      void __fastcall InsereMutuasRede(void);
      void __fastcall ReiniciaCelulas(int num_lin=-1, int num_col=-1, int dim_cel_m=-1);
      void __fastcall SelecionaCelula(int x1,int y1, int x2, int y2, TList *lisCEL, bool vizinhas=false);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisCELULA;
      TList *lisATIVA;
      TList *lisEQP;
      TList *lisTMP;
      struct   {
               int    num_lin, num_col;
               int    x1, y1, x2, y2;
               int    dx, dy;
               }grid;
   };

//---------------------------------------------------------------------------
#endif
//eof
