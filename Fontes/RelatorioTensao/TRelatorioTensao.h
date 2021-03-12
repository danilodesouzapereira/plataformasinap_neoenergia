//---------------------------------------------------------------------------
#ifndef TRelatorioTensaoH
#define TRelatorioTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTProgresso;
class TFaixaTensao;
class TNivelTensao;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TRelatorioTensao : public TObject
{
    public:
			  __fastcall  TRelatorioTensao (VTApl *apl);
			  __fastcall ~TRelatorioTensao(void);
		void  __fastcall  AtualizaGrafico(TFaixaTensao *faixa);
		void  __fastcall  CopiaFaixas(TNivelTensao *nivel_ori, TNivelTensao *nivel_des);
		bool  __fastcall  Executa(void);
		bool  __fastcall  FluxoOk(void);
		void  __fastcall  ProgressoStart(void);
		void  __fastcall  ProgressoStop(void);

	public:
		TNivelTensao *nivel_at, *nivel_mt, *nivel_bt;
		bool tensao_fase, tensao_minima, config_alterada;
		int ind_pat;

	private: //métodos
		void __fastcall LeDados(void);
		void __fastcall PintaBarras(TNivelTensao *nivel);
		void __fastcall SalvaDados(void);

	private: //dados externos
		VTApl *apl;

	private: //dados locais
		VTProgresso *progresso;

};

//---------------------------------------------------------------------------
#endif
