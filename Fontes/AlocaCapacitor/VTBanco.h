//---------------------------------------------------------------------------
#ifndef VTBancoH
#define VTBancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCapacitor;
class VTLigacao;

//---------------------------------------------------------------------------
class VTBanco : public TObject
   {
   public:  //property
      __property VTBarra   *Barra   = {read=PD.barra,   write=PD.barra};
      __property VTLigacao *Ligacao = {read=PD.ligacao, write=PD.ligacao};

   public:
							  __fastcall  VTBanco(void) {};
      virtual          __fastcall ~VTBanco(void) {};
		virtual bool     __fastcall  InsereCapacitor(VTCapacitor *capacitor) = 0;
		virtual TList*   __fastcall  LisCapacitor(void) = 0;
		virtual int      __fastcall  NumCapacitor(void) = 0;
		virtual double   __fastcall  Q_mvar(int ind_pat, bool nominal=true) = 0;
		virtual double   __fastcall  Qtotal_mvar(void) = 0;

   protected:
      struct{
            VTBarra   *barra;
            VTLigacao *ligacao;
            }PD;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBanco* __fastcall NewObjBanco(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

