//---------------------------------------------------------------------------
#ifndef TCaminhoH
#define TCaminhoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCaminho.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;

//---------------------------------------------------------------------------
class TRamo : public TObject
   {
   public:
      VTBarra    *bar_ini;
      VTBarra    *bar_fim;
      VTLigacao  *ligacao;
      TRamo      *pai;
   };


//---------------------------------------------------------------------------
class TCaminho : public VTCaminho
   {
   public:
           __fastcall  TCaminho(VTApl *apl);
           __fastcall ~TCaminho(void);
      void __fastcall  Executa(VTEqpto *eqpto1, VTEqpto *eqpto2, TList *lisLIG_CAM);
      void __fastcall  Executa(TList *lisEQP_INI, TList *lisEQP_FIM, TList *lisLIG_ALL, TList *lisLIG_CAM);

   private: //métodos
      VTBarra* __fastcall BarraDoEqpto(VTEqpto *eqpto);
      bool     __fastcall ChaveAberta(VTLigacao *ligacao);
      TRamo*   __fastcall ExisteRamo(VTLigacao *ligacao_pai, VTLigacao *ligacao_filha);
      TRamo*   __fastcall ExisteRamo(VTLigacao *ligacao, VTBarra *bar_ini, VTBarra *bar_fim);
      TRamo*   __fastcall ExisteRamoBarraFinal(VTBarra *barra);
      TRamo*   __fastcall ExisteRamoBarraFinal(TList *lisBAR_FIM);
      void     __fastcall InsereLigacao(VTLigacao *ligacao, TList *lisEXT);
      void     __fastcall InsereRamo(VTBarra *bar_ini, VTBarra *bar_fim,
                                     VTLigacao *ligacao, TRamo *pai);
      void     __fastcall MontaLigacoesInterligadas(TList *lisEXT);
      void     __fastcall MontaRamos(VTBarra *barra);
      void     __fastcall MontaRamosFilhos(TRamo *ramo_pai, VTBarra *bar_ini);
      void     __fastcall SelecionaRamosInterligados(VTBarra *barra1, VTBarra *barra2);
      bool     __fastcall ExisteCaminhoBarraIniBarraFim(void);

   private: //dados externos
      VTApl  *apl;

   private: //dados locais
      TList *lisBAR_INI;    //lista de Barras
      TList *lisBAR_FIM;    //lista de Barras
      TList *lisLIG;        //lista de Ligacoes
      TList *lisRAMO;       //lista de objetos TRamo
      TList *lisSEL;        //lista de objetos TRamo selecionados
   };

//---------------------------------------------------------------------------
#endif
//eof

