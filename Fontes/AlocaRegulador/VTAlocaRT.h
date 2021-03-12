//---------------------------------------------------------------------------
#ifndef VTAlocaBCH
#define VTAlocaBCH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTAlocaRT : public TObject
   {
	public:  //property
	  __property bool    OP_TRONCO   = {read=PD.op_tronco,     write=PD.op_tronco};
	  __property bool    OP_DIST     = {read=PD.dist_definido, write=PD.dist_definido};
	  __property double  V_ALVO      = {read=PD.v_alvo,        write=PD.v_alvo};
	  __property double  V_MIN       = {read=PD.v_min,         write=PD.v_min};
	  __property double  DIST_M      = {read=PD.dist_m,        write=PD.dist_m};
	  __property int     MAX_REG     = {read=PD.max_reg,       write=PD.max_reg};
	  __property int     TIPO_AJUSTE = {read=PD.tipo_ajuste,   write=PD.tipo_ajuste};
	  __property int     TAP_FIXO    = {read=PD.tap_fixo,      write=PD.tap_fixo};

   public:
                   __fastcall  VTAlocaRT(void) {};
      virtual      __fastcall ~VTAlocaRT(void) {};
      virtual bool __fastcall  Executa(TList *lisTabReg) = 0;
      virtual bool __fastcall  InsereBancosAlocadosNaRede(void) = 0;
      virtual int  __fastcall  ListaReguladorAlocado(TList *lisEXT) = 0;
      virtual void __fastcall  Reinicia(void) = 0;
		virtual bool __fastcall  RetiraBancosAlocadosDaRede(void) = 0;
		virtual bool __fastcall  RetiraBancosAlocadosDaRede(TList *lisReg, bool remove_banco) = 0; //DVK 2014.02.11


	protected:
		struct{
			bool        op_tronco;
			bool        dist_definido;
			double      v_alvo;
			double      v_min;
			double		dist_m;
			int			max_reg;
			int         tipo_ajuste;   //{0:fixo_posdefinido, 1: fixo_predefinido, 2: automático barra fixa, 3: automatico unidirecional}
			int			tap_fixo;      //valor do tap para ajuste de tap fixo, pré-definido
			}PD;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTAlocaRT* __fastcall NewObjAlocaRT(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


