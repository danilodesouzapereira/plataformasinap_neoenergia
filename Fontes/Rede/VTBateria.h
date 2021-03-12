//---------------------------------------------------------------------------
#ifndef VTBateriaH
#define VTBateriaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\TEqbar.h"

//---------------------------------------------------------------------------
enum enumBATERIA_ESTADO        {esCARREGADA=0, esINTERMEDIARIO, esDESCARREGADA};
enum enumBATERIA_MODO_OPERACAO {moCARGA=0, moDESCARGA, moOCIOSA};
enum enumBATERIA_TIPO_OPERACAO {toMANUAL=0, toAUTO};

//---------------------------------------------------------------------------
class VTCurva;

//---------------------------------------------------------------------------
class VTBateria : public TEqbar
   {
   public: //property
	__property double	Cap_inv_kva		  = {read=PD_BATERIA.cap_inv_kva,		write=PD_BATERIA.cap_inv_kva};
	__property double	Earmaz_inic_pu	  = {read=PD_BATERIA.earmaz_inic_pu,	write=PD_BATERIA.earmaz_inic_pu};
	__property double	Enom_kwh	        = {read=PD_BATERIA.enom_kwh,         write=PD_BATERIA.enom_kwh};
	__property int		Fases            = {read=PD_BATERIA.fases,            write=PD_BATERIA.fases};
	__property double	Gatilho_carga_pu = {read=PD_BATERIA.gatilho_carga_pu, write=PD_BATERIA.gatilho_carga_pu};
	__property double	Gatilho_desc_pu  = {read=PD_BATERIA.gatilho_desc_pu,	write=PD_BATERIA.gatilho_desc_pu};
	__property double	Pnom_kw			  = {read=PD_BATERIA.pnom_kw,				write=PD_BATERIA.pnom_kw};
	__property double	Pext_carga_kw	  = {read=PD_BATERIA.pext_carga_kw,		write=PD_BATERIA.pext_carga_kw};
	__property double	Qext_carga_kvar  = {read=PD_BATERIA.qext_carga_kvar,	write=PD_BATERIA.qext_carga_kvar};
	__property double	Pext_desc_kw	  = {read=PD_BATERIA.pext_desc_kw,     write=PD_BATERIA.pext_desc_kw};
	__property double	Qext_desc_kvar   = {read=PD_BATERIA.qext_desc_kvar,	write=PD_BATERIA.qext_desc_kvar};
	__property double	P_ociosa_pu		  = {read=PD_BATERIA.p_ociosa_pu,		write=PD_BATERIA.p_ociosa_pu};
	__property double	Q_ociosa_pu		  = {read=PD_BATERIA.q_ociosa_pu,		write=PD_BATERIA.q_ociosa_pu};
	__property double	Rend_carga_pu	  = {read=PD_BATERIA.rend_carga_pu,		write=PD_BATERIA.rend_carga_pu};
	__property double	Rend_desc_pu	  = {read=PD_BATERIA.rend_desc_pu,     write=PD_BATERIA.rend_desc_pu};
	__property int		Tipo_operacao	  = {read=PD_BATERIA.tipo_operacao,    write=PD_BATERIA.tipo_operacao}; // Cf. 'enumBATERIA_TIPO_OPERACAO'
	__property double	Icte_pu      	  = {read=PD_BATERIA.Icte,       		write=PD_BATERIA.Icte};
	__property double	Scte_pu       	  = {read=PD_BATERIA.Scte,       		write=PD_BATERIA.Scte};
	__property double	Zcte_pu       	  = {read=PD_BATERIA.Zcte,       		write=PD_BATERIA.Zcte};

	__property int		ModeloCarga		  = {read=PM_GetModeloCarga};
	__property int		ModeloDesc		  = {read=PM_GetModeloDesc};

	__property int  	Estado[int np]           = {read=PM_GetEstado,				write=PM_SetEstado}; // Cf. 'enumBATERIA_ESTADO'
	__property int		Modo_operacao[int np]    = {read=PM_GetModo_operacao,    write=PM_SetModo_operacao}; // Cf. 'enumBATERIA_MODO_OPERACAO'
	__property double	Earmaz_atual_kwh[int np] = {read=PM_GetEarmaz_atual_kwh,	write=PM_SetEarmaz_atual_kwh};
	__property double	Pext_kw[int np]          = {read=PM_GetPext_kw,				write=PM_SetPext_kw};
	__property double	Qext_kvar[int np]        = {read=PM_GetQext_kvar,			write=PM_SetQext_kvar};

   public:  // Métodos
						 __fastcall  VTBateria(void) {};
	 virtual             __fastcall ~VTBateria(void) {};
	 virtual VTBateria * __fastcall  Clone(void) = 0;
	 virtual void        __fastcall  DefineCurva(VTCurva *curva)=0;
	 virtual bool 		 __fastcall  EstadoBateria(void)=0;
	 virtual void        __fastcall  IniciaVetores(void) = 0;
     virtual void        __fastcall  LiberaVetores(void) = 0;
     virtual void        __fastcall  ReiniciaNumPat(int np) = 0;

   protected:  // Métodos acessados via property
      //Get
	   virtual int    __fastcall PM_GetModeloCarga(void) = 0;
      virtual int    __fastcall PM_GetModeloDesc(void) = 0;
      virtual int    __fastcall PM_GetEstado(int ind_pat) = 0;
      virtual int    __fastcall PM_GetModo_operacao(int ind_pat) = 0;
      virtual double __fastcall PM_GetEarmaz_atual_kwh(int ind_pat) = 0;
      virtual double __fastcall PM_GetPext_kw(int ind_pat) = 0;
      virtual double __fastcall PM_GetQext_kvar(int ind_pat) = 0;
      //Set
      virtual void   __fastcall PM_SetEstado(int ind_pat,
											            int estado) = 0;
      virtual void   __fastcall PM_SetModo_operacao(int ind_pat,
											                   int modo_operacao) = 0;
      virtual void   __fastcall PM_SetEarmaz_atual_kwh(int    ind_pat,
													                double earmaz_kwh) = 0;
      virtual void   __fastcall PM_SetPext_kw(int    ind_pat,
											             double pext_kw) = 0;
      virtual void   __fastcall PM_SetQext_kvar(int    ind_pat,
												            double qext_kvar) = 0;

   public:  // Objetos externos
	  VTCurva *curvaCargaDescarga;

   protected:  // Dados
	  struct{
			// Valores nominais (fixos)
			int    fases;
			int    tipo_operacao;
         int    modelo_carga;  // Icte, Scte ou Zcte
         int    modelo_desc;   // Icte, Scte ou Zcte
			double Icte, Scte, Zcte;   //modelo de carga (pu)
			double pnom_kw;
			double enom_kwh;
			double cap_inv_kva;
			double earmaz_inic_pu;
         double pext_carga_kw;
         double qext_carga_kvar;
         double pext_desc_kw;
         double qext_desc_kvar;
			double p_ociosa_pu;
			double q_ociosa_pu;
			double rend_carga_pu;
			double rend_desc_pu;
			double gatilho_carga_pu;
			double gatilho_desc_pu;
			// Dados de operação (variáveis)
			int    *estado;
			int    *modo_operacao;
			double *earmaz_kwh;
			double *pext_kw;
			double *qext_kvar;
			} PD_BATERIA;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Bateria
//---------------------------------------------------------------------------
VTBateria* __fastcall NewObjBateria (int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

