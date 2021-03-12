//-----------------------------------------------------------------------------
#ifndef VTSinapClientH
#define VTSinapClientH

//-----------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTSinapClient  : public TObject
   {
   public:
      __property bool Ativo = {read=PM_GetAtivo};

   public:
                    __fastcall  VTSinapClient(void) {};
      virtual       __fastcall ~VTSinapClient(void) {};
      virtual bool  __fastcall  Conecta(AnsiString ip_address, int server_port) = 0;
      virtual void  __fastcall  Desconecta(void) = 0;
      virtual bool  __fastcall  GetRegistro(int eqpto_tipo, int eqpto_id,  TDateTime dh) = 0;
      virtual bool  __fastcall  GetRegistro(int eqpto_tipo, AnsiString eqpto_cod, TDateTime dh) = 0;
		virtual bool  __fastcall  ResetFalta(void) = 0;
		virtual bool  __fastcall  SetCapacitorEstado(int eqpto_tipo, AnsiString eqpto_cod, bool ligado, TDateTime dh) = 0;
		virtual bool __fastcall   SetCapacitorEstado(int eqpto_tipo, int eqpto_id, bool ligado, TDateTime dh) = 0;
		virtual bool  __fastcall  SetFalta(int eqpto_tipo, AnsiString eqpto_cod, int tipo_falta, double rdef_ohm,
													  double dist_bar1_pu, bool pre_falta, TDateTime dh) = 0;
		virtual bool  __fastcall  SetFalta(int eqpto_tipo, int eqpto_id, int tipo_falta, double rdef_ohm,
                                         double dist_bar1_pu, bool pre_falta, TDateTime dh) = 0;
		virtual bool  __fastcall  SetManobraChave(int eqpto_tipo, AnsiString eqpto_cod, bool abrir, TDateTime dh) = 0;
		virtual bool  __fastcall  SetManobraChave(int eqpto_tipo, int eqpto_id, bool abrir, TDateTime dh) = 0;
		virtual bool  __fastcall  SetSupervisaoAutomatica(bool enabled) = 0;
		virtual bool  __fastcall  SetTrafoTap(int eqpto_tipo, AnsiString eqpto_cod, int variacao_tap, TDateTime dh) = 0;
		virtual bool  __fastcall  SetTrafoTap(int eqpto_tipo, int eqpto_id, int variacao_tap, TDateTime dh) = 0;

   protected:
      virtual bool  __fastcall  PM_GetAtivo(void) = 0;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTSinapClient
//-----------------------------------------------------------------------------
VTSinapClient* __fastcall NewObjSinapClient(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
