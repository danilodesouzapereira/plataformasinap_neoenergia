//---------------------------------------------------------------------------
#ifndef VTCfgGrafH
#define VTCfgGrafH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum eView { viewTENSAO = 0, viewCARREG};

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCarga;
class VTLigacao;

//---------------------------------------------------------------------------
class VTCfgGraf: public TObject
	{
	public:  //property
		__property int     View = {read=PD.view,     write=PD.view};
		__property double  DRC =  {read=PD.drc,      write=PD.drc};
		__property double  DRP =  {read=PD.drp,      write=PD.drp};
		__property double  Smed = {read=PD.smedio,   write=PD.smedio};
		__property double  Snom = {read=PD.snominal, write=PD.snominal};

   public:
                     __fastcall  VTCfgGraf(void) {};
                     __fastcall ~VTCfgGraf(void) {};
      virtual TColor __fastcall  CorBarra(VTBarra *barra) = 0;
      virtual TColor __fastcall  CorCarga(VTCarga *carga) = 0;
      virtual TColor __fastcall  CorLigacao(VTLigacao *ligacao) = 0;

	protected:  //dados acessados via property
		struct{//atributos publicos
				int view;
				//valores configuraveis
				double drc, drp;         //%
				double smedio, snominal; //kVA
				}PD;
     };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTCfgGraf *NewObjCfgGrafReg(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

