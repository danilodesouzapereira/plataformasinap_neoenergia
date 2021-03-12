  //---------------------------------------------------------------------------
#ifndef VTImportaMercadoH
#define VTImportaMercadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class  VTApl;
class  VTCarga;
class  VTCenario;
class  VTCrescimento;
class  VTRede;
class  VTRedes;
class  VTTipoRede;
class  VTSegmento;
struct strUTM;

//---------------------------------------------------------------------------
#define ANO_INDEF  -1

//---------------------------------------------------------------------------
class VTImportaMercado : public TObject
   {
   public:  //property
	  __property bool        BatchMode       = {read=batchMode,   write=batchMode  };


   public:  //m�todos
					 __fastcall  VTImportaMercado(void) {};
		virtual      __fastcall ~VTImportaMercado(void) {};
		virtual bool __fastcall Executa(AnsiString arq_medicao) = 0;
		virtual bool __fastcall ExecutaV2(AnsiString arq_medicao) = 0;

   protected:  //m�todos acessados via property
	 bool  batchMode;

   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTImportaMercado
//---------------------------------------------------------------------------
VTImportaMercado* __fastcall NewObjImportaMercado(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
