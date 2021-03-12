//---------------------------------------------------------------------------
#ifndef TImportaMercadoH
#define TImportaMercadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <System.IOUtils.hpp>
#include <System.JSON.hpp>
#include <System.Classes.hpp>
#include "VTImportaMercado.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTClasse;
class VTCrescimento;
class VTCrescimentoJSON;
class VTMercadoJSON;
class VTSegmentoJSON;
class VTSegmento;
class VTTaxaJSON;
class VTRede;
class VTTipoRede;

//---------------------------------------------------------------------------
class TImportaMercado : public VTImportaMercado
{
	public:
		__fastcall  TImportaMercado(VTApl *apl_owner);
		__fastcall ~TImportaMercado(void);
		 bool __fastcall Executa(AnsiString arq_medicao);
		 bool __fastcall ExecutaV2(AnsiString arq_medicao);

	private: //métodos
		void __fastcall AtualizaSegmento(AnsiString classeNome);
		void __fastcall AtualizaSegmento(VTRede *rede, AnsiString classeNome);
		void __fastcall AtualizaSegmento(VTTipoRede *tiporede, AnsiString classeNome);
		void __fastcall DefineCrescimento(VTRede *rede);
		void __fastcall DefineCrescimentoClasse(VTSegmento *segmento, VTClasse *classe);
		void __fastcall DefineCrescimentoTodas(VTSegmento *segmento);
		void __fastcall DefineTaxas(VTCrescimento *crescimento);
		void __fastcall DefineTaxasV2(VTCrescimentoJSON *crescimentoJSON);
		bool __fastcall DefineTipoSegmento(VTSegmentoJSON *segmentoJSON, AnsiString classeNome);
        VTRede* __fastcall ExisteCodigoRede(AnsiString codigo_rede);
		VTRede* __fastcall ExisteRede(AnsiString num_rede);
		VTTipoRede* __fastcall ExisteTipoRede(AnsiString tipo_rede);
		bool __fastcall LeituraCrescimento(TJSONValue* jCrescimento, VTSegmentoJSON *segmentoJSON);
		bool __fastcall LeituraSegmento(TJSONValue* jSegmento, VTMercadoJSON *mercadoJSON);
		bool __fastcall LeituraTaxa(TJSONValue* jTaxa, VTCrescimentoJSON *crescimentoJSON);
		void __fastcall ValidaAnoFinal(void);
		bool __fastcall ValidaAnosHorizonte(VTMercadoJSON *mercadoJSON);

	private: //objetos externos
		VTApl *apl;
		VTTaxaJSON *taxaJSON;
		VTCrescimento *crescimento;
		VTSegmento *segmento;

	private: //objetos locais
		TStringList* campos;
		TStringList* linhas;
        TList* lisCARGA;
		int ind_taxa; //a partir desse campo, contem as taxas de crescimento no arquivo

};

//---------------------------------------------------------------------------
#endif
