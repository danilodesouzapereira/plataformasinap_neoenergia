//---------------------------------------------------------------------------
#ifndef VTOcorrGeralH
#define VTOcorrGeralH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//hk4 #include <TeEngine.hpp>
//---------------------------------------------------------------------------
class VTEqptoOc;  //ok
class VTCausa;    //ok
class VTClasse;   //ok
class VTAno;      //ok
class VTTipoOcorr;//ok
class VTCircuito; //ok
class VTClasseCliente;

//---------------------------------------------------------------------------
class VTOcorrGeral : public TObject
	{
	public: //property
	__property TDateTime DhInicial   = {read = GetDhInicial, write = SetDhInicial};
	__property TDateTime DhFinal     = {read = GetDhFinal, write = SetDhFinal};
	__property int  Cliente          = {read = GetCliente, write = SetCliente};
	__property double ClienteHora    = {read = GetClienteHora, write = SetClienteHora};
	__property bool FlagRamal        = {read=PM_GetFlagRamal, write=PM_SetFlagRamal};
	__property TObject* Obj          = {read=PM_Get_Obj, write=PM_Set_Obj}; //hk201703

	public:
										 __fastcall VTOcorrGeral(void) {};
		virtual                  __fastcall ~VTOcorrGeral(void) {};


	public: //dados locais

	protected:  //métodos acessados via property
//Auxiliares: transicao hk201612
	virtual TDateTime 		 __fastcall GetDhInicial(void) = 0;
	virtual TDateTime 		 __fastcall GetDhFinal(void) = 0;
	virtual int 				 __fastcall GetCliente(void) = 0;
	virtual bool             __fastcall PM_GetFlagRamal(void) = 0;
	virtual double 			 __fastcall GetClienteHora(void) = 0;

	virtual void 				 __fastcall SetDhInicial(TDateTime dhInicial) = 0;
	virtual void 			    __fastcall SetDhFinal(TDateTime dhFinal) = 0;
	virtual void 				 __fastcall SetCliente(int cliente) = 0;
	virtual void 			    __fastcall SetClienteHora(double clienteHora) = 0;
	virtual void             __fastcall PM_SetFlagRamal(bool flagRamal) = 0;

	virtual TObject*         __fastcall PM_Get_Obj(void) = 0; //hk201703
	virtual void             __fastcall PM_Set_Obj(TObject *obj) = 0;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTOcorr
//---------------------------------------------------------------------------
VTOcorrGeral* __fastcall NewObjOcorrGeral(void);
//---------------------------------------------------------------------------
#endif
