//---------------------------------------------------------------------------
#ifndef TPreMontaH
#define TPreMontaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTPreMonta.h"

//---------------------------------------------------------------------------
class TDbar;
class TDlin;
class TDinj;
class TDgbt;
class VTLog;
class VTMonta;
class VTTrecho;

//---------------------------------------------------------------------------
class TPreMonta : public VTPreMonta
   {
   public:
           __fastcall TPreMonta(VTApl *apl);
           __fastcall ~TPreMonta(void);
      bool __fastcall InsereDBAR(TStringList *strList);
      bool __fastcall InsereDBSH(TStringList *strList);
      bool __fastcall InsereDGBA(TStringList *strList);
      bool __fastcall InsereDGBT(TStringList *strList);
      bool __fastcall InsereDINJ(TStringList *strList);
      bool __fastcall InsereDLIN(TStringList *strList);
      void __fastcall Reinicia(void);
      bool __fastcall Sucesso(void);
      bool __fastcall TrataFimDados(void);

   private: //métodos
      void     __fastcall ConverteCoordenadaDbar(void);
      double   __fastcall Converte_pu_ohm(double valor_pu, double Sbase, double Vbase);
      void     __fastcall DefineCoordenadaDbar(void);
      void     __fastcall DefineDistEqbar(void);
      bool     __fastcall DefineIadmTrecho(void);
      int      __fastcall DefineTiposup(TDbar *dbar);
      bool     __fastcall DefineVnomTrafo(void);
      TDbar*   __fastcall ExisteDbar(int numero);
      TDgbt*   __fastcall ExisteDgbt(AnsiString grupo);
      double   __fastcall LeValorPtoImplicito(AnsiString valor, int ncd);
      bool     __fastcall MontaBarra(void);
      bool     __fastcall MontaCapacitor(void);
      bool     __fastcall MontaCapacitorEquivalente(void);
      bool     __fastcall MontaCapacitorShunt(void);
      bool     __fastcall MontaCapserie(void);
      bool     __fastcall MontaCarga(void);
      void     __fastcall MontaChaves(TDlin *dlin, VTTrecho *trecho);
      bool     __fastcall MontaEqbar(void);
      bool     __fastcall MontaGerador(void);
      bool     __fastcall MontaLigacao(void);
      bool     __fastcall MontaReator(void);
      bool     __fastcall MontaReatorEquivalente(void);
      bool     __fastcall MontaRede(void);
      bool     __fastcall MontaRegulador(void);
      bool     __fastcall MontaTrafo(void);
      bool     __fastcall MontaTrecho(void);
      bool     __fastcall MontaSuprimentoEquivalente(void);
      bool     __fastcall PontoDecimalExplicito(AnsiString valor);
      void     __fastcall RedefineTensaoDgbt_1KV(void);
      bool     __fastcall RelacionaDbar_Dgbt(void);
      bool     __fastcall ValidaDgbt(void);
//      bool     __fastcall ValorInfinito(AnsiString valor);
      bool     __fastcall ValorInfinito(double valor);

   private: //objetos externos
      VTApl   *apl;
      VTLog   *plog;
      VTMonta *monta;

   private: //dados locais
      #define   DXY_BAR   50
      bool      sucesso;
      bool      coord_definida;
      int       rede_id;
      double    Sbase;
      TList     *lisDBAR_SEL;
      TList     *lisDBAR;
      TList     *lisDBSH;
      TList     *lisDINJ;
      TList     *lisDLIN;
      TList     *lisDGBT;
   };

#endif
//-----------------------------------------------------------------------------
// eof
