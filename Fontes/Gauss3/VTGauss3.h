//---------------------------------------------------------------------------
#ifndef VTGauss3H
#define VTGauss3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>

class  VTApl;
class  VTAtualiza3;
class  VTBarra;
class  VTCalculo3;
class  VTData;
class  VTImpressao3;
class  VTMonta3;
class  VTNet;
class  VTRede3;
class  VTRedeCC;
struct FluxoH3_G3;

//---------------------------------------------------------------------------
struct strSinapCurtoMA  // Passagem de dados entre Sinap e Gauss3 (CurtoMA)
   {
   VTBarra *barra_sinap; // Objeto Sinap
   int     fases_sinap;  // Fases do defeito (codificação Sinap: faseAN, faseBN, ...)
   };

//---------------------------------------------------------------------------
class VTGauss3 : public TObject
   {
   public:  //property
      __property bool ModelaBarraPV    = {read=PD.barraPV,            write=PD.barraPV};
      __property bool RelatorioBarDesc = {read=PD.relatorio_bar_desc, write=PD.relatorio_bar_desc};
      __property int  TipoRelatorio    = {read=PD.tipo_relatorio,     write=PD.tipo_relatorio};

   public:
                   __fastcall  VTGauss3() {};
      virtual      __fastcall ~VTGauss3(void) {};
      virtual bool __fastcall  CalculaRedeFluxoH3(int np) = 0;
      virtual bool __fastcall  ExecutaCurtoMA(int      ind_pat,
                                              VTData   *data,
                                              VTBarra  *barra_sinap,
                                              VTNet    *net,
                                              VTRedeCC *redeCC,
                                              TList    *lisBAR_DEF_SINAP=NULL) = 0;
      virtual bool __fastcall  ExecutaFluxo(VTData *data) = 0;
      virtual bool __fastcall  InicializaGauss3_FluxoH3(FluxoH3_G3 *str) = 0;
      virtual int  __fastcall  NumPatamaresOK(void) = 0;

   protected:  //dados acessados via property
      struct{
            bool barraPV;
            bool relatorio_bar_desc;
            int  tipo_relatorio;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTGauss3
//---------------------------------------------------------------------------
VTGauss3* __fastcall NewObjGauss3(VTApl *apl);

#endif
//---------------------------------------------------------------------------

