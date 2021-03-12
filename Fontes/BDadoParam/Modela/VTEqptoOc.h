//---------------------------------------------------------------------------
#ifndef VTEqptoOcH
#define VTEqptoOcH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//hk4 #include <TeEngine.hpp>
//hk201611: nova property e Tat sem write
//		__property double     Tat              = {read=PM_GetTat}; //hk201611,              write=PM_SetTat};
//		__property double     DurTotal         = {read=PM_GetDurTotal,         write=PM_SetDurTotal}; //hk201611
//---------------------------------------------------------------------------
class VTBloco;
class VTEqpto;

//---------------------------------------------------------------------------
class VTEqptoOc : public TObject
	{
	public: //property
		__property VTBloco*   Bloco            = {read=PM_GetBloco,            write=PM_SetBloco};
		__property double     ComprimentoBloco = {read=PM_GetComprimentoBloco, write=PM_SetComprimentoBloco};
		__property VTEqpto*   Eqpto            = {read=PM_GetEqpto,            write=PM_SetEqpto};
		__property AnsiString EqptoCodBase     = {read=PM_GetEqptoCodBase,     write=PM_SetEqptoCodBase};
		__property int        Noc              = {read=PM_GetNoc,              write=PM_SetNoc};
		__property double     TaxaFalhas       = {read=PM_GetTaxaFalhas,       write=PM_SetTaxaFalhas};
		__property double     Tat              = {read=PM_GetTat}; //hk201611,              write=PM_SetTat};
		__property double     DurTotal         = {read=PM_GetDurTotal,         write=PM_SetDurTotal}; //hk201611
		__property double     DurMes[int i]    = {read=PM_GetDurMes,           write=PM_SetDurMes};
		__property int        NocHor[int i]    = {read=PM_GetNocHor,           write=PM_SetNocHor};
		__property int        NocMes[int i]    = {read=PM_GetNocMes,           write=PM_SetNocMes};
		__property double     TatMes[int i]    = {read=PM_GetTatMes,           write=PM_SetTatMes};
		__property int        QtdOcorrEqpto    = {read=PM_GetQtdOcorrEqpto,    write=PM_SetQtdOcorrEqpto};
		__property int        QtdOcorr    		= {read=PM_GetQtdOcorr,    	  write=PM_SetQtdOcorr};

	public:
								 __fastcall VTEqptoOc(void) {};
		virtual            __fastcall ~VTEqptoOc(void) {};
		virtual void       __fastcall Reinicia(void) = 0;

	public: //métodos
		virtual TList*     __fastcall ListaBlocoViz(void) = 0;
		virtual TList*     __fastcall ListaInterrAcid(void) = 0;
		virtual TList*     __fastcall ListaInterrProg(void) = 0;
		virtual TList*     __fastcall ListaOcorr(void) = 0;
		virtual TList*     __fastcall ListaCircuitos(void) = 0;//hkmuda

	protected:  //métodos acessados via property
		virtual VTBloco*   __fastcall PM_GetBloco(void) = 0;
		virtual double     __fastcall PM_GetComprimentoBloco(void) = 0;
		virtual VTEqpto*   __fastcall PM_GetEqpto(void) = 0;
		virtual AnsiString __fastcall PM_GetEqptoCodBase(void) = 0;
		virtual int        __fastcall PM_GetNoc(void) = 0;
		virtual double     __fastcall PM_GetTat(void) = 0;
		virtual double     __fastcall PM_GetTaxaFalhas(void) = 0;
		virtual double     __fastcall PM_GetDurMes(int i) = 0;
		virtual int        __fastcall PM_GetNocHor(int i) = 0;
		virtual int        __fastcall PM_GetNocMes(int i) = 0;
		virtual double     __fastcall PM_GetTatMes(int i) = 0;
		virtual int		  	 __fastcall PM_GetQtdOcorrEqpto(void) = 0;
		virtual int		  	 __fastcall PM_GetQtdOcorr(void) = 0;
		virtual void       __fastcall PM_SetBloco(VTBloco* bloco) = 0;
		virtual void       __fastcall PM_SetComprimentoBloco(double comp) = 0;
		virtual void       __fastcall PM_SetEqpto(VTEqpto* eqpto) = 0;
		virtual void       __fastcall PM_SetEqptoCodBase(AnsiString cod) = 0;
		virtual void       __fastcall PM_SetNoc(int noc) = 0;
		//hk201611 virtual void       __fastcall PM_SetTat(double tat) = 0;
		virtual void       __fastcall PM_SetTaxaFalhas(double taxa_falhas) = 0;
		virtual void       __fastcall PM_SetDurMes(int i, double valor) = 0;
		virtual void       __fastcall PM_SetNocHor(int i, int valor) = 0;
		virtual void       __fastcall PM_SetNocMes(int i, int valor) = 0;
		virtual void       __fastcall PM_SetTatMes(int i, double valor) = 0;
		virtual double     __fastcall PM_GetDurTotal(void) = 0;  //hk201611
		virtual void       __fastcall PM_SetDurTotal(double valor) = 0;
		virtual void		 __fastcall PM_SetQtdOcorrEqpto(int qtd) = 0;
		virtual void  	  	 __fastcall PM_SetQtdOcorr(int qtd) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEqptoOc
//---------------------------------------------------------------------------
VTEqptoOc* __fastcall NewObjEqptoOc(void);
//---------------------------------------------------------------------------
#endif
