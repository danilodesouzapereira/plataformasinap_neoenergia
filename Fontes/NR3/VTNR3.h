//---------------------------------------------------------------------------
#ifndef VTNR3H
#define VTNR3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>

class VTApl;
class VTData;

//---------------------------------------------------------------------------
class VTNR3 : public TObject
   {
   public:  //property
      __property bool RelatorioBarDesc = {read=PD.relatorio_bar_desc, write=PD.relatorio_bar_desc};
      __property int  TipoRelatorio    = {read=PD.tipo_relatorio,     write=PD.tipo_relatorio};

   public:
                   __fastcall  VTNR3() {};
      virtual      __fastcall ~VTNR3(void) {};
      virtual void __fastcall  CfgAvisoErro(bool enabled) = 0;
      virtual bool __fastcall  Executa(VTData *data) = 0;
      virtual int  __fastcall  NumPatamaresOK(void) = 0;

   protected:  //dados acessados via property
      struct{
            bool relatorio_bar_desc;
            int  tipo_relatorio;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTNR3
//---------------------------------------------------------------------------
VTNR3* __fastcall NewObjNR3(VTApl *apl);

#endif
//---------------------------------------------------------------------------

