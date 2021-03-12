//---------------------------------------------------------------------------
#ifndef VTAtivoH
#define VTAtivoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
class VTEqpto;
class VTObra;
class VTRede;

//---------------------------------------------------------------------------
enum acaoATIVO  {acaoINSERE_ATIVO=1, acaoRETIRA_ATIVO};

/*
enum tipoATIVO  {ativoNAO_DEFINIDO=-1,
                 ativoREDE=1,
                 ativoTRAFO,
                 ativoTRECHO_REDE,
                 ativoCHAVE_DISJUNTOR,
                 ativoCHAVE_RELIGADORA,
                 ativoREGULADOR,
                 ativoBCO_CAPACITOR,
                 ativoBCO_REATOR,
                };
*/
enum tipoATIVO {
               ativoNAO_DEFINIDO = -1,
               ativoBARRA        =  1,
               ativoCAPACITOR,
               ativoCAPSERIE,
               ativoCARGA,
               ativoCHAVE,
               ativoCHAVE_DISJUNTOR,
               ativoCHAVE_RELIGADORA,
               ativoGERADOR,
               ativoFILTRO,
               ativoREATOR,
               ativoREDE,
               ativoREDE_SDBT,
               ativoREDE_SDMT,
               ativoREDE_SED,
               ativoREGULADOR,
               ativoSUPRIMENTO,
               ativoTRAFO,
               ativoTRAFO3E,
               ativoTRAFOZZ,
               ativoTRECHO_REDE
               };

//---------------------------------------------------------------------------
class VTAtivo : public TObject
   {
   public:  //property
      __property bool       Enabled        = {read=PD.enabled      ,    write=PD.enabled     };
      __property int        TipoRede       = {read=PD.tipo_rede    ,    write=PD.tipo_rede   };
      __property int        TipoAcao       = {read=PD.tipo_acao    ,    write=PD.tipo_acao   };
      __property int        TipoAtivo      = {read=PD.tipo_ativo   ,    write=PD.tipo_ativo  };
      __property int        Ano            = {read=PD.ano          ,    write=PD.ano         };
      __property VTObra*    Obra           = {read=PD.obra         ,    write=PD.obra        };
      __property VTEqpto*   Eqpto          = {read=PD.eqpto        ,    write=PD.eqpto       };
      __property VTRede*    RedeDoEqpto    = {read=PD.rede         ,    write=PD.rede        };
      __property bool       Padrao         = {read=PD.padrao       ,    write=PD.padrao      };
      __property AnsiString bdId           = {read=PD.bdId         ,    write=PD.bdId        };
      __property AnsiString bdDescr        = {read=PD.bdDescr      ,    write=PD.bdDescr     };
      //somente p/ ativoTRAFO e ativoETD
      __property double     IA             = {read=PD.ia           ,    write=PD.ia          };
      //somente p/ ativoTRECHO_REDE
      __property double     Comprimento_km = {read=PD.comp_km      ,    write=PD.comp_km     };

   public:
                        __fastcall  VTAtivo(void) {};
     virtual            __fastcall ~VTAtivo(void) {};
     virtual AnsiString __fastcall TipoAcaoAsString(void) = 0;
     virtual AnsiString __fastcall TipoAtivoAsString(void) = 0;

   private:  //métodos

   protected:  //dados acessados via property
      struct   {
               bool       enabled;
               int        tipo_rede;
               int        tipo_acao;
               int        tipo_ativo;
               int        ano;
               VTObra     *obra;
               VTEqpto    *eqpto;
               VTRede     *rede; //Rede do Eqpto
               bool       padrao;
               AnsiString bdId;
               AnsiString bdDescr;
               double     ia;
               double     comp_km;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTAtivo* __fastcall NewObjAtivo(void);

#endif
//---------------------------------------------------------------------------
//eof
