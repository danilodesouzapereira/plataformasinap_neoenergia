//-----------------------------------------------------------------------------
#ifndef VTBatchPRH
#define VTBatchPRH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTBatchPR : public TObject
   {
   public:
      __property TDate       DataIni      = {read=PD.data_ini,     write=PD.data_ini     };
      __property TDate       DataFim      = {read=PD.data_fim,     write=PD.data_fim     };
      __property TListBox   *LBoxLogRede  = {read=PD.LBoxLogRede,  write=PD.LBoxLogRede  };
      __property TListBox   *LBoxLogPasso = {read=PD.LBoxLogPasso, write=PD.LBoxLogPasso };

   public:
						 __fastcall  VTBatchPR(void) {};
      virtual      __fastcall ~VTBatchPR(void) {};
		virtual bool __fastcall  Executa(void) = 0;

   protected:  //dados acessados via property
      struct{
            TDate       data_ini;
            TDate       data_fim;
            TListBox   *LBoxLogRede;
            TListBox   *LBoxLogPasso;
            }PD;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTBatchPR* __fastcall NewObjBatchPR(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
