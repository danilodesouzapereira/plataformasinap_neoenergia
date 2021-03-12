//---------------------------------------------------------------------------
#ifndef VTDadosExtH
#define VTDadosExtH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------

class VTCurva;

//---------------------------------------------------------------------------
class VTDadosExt : public TObject
{
    public:  //property
		__property int        IdAlimPai          = {read=PD.id_alim_pai,         write=PD.id_alim_pai    	 };
		__property AnsiString CodChave           = {read=PD.cod_chave,           write=PD.cod_chave   	 	 };
		__property VTCurva*   CurvaDemanda       = {read=PM_GetCurva                                   		 };
		__property AnsiString Regional           = {read=PD.regional,            write=PD.regional     		 };
		__property AnsiString Sigla              = {read=PD.sigla,               write=PD.sigla        		 };
		__property bool		  MaeFilhaConectada  = {read=PD.mae_filha_conectada, write=PD.mae_filha_conectada};

    public:
                             __fastcall  VTDadosExt(void) {};
        virtual              __fastcall ~VTDadosExt(void) {};
        virtual VTDadosExt*  __fastcall  Clone(void) = 0;
        virtual void         __fastcall  CopiaAtributosDe(VTDadosExt &ref) = 0;
        virtual void         __fastcall  DefineCurva(VTCurva* curva) = 0;

    protected:  //métodos acessados via Property
        virtual VTCurva*   __fastcall PM_GetCurva(void) = 0;
        struct
        {
			int id_alim_pai;
			bool mae_filha_conectada;
            AnsiString cod_chave;
            AnsiString sigla;
            AnsiString regional;
        }PD;
};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTDadosExt
//---------------------------------------------------------------------------
VTDadosExt* __fastcall NewObjDadosExt(void);

//---------------------------------------------------------------------------
#endif
//eof

 