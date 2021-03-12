//---------------------------------------------------------------------------
#ifndef VTAjustaEnergiaH
#define VTAjustaEnergiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TMedicao;

//---------------------------------------------------------------------------
class VTAjustaEnergia : public TObject
   {
	public:
                     __fastcall  VTAjustaEnergia(void) {};
		virtual        __fastcall ~VTAjustaEnergia(void) {};
      virtual double __fastcall  ExecutaAjusteComPerda(int dias_mes) = 0;
      virtual bool   __fastcall  ExecutaAjusteSemPerda(int dias_mes) = 0;
      virtual bool   __fastcall  IniciaAllMedicao(int dias_mes, bool divide_perda_comercial=true) = 0;
      virtual bool   __fastcall  IniciaDivisaoPerdaComercial(TMedicao *medicao) = 0;
      virtual bool   __fastcall  IniciaUmaMedicao(int dias_mes, TMedicao *medicao) = 0;
      virtual void   __fastcall  SalvaCargaFinal(void) = 0;
      virtual bool   __fastcall  TESTE_CriaMedicoes(int dia_mes) = 0;
	  virtual void 	 __fastcall RemoveMedicaoInvalida(void)=0;

   public://diagnóstico das medições
      TStringList *strDIAG_MED;
   };

//---------------------------------------------------------------------------
VTAjustaEnergia* __fastcall NewObjAjustaEnergia(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 