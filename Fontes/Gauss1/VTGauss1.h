//---------------------------------------------------------------------------
#ifndef VTGauss1H
#define VTGauss1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class  VTApl;
class  VTCalculo1;
class  VTData;
class  VTMonta1;
class  VTRede1;
class  VTSisLin;
struct Est1_G1_NR1;
struct ResumoProc;

//---------------------------------------------------------------------------
class VTGauss1 : public TObject
   {
   public:  //property
      __property bool RelatorioBarDesc = {read=PD.relatorio_bar_desc, write=PD.relatorio_bar_desc};
      __property int  TipoRelatorio    = {read=PD.tipo_relatorio,     write=PD.tipo_relatorio};

   public:
                   __fastcall  VTGauss1() {};
      virtual      __fastcall ~VTGauss1(void) {};
      virtual bool __fastcall  CalculaRede_EST1_PM1(int      np,
                                                    VTSisLin **Ynod2,
                                                    TList    *lisBAR_GER=NULL) = 0;
      virtual bool __fastcall  Executa(VTData *data) = 0;
      virtual bool __fastcall  InicializaGauss1_EST1_PM1(Est1_G1_NR1 *str)= 0;
      virtual int  __fastcall  NumPatamaresOK(void) = 0;

   protected:  //dados acessados via property
      struct{
            bool relatorio_bar_desc;
            int  tipo_relatorio;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTGauss1
//---------------------------------------------------------------------------
VTGauss1* __fastcall NewObjGauss1(VTApl *apl);


#endif
//---------------------------------------------------------------------------

