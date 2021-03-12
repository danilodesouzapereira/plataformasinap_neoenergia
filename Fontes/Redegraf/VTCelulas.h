//---------------------------------------------------------------------------
#ifndef VTCelulasH
#define VTCelulasH
     
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTRede;
class VTReducao;
class VTCelula;

//---------------------------------------------------------------------------
class VTCelulas : public TObject
   {
//=====
//ATENÇÃO: este property será desabilitado em breve (mantido por compatibilidade)
   public:  //property
      __property int    NumCelula = {read=PM_GetNumCelula, write=PM_SetNumCelula };
//=====
   public:  //property
      __property int    NumLinha     = {read=PM_GetNumLinha };
      __property int    NumColuna    = {read=PM_GetNumColuna};
      __property bool   EqptoEnabled = {read=PD.eqpto_enabled, write=PD.eqpto_enabled};

   public:
                        __fastcall  VTCelulas(void) {};
      virtual           __fastcall ~VTCelulas(void) {};
      virtual void      __fastcall  DefineAreaTotal(void) = 0;
      virtual void      __fastcall  DefineAreaAtiva(VTArea *area) = 0;
      virtual void      __fastcall  DefineGrid(int num_lin, int num_col) = 0;
      virtual void      __fastcall  DefineGridQuadrado(int dim_cel_m) = 0;
      virtual int       __fastcall  Distancia(VTCelula *celula1, VTCelula *celula2) = 0;
      virtual VTCelula* __fastcall  ExisteCelula(int x, int y) = 0;
      virtual bool      __fastcall  IndicesDaCelula(VTCelula *celula, int &linha, int &coluna) = 0;
      virtual TList*    __fastcall  LisCelula(void) = 0;
      virtual TList*    __fastcall  LisCelulaVisivel(void) = 0;
      virtual void      __fastcall  LisCelulaVizinha(TList *lisEXT, VTCelula *celula, int delta) = 0;
      virtual void      __fastcall  LisEqptoArea(TList *lisEXT, VTArea *area, int tipo=eqptoINDEF) = 0;
      virtual void      __fastcall  LisEqptoRedeAreaAtiva(TList *lisEXT, VTRede *rede=NULL, int tipo=eqptoINDEF) = 0;
      virtual void      __fastcall  LisEqptoReducaoAreaAtiva(TList *lisEXT, VTReducao *reducao=NULL, int tipo=eqptoINDEF) = 0;
      virtual void      __fastcall  LisEqptoVisivelArea(TList *lisEXT, VTArea *area, int tipo=eqptoINDEF) = 0;
      virtual void      __fastcall  LisRedeArea(TList *lisEXT, VTArea *area) = 0;
      virtual void      __fastcall  LisReducaoArea(TList *lisEXT, VTArea *area) = 0;
      virtual int       __fastcall  QtdeEqptoRedeAreaAtiva(VTRede *rede, int tipo=eqptoINDEF) = 0;

   protected:  //métodos acessados via property
      virtual int     __fastcall PM_GetNumColuna(void) = 0;
      virtual int     __fastcall PM_GetNumLinha(void) = 0;
      virtual int     __fastcall PM_GetNumCelula(void) = 0;
      virtual void    __fastcall PM_SetNumCelula(int num_celula) = 0;

   protected:  //dados acessados via property
      struct   {
               bool eqpto_enabled;
               }PD;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objetos da classe VTCelulas
//-----------------------------------------------------------------------------
VTCelulas* NewObjCelulas(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
