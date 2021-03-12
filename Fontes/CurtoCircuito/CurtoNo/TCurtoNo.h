//---------------------------------------------------------------------------
#ifndef TCurtoNoH
#define TCurtoNoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCurtoNo.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTNo;
class VTRedeCC;

//---------------------------------------------------------------------------
class TCurtoNo : public VTCurtoNo
   {
   public:
           __fastcall TCurtoNo(VTApl *apl);
           __fastcall ~TCurtoNo(void);
       bool __fastcall Executa(VTNet *net);

   private: //métodos
      void __fastcall Executa(VTNo *pno);
      void __fastcall Executa(VTBarra *pbarra, VTLigacao *ligacao, TList *lisLIG);
      bool __fastcall ExisteBarraComApenasUmaLigacao(VTBarra **pbarra, VTLigacao **ligacao);
      void __fastcall SelecionaLigacoesBarra(VTBarra *pbarra, TList *lisORIG, TList *lisDEST);
      //void __fastcall TensoesBarras(VTNo *pno);

   private: //objetos externos
      VTApl    *apl;
      VTNet    *net;
      VTRedeCC *redeCC;

   private: //dados
      TList    *lisBAR_NO;    //lista de Barras de um No
      TList    *lisLIG_NO;    //lista de Barras de um No
      TList    *lisTMP;    //lista p/ uso temporário dentro de uma função
   };

#endif
//---------------------------------------------------------------------------
//eof

 