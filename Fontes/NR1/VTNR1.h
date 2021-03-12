//---------------------------------------------------------------------------
#ifndef VTNR1H
#define VTNR1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class  VTApl;
class  VTCalculo1;
class  VTData;
class  VTMonta1;
class  VTNet;
class  VTRede1;
class  VTSisLin;
struct Est1_G1_NR1;

//---------------------------------------------------------------------------
class VTNR1 : public TObject
   {
   public:  //property
      __property bool RelatorioBarDesc = {read=PD.relatorio_bar_desc, write=PD.relatorio_bar_desc};
      __property int  TipoRelatorio    = {read=PD.tipo_relatorio,     write=PD.tipo_relatorio};

   public:
                 __fastcall  VTNR1(void) {};
                 __fastcall ~VTNR1(void) {};
    virtual bool __fastcall  CalculaRede_EST1_PM1(int   np,
                                                 bool  flag_inicializa_Jacob,
                                                 TList *lisBAR_GER=NULL) = 0;
    virtual void __fastcall  CfgAvisoErro(bool enabled=true) = 0;
    virtual bool __fastcall  Executa(VTData *data) = 0;
    virtual bool __fastcall  InicializaNR1_EST1_PM1(Est1_G1_NR1 *str) = 0;
    virtual int  __fastcall  NumPatamaresOK(void) = 0;

   protected:  //dados acessados via property
      struct{
            bool relatorio_bar_desc;
            int  tipo_relatorio;
            }PD;
    };

//---------------------------------------------------------------------------
//método p/ criar objetos da classe VTNR1
//---------------------------------------------------------------------------
VTNR1* __fastcall NewObjNR1(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
