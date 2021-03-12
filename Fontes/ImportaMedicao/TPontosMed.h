//---------------------------------------------------------------------------
#ifndef TPontosMedH
#define TPontosMedH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEdita;
class VTMedicao;
class VTMedidor;
class VTChave;
class VTCurva;
class VTMedidor;
class VTPrimario;
class VTRede;

//---------------------------------------------------------------------------
class TPontosMed : public TObject
   {
	public:
					  __fastcall  TPontosMed(VTApl *apl_owner);
					  __fastcall ~TPontosMed(void);
      bool       __fastcall  ChaveFicticia(VTChave *chave);
		VTChave*   __fastcall  CriaChaveFicticia(VTCanal *canal);
		VTChave*   __fastcall  CriaChaveFicticia(VTPrimario *primario);
      VTChave*   __fastcall  CriaChaveFicticia(VTRede* rede);
		VTMedicao* __fastcall  CriaMedicaoMedidor(VTMedidor *medidor);
		bool       __fastcall  CriaMedicoes(TList *lisMED_ALL);
		VTMedidor* __fastcall  CriaMedidor(VTChave *chave, int ind_bar);
//		TList*	  __fastcall  LisMedicaoFic(void);
//		void       __fastcall  UndoChave(void);
//		void       __fastcall  UndoMedidor(void);
      void       __fastcall  MoveMedidores(TList *lisMEDIDORES);
      void       __fastcall  RemoveChaveFicticia(void);

	private: //métodos
		VTBarra*   __fastcall ClonaBarra(VTBarra *barra);
		bool       __fastcall CriaMedicoes(VTPrimario *primario);
		bool       __fastcall CriaMedicoes(VTSecundario *secundario);
		VTChave*   __fastcall ExisteChave(VTMedidor *medidor);
		VTMedicao* __fastcall ExisteMedicao(VTMedidor *medidor);
//		bool 		  __fastcall MedicaoI(TList *lisI, VTMedicao *medicao);
//		bool 		  __fastcall MedicaoPQ(TList *lisPQ, VTMedicao *medicao);
		void       __fastcall MoveEqptos(TList *lisLIG, VTBarra *barra_dest);

	private: //objetos externos
		VTApl       *apl;
		TList			*lisMED_ALL;

	private: //dados locais
		TList       *lisMEDIDOR_FIC, *lisCHV_FIC;
		VTEdita		*edita;

    };

//---------------------------------------------------------------------------
#endif
//eof
