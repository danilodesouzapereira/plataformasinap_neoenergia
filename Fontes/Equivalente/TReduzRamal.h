//---------------------------------------------------------------------------
#ifndef TReduzRamalH
#define TReduzRamalH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTLigacao;
class VTClasses;
class VTDemanda;
class VTFlow;
//class VTGrafico;
class VTRadial;
class VTRedes;
class VTTopologia;
class VTPrimario;

//---------------------------------------------------------------------------
class TReduzRamal : public TObject
   {
   public:  //métodos
                 __fastcall  TReduzRamal(VTApl *apl_owner);
                 __fastcall ~TReduzRamal(void);
      bool       __fastcall  Executa(void);

   private:  //métodos
      bool __fastcall Executa(VTPrimario *primario);
      bool __fastcall IniciaLisCapacitor(void);
      bool __fastcall IniciaLisChaveWise(void);
      bool __fastcall InsereCargaEquivalente(VTBarra *barra, VTLigacao *ligacao);
      bool __fastcall MarcaLigacoesInOut(TList *lisLIGACAO, TList *lisCHAVE);
      bool __fastcall RemoveLigacoesNaoSelecionadas(VTPrimario *primario);

   private:  //objetos externos
      VTApl       *apl;
      VTClasses   *classes;
      VTDemanda   *demanda;
      VTFlow      *flow;
      //VTGrafico   *grafico;
      VTRedes     *redes;
      VTTopologia *topologia;

   private: //dados locais
      TList    *lisBAR_ORD;
      TList    *lisLIG_ORD;
      TList    *lisCHV_WISE;
      TList    *lisCAP;
      TList    *lisEQP;
      VTRadial *radial;
   };

//---------------------------------------------------------------------------
TReduzRamal* __fastcall NewObjReduzRamal(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
