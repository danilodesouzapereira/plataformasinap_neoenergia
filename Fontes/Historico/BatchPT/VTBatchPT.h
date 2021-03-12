//-----------------------------------------------------------------------------
#ifndef VTBatchPTH
#define VTBatchPTH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTBatchPT : public TObject
   {
   public:
      __property bool      AjusteDemandaEnabled  = {read=PD.ajuste_demanda,   write=PD.ajuste_demanda    };
      __property TDate     DataIni               = {read=PD.data_ini,         write=PD.data_ini          };
      __property TDate     DataFim               = {read=PD.data_fim,         write=PD.data_fim          };
      __property TListBox *LBoxLogRede           = {read=PD.LBoxLogRede,      write=PD.LBoxLogRede  };
      __property TListBox *LBoxLogPasso          = {read=PD.LBoxLogPasso,     write=PD.LBoxLogPasso };

   public:
						 __fastcall  VTBatchPT(void) {};
      virtual      __fastcall ~VTBatchPT(void) {};
		virtual bool __fastcall  Executa(void) = 0;

   protected:  //dados acessados via property
      struct{
            bool      ajuste_demanda;
            TDate     data_ini;
            TDate     data_fim;
            TListBox  *LBoxLogRede;
            TListBox  *LBoxLogPasso;
            }PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTBatchPT* __fastcall NewObjBatchPT(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
