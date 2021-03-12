//---------------------------------------------------------------------------
#ifndef TCurtoReducaoH
#define TCurtoReducaoH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "VTCurtoReducao.h"
#include "..\..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class VTCorte;
class VTLigacao;
class VTReducao;
class VTRedeCC;

//---------------------------------------------------------------------------
class TCurtoReducao : public VTCurtoReducao
   {
   public:
           __fastcall TCurtoReducao(VTApl *apl);
           __fastcall ~TCurtoReducao(void);
      bool __fastcall Executa(void);

   private: //métodos
      void     __fastcall Executa(VTReducao *reducao);
      void     __fastcall CalculaCorrenteNaLigacaoFicticia(VTLigacao *liga_fic, VTBarra *barra, TList *lisLIG);
      void     __fastcall MontaLisLigacao(VTBarra *pbarra, VTCorte *corte, TList *lisLIG);
      void     __fastcall MontaLisLigCC(TList *lisLIG, TList *lisLIG_CC);

   private: //objetos externos
      VTApl       *apl;
      VTRedes     *redes;
      VTRedeCC    *redeCC;

   private: //dados locais
      TList       *lisMR;
      TList       *lisEQP;
      TList       *lisLIG;
      TList       *lisLIG_CC;
   };

#endif
//---------------------------------------------------------------------------
//eof

 