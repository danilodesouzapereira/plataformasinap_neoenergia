//---------------------------------------------------------------------------
#ifndef TReduzSecaoH
#define TReduzSecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
//class VTBarra;
class VTLigacao;
class VTDemanda;
class VTRadial;
class VTRedes;
class VTPrimario;

//---------------------------------------------------------------------------
class TReduzSecao : public TObject
   {
   public:  //métodos
                 __fastcall  TReduzSecao(VTApl *apl_owner);
                 __fastcall ~TReduzSecao(void);
      bool       __fastcall  Executa(void);

   private:  //métodos
      bool __fastcall Executa(VTPrimario *primario);
      void __fastcall IniciaLisBarraFixa(void);
      void __fastcall IniciaLisChaveFixa(void);
      bool __fastcall LigacaoConectaBarraFixa(VTLigacao *ligacao);
      void __fastcall MarcaBarraComoFixa_NaoFixa(void);
      void __fastcall SelecionaBarraChaveFixa(void);
      void __fastcall SelecionaBarraComBifurcacao(void);
      void __fastcall SelecionaBarraEqbar(int tipo_eqbar);
      void __fastcall SelecionaBarraLigacao(int tipo_ligacao);
      //void __fastcall SelecionaBarraLigacoesComFasesDistintas(void);
      void __fastcall SelecionaBarraTrechoConectaChave(void);

   private:  //objetos externos
      VTApl       *apl;
      VTDemanda   *demanda;
      VTRedes     *redes;

   private: //dados locais
      TList    *lisBAR_FIX;
      TList    *lisCHV_FIX;
      TList    *lisBAR_ORD;
      TList    *lisLIG_ORD;
      TList    *lisEQP;
      TList    *lisSECAO;
      VTRadial *radial;
   };

//---------------------------------------------------------------------------
TReduzSecao* __fastcall NewObjReduzSecao(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
