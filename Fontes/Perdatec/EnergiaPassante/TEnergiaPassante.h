//---------------------------------------------------------------------------
#ifndef TEnergiaPassanteH
#define TEnergiaPassanteH

//---------------------------------------------------------------------------
#include "VTEnergiaPassante.h"

//---------------------------------------------------------------------------
class TEnergiaPassante : public VTEnergiaPassante
   {
   public:
           __fastcall  TEnergiaPassante(void);
           __fastcall ~TEnergiaPassante(void);
      bool __fastcall  Agrega(VTEnergiaPassante *energiaPassante);

   private:  //métodos acessados via property
	  AnsiString __fastcall PM_GetDescricao(void);
	  double     __fastcall PM_GetEnergiaEntregueExt_mwh(int catPt);
	  double     __fastcall PM_GetEnergiaRecebidaExt_mwh(int catPt);
      TObject*   __fastcall PM_GetObject(void);
      //
	  void       __fastcall PM_SetDescricao(AnsiString descricao);
	  void       __fastcall PM_SetEnergiaEntregueExt_mwh(int catPt, double energia_mwh);
	  void       __fastcall PM_SetEnergiaRecebidaExt_mwh(int catPt, double energia_mwh);
      void       __fastcall PM_SetObject(TObject *object);

   private: //dados locais acessados via property
      struct   {
               AnsiString descricao;
               TObject   *object;
			   struct   {
						double entregue_ext_mwh[catptCOUNT];
						double recebida_ext_mwh[catptCOUNT];
                        }energia;
               }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

