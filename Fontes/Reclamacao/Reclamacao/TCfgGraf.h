//---------------------------------------------------------------------------
#ifndef TCfgGrafH
#define TCfgGrafH
//---------------------------------------------------------------------------
#include <Fontes\Constante\Fases.h>
#include <Fontes\Grafico\VTCfgGraf.h>
//---------------------------------------------------------------------------
class TCfgGraf : public VTCfgGraf
   {
   public:
             __fastcall  TCfgGraf(VTApl *apl);
             __fastcall ~TCfgGraf(void);
      TColor __fastcall  CorBarra(VTBarra *barra);
      TColor __fastcall  CorCarga(VTCarga *carga);
      TColor __fastcall  CorLigacao(VTLigacao *ligacao);

   private: //métodos
      TColor __fastcall CorCarregamento(VTBarra *barra);
		TColor __fastcall CorCarregamento(VTLigacao *ligacao);
		//DVK 2015.03.26 determina fases da zona da barra
		TColor __fastcall CorTensao(VTBarra *barra, int fases);
      TColor __fastcall CorTensao(VTCarga *carga);
		TColor __fastcall CorTensao(VTLigacao *ligacao);
		bool   __fastcall LeDados(void);

   private: //objetos externos
      VTApl *apl;
   };

//---------------------------------------------------------------------------
#endif
//eof
