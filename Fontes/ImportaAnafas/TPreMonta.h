//---------------------------------------------------------------------------
#ifndef TPreMonta_H
#define TPreMonta_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTPreMonta.h"

//---------------------------------------------------------------------------
class TDbar;
class TDcir;
class TDmut;
class TDtrf;
class VTLog;
class VTMonta;

//---------------------------------------------------------------------------
class TPreMonta : public VTPreMonta
   {
   public:
           __fastcall TPreMonta(VTApl *apl);
           __fastcall ~TPreMonta(void);
      bool __fastcall InsereDBAR(int num_seq, TStringList *strList);
      bool __fastcall InsereDCIR(int num_seq, TStringList *strList);
      bool __fastcall InsereDMUT(int num_seq, TStringList *strList);
      int  __fastcall NumeroErros(void);
      void __fastcall Reinicia(void);
      bool __fastcall Sucesso(void);
      bool __fastcall TrataFimDados(void);

   private: //métodos
      double   __fastcall Converte_pu_ohm(double valor_pu, double Sbase, double Vbase);
      void     __fastcall DefineCoordenadaDbar(void);
      void     __fastcall DefineDistEqbar(void);
      void     __fastcall DefinePrimario(TDtrf *dtrf, TDbar *dbar, int ligacao);
      void     __fastcall DefineSecundario(TDtrf *dtrf, TDbar *dbar, int ligacao);
      TDbar*   __fastcall ExisteDbar(int NB);
      TDcir*   __fastcall ExisteDcir(int BF, int BT, int NC);
      int      __fastcall ExisteDcir(TDbar *dbar1, TDbar *dbar2, TList *lisSEL);
      TDtrf*   __fastcall ExisteDtrf(TDbar *dbar_md, TDbar *dbar1, TDbar *dbar2);
      bool     __fastcall InsereDbarRef(void);
      bool     __fastcall InsereDtrf(TDcir *dcir);
      double   __fastcall LeValorNaoInfinito(AnsiString valor, int ncd);
      double   __fastcall LeValorPtoImplicito(AnsiString valor, int ncd);
      bool     __fastcall Monta(void);
      bool     __fastcall MontaBarra(TDbar *dbar);
      bool     __fastcall MontaCapacitor(TDcir *dcir, TDbar *dbar);
      bool     __fastcall MontaCapserie(TDcir *dcir);
      bool     __fastcall MontaCarga(TDcir *dcir, TDbar *dbar);
      bool     __fastcall MontaEqpto(TDcir *dcir);
      bool     __fastcall MontaGerador(TDcir *dcir, TDbar *dbar);
      bool     __fastcall MontaMutua(TDmut *dmut);
      bool     __fastcall MontaReator(TDcir *dcir, TDbar *dbar);
      bool     __fastcall MontaRede(void);
      bool     __fastcall MontaRegulador(TDcir *dcir);
      bool     __fastcall MontaTrafo(TDtrf *dtrf);
      bool     __fastcall MontaTrecho(TDcir *dcir);
      bool     __fastcall PontoDecimalExplicito(AnsiString valor);
      bool     __fastcall ValorInfinito(AnsiString valor);
      bool     __fastcall ValorZero(double valor);
      double   __fastcall VbaseDbar(TStringList *strList);
      bool     __fastcall Zinfinito(AnsiString r, AnsiString x);

   private: //objetos externos
      VTApl   *apl;
      VTLog   *plog;
      VTMonta *monta;

   private: //dados locais
      bool      sucesso;
      bool      coord_definida;
      int       num_erro;
      int       rede_id;
      double    Sbase;
      double    fator_perc_pu;
      TList     *lisDBAR;
      TList     *lisDCIR;
      TList     *lisDMUT;
      TList     *lisDTRF;
      TList     *lisTMP;
   };

#endif
//-----------------------------------------------------------------------------
// eof
