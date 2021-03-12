//---------------------------------------------------------------------------
#ifndef TEnergiaPassantesH
#define TEnergiaPassantesH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEnergiaPassantes.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTBarra;
class VTFluxo;
class VTRede;
class VTRedes;
class VTTrafo;
//---------------------------------------------------------------------------
class TEnergiaPassantes : public VTEnergiaPassantes
   {
   public:
				 __fastcall TEnergiaPassantes(VTApl *apl);
				 __fastcall ~TEnergiaPassantes(void);
	  bool       __fastcall Executa(void);
	  VTEnergiaPassante* __fastcall ExisteEnergiaPassante(VTRede *rede);
	  TList*     __fastcall LisEnergiaPassante(void);
	  void       __fastcall Reinicia(void);

   private: //métodos
	  int        __fastcall CategoriaTrafo(VTTrafo *trafo);
	  int        __fastcall DeterminaCategoriaPT(int catTRAFO);
	  bool       __fastcall IniciaEnergiaPassanteRede(void);
	  void       __fastcall InsereEnergiaPassante(TList *lisENEP, VTEnergiaPassante *energiaPassante);
	  int        __fastcall InverteCategoriaPT(int enumTCATPT);
	  bool       __fastcall MontaEnergiaPassanteRede(void);
	  bool       __fastcall MontaFluxoEnergiaEnergiaPassanteRede(void);
	  int        __fastcall NivelTensao(VTBarra *barra);
//	  void       __fastcall RedistribuiErroFluxoPorRede(void);
//	  void       __fastcall RedistribuiPerdasPorFora(void);
//	  void       __fastcall SomaEnergiaNaEntrada(VTRede *rede, VTEnergiaPassante *energiaPassante, double perdas_outras);
//	  void       __fastcall SomaEnergiaNaSaida(VTEnergiaPassante *energiaPassante, double perdas_outras);
//	  double     __fastcall TotalizaEnergiaDaEntrada(TList *lisREDE);


   private: //objetos externos
      VTApl    *apl;
      VTArvore *arvore;
      VTRedes  *redes;

   private: //dados locais
      VTFluxo *fluxo;
      TList   *lisTMP;
	  TList   *lisENE_PAS;

	  enum catTRAFO
	  {
		 cattSENTIDODIRETO	=0x00000001,
		 cattA1     		=0x00000002,
		 cattA2	    		=0x00000004,
		 cattA3				=0x00000008,
		 cattA3a    		=0x00000010,
		 cattA4	 			=0x00000020,
		 cattB      		=0x00000040
	  };
   };

#endif
//---------------------------------------------------------------------------
//eof

