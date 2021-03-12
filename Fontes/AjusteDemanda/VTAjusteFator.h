//---------------------------------------------------------------------------

#ifndef VTAjusteFatorH
#define VTAjusteFatorH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTRede;
class VTMedicao;
//---------------------------------------------------------------------------
enum adTIPO_FATORES
{
	adtfFATOR_UNICO=0,
	adtfFATOR_PQ,
	adtfFATOR_PFP,
	adtfCOUNT
};
//---------------------------------------------------------------------------
class VTAjusteFator : public TObject
   {
   public:
		   __fastcall  VTAjusteFator(void){};
		   __fastcall ~VTAjusteFator(void){};

	__property bool       Ativo			    = {write=ativo,         read=ativo};
	__property int        TipoFatorCorrec   = {write=tipo,          read=tipo};
	__property double     FatorCorrecP      = {write=multP,         read=multP};
	__property double     FatorCorrecQ      = {write=multQ,         read=multQ};
	__property double     FatorCorrecFP     = {write=fp,            read=fp};
	__property VTRede*    Rede			    = {write=rede,          read=rede};
	__property VTMedicao* Medicao 		    = {write=medicao,       read=medicao};

   protected:
		bool   ativo;
		double multP;
		double multQ;
		double fp;
		int tipo;
		VTRede *rede;
		VTMedicao *medicao;
   };

//---------------------------------------------------------------------------
#endif
//eof

