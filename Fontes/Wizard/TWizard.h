//-----------------------------------------------------------------------------
#ifndef TWizardH
#define TWizardH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTWizard.h"

//---------------------------------------------------------------------------
class VTArea;
class VTBusca;
class VTFiguraComentario;
class VTEqpto;
class VTLegenda;

//---------------------------------------------------------------------------
class TWizard : public VTWizard
   {
   public:
                 __fastcall TWizard(VTApl *apl);
                 __fastcall ~TWizard(void);
      bool  __fastcall Executa(TWinControl *wincrtl, int x, int y, TColor cor_fundo);

   private: //métodos
	  VTFiguraComentario* __fastcall BuscaComentario(int x, int y);
	  VTEqpto*      __fastcall BuscaEqpto(int x, int y);
	  void          __fastcall ImageLimpa(TImage *image, TColor color);
	  void          __fastcall ImagePreenche(TImage *image, TColor color);
      void          __fastcall ImagePreenche(TImage *image, TStrings *texto);
	  void          __fastcall MontaLisDado(VTEqpto *eqpto);
	  void          __fastcall LoadShowFromFile(void);
	  void          __fastcall SaveShowToFile(void);
	  void          __fastcall SetShowAtributo(unsigned &show, int bit_index, bool enabled);
	  void          __fastcall SetShowDefault(void);

   private: //objetos externos
	  VTApl        *apl;
	  VTBusca      *busca;
	  VTEqpto      *ult_eqpto;
	  VTFiguraComentario *ult_comentario;
      TWinControl  *wincrtl;

   private: //dados locais
      VTArea    *area;
      VTLegenda *legenda;
      TImage    *image;
      TList     *lisDADO;
      #define  DIM_VET_EQPTO   50
      struct   {
               bool      enabled;
               unsigned  show;
               }vet_eqpto[DIM_VET_EQPTO];
   };

#endif
//---------------------------------------------------------------------------
//eof
