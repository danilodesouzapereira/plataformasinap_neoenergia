//---------------------------------------------------------------------------
#ifndef VTMontaDataH
#define VTMontaDataH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTData;
class VTNet;

//---------------------------------------------------------------------------
class VTMontaData : public TObject
   {
   public:
                   __fastcall VTMontaData(void) {};
      virtual      __fastcall ~VTMontaData(void) {};
      virtual bool __fastcall Inicia(VTNet *net, VTData *data, int ind_pat, double sbase=100.) = 0;
      virtual bool __fastcall InsereBarra(void) = 0;
      virtual bool __fastcall InsereCapacitor(void) = 0;
      virtual bool __fastcall InsereCapserie(void) = 0;
      virtual bool __fastcall InsereCarga(void) = 0;
      virtual bool __fastcall InsereEqbar(int tipo=-1) = 0;
      virtual bool __fastcall InsereFiltro(void) = 0;
      virtual bool __fastcall InsereGerador(void) = 0;
      virtual bool __fastcall InsereLigacao(int tipo=-1) = 0;
      virtual bool __fastcall InsereMutua(void) = 0;
      virtual bool __fastcall InsereReator(void) = 0;
      virtual bool __fastcall InsereRegulador(void) = 0;
      virtual bool __fastcall InsereSuprimento(void) = 0;
      virtual bool __fastcall InsereTrecho(void) = 0;
      virtual bool __fastcall InsereTrafo2E(void) = 0;
      virtual bool __fastcall InsereTrafo3E(void) = 0;
      virtual bool __fastcall InsereTrafoZZ(void) = 0;
      virtual bool __fastcall InsereYref(void) = 0;
      virtual bool __fastcall SalvaResBarra(void) = 0;
      virtual bool __fastcall SalvaResGerador(void) = 0;
      virtual bool __fastcall SalvaResSuprimento(void) = 0;
      virtual bool __fastcall SalvaResTrafo2E(void) = 0;
      virtual bool __fastcall SalvaResTrecho(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaData
//---------------------------------------------------------------------------
VTMontaData* __fastcall NewObjMontaData(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


