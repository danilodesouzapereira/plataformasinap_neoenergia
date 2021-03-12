//---------------------------------------------------------------------------
#ifndef TBalancosH
#define TBalancosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBalancos.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTFluxo;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class TBalancos : public VTBalancos
   {
   public:
                 __fastcall TBalancos(VTApl *apl);
                 __fastcall ~TBalancos(void);
      bool       __fastcall Executa(void);
      VTBalanco* __fastcall ExisteBalanco(VTRede *rede);
      TList*     __fastcall LisBalanco(void);
      void       __fastcall Reinicia(void);

   private: //métodos
      bool       __fastcall IniciaBalancoRede(void);
      void       __fastcall InsereBalanco(TList *lisBAL, VTBalanco *balanco);
      bool       __fastcall MontaBalancoRede(void);
      bool       __fastcall MontaFluxoEnergiaBalancoRede(void);
      void       __fastcall RedistribuiErroFluxoPorRede(void);
      void       __fastcall RedistribuiPerdasPorFora(void);
      void       __fastcall SomaEnergiaNaEntrada(VTRede *rede, VTBalanco *balanco, double perdas_outras);
      void       __fastcall SomaEnergiaNaSaida(VTBalanco *balanco, double perdas_outras);
      double     __fastcall TotalizaEnergiaDaEntrada(TList *lisREDE);


   private: //objetos externos
      VTApl    *apl;
      VTArvore *arvore;
      VTRedes  *redes;

   private: //dados locais
      VTFluxo *fluxo;
      TList   *lisTMP;
      TList   *lisBALANCO;
   };

#endif
//---------------------------------------------------------------------------
//eof

