//-----------------------------------------------------------------------------
#ifndef VTBatchDCH
#define VTBatchDCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
//-----------------------------------------------------------------------------
class VTApl;
class VTImportaRedeGIS;

//-----------------------------------------------------------------------------
class VTBatchDC : public TObject
   {
   public:
		__property bool         AjusteDemandaEnabled  = {read=PD.ajuste_demanda,   write=PD.ajuste_demanda    };
		__property bool         BaseUnica             = {read=PD.base_unica,       write=PD.base_unica        };
		__property TDate        DataIni               = {read=PD.data_ini,         write=PD.data_ini          };
		__property TDate        DataFim               = {read=PD.data_fim,         write=PD.data_fim          };
		__property AnsiString   DirRede               = {read=PD.dir_rede,         write=PD.dir_rede          };
		__property strOPImporta Opcao                 = {read=PD.opcao,            write=PD.opcao             };

   public:
						 __fastcall  VTBatchDC(void) {};
      virtual      __fastcall ~VTBatchDC(void) {};
		virtual bool __fastcall  Executa(TListBox *LBoxLog=NULL) = 0;

   protected:  //dados acessados via property
      struct{
				bool       	 ajuste_demanda;
				bool       	 base_unica;
				AnsiString 	 dir_rede;
				TDate      	 data_ini;
				TDate      	 data_fim;
				strOPImporta opcao;
				}PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTBatchDC
//-----------------------------------------------------------------------------
VTBatchDC* __fastcall NewObjBatchDC(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
