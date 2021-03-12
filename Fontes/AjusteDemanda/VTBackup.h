//---------------------------------------------------------------------------
#ifndef VTBackupH
#define VTBackupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTPrimario;

//---------------------------------------------------------------------------
class VTBackup : public TObject
   {
   public:
				 __fastcall  VTBackup(void){};
	virtual		 __fastcall ~VTBackup(void){};
	virtual void __fastcall  Clear() = 0;
	virtual bool __fastcall  DuplicaCargas(TList *lisCARGA) = 0;
	virtual bool __fastcall  RestauraCargaMedicao(TList *lisMED_ERRO) = 0;
	virtual bool __fastcall  RestauraCargaTodosPrimarios(void) = 0;
	virtual bool __fastcall  RestauraCargaUmPrimario(VTPrimario *primario) = 0;
	virtual bool __fastcall  RestauraCurvaCargaAtual(TList *lisCARGA) = 0;
	virtual bool __fastcall  SalvaCargaUmPrimario(VTPrimario *rede) = 0;
	};

//---------------------------------------------------------------------------
#endif
//eof

