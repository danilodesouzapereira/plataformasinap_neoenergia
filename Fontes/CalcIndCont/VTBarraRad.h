//---------------------------------------------------------------------------
#ifndef VTBarraRadH
#define VTBarraRadH
//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
//---------------------------------------------------------------------------
class VTBarraRad : public TObject
	{
	public:  //property
		//__property strResBloco* ResBloco[TObject *bloco] = {read=PM_GetResBloco};

	public:
								 __fastcall VTBarraRad(void) {};
		virtual            __fastcall ~VTBarraRad(void) {};
		virtual		  void __fastcall Inicia(void) = 0;

	protected:  //métodos acessados via property
		//virtual strResBloco* __fastcall PM_GetResBloco(TObject *bloco) = 0;

	public: //dados
		VTLigacao *pligaPai;
		VTBarra   *pbarPai;
		VTBarraRad *pbarRadPai;
		VTBarra   *pbarra;
		TList     *lisBarRad; //lista de barRads a jusante
		int       nconsMon; //Número de consumidores a montante
		int       nconsJus; //numero de consumidores a jusante
		double    dcompJus; //Comprimento a jusante
		double    dcompMon; //comprimento até a barra

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedeViz
//---------------------------------------------------------------------------
VTBarraRad* __fastcall NewObjBarraRad(VTBarra *pbarra);

//---------------------------------------------------------------------------
#endif
//eof
