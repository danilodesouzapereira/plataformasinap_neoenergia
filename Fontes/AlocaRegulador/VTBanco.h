//---------------------------------------------------------------------------
#ifndef VTBancoH
#define VTBancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTLigacao;
class VTRegulador;

//---------------------------------------------------------------------------
class VTBanco : public TObject
   {
   public:  //property
      __property double       FatorAjuste = {read=PD.fator_ajuste, write=PD.fator_ajuste};
      __property VTBarra     *BarraRede   = {read=PD.barra_rede,   write=PD.barra_rede};
      __property VTBarra     *BarraNova   = {read=PD.barra_nova,   write=PD.barra_nova};
      __property VTLigacao   *LigacaoPai  = {read=PD.ligacao_pai,  write=PD.ligacao_pai};
      __property VTRegulador *Regulador   = {read=PD.regulador,    write=PD.regulador};

   public:
							  __fastcall  VTBanco(void) {};
      virtual          __fastcall ~VTBanco(void) {};
      virtual void     __fastcall  DefineCoordenadasBarraNova(void) = 0;
		virtual int      __fastcall  NumUnidades(void) = 0;
		virtual double   __fastcall  Snom_mvar(void) = 0;

   protected:
      struct{
            double      fator_ajuste;
            VTBarra     *barra_rede;
            VTBarra     *barra_nova;
            VTLigacao   *ligacao_pai;
            VTRegulador *regulador;
            }PD;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBanco* __fastcall NewObjBanco(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

